#include "RestaurantDAO.h"
#include "Restaurant.h"
#include <iostream>
#include "sqlite3.h"

using namespace std;

RestaurantDAO::RestaurantDAO(Database& database) : db(database) {}

int restaurantReadCallback(void* data, int argc, char** argv, char** azColName) {
    auto* list = static_cast<vector<Restaurant>*>(data);

    Restaurant res;
    if (argv[0] != nullptr) res.setID(stoi(argv[0]));
    if (argv[1] != nullptr) res.setName(string(argv[1]));
    if (argv[2] != nullptr) res.setAddress(string(argv[2]));
    if (argv[3] != nullptr) res.setInActive(stoi(argv[3]) != 0);
    if (argv[4] != nullptr) res.setOrderPreparationTime(stoi(argv[4]));
    if (argv[5] != nullptr) res.setPhoneNumber(string(argv[5]));
    if (argv[6] != nullptr) res.setDescription(string(argv[6]));

    list->push_back(res);
    return 0;
}
bool RestaurantDAO::addRestaurant(const Restaurant& restaurant) {
    string sql = "INSERT INTO Restaurants (id, name, address, status, prepTime, phoneNumber, description) VALUES (" +
                 to_string(restaurant.getID()) + ", '" +
                 restaurant.getName() + "', '" +
                 restaurant.getAddress() + "', '" +
                 to_string(restaurant.getInActive()) + "', " +
                 to_string(restaurant.getOrderPreparationTime()) + ", '" +
                 restaurant.getPhoneNumber() + "', '" +
                 restaurant.getDescription() + "');";

    char* errorMessage = nullptr;
    int exit = sqlite3_exec(db.getConnection(), sql.c_str(), nullptr, nullptr, &errorMessage);

    if (exit != SQLITE_OK) {
        cerr << "addRestaurant error: " << errorMessage << endl;
        sqlite3_free(errorMessage);
        return false;
    }
    return true;
}

vector<Restaurant> RestaurantDAO::getAllRestaurants() {
    vector<Restaurant> list;
    string sql = "SELECT id, name, address, status, prepTime, phoneNumber, description FROM Restaurants;";
    char* errorMessage = nullptr;

    sqlite3_exec(db.getConnection(), sql.c_str(), restaurantReadCallback, &list, &errorMessage);

    if (errorMessage) {
        cerr << "getAllRestaurants Error: " << errorMessage << endl;
        sqlite3_free(errorMessage);
    }
    return list;
}

Restaurant* RestaurantDAO::getRestaurantById(int restaurantId) {
    vector<Restaurant> tempContainer;
    string sql = "SELECT id, name, address, status, prepTime, phoneNumber, description FROM Restaurants WHERE id = " + to_string(restaurantId) + ";";
    char* errorMessage = nullptr;

    sqlite3_exec(db.getConnection(), sql.c_str(), restaurantReadCallback, &tempContainer, &errorMessage);

    if (errorMessage) {
        cerr << "getRestaurantById Error: " << errorMessage << endl;
        sqlite3_free(errorMessage);
        return nullptr;
    }

    if (!tempContainer.empty()) {
        Restaurant* res = new Restaurant(tempContainer[0]);

        MenuDAO menuDao(db);
        vector<menuItems*> menuData = menuDao.getMenuByRestaurantId(restaurantId);
        res->setMenu(menuData);

        return res;
    }
    return nullptr;
}

bool RestaurantDAO::updateRestaurant(int restaurantId, const Restaurant& restaurant) {
    string sql = "UPDATE Restaurants SET name = '" + restaurant.getName() +
                 "', address = '" + restaurant.getAddress() +
                 "', status = '" + to_string(restaurant.getInActive()) +
                 "', prepTime = " + to_string(restaurant.getOrderPreparationTime()) +
                 ", phoneNumber = '" + restaurant.getPhoneNumber() +
                 "', description = '" + restaurant.getDescription() +
                 "' WHERE id = " + to_string(restaurantId) + ";";

    char* errorMessage = nullptr;
    int exit = sqlite3_exec(db.getConnection(), sql.c_str(), nullptr, nullptr, &errorMessage);

    if (exit != SQLITE_OK) {
        cerr << "updateRestaurant error: " << errorMessage << endl;
        sqlite3_free(errorMessage);
        return false;
    }
    return true;
}

bool RestaurantDAO::updateStatus(int restaurantId, bool isInactive) {
    string sql = "UPDATE Restaurants SET status = '" + to_string(isInactive) + "' WHERE id = " + to_string(restaurantId) + ";";
    char* errorMessage = nullptr;
    int exit = sqlite3_exec(db.getConnection(), sql.c_str(), nullptr, nullptr, &errorMessage);

    if (exit != SQLITE_OK) {
        cerr << "updateStatus error: " << errorMessage << endl;
        sqlite3_free(errorMessage);
        return false;
    }
    return true;
}

bool RestaurantDAO::deleteRestaurant(int restaurantId) {
    string sql = "DELETE FROM Restaurants WHERE id = " + to_string(restaurantId) + ";";
    char* errorMessage = nullptr;
    int exit = sqlite3_exec(db.getConnection(), sql.c_str(), nullptr, nullptr, &errorMessage);

    if (exit != SQLITE_OK) {
        cerr << "deleteRestaurant error: " << errorMessage << endl;
        sqlite3_free(errorMessage);
        return false;
    }
    return true;
}