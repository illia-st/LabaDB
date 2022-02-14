#pragma once
#include <string>
#include <cstdint>
#define BUYER_DATA "Buyer.fl"
#define BUYER_INDEX "Buyer.ind"
#define BUYER_GARBAGE "BuyerGarbage.txt"

struct Buyer
{
	int id;
	int itemsCount = 0;
	std::string name;
	int32_t firstItemAddress = -1;
	
};
struct Index {
	int id;
	int32_t address;
	bool ifExist;
};
bool getBuyer(Buyer& buyer, int id);
bool deleteBuyer(Buyer& buyer);
bool updateBuyer(Buyer& buyer);
void insertBuyer(Buyer& buyer);
