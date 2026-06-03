#ifndef RESTAURANT_H
#define RESTAURANT_H
#include <string>
#include "Menu.h"

using namespace std;

class Restaurant{

public:
    Restaurant();
    ~Restaurant();

    void setID(int Sid);
    void setName(const string& Sname);
    void setAddress(const string& Saddress);
    void setInActive(bool SinAcvtive);
    void setOrderPreparationTime(int SorderPreparationTime);
    void setPhoneNumber(const string& SphoneNumber);
    void setDescription(const string& Sdescription);
    void setMenu(const vector<menuItems*>& menu);


    int getID() const;
    string getName() const;
    string getAddress() const;
    bool getInActive() const;
    int getOrderPreparationTime() const;
    string getPhoneNumber() const;
    string getDescription() const;
    vector<menuItems*> getMenu() const;

    void printMyMenu() const;
private:
    int ID;
    string name;
    string address;
    bool inActive;
    int orderPreparationTime;
    string phoneNumber;
    string description;

    vector<menuItems*> restaurantMenu;
};


#endif