#ifndef RESTAURANT_DAO_H
#define RESTAURANT_DAO_H

#include "Database.h"
#include "Restaurant.h"
#include "MenuDAO.h"
#include <vector>
#include <string>

using namespace std;

class RestaurantDAO {
private:
    Database& db;

public:
    RestaurantDAO(Database& database);

    bool addRestaurant(const Restaurant& restaurant);

    vector<Restaurant> getAllRestaurants();
    Restaurant* getRestaurantById(int restaurantId);

    bool updateRestaurant(int restaurantId, const Restaurant& restaurant);
    bool updateStatus(int restaurantId, bool isInactive);

    bool deleteRestaurant(int restaurantId);
};

#endif