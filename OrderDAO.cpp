#include "OrderDAO.h"
#include "sqlite3.h"
#include <iostream>

using namespace std;

OrderDAO::OrderDAO(Database& db) : database(db) {}

bool OrderDAO::addOrder(Order& order) {

    const vector<OrderItem*>& items = order.getOrderItems();



    sqlite3* db = database.getConnection();
    if (!db) return false;


    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

    string insertOrderSQL = "INSERT INTO Orders (TotalPrice, Condition, CustomerID) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, insertOrderSQL.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        return false;
    }

    sqlite3_bind_double(stmt, 1, order.getTotalPrice());

    string conditionStr = order.getOrderCondition();
    sqlite3_bind_text(stmt, 2, conditionStr.c_str(), -1, (sqlite3_destructor_type)-1);

    if (order.getCustomer() != nullptr) {
        sqlite3_bind_int(stmt, 3, order.getCustomer()->getCustomerID());
    } else {
        sqlite3_bind_null(stmt, 3);
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        return false;
    }
    sqlite3_finalize(stmt);

    int generatedOrderId = (int)sqlite3_last_insert_rowid(db);
    order.setOrderID(generatedOrderId);

    string insertItemsSQL = "INSERT INTO Order_Items (OrderID, ItemID, Quantity) VALUES (?, ?, ?);";

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

    if (order.getCustomer() != nullptr) {
        Customer* customer = order.getCustomer();

        double multiplier = 1.0;
        string levelNameBefore = customer->getCustomerLevel();

        if (levelNameBefore == "VIP") multiplier = 2.0;
        else if (levelNameBefore == "Gold") multiplier = 1.5;
        else if (levelNameBefore == "Silver") multiplier = 1.2;
        else multiplier = 1.0;

        int basePoints = static_cast<int>(order.getTotalPrice() / 10.0);
        int earnedPoints = static_cast<int>(basePoints * multiplier);

        if (earnedPoints > 0) {
            customer->addCustomerPoints(earnedPoints);
            cout << earnedPoints << "points were earned from this order with a point multiplier of " << multiplier << endl;

            customer->updateMembership();
            string levelNameAfter = customer->getCustomerLevel();

            if (levelNameBefore != levelNameAfter) {
                cout << "Your level upgraded to : " << levelNameAfter << endl;
            }

            string updateCustomerSQL = "UPDATE Customers SET points = ?, current_level = ? WHERE CustomerID = ?;";
            sqlite3_stmt* custStmt;
            if (sqlite3_prepare_v2(db, updateCustomerSQL.c_str(), -1, &custStmt, nullptr) == SQLITE_OK) {
                sqlite3_bind_int(custStmt, 1, customer->getCustomerPoints());
                sqlite3_bind_text(custStmt, 2, levelNameAfter.c_str(), -1, (sqlite3_destructor_type)-1);
                sqlite3_bind_int(custStmt, 3, customer->getCustomerID());
                sqlite3_step(custStmt);
                sqlite3_finalize(custStmt);
            }
        }
    }

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

    sqlite3_bind_text(stmt, 1, newCondition.c_str(), -1, (sqlite3_destructor_type)-1);
    sqlite3_bind_int(stmt, 2, orderId);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

bool OrderDAO::cancelOrder(Order& order) {
    sqlite3* db = database.getConnection();
    if (!db) return false;

    if (!updateOrderCondition(order.getOrderID(), "Canceled")) {
        return false;
    }
    order.setOrderCondition("Canceled");

    if (order.getCustomer() != nullptr) {
        Customer* customer = order.getCustomer();

        double multiplier = 1.0;
        string levelNameBefore = customer->getCustomerLevel();

        if (levelNameBefore == "VIP") multiplier = 2.0;
        else if (levelNameBefore == "Gold") multiplier = 1.5;
        else if (levelNameBefore == "Silver") multiplier = 1.2;
        else multiplier = 1.0;

        int basePoints = static_cast<int>(order.getTotalPrice() / 10.0);
        int lostPoints = static_cast<int>(basePoints * multiplier);

        if (lostPoints > 0) {
            customer->addCustomerPoints(-lostPoints);

            if (customer->getCustomerPoints() < 0) {
                customer->setCustomerPoints(0);
            }

            cout << "The order was cancelled. " << lostPoints << "points were deducted from your account." << endl;

            customer->updateMembership();
            string levelNameAfter = customer->getCustomerLevel();

            if (levelNameBefore != levelNameAfter) {
                cout << "Your level has dropped due to a decrease in points to :" << levelNameAfter <<endl;
            }

            string updateCustomerSQL = "UPDATE Customers SET points = ?, current_level = ? WHERE CustomerID = ?;";
            sqlite3_stmt* custStmt;
            if (sqlite3_prepare_v2(db, updateCustomerSQL.c_str(), -1, &custStmt, nullptr) == SQLITE_OK) {
                sqlite3_bind_int(custStmt, 1, customer->getCustomerPoints());
                sqlite3_bind_text(custStmt, 2, levelNameAfter.c_str(), -1, (sqlite3_destructor_type)-1);
                sqlite3_bind_int(custStmt, 3, customer->getCustomerID());
                sqlite3_step(custStmt);
                sqlite3_finalize(custStmt);
            }
        }
    }
    return true;
}

bool OrderDAO::loadOrderItems(Order* order) {
    if (order == nullptr) return false;

    sqlite3* db = database.getConnection();
    if (!db) return false;

    string sql = "SELECT ItemID, Quantity FROM Order_Items WHERE OrderID = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_int(stmt, 1, order->getOrderID());

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int itemId = sqlite3_column_int(stmt, 0);
        int quantity = sqlite3_column_int(stmt, 1);

        ConcreteMenuItem* tempItem = new ConcreteMenuItem();
        tempItem->setItemID(itemId);
        order->addItem(tempItem, quantity);
    }

    sqlite3_finalize(stmt);
    return true;
}

vector<Order*> OrderDAO::getAllOrders() {
    vector<Order*> orderList;
    sqlite3* db = database.getConnection();
    if (!db) return orderList;

    string sql = "SELECT OrderID, TotalPrice, Condition, CustomerID FROM Orders;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return orderList;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Order* order = new Order();
        order->setOrderID(sqlite3_column_int(stmt, 0));

        double savedTotalPrice = sqlite3_column_double(stmt, 1);

        const char* conditionText = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        order->setOrderCondition(conditionText ? conditionText : "No Condition");

        int customerId = -1;
        if (sqlite3_column_type(stmt, 3) != SQLITE_NULL) {
            customerId = sqlite3_column_int(stmt, 3);
        }

        loadOrderItems(order);
        order->setTotalPrice(savedTotalPrice);

        orderList.push_back(order);
    }

    sqlite3_finalize(stmt);
    return orderList;
}




bool OrderDAO::adminUpdateCustomerPoints(Customer* customer, int newPoints) {
    if (customer == nullptr) return false;

    sqlite3* db = database.getConnection();
    if (!db) return false;

    string levelNameBefore = customer->getCustomerLevel();

    customer->setCustomerPoints(newPoints);

    string levelNameAfter = customer->getCustomerLevel();

    cout << "Customer points successfully updated to: " << newPoints << endl;
    if (levelNameBefore != levelNameAfter) {
        cout << "Membership level changed from [" << levelNameBefore << "] to [" << levelNameAfter << "]." << endl;
    }

    string updateSQL = "UPDATE Customers SET points = ?, current_level = ? WHERE CustomerID = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, updateSQL.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_int(stmt, 1, customer->getCustomerPoints());
    sqlite3_bind_text(stmt, 2, levelNameAfter.c_str(), -1, (sqlite3_destructor_type)-1);
    sqlite3_bind_int(stmt, 3, customer->getCustomerID());

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);

    return success;
}


void OrderDAO::printInvoice(const Order& order) {

    string levelName = "Guest";
    double rawPrice = 0.0;

    const vector<OrderItem*>& items = order.getOrderItems();
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr) {
            rawPrice += items[i]->getItemTotalPrice();
        }
    }

    if (order.getCustomer() != nullptr) {
        Customer* customer = order.getCustomer();
        levelName = customer->getCustomerLevel();
        cout << "Customer Name: " << customer->getCustomerName() << endl;
        cout << "Current Level: " << levelName << endl;
    } else {
        cout << "Customer: Guest" << endl;
    }

    cout << "----------------------------------------" << endl;
    cout << "Items List:" << endl;

    for (int i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr && items[i]->getItem() != nullptr) {
            string itemName = items[i]->getItem()->getItemName();
            double itemPrice = items[i]->getItem()->getItemPrice();
            int quantity = items[i]->getOrderNumber();
            double totalItemPrice = items[i]->getItemTotalPrice();

            cout << "- " << itemName << " x" << quantity
                 << " ($" << itemPrice << " each) -> $" << totalItemPrice << endl;
        }
    }

    cout << "----------------------------------------" << endl;
    cout << "Subtotal (Base Price): $" << rawPrice << endl;

    if (levelName != "Guest" && levelName != "Normal") {
        double discountAmount = rawPrice - order.getTotalPrice();
        cout << "Membership Discount (" << levelName << "): -$" << discountAmount << endl;
    }

    cout << "Total Amount to Pay: $" << order.getTotalPrice() << endl;
    cout << "=========================================" << endl;
}