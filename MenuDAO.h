#ifndef MENU_DAO_H
#define MENU_DAO_H
#include "Database.h"
#include "Menu.h"
#include <vector>
#include <string>

using namespace std;

class MenuDAO {
private:
    Database& db;

public:
    MenuDAO(Database& database);

    bool addFood(int restaurantId, const Food& food);
    bool addDrink(int restaurantId, const Drink& drink);

    vector<menuItems*> getMenuByRestaurantId(int restaurantId);
    menuItems* getMenuItemById(int itemId);

    bool updateFood(int itemId, const Food& food);
    bool updateDrink(int itemId, const Drink& drink);

    bool deleteMenuItem(int itemId);
    bool deleteAllMenuItemsOfRestaurant(int restaurantId);
};


#endif