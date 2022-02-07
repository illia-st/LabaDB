#include <iostream>

#include "Buyer.h"
#include "Item.h"
#include <map>



void allInfo();
bool checkKeyPairUniqueness(Buyer& buyer, int itemId);
void menu(int& request);
int sbSize();
Buyer buyer_info;

int main()
{
    Buyer buyer;
    Item item;
    while (true) {
        std::string temp = "";
        int request = 0;
        int id = 0, item_id = 0, price = 0;
        menu(request);
        switch (request) {
        case 0:
            
            std::cout << "Enter the buyer's id: ";
            std::cin >> id;
            if (getBuyer(buyer, id)) {
                std::cout << "Correct" << std::endl;
            }
            break;
        case 1:
            
            std::cout << "Enter the buyer's id: ";
            std::cin >> id;

            if (getBuyer(buyer, id)) {
                int item_id = 0;
                std::cout << "Enter the item's id: ";
                std::cin >> item_id;

                if (getItem(buyer, item, item_id)) {
                    std::cout << "Correct" << std::endl;
                }
                else {
                    std::cout << "No item" << std::endl;
                }
            }
            else {
                std::cout << "No buyer" << std::endl;
            }
            break;
        case 2:
            
            std::cout << "Enter the buyer's id: ";
            std::cin >> id;
            if (getBuyer(buyer, id)) {
                if (deleteBuyer(buyer)) {
                    std::cout << "Correct" << std::endl;
                    break;
                }
                std::cout << "Pizda" << std::endl;
            }
            else {
                std::cout << "No buyer" << std::endl;
            }
            break;
        case 3:
            
            std::cout << "Enter the buyer's id: ";
            std::cin >> id;
            if (getBuyer(buyer, id)) {
                int item_id = 0;
                std::cout << "Enter the item's id: ";
                std::cin >> item_id;
                if (getItem(buyer, item, item_id)) {
                    deleteItem(buyer, item);
                    std::cout << "Correct" << std::endl;
                    break;
                }
                std::cout << "No item" << std::endl;
            }
            else {
                std::cout << "No buyer" << std::endl;
            }
            break;
        case 4:
            std::cout << "Enter the buyer's id: ";
            std::cin >> id;
            if (getBuyer(buyer, id)) {
                std::cout << "Enter a new buyer's name: ";
                std::cin >> temp;
                buyer.name = temp;
                if (updateBuyer(buyer)) {
                    std::cout << "Correct" << std::endl;
                    break;
                }
                std::cout << "Pizda" << std::endl;
            }
            else {
                std::cout << "No buyer" << std::endl;
            }
            break;
        case 5:
            std::cout << "Enter the buyer's id: ";
            std::cin >> id;
            if (getBuyer(buyer, id)) {
                std::cout << "Enter the item's id: ";
                std::cin >> item_id;
                
                if (getItem(buyer, item, item_id)) {
                    std::cout << "Enter a new item's price: ";
                    std::cin >> price;
                    item.price = price;
                    updateItem(item);

                    
                    break;
                }
                std::cout << "Pizda" << std::endl;
            }
            else {
                std::cout << "No buyer" << std::endl;
            }
            break;
        case 6:
            std::cout << "Enter the buyer's name: ";
            std::cin >> temp;
            buyer.name = temp;

            insertBuyer(buyer);
            std::cout << buyer.id << std::endl;
            break;
        case 7:
            
            std::cout << "Enter the buyer's id: ";
            std::cin >> id;
            if (getBuyer(buyer, id)) {
                int price = 0;
                std::cout << "Enter the item's price: ";
                std::cin >> price;
                std::cout << "Enter the item's id: ";
                std::cin >> item_id;
                item.buyerId = id;
                item.price = price;
                if (checkKeyPairUniqueness(buyer, item_id)) {
                    item.itemId = item_id;
                    insertItem(buyer, item);
                    std::cout << item.itemId << std::endl;
                }
            }
            break;
        case 8:
            allInfo();
            break;
        case 9:
            return 0;
        }

    }
}
bool checkKeyPairUniqueness(Buyer& buyer,int itemId)
{
    FILE* item_db = fopen(ITEM_DATA, "r+b");
    if (item_db == nullptr) {
        return true;
    }
    Item item;

    fseek(item_db, buyer.firstItemAddress, SEEK_SET);

    for (int i = 0; i < buyer.itemsCount; i++)
    {
        fread(&item, sizeof(Item), 1, item_db);
        fclose(item_db);

        if (item.itemId == itemId)
        {
            return false;
        }

        item_db = fopen(ITEM_DATA, "r+b");
        fseek(item_db, item.nextAddress, SEEK_SET);
    }

    fclose(item_db);

    return true;
}
void menu(int& request) {
    std::cout << "Choose one of the commands:\n" <<
        "0 - Get buyer,\n" <<
        "1 - Get item,\n" <<
        "2 - Delete buyer,\n" <<
        "3 - Delete item,\n" <<
        "4 - Update buyer,\n" <<
        "5 - Update item,\n" <<
        "6 - Insert buyer,\n" <<
        "7 - Insert item,\n" <<
        "8 - Get info about DB,\n" <<
        "9 - Exit" << std::endl;
    std::cin >> request;
}
int sbSize() {
    FILE* buyer_index = fopen(BUYER_INDEX, "rb");
    if (buyer_index == nullptr) {
        std::cout << "There are no buyers yet in the DB" << std::endl;
        return 0;
    }
    fseek(buyer_index, 0, SEEK_END);
    int64_t tableSize = ftell(buyer_index);
    if (!tableSize) {
        std::cout << "There are no buyers yet in the DB" << std::endl;
        return 0;
    }
    fclose(buyer_index);
    return tableSize / sizeof(Index);
}
void allInfo() {

    std::map<int, int> info;

    int indAmount = sbSize();
   
    for (int i = 1; i <= indAmount; ++i) {
        if (getBuyer(buyer_info, i)) {
            info[i] = buyer_info.itemsCount;
        }
    }
    for (const auto& [key, value] : info) {
        std::cout << "Buyer with ID " << key << " has " << value << " items." << std::endl;
    }
    
}