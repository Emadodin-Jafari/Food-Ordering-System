#ifndef ORDER_DAO_H
#define ORDER_DAO_H

#include "Database.h"
#include "Order.h"
#include <vector>

class OrderDAO {
private:
    Database& database;

public:
    OrderDAO(Database& db);

    bool addOrder(Order& order);

    std::vector<Order*> getAllOrders();

    bool updateOrderCondition(int orderId, const std::string& newCondition);
};

#endif