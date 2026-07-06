#ifndef ORDER_H
#define ORDER_H
#include "Restaurant.h"
#include "Menu.h"
#include "Customer.h"
#include <string>
#include <vector>

using namespace std;


class OrderItem{

public:
    OrderItem();

    void setOrderItemID(const int SorderItemID);
    void setItem(menuItems &Sitem);
    void setOrderNumber(const int SorderNumber);

    int getOrderItemID() const;
    menuItems* getItem() const;
    int getOrderNumber() const;

    double getItemTotalPrice() const;

private:
    int orderItemID;
    menuItems* item;
    int orderNumber;
};


class Order{
public:
    Order();
    ~Order();

    void setOrderID(const int SorderID);
    void setTotalPrice(const double StotalPrice);
    void setOrderCondition(const string SorderCondition);
    void setCustomer(Customer* Scustomer);


    int getOrderID() const;
    double getTotalPrice() const;
    string getOrderCondition() const;
    const vector<OrderItem*>& getOrderItems() const;
    Customer* getCustomer() const;


    void updateOrderNumber(int itemId, int newNumber);
    void removeItem(int itemId);
    void addItem(menuItems* menuItem, int number = 1);

    void calculateTotalPrice();


private:
    int orderID;
    double totalPrice;
    string orderCondition;
    vector<OrderItem*> orderItems;
    Customer* customer;
};



#endif