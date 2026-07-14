#include "Customer.h"
#include <iostream>


Customer::Customer() {
    customerID = -1;
    customerName = "No Name";
    customerPhoneNumber = "No Phone Number";
    customerPoints = 0;
    memberLevel = nullptr;
    updateMembership();
}

Customer::Customer(const Customer &other) {
    customerID = other.customerID;
    customerName = other.customerName;
    customerPhoneNumber = other.customerPhoneNumber;
    customerPoints = other.customerPoints;
    memberLevel = nullptr;
    updateMembership();
}

Customer::~Customer(){
    delete memberLevel;
}


void Customer::setCustomerID(const int ScustomerID) {
    customerID = ScustomerID;
}

void Customer::setCustomerName(const std::string &ScustomerName) {
    customerName = ScustomerName;
}

void Customer::setCustomerPhoneNumber(const std::string &ScustomerPhoneNumber) {
    customerPhoneNumber = ScustomerPhoneNumber;
}

void Customer::setCustomerPoints(const int ScustomerPoints) {
    if(ScustomerPoints >= 0) {
        customerPoints = ScustomerPoints;
        updateMembership();
    } else {
        cout << "Invalid point!" << endl;
    }
}





int Customer::getCustomerID() const {
    return customerID;
}

string Customer::getCustomerName() const {
    return customerName;
}

string Customer::getCustomerPhoneNumber() const {
    return customerPhoneNumber;
}

int Customer::getCustomerPoints() const {
    return customerPoints;
}

string Customer::getCustomerLevel() const {
    if (memberLevel != nullptr) {
        return memberLevel->getLevelName();
    }
    return "Normal";
}



void Customer::addCustomerPoints(int pointsToAdd) {
    this->customerPoints += pointsToAdd;
    updateMembership();
}


void Customer::updateMembership() {
    if(memberLevel != nullptr) delete memberLevel;

    if(customerPoints >= 700){
        memberLevel = new VIPlevel;
    }
    else if(customerPoints >= 300){
        memberLevel = new GoldLevel;
    }
    else if(customerPoints >= 100){
        memberLevel = new SilverLevel;
    }
    else{
        memberLevel = new NormalLevel;
    }
}

double Customer::applyDiscount(double price) const {
    if (memberLevel != nullptr) {
        return memberLevel->calculateFinalPrice(price);
    }
    return price;
}

double Customer::getPointsMultiplier() const {
    if (memberLevel != nullptr) {
        return memberLevel->getPointsMultiplier();
    }
    return 1.0;
}

double Customer::calculateDeliveryFee(double totalPrice) const {
    if (memberLevel != nullptr) {
        return memberLevel->calculateDeliveryFee(totalPrice);
    }
    return 100.0;
}

int Customer::getMonthlyCouponCount() const {
    if (memberLevel != nullptr) {
        return memberLevel->getMonthlyCouponCount();
    }
    return 0;
}

string Customer::getNextLevelRequirement() const {
    if (memberLevel != nullptr) {
        return memberLevel->getNextLevelRequirement(customerPoints);
    }
    return "";
}