#include "Buyer.h"
#include "Item.h"
#include <vector>
#include <cstdint>
#include <iostream>


void rebornBuyer(FILE* garbage, Buyer& buyer, int& garbageCount) {
	std::vector<int> deletedItems(garbageCount);
	for (int i = 0; i < garbageCount; ++i) {
		fread(&deletedItems[i], sizeof(int), 1, garbage);
	}
	buyer.id = deletedItems[0];
	

	fclose(garbage);
	garbage = fopen(BUYER_GARBAGE, "wb");
	--garbageCount;
	fwrite(&garbageCount, sizeof(int), 1, garbage);
	for (int i = 1; i <= garbageCount; ++i) {
		fwrite(&deletedItems[i], sizeof(int), 1, garbage);
	}
	fclose(garbage);
}
void printGarbage() {
	FILE* garbage = fopen(BUYER_GARBAGE, "wb");
	int garbageCount = 4;
	std::cout << "ids is the garbage: ";
	for (int i = 0; i < garbageCount; ++i) {
		int temp = 0;
		fread(&temp, sizeof(int), 1, garbage);
		std::cout << temp << " ";
	}
	std::cout << std::endl;
}
void printGarbage_() {
	FILE* garbage = fopen(BUYER_GARBAGE, "rb");
	int garbageCount = 4;
	std::cout << "ids is the garbage: ";
	for (int i = 0; i < garbageCount; ++i) {
		int temp = 0;
		fread(&temp, sizeof(int), 1, garbage);
		std::cout << temp << " ";
	}
	std::cout << std::endl;
}
void addBuyerToGarbage(Buyer& buyer) {
	int garbageCount = 0;
	FILE* garbage = fopen(BUYER_GARBAGE, "rb");
	fread(&garbageCount, sizeof(int), 1, garbage);


	std::vector<int> deletedBuyers(garbageCount);
	for (int i = 0; i < garbageCount; ++i) {
		fread(&deletedBuyers[i], sizeof(int), 1, garbage);
	}
	fclose(garbage);
	garbage = fopen(BUYER_GARBAGE, "wb");
	++garbageCount;
	fwrite(&garbageCount, sizeof(int), 1, garbage);
	for (int i = 0; i < garbageCount - 1; ++i) {
		fwrite(&deletedBuyers[i], sizeof(int), 1, garbage);
	}
	fwrite(&buyer.id, sizeof(int), 1, garbage);
	fclose(garbage);
}
bool getBuyer(Buyer& buyer, int id) {
	FILE* buyer_db = fopen(BUYER_DATA, "rb");
	FILE* buyer_index = fopen(BUYER_INDEX, "rb");
	if (buyer_db == NULL || buyer_index == NULL) {
		return false;
	}
	fseek(buyer_index, 0, SEEK_END);
	int64_t tableSize= ftell(buyer_index);
	if (!tableSize || id * sizeof(Index) > tableSize) {
		fclose(buyer_db);
		fclose(buyer_index);
		return false;
	}
	Index index;
	fseek(buyer_index, (id - 1) * sizeof(Index), SEEK_SET);
	fread(&index, sizeof(Index), 1, buyer_index);

	if (!index.ifExist) {
		fclose(buyer_db);
		fclose(buyer_index);
		return false;
	}

	fseek(buyer_db, index.address, SEEK_SET);
	fread(&buyer, sizeof(Buyer), 1, buyer_db);
	fclose(buyer_db);
	fclose(buyer_index);
	return true;
}

bool deleteBuyer(Buyer& buyer) {
	FILE* buyer_db = fopen(BUYER_DATA, "r+b");
	FILE* buyer_index = fopen(BUYER_INDEX, "r+b");

	if (buyer_index == nullptr) {
		return false;
	}
	fseek(buyer_index, 0, SEEK_END);
	int64_t tableSize = ftell(buyer_index);
	if (!tableSize || buyer.id * sizeof(Index) > tableSize) {
		fclose(buyer_db);
		fclose(buyer_index);
		return false;
	}
	Index index;
	fseek(buyer_index, (buyer.id - 1) * sizeof(Index), SEEK_SET);
	fread(&index, sizeof(Index), 1, buyer_index);

	index.ifExist = false;
	fseek(buyer_index, (buyer.id - 1) * sizeof(Index), SEEK_SET);
	fwrite(&index, sizeof(Index), 1, buyer_index);
	fclose(buyer_index);
	addBuyerToGarbage(buyer);

	if (buyer.itemsCount) {
		FILE* items_db = fopen(ITEM_DATA, "r+b");
		fseek(items_db, buyer.firstItemAddress, SEEK_SET);

		Item item;
		int itemsCount = buyer.itemsCount;
		for (int i = 0; i < itemsCount; ++i) {
			fread(&item, sizeof(Item), 1, items_db);
			fclose(items_db);
			deleteItem(buyer, item);

			items_db = fopen(ITEM_DATA, "r+b");
			fseek(items_db, item.nextAddress, SEEK_SET);
		}
		fclose(items_db);
	}
	return true;
}
bool updateBuyer(Buyer& buyer) {
	FILE* buyer_index = fopen(BUYER_INDEX, "r+b");
	FILE* buyer_db = fopen(BUYER_DATA, "r+b");

	if (buyer_index == nullptr || buyer_db == nullptr) {
		return false;
	}

	Index index;
	int id = buyer.id;
	fseek(buyer_index, 0, SEEK_END);
	int64_t tableSize = ftell(buyer_index);
	if (!tableSize || id * sizeof(Index) > tableSize) {
		fclose(buyer_index);
		fclose(buyer_db);
		return false;
	}
	

	fseek(buyer_index, (id - 1) * sizeof(Index), SEEK_SET);
	fread(&index, sizeof(Index), 1, buyer_index);

	if (!index.ifExist) {
		fclose(buyer_index);
		fclose(buyer_db);
		return false;
	}

	fseek(buyer_db, index.address, SEEK_SET);
	fwrite(&buyer, sizeof(Buyer), 1, buyer_db);
	fclose(buyer_index);
	fclose(buyer_db);

	return true;
}
void insertBuyer(Buyer& buyer) {
	FILE * buyer_index = fopen(BUYER_INDEX, "a+b");
	FILE* buyer_db = fopen(BUYER_DATA, "a+b");
	FILE* garbage = fopen(BUYER_GARBAGE, "a+b");

	int garbageCount = 0;
	fread(&garbageCount, sizeof(int), 1, garbage);
	Index index;
	if (garbageCount) {
		rebornBuyer(garbage, buyer, garbageCount);
		fclose(buyer_index);
		fclose(buyer_db);
		buyer_index = fopen(BUYER_INDEX, "r+b");
		buyer_db = fopen(BUYER_DATA, "r+b");
		fseek(buyer_index, (buyer.id - 1) * sizeof(Index), SEEK_SET);
		fread(&index, sizeof(Index), 1, buyer_index);
		fseek(buyer_db, index.address, SEEK_SET);
	}
	else {
		fseek(buyer_index, 0, SEEK_END);

		if (ftell(buyer_index))
		{
			int temp_size = sizeof(Index);
			fseek(buyer_index, -temp_size, SEEK_END);
			fread(&index, sizeof(Index), 1, buyer_index);

			buyer.id = index.id + 1;
		}
		else
		{
			buyer.id = 1;
		}
	}
	fwrite(&buyer, sizeof(Buyer), 1, buyer_db);

	index.id = buyer.id;
	index.address = (buyer.id - 1) * sizeof(Buyer);
	index.ifExist = true;

	fseek(buyer_index, (buyer.id - 1) * sizeof(Index), SEEK_SET);
	fwrite(&index, sizeof(Index), 1, buyer_index);
	fclose(buyer_index);
	fclose(buyer_db);
}
