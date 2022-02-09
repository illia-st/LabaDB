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
	int64_t firstItemAddress = -1;
	~Buyer() {
		name.clear();
	}
};
struct Index {
	int id;
	int64_t address;
	bool ifExist;
};
bool getBuyer(Buyer& buyer, int id);
bool deleteBuyer(Buyer& buyer);
bool updateBuyer(Buyer& buyer);
void insertBuyer(Buyer& buyer);
void printGarbage();
void printGarbage_();