#include "Menu.h"
#include <string>
#include <iostream>

using namespace std;





Food::Food() {
    cookingTime = -1;
}

void Food::setCookingTime(int ScookingTime) {
    if(ScookingTime > 0)
    cookingTime = ScookingTime;
    else cout << "Invalid cooking time!" << endl;
}

int Food::getCookingTime() const {
    return cookingTime;
}

void Food::printMenuItem() const {
    cout << "ID : " << itemID << " | Name : " << itemName <<
    " | Price : " << itemPrice << " | Availability : " << (itemAvailability ? "Yes" : "No") <<
     " | Item Description : " << itemDescription << " | Cooking Time : " << cookingTime << endl;
}


Drink::Drink() {
    drinkVolume = -1 ;
}

void Drink::setDrinkVolume(int SdrinkVolume) {
    if( SdrinkVolume > 0) drinkVolume = SdrinkVolume;
    else cout << "Invalid Drink Volume!" << endl;
}

int Drink::getDrinkVolume() const {
    return drinkVolume;
}

void Drink::printMenuItem() const {
    cout << "ID : " << itemID << " | Name : " << itemName <<
         " | Price : " << itemPrice << " | Availability : " << (itemAvailability ? "Yes" : "No") <<
         " | Item Description : " << itemDescription <<  " | Drink Volume : " << drinkVolume << endl;
}

void Menu::printMenu() const {
    for (const auto& item : MenuItems) {
        item->printMenuItem();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////


menuItems::menuItems() {
    itemID = -1;
    itemName = "No itemName";
    itemDescription = "No itemDescription";
    itemPrice = -1;
    itemAvailability = false;
}



//////////////////////////////////////Menu Items Setter Functions//////////////////////////

void menuItems::setItemID(int SitemID) {
    if(SitemID > 0)
    itemID = SitemID;

    else cout << "Invalid Item ID" << endl;
}

void menuItems::setItemName(std::string SitemName) {
    itemName = SitemName;
}

void menuItems::setItemDescription(string SitemDescription) {
    itemDescription = SitemDescription;
}

void menuItems::setItemPrice(double SitemPrice) {
    if(SitemPrice > 0) itemPrice = SitemPrice;
    else cout << "Invalid Item Price" << endl;
}

void menuItems::setItemAvailability(bool SitemAvailability) {
    itemAvailability = SitemAvailability;
}



//////////////////////////////////////Menu Items Getter Functions//////////////////////////


int menuItems::getItemID() const {
    return itemID;
}

string menuItems::getItemName() const {
    return itemName;
}

string menuItems::getItemDescription() const {
    return itemDescription;
}

double menuItems::getItemPrice() const {
    return itemPrice;
}

bool menuItems::getItemAvailability() const {
    return itemAvailability;
}



///////////////////////////////////////////////////////////////////////////////////////////////

void Menu::addMenuItem(menuItems* item) {
    MenuItems.push_back(item);
}

Menu::Menu() {}

Menu::~Menu() {
    for (auto item : MenuItems) {
        delete item;
    }
    MenuItems.clear();
}