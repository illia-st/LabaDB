#pragma once
#include "Structures.h"
#define ITEM_DATA "Item.fl"
#define ITEM_GARBAGE "Item_garbage.txt"

bool getItem(Buyer& buyer, Item& item, int item_id);
void updateItem(Item& item);
bool insertItem(Buyer& buyer, Item& item);
bool deleteItem(Buyer& buyer, Item& item);


