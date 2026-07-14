#ifndef ORDER_DAO_H
#define ORDER_DAO_H

#include "Database.h"
#include "Order.h"
#include <vector>

class OrderDAO {
private:
    Database& database;
    bool loadOrderItems(Order* order);

public:
    OrderDAO(Database& db);

    bool addOrder(Order& order);
    bool cancelOrder(Order& order);

    std::vector<Order*> getAllOrders();

    bool updateOrderCondition(int orderId, const std::string& newCondition);
    bool adminUpdateCustomerPoints(Customer* customer, int newPoints);
    void printInvoice(const Order& order);
    vector<Order*> getCustomerOrders(int customerId);
    vector<Order*> getRestaurantOrders(int restaurantId);
    Order* getOrderById(int orderId);

    bool logLevelChange(sqlite3* db, int customerId, const string& oldLevel, const string& newLevel);
    void printLevelHistory(int customerId);
    void printCustomerCountPerLevel();

    void checkAndAllocateMonthlyCoupons(int customerId, const string& level);
    int getActiveCouponsCount(int customerId);
    void printAllCustomersWithDetails();
    int getCustomerCount();
    };

#endif