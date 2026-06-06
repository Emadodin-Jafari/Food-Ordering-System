#include "MenuDAO.h"
#include "Menu.h"
#include "Restaurant.h"
#include <iostream>
#include "sqlite3.h"

using namespace std;

/////////////////////////////////////////////////////////////////Create Menu////////////////////////////////////////////

MenuDAO::MenuDAO(Database& database) : db(database) {}

bool MenuDAO::addFood(int restaurantId, const Food& food) {
    string sql =
            "INSERT INTO MenuItems (id, restaurant_id, name, price, type, cookingTime, drinkVolume, description, availability) VALUES (" +
            to_string(food.getItemID()) + ", " +
            to_string(restaurantId) + ", '" +
            food.getItemName() + "', " +
            to_string(food.getItemPrice()) + ", 'Food', " +
            to_string(food.getCookingTime()) + ", 0, '" +
            food.getItemDescription() + "', " +
            (food.getItemAvailability() ? "1" : "0") + ");";

    char* errorMessage = nullptr;
    int exit = sqlite3_exec(db.getConnection(), sql.c_str(), nullptr, nullptr, &errorMessage);

    if (exit != SQLITE_OK) {
        std::cerr << "addFood error: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
        return false;
    }
    return true;
}

bool MenuDAO::addDrink(int restaurantId, const Drink& drink) {
    string sql =
            "INSERT INTO MenuItems (id, restaurant_id, name, price, type, cookingTime, drinkVolume, description, availability) VALUES (" +
            to_string(drink.getItemID()) + ", " +
            to_string(restaurantId) + ", '" +
            drink.getItemName() + "', " +
            to_string(drink.getItemPrice()) + ", 'Drink', 0, " +
            to_string(drink.getDrinkVolume()) + ", '" +
            drink.getItemDescription() + "', " +
            (drink.getItemAvailability() ? "1" : "0") + ");";

    char* errorMessage = nullptr;
    int exit = sqlite3_exec(db.getConnection(), sql.c_str(), nullptr, nullptr, &errorMessage);

    if (exit != SQLITE_OK) {
        std::cerr << "addDrink error: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
        return false;
    }
    return true;
}
///////////////////////////////////////////////////////////////////Read Menu////////////////////////////////////////////

int menuReadCallback(void* data, int argc, char** argv, char** azColName) {
    auto* menuList = static_cast<vector<menuItems*>*>(data);
    string type = argv[4] ? argv[4] : "";
    menuItems* item = nullptr;

    if (type == "Food") {
        Food* f = new Food();
        if (argv[5]) {
            f->setCookingTime(stoi(argv[5]));
        }
        item = f;
    }
    else if (type == "Drink") {
        Drink* d = new Drink();
        if (argv[6]) {
            d->setDrinkVolume(stoi(argv[6]));
        }
        item = d;
    }

    if (item != nullptr) {
        if (argv[0]) item->setItemID(stoi(argv[0]));
        if (argv[2]) item->setItemName(argv[2]);
        if (argv[3]) item->setItemPrice(stod(argv[3]));

        if (argv[7]) item->setItemDescription(argv[7]);

        if (argv[8]) {
            item->setItemAvailability(stoi(argv[8]) == 1);
        } else {
            item->setItemAvailability(true);
        }        menuList->push_back(item);
    }

    return 0;
}

vector<menuItems*> MenuDAO::getMenuByRestaurantId(int restaurantId) {
    vector<menuItems*> restaurantMenu;
    string sql = "SELECT id, restaurant_id, name, price, type, cookingTime, drinkVolume, description , availability FROM MenuItems WHERE restaurant_id = " + to_string(restaurantId) + ";";
    char* errorMessage = nullptr;

    sqlite3_exec(db.getConnection(), sql.c_str(), menuReadCallback, &restaurantMenu, &errorMessage);

    if (errorMessage) {
        cerr << "getMenuByRestaurantId Error: " << errorMessage << endl;
        sqlite3_free(errorMessage);
    }

    return restaurantMenu;
}

menuItems* MenuDAO::getMenuItemById(int itemId) {
    vector<menuItems*> tempContainer;
    string sql = "SELECT id, restaurant_id, name, price, type, cookingTime, drinkVolume, description , availability FROM MenuItems WHERE id = " + to_string(itemId) + ";";
    char* errorMessage = nullptr;

    sqlite3_exec(db.getConnection(), sql.c_str(), menuReadCallback, &tempContainer, &errorMessage);

    if (errorMessage) {
        cerr << "getMenuItemById Error: " << errorMessage << endl;
        sqlite3_free(errorMessage);
        return nullptr;
    }

    if (!tempContainer.empty()) {
        return tempContainer[0];
    }

    return nullptr;
}


///////////////////////////////////////////////////////////////////Update Menu//////////////////////////////////////////

bool MenuDAO::updateFood(int itemId, const Food& food) {
    string sql =
            "UPDATE MenuItems SET name = '" + food.getItemName() +
            "', price = " + to_string(food.getItemPrice()) +
            ", cookingTime = " + to_string(food.getCookingTime()) +
            ", description = '" + food.getItemDescription() + "', " +
            "drinkVolume = NULL, type = 'Food', " +
            "availability = " + (food.getItemAvailability() ? "1" : "0") + " " +
            "WHERE id = " + to_string(itemId) + ";";

    char* errorMessage = nullptr;
    int exit = sqlite3_exec(db.getConnection(), sql.c_str(), nullptr, nullptr, &errorMessage);

    if (exit != SQLITE_OK) {
        std::cerr << "updateFood error: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
        return false;
    }
    return true;
}

bool MenuDAO::updateDrink(int itemId, const Drink& drink) {
    string sql =
            "UPDATE MenuItems SET name = '" + drink.getItemName() +
            "', price = " + to_string(drink.getItemPrice()) +
            ", drinkVolume = " + to_string(drink.getDrinkVolume()) +
            ", description = '" + drink.getItemDescription() + "', " +
            "cookingTime = NULL, type = 'Drink', " +
            "availability = " + (drink.getItemAvailability() ? "1" : "0") + " " +
            "WHERE id = " + to_string(itemId) + ";";

    char* errorMessage = nullptr;
    int exit = sqlite3_exec(db.getConnection(), sql.c_str(), nullptr, nullptr, &errorMessage);

    if (exit != SQLITE_OK) {
        std::cerr << "updateDrink error: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
        return false;
    }
    return true;
}


///////////////////////////////////////////////////Delete Menu//////////////////////////////////////////////////////////

bool MenuDAO::deleteMenuItem(int itemId) {
    string sql = "DELETE FROM MenuItems WHERE id = " + std::to_string(itemId) + ";";

    char* errorMessage = nullptr;
    int exit = sqlite3_exec(db.getConnection(), sql.c_str(), nullptr, nullptr, &errorMessage);

    if (exit != SQLITE_OK) {
        std::cerr << "deleteMenuItem error: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
        return false;
    }
    return true;
}


bool MenuDAO::deleteAllMenuItemsOfRestaurant(int restaurantId) {
    string sql = "DELETE FROM MenuItems WHERE restaurant_id = " + to_string(restaurantId) + ";";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(db.getConnection(), sql.c_str(), nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "deleteAllMenuItemsOfRestaurant error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}