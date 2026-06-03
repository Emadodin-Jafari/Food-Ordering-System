#include "OrderDAO.h"
#include "sqlite3.h"
#include <iostream>

using namespace std;

OrderDAO::OrderDAO(Database& db) : database(db) {}

bool OrderDAO::addOrder(Order& order) {
    sqlite3* db = database.getConnection();
    if (!db) return false;

    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

    string insertOrderSQL = "INSERT INTO Orders (TotalPrice, Condition) VALUES (?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, insertOrderSQL.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        return false;
    }

    sqlite3_bind_double(stmt, 1, order.getTotalPrice());
    sqlite3_bind_text(stmt, 2, order.getOrderCondition().c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        return false;
    }
    sqlite3_finalize(stmt);

    int generatedOrderId = (int)sqlite3_last_insert_rowid(db);
    order.setOrderID(generatedOrderId);

    string insertItemsSQL = "INSERT INTO Order_Items (OrderID, ItemID, Quantity) VALUES (?, ?, ?);";

    const vector<OrderItem*>& items = order.getOrderItems();

    for (int i = 0; i < items.size(); ++i) {
        if (sqlite3_prepare_v2(db, insertItemsSQL.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            return false;
        }

        sqlite3_bind_int(stmt, 1, generatedOrderId);
        sqlite3_bind_int(stmt, 2, items[i]->getOrderItemID());
        sqlite3_bind_int(stmt, 3, items[i]->getOrderNumber());

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            return false;
        }
        sqlite3_finalize(stmt);
    }

    sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
    return true;
}

bool OrderDAO::updateOrderCondition(int orderId, const string& newCondition) {
    sqlite3* db = database.getConnection();
    if (!db) return false;

    string sql = "UPDATE Orders SET Condition = ? WHERE OrderID = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, newCondition.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, orderId);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

vector<Order*> OrderDAO::getAllOrders() {
    vector<Order*> orderList;
    sqlite3* db = database.getConnection();
    if (!db) return orderList;

    string sql = "SELECT * FROM Orders;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return orderList;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Order* order = new Order();
        order->setOrderID(sqlite3_column_int(stmt, 0));
        order->setTotalPrice(sqlite3_column_double(stmt, 1));
        order->setOrderCondition(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));


        orderList.push_back(order);
    }

    sqlite3_finalize(stmt);
    return orderList;
}