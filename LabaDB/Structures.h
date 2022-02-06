#pragma once
#include <cstdint>
#include <string>

struct Buyer
{
	int id;
	int itemsCount = 0;
	std::string name;
	int64_t firstItemAddress = -1;
};

struct Item {
	int itemId;
	int buyerId;
	int price;
	bool ifExist;
	int64_t address;
	int64_t nextAddress;
};

struct Index {
	int id;
	int64_t address;
	bool ifExist;
};