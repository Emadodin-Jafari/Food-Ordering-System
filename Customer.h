#ifndef CUSTOMER_H
#define CUSTOMER_H
#include "MembershipLevel.h"
#include <string>

using namespace std;


class Customer{

public:
    Customer();
    Customer(const Customer &other);
    ~Customer();

    void setCustomerID(const int ScustomerID);
    void setCustomerName(const string &ScustomerName);
    void setCustomerPhoneNumber(const string &ScustomerPhoneNumber);
    void setCustomerPoints(const int ScustomerPoints);

    int getCustomerID() const;
    string getCustomerName() const;
    string getCustomerPhoneNumber() const;
    int getCustomerPoints() const;
    string getCustomerLevel() const;

    void addCustomerPoints(int pointsToAdd);
    void updateMembership();
    double applyDiscount(double price) const;

private:
    int customerID;
    string customerName;
    string customerPhoneNumber;
    int customerPoints;
    MembershipLevel* memberLevel;
};



#endif