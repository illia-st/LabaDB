#include <iostream>
#include <fstream>
#include <vector>
#include "Item.h"
#include "Buyer.h"


void rebornItem(FILE * garbage, int& garbageCount, Item& item) {
	std::vector<int32_t> deletedItems(garbageCount);
	for (int i = 0; i < garbageCount; ++i) {
		fread(&deletedItems[i], sizeof(int32_t), 1, garbage);
	}
	item.address = deletedItems[0];
	item.nextAddress = deletedItems[0];

	fclose(garbage);
	garbage = fopen(ITEM_GARBAGE, "wb");
	--garbageCount;
	fwrite(&garbageCount, sizeof(int), 1, garbage);
	for (int i = 1; i <= garbageCount; ++i) {
		fwrite(&deletedItems[i], sizeof(int32_t), 1, garbage);
	}
	fclose(garbage);
}

void addItemToGarbage(Item& item) {
	int garbageCount = 0;
	FILE* garbage = fopen(ITEM_GARBAGE, "rb");
	fread(&garbageCount, sizeof(int), 1, garbage);


	std::vector<int32_t> deletedItems(garbageCount);
	for (int i = 0; i < garbageCount; ++i) {
		fread(&deletedItems[i], sizeof(int32_t), 1, garbage);
	}
	fclose(garbage);
	garbage = fopen(ITEM_GARBAGE, "wb");
	++garbageCount;
	fwrite(&garbageCount, sizeof(int), 1, garbage);
	for (int i = 0; i < garbageCount - 1; ++i) {
		fwrite(&deletedItems[i], sizeof(int32_t), 1, garbage);
	}
	fwrite(&item.address, sizeof(int32_t), 1, garbage);
	fclose(garbage);
}

bool insertItem(Buyer &buyer, Item& item) {
	item.ifExist = true;
	FILE* db = fopen(ITEM_DATA, "a+b");
	FILE* garbage = fopen(ITEM_GARBAGE, "a+b");
	fseek(garbage, 0, SEEK_SET);
	int garbageCount = 0;
	fread(&garbageCount, sizeof(int), 1, garbage);


	if (garbageCount) {
		rebornItem(garbage, garbageCount, item);
		fclose(db);
		db = fopen(ITEM_DATA, "r+b");
		fseek(db, item.address, SEEK_SET);
	}
	else {
		
		fseek(db, 0, SEEK_END);
		int db_size = ftell(db);

		item.address = db_size;
		item.nextAddress = db_size;
	}
	fwrite(&item, sizeof(Item), 1, db);
	if (!buyer.itemsCount) {
		buyer.firstItemAddress = item.address;
	}
	else {
		fclose(db);
		db = fopen(ITEM_DATA, "r+b");
		Item previous;

		fseek(db, buyer.firstItemAddress, SEEK_SET);

		for (int i = 0; i < buyer.itemsCount; i++)
		{
			fread(&previous, sizeof(Item), 1, db);
			fseek(db, previous.nextAddress, SEEK_SET);
		}

		previous.nextAddress = item.address;				
		fwrite(&previous, sizeof(Item), 1, db);
	}
	fclose(db);
	buyer.itemsCount += 1;
	updateBuyer(buyer);

	return true;
}

void updateItem(Item& item) {
	FILE* db = fopen(ITEM_DATA, "r+b");

	fseek(db, item.address, SEEK_SET);
	fwrite(&item, sizeof(Item), 1, db);

	fclose(db);
}

bool getItem(Buyer& buyer, Item& item ,int item_id) {
	if (!buyer.itemsCount) {
		return false;
	}
	FILE* db = fopen(ITEM_DATA, "rb");
	fseek(db, buyer.firstItemAddress, SEEK_SET);
	fread(&item, sizeof(Item), 1, db);

	for (int i = 0; i < buyer.itemsCount; ++i) {
		if (item.itemId == item_id) {
			fclose(db);
			return true;
		}
		fseek(db, item.nextAddress, SEEK_SET);
		fread(&item, sizeof(Item), 1, db);
	}
	fclose(db);
	return false;
}
void makeLinks(Buyer& buyer, Item& prev, Item& item_to_delete, FILE * db) {
	if (item_to_delete.address == buyer.firstItemAddress){

		if (item_to_delete.address == item_to_delete.nextAddress){
			buyer.firstItemAddress = -1;					
		}
		else{
			buyer.firstItemAddress = item_to_delete.nextAddress;
		}
	}
	else{
		if (item_to_delete.address == item_to_delete.nextAddress){
			prev.nextAddress = prev.address;
		}
		else{
			prev.nextAddress = item_to_delete.nextAddress;
		}

		fseek(db, prev.address, SEEK_SET);
		fwrite(&prev, sizeof(Item), 1, db);
	}
}
void deleteItem(Buyer& buyer, Item& item) {
	FILE* db = fopen(ITEM_DATA, "r+b");
	Item prev;
	fseek(db, buyer.firstItemAddress, SEEK_SET);
	fread(&prev, sizeof(Item), 1, db);

	for (int i = 0; i < buyer.itemsCount; ++i) {
		if (prev.address == item.address || prev.nextAddress == item.address) {
			break;
		}
	}
	makeLinks(buyer, prev, item, db);
	
	item.ifExist = 0;
	addItemToGarbage(item);


	fseek(db, item.address, SEEK_SET);
	fwrite(&item, sizeof(Item), 1, db);
	fclose(db);

	buyer.itemsCount -= 1;
	updateBuyer(buyer);
	
}