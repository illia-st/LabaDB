#pragma once
#include "Structures.h"
#define BUYER_DATA "Buyer.fl"
#define BUYER_INDEX "Buyer.ind"
#define BUYER_GARBAGE "Buyer_garbage.txt"


bool getBuyer(Buyer& buyer, int id);
bool deleteBuyer(Buyer& buyer);
bool updateBuyer(Buyer& buyer);
void insertBuyer(Buyer& buyer);