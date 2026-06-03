#include "Order.h"
#include <iostream>

using namespace std;


OrderItem::OrderItem() {
    orderItemID = -1;
    item = nullptr;
    orderNumber = -1;
}


void OrderItem::setOrderItemID(const int SorderItemID) {
    orderItemID = SorderItemID;
}

void OrderItem::setItem(menuItems &Sitem) {
    item = &Sitem;
}

void OrderItem::setOrderNumber(const int SorderNumeb) {
    orderNumber = SorderNumeb;
}


int OrderItem::getOrderItemID() const {
    return orderItemID;
}

menuItems *OrderItem::getItem() const {
    return item;
}

int OrderItem::getOrderNumber() const {
    return orderNumber;
}

double OrderItem::getItemTotalPrice() const {
    return (item->getItemPrice() * orderNumber);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


Order::Order() {
    orderID = -1;
    totalPrice = -1;
    orderCondition = "No Condition";
}

Order::~Order() {
    for (int i = 0; i < orderItems.size(); ++i) {
        delete orderItems[i];
    }
    orderItems.clear();
}

void Order::setOrderID(const int SorderID) {
    orderID = SorderID;
}

void Order::setTotalPrice(const double StotalPrice) {
    totalPrice = StotalPrice;
}

void Order::setOrderCondition(const std::string SorderCondition) {
    orderCondition = SorderCondition;
}


int Order::getOrderID() const {
    return orderID;
}


double Order::getTotalPrice() const {
    return totalPrice;
}

string Order::getOrderCondition() const {
    return orderCondition;
}

const vector<OrderItem*>& Order::getOrderItems() const {
    return orderItems;
}


void Order::removeItem(int itemId) {
    bool findOrderItem = false;
    for (int i = 0; i < orderItems.size(); ++i) {
        if (orderItems[i]->getOrderItemID() == itemId){
            delete orderItems[i];
            orderItems.erase(orderItems.begin() + i);
            findOrderItem = true;
            break;
        }
    }

    if(!findOrderItem){
        cout << "There is no item with ID : " << itemId << endl;
    }

    calculateTotalPrice();
}

void Order::updateOrderNumber(int itemId, int newNumber) {
    bool findOrderItem = false;
    if (newNumber <= 0){
        removeItem(itemId);
        return;
    }

    for (int i = 0; i < orderItems.size(); ++i) {
        if (orderItems[i]->getOrderItemID() == itemId){
            orderItems[i]->setOrderNumber(newNumber);
            findOrderItem = true;
            break;
        }
    }

    if(!findOrderItem){
        cout << "There is no item with ID : " << itemId << endl;
    }

    calculateTotalPrice();

}

void Order::addItem(menuItems *menuItem, int number) {
    if(number <= 0){
        cout << "Invalid number of item!" << endl;
        return;
    }
    else{
        for (int i = 0; i < orderItems.size(); ++i) {
            if(orderItems[i]->getOrderItemID() == menuItem->getItemID()){
                orderItems[i]->setOrderNumber(orderItems[i]->getOrderNumber() + number);
                calculateTotalPrice();
                return;
            }
        }

        OrderItem *newItem = new OrderItem;
        newItem->setOrderItemID(menuItem->getItemID());
        newItem->setItem(*menuItem);
        newItem->setOrderNumber(number);

        orderItems.push_back(newItem);
        calculateTotalPrice();
    }
}

void Order::calculateTotalPrice() {
    totalPrice = 0.0;
    for (int i = 0; i < orderItems.size(); ++i) {
        totalPrice += orderItems[i]->getItemTotalPrice();
    }
}