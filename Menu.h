#ifndef MENU_H
#define MENU_H
#include <vector>
#include <string>

using namespace std;



class menuItems{


public:
    menuItems();
    virtual ~menuItems() {};

    void setItemID(int SitemID);
    void setItemName(string SitemName);
    void setItemDescription(string SitemDescription);
    void setItemPrice(double SitemPrice);
    void setItemAvailability(bool SitemAvailability);


    int getItemID() const;
    string getItemName() const;
    string getItemDescription() const;
    double getItemPrice() const;
    bool getItemAvailability() const;

    virtual void printMenuItem() const = 0;


protected:
    int itemID;
    string itemName;
    string itemDescription;
    double itemPrice;
    bool itemAvailability;
};
////////////////////////////////////////////////////////////

class Food : public menuItems{

public:
    Food();

    void setCookingTime(int ScookingTime);
    int getCookingTime() const;

    void printMenuItem() const override;
private:
    int cookingTime;
};

class Drink : public menuItems{

public:
    Drink();

    void setDrinkVolume(int SdrinkVolume);
    int getDrinkVolume() const;

    void printMenuItem() const override;

private:
    int drinkVolume;
};

////////////////////////////////////////////////////////////////

class Menu{

public:
    Menu();
    ~Menu();
    void addMenuItem(menuItems* item);
    void printMenu() const;

private:
    vector<menuItems*> MenuItems;
};


#endif