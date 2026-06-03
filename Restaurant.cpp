#include "Restaurant.h"
#include <string>
#include <iostream>
#include <vector>

using namespace std;

Restaurant::Restaurant() {
    ID = -1;
    name = "No name";
    address = "No address";
    inActive = false;
    orderPreparationTime = -1;
    phoneNumber = "-1";
    description = "No description";
}

Restaurant::~Restaurant() {
    for (auto item : restaurantMenu) {
        delete item;
    }
    restaurantMenu.clear();
}
/////////////////////////////////Setter Functions//////////////////////////////////


void Restaurant::setID(int Sid) {
    if(Sid > 0 ) ID = Sid;
    else {
        cout << "Invalid ID" << endl;
    }
}

void Restaurant::setName(const string& Sname) {
    name = Sname;
}

void Restaurant::setAddress(const string& Saddress) {
    address = Saddress;
}

void Restaurant::setInActive(bool SinAcvtive) {
    inActive = SinAcvtive;
}

void Restaurant::setOrderPreparationTime(int SorderPreparationTime) {
    if(SorderPreparationTime > 0)
    orderPreparationTime = SorderPreparationTime;

    else cout << "Invalid Preparation Time" << endl;
}

void Restaurant::setPhoneNumber(const string& SphoneNumber) {
    phoneNumber = SphoneNumber;
}

void Restaurant::setDescription(const string& Sdescription) {
    description = Sdescription;
}

void Restaurant::setMenu(const vector<menuItems*>& menu) {
    this->restaurantMenu = menu;
}

///////////////////////////////////Getter Functions///////////////////////////////////
int Restaurant::getID() const {
    return ID;
}

string Restaurant::getName() const {
    return name;
}

string Restaurant::getAddress() const {
    return address;
}

bool Restaurant::getInActive() const {
    return inActive;
}

int Restaurant::getOrderPreparationTime() const {
    return orderPreparationTime;
}

string Restaurant::getPhoneNumber() const {
    return phoneNumber;
}

string Restaurant::getDescription() const {
    return description;
}

vector<menuItems*> Restaurant::getMenu() const {
    return this->restaurantMenu;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Restaurant::printMyMenu() const {
    cout << "Menu name: " << name << endl;

    if (restaurantMenu.empty()) {
        cout << "This restaurant has no items in its menu!" << endl;
        return;
    }

    for (const auto& item : restaurantMenu) {
        cout << "ID: " << item->getItemID()
        << " | Name: " << item->getItemName()
        << " | Description : " << item->getItemDescription()
        << " | Exist : " << item->getItemAvailability()
        << " | Price: " << item->getItemPrice() << endl;
    }
}
