#include "OrderDAO.h"
#include "sqlite3.h"
#include <iostream>

using namespace std;

OrderDAO::OrderDAO(Database& db) : database(db) {}

bool OrderDAO::addOrder(Order& order) {
    const vector<OrderItem*>& items = order.getOrderItems();
    sqlite3* db = database.getConnection();
    if (!db) return false;

    if (order.getCustomer() != nullptr) {
        Customer* customer = order.getCustomer();
        double multiplier = 1.0;
        string levelNameBefore = customer->getCustomerLevel();

        if (levelNameBefore == "VIP") multiplier = 2.0;
        else if (levelNameBefore == "Gold") multiplier = 1.5;
        else if (levelNameBefore == "Silver") multiplier = 1.2;
        else multiplier = 1.0;

        double priceBeforeDiscount = 0.0;
        for (size_t i = 0; i < items.size(); ++i) {
            if (items[i] != nullptr && items[i]->getItem() != nullptr) {
                priceBeforeDiscount += (items[i]->getItem()->getItemPrice() * items[i]->getOrderNumber());
            }
        }

        int basePoints = static_cast<int>(priceBeforeDiscount / 10.0);
        int earnedPoints = static_cast<int>(basePoints * multiplier);

        order.setOrderPoints(earnedPoints);
        order.setFinalPaidPrice(order.getTotalPrice());

        if (earnedPoints > 0) {
            customer->addCustomerPoints(earnedPoints);
            cout << earnedPoints << " points were earned from this order!" << endl;

            customer->updateMembership();
            string levelNameAfter = customer->getCustomerLevel();

            if (levelNameBefore != levelNameAfter) {
                cout << "Your level upgraded to : " << levelNameAfter << endl;
            }

            string updateCustomerSQL = "UPDATE Customers SET points = ?, current_level = ? WHERE id = ?;";
            sqlite3_stmt* custStmt;
            if (sqlite3_prepare_v2(db, updateCustomerSQL.c_str(), -1, &custStmt, nullptr) == SQLITE_OK) {
                sqlite3_bind_int(custStmt, 1, customer->getCustomerPoints());
                sqlite3_bind_text(custStmt, 2, levelNameAfter.c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_int(custStmt, 3, customer->getCustomerID());
                sqlite3_step(custStmt);
                sqlite3_finalize(custStmt);
            }
        }
    } else {
        order.setOrderPoints(0);
        order.setFinalPaidPrice(order.getTotalPrice());
    }

    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

    string insertOrderSQL = "INSERT INTO Orders (TotalPrice, Condition, CustomerID, EarnedPoints, FinalPaid) VALUES (?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, insertOrderSQL.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        return false;
    }

    sqlite3_bind_double(stmt, 1, order.getTotalPrice());
    string conditionStr = order.getOrderCondition();
    sqlite3_bind_text(stmt, 2, conditionStr.c_str(), -1, SQLITE_TRANSIENT);

    if (order.getCustomer() != nullptr) {
        sqlite3_bind_int(stmt, 3, order.getCustomer()->getCustomerID());
    } else {
        sqlite3_bind_null(stmt, 3);
    }
    sqlite3_bind_int(stmt, 4, order.getOrderPoints());
    sqlite3_bind_double(stmt, 5, order.getFinalPaidPrice());

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        return false;
    }
    sqlite3_finalize(stmt);

    int generatedOrderId = (int)sqlite3_last_insert_rowid(db);
    order.setOrderID(generatedOrderId);

    string insertItemsSQL = "INSERT INTO Order_Items (OrderID, ItemID, Quantity) VALUES (?, ?, ?);";
    sqlite3_stmt* itemStmt;

    if (sqlite3_prepare_v2(db, insertItemsSQL.c_str(), -1, &itemStmt, nullptr) != SQLITE_OK) {
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        return false;
    }

    for (int i = 0; i < items.size(); ++i) {
        sqlite3_bind_int(itemStmt, 1, generatedOrderId);
        sqlite3_bind_int(itemStmt, 2, items[i]->getOrderItemID());
        sqlite3_bind_int(itemStmt, 3, items[i]->getOrderNumber());

        if (sqlite3_step(itemStmt) != SQLITE_DONE) {
            sqlite3_finalize(itemStmt);
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            cout << "Error: Failed to insert order item!" << endl;
            return false;
        }

        sqlite3_reset(itemStmt);
    }

    sqlite3_finalize(itemStmt);
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

    sqlite3_bind_text(stmt, 1, newCondition.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, orderId);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

bool OrderDAO::cancelOrder(Order& order) {
    sqlite3* db = database.getConnection();
    if (!db) return false;

    int orderId = order.getOrderID();

    if (order.getCustomer() != nullptr) {
        Customer* customer = order.getCustomer();
        string levelNameBefore = customer->getCustomerLevel();

        int lostPoints = order.getOrderPoints();
        double refundedMoney = order.getFinalPaidPrice();

        if (lostPoints > 0) {
            customer->addCustomerPoints(-lostPoints);
            if (customer->getCustomerPoints() < 0) {
                customer->setCustomerPoints(0);
            }

            cout << "The order was cancelled. " << lostPoints << " points were deducted from your account." << endl;
            cout << "An amount of $" << refundedMoney << " has been refunded to your account." << endl;

            customer->updateMembership();
            string levelNameAfter = customer->getCustomerLevel();

            if (levelNameBefore != levelNameAfter) {
                cout << "Your level has dropped due to a decrease in points to: " << levelNameAfter << endl;
            }

            string updateCustomerSQL = "UPDATE Customers SET points = ?, current_level = ? WHERE id = ?;";
            sqlite3_stmt* custStmt;
            if (sqlite3_prepare_v2(db, updateCustomerSQL.c_str(), -1, &custStmt, nullptr) == SQLITE_OK) {
                sqlite3_bind_int(custStmt, 1, customer->getCustomerPoints());
                sqlite3_bind_text(custStmt, 2, levelNameAfter.c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_int(custStmt, 3, customer->getCustomerID());
                sqlite3_step(custStmt);
                sqlite3_finalize(custStmt);
            }
        }
    }

    string deleteItemsSQL = "DELETE FROM Order_Items WHERE OrderID = ?;";
    sqlite3_stmt* stmt1;
    if (sqlite3_prepare_v2(db, deleteItemsSQL.c_str(), -1, &stmt1, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt1, 1, orderId);
        sqlite3_step(stmt1);
        sqlite3_finalize(stmt1);
    } else {
        return false;
    }

    string deleteOrderSQL = "DELETE FROM Orders WHERE OrderID = ?;";
    sqlite3_stmt* stmt2;
    bool success = false;
    if (sqlite3_prepare_v2(db, deleteOrderSQL.c_str(), -1, &stmt2, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt2, 1, orderId);
        if (sqlite3_step(stmt2) == SQLITE_DONE) {
            success = true;
        }
        sqlite3_finalize(stmt2);
    }

    return success;
}

bool OrderDAO::loadOrderItems(Order* order) {
    if (order == nullptr) return false;

    sqlite3* db = database.getConnection();
    if (!db) return false;

    string sql = "SELECT oi.ItemID, oi.Quantity, m.name, m.price, r.name "
                 "FROM Order_Items oi "
                 "LEFT JOIN MenuItems m ON oi.ItemID = m.id "
                 "LEFT JOIN Restaurants r ON m.restaurant_id = r.id "
                 "WHERE oi.OrderID = ?;";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_int(stmt, 1, order->getOrderID());

    bool isRestaurantNameSet = false;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int itemId = sqlite3_column_int(stmt, 0);
        int quantity = sqlite3_column_int(stmt, 1);

        const char* nameText = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        double price = sqlite3_column_double(stmt, 3);
        const char* resNameText = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));

        if (!isRestaurantNameSet && resNameText) {
            order->setRestaurantName(resNameText);
            isRestaurantNameSet = true;
        }

        ConcreteMenuItem* tempItem = new ConcreteMenuItem();
        tempItem->setItemID(itemId);
        tempItem->setItemName(nameText ? nameText : "Unknown Item");
        tempItem->setItemPrice(price);

        order->addItem(tempItem, quantity);

        const vector<OrderItem*>& items = order->getOrderItems();
        if (!items.empty()) {
            OrderItem* lastInsertedItem = items.back();
            if (lastInsertedItem != nullptr && lastInsertedItem->getItem() != nullptr) {
                lastInsertedItem->getItem()->setItemName(nameText ? nameText : "Unknown Item");
            }
        }
    }

    if (!isRestaurantNameSet) {
        order->setRestaurantName("Unknown Restaurant");
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

    string updateSQL = "UPDATE Customers SET points = ?, current_level = ? WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, updateSQL.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_int(stmt, 1, customer->getCustomerPoints());
    sqlite3_bind_text(stmt, 2, levelNameAfter.c_str(), -1, SQLITE_TRANSIENT);
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



vector<Order*> OrderDAO::getCustomerOrders(int customerId) {
    vector<Order*> customerOrderList;
    sqlite3* db = database.getConnection();
    if (!db) return customerOrderList;

    string sql = "SELECT OrderID, TotalPrice, Condition FROM Orders WHERE CustomerID = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return customerOrderList;
    }

    sqlite3_bind_int(stmt, 1, customerId);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Order* order = new Order();
        order->setOrderID(sqlite3_column_int(stmt, 0));

        double savedTotalPrice = sqlite3_column_double(stmt, 1);

        const char* conditionText = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        order->setOrderCondition(conditionText ? conditionText : "No Condition");

        loadOrderItems(order);
        order->setTotalPrice(savedTotalPrice);

        customerOrderList.push_back(order);
    }

    sqlite3_finalize(stmt);
    return customerOrderList;
}

vector<Order*> OrderDAO::getRestaurantOrders(int restaurantId) {
    vector<Order*> restaurantOrderList;
    sqlite3* db = database.getConnection();
    if (!db) return restaurantOrderList;

    string sql = "SELECT DISTINCT o.OrderID, o.TotalPrice, o.Condition "
                 "FROM Orders o "
                 "JOIN Order_Items oi ON o.OrderID = oi.OrderID "
                 "JOIN MenuItems m ON oi.ItemID = m.id "
                 "WHERE m.restaurant_id = ?;";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return restaurantOrderList;
    }

    sqlite3_bind_int(stmt, 1, restaurantId);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Order* order = new Order();
        order->setOrderID(sqlite3_column_int(stmt, 0));
        order->setTotalPrice(sqlite3_column_double(stmt, 1));

        const char* conditionText = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        order->setOrderCondition(conditionText ? conditionText : "No Condition");

        loadOrderItems(order);

        restaurantOrderList.push_back(order);
    }

    sqlite3_finalize(stmt);
    return restaurantOrderList;
}

Order* OrderDAO::getOrderById(int orderId) {
    sqlite3* db = database.getConnection();
    if (!db) return nullptr;

    string sql = "SELECT OrderID, TotalPrice, Condition, EarnedPoints, FinalPaid FROM Orders WHERE OrderID = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return nullptr;
    }

    sqlite3_bind_int(stmt, 1, orderId);
    Order* foundOrder = nullptr;

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        foundOrder = new Order();
        foundOrder->setOrderID(sqlite3_column_int(stmt, 0));
        foundOrder->setTotalPrice(sqlite3_column_double(stmt, 1));

        const char* conditionText = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        foundOrder->setOrderCondition(conditionText ? conditionText : "No Condition");

        foundOrder->setOrderPoints(sqlite3_column_int(stmt, 3));
        foundOrder->setFinalPaidPrice(sqlite3_column_double(stmt, 4));

        loadOrderItems(foundOrder);
    }

    sqlite3_finalize(stmt);
    return foundOrder;
}