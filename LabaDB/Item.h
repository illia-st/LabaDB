#pragma once
#include <string>
#include <cstdint>
#include "Buyer.h"
#define ITEM_DATA "Item.fl"
#define ITEM_GARBAGE "Item_garbage.txt"

struct Item {
	int itemId;
	int buyerId;
	int price;
	bool ifExist;
	int64_t address;
	int64_t nextAddress;
};

bool getItem(Buyer& buyer, Item& item, int item_id);
void updateItem(Item& item);
bool insertItem(Buyer& buyer, Item& item);
void deleteItem(Buyer& buyer, Item& item);


