#include "Database.h"
#include <iostream>

using namespace std;

Database::Database() : db(nullptr) {}

bool Database::open(const std::string& filename) {
    if (sqlite3_open(filename.c_str(), &db) == SQLITE_OK) {
        std::cout << "Opened Database Successfully!" << std::endl;
        return true;
    } else {
        std::cerr << "Error open DB: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
}

bool Database::createTables() {
    char* errMsg = nullptr;
    int rc;

    string createOrdersTable =
            "CREATE TABLE IF NOT EXISTS Orders ("
            "OrderID INTEGER PRIMARY KEY AUTOINCREMENT, "
            "TotalPrice REAL NOT NULL, "
            "Condition TEXT NOT NULL);";

    rc = sqlite3_exec(db, createOrdersTable.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "SQL error (Orders Table): " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }

    string createOrderItemsTable =
            "CREATE TABLE IF NOT EXISTS Order_Items ("
            "OrderID INTEGER, "
            "ItemID INTEGER, "
            "Quantity INTEGER NOT NULL, "
            "PRIMARY KEY (OrderID, ItemID), "
            "FOREIGN KEY (OrderID) REFERENCES Orders(OrderID) ON DELETE CASCADE);";

    rc = sqlite3_exec(db, createOrderItemsTable.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "SQL error (OrderItems Table): " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }

    string sqlRestaurants =
            "CREATE TABLE IF NOT EXISTS Restaurants ("
            "id INTEGER PRIMARY KEY, "
            "name TEXT NOT NULL, "
            "address TEXT, "
            "status TEXT, "
            "prepTime INTEGER, "
            "phoneNumber TEXT, "
            "description TEXT"
            ");";

    rc = sqlite3_exec(db, sqlRestaurants.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Error creating Restaurants table: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }

    string sqlMenuItems =
            "CREATE TABLE IF NOT EXISTS MenuItems ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "restaurant_id INTEGER, "
            "name TEXT NOT NULL, "
            "price REAL, "
            "type TEXT, "
            "cookingTime INTEGER, "
            "drinkVolume INTEGER, "
            "description TEXT, "
            "availability INTEGER DEFAULT 1, "
            "FOREIGN KEY(restaurant_id) REFERENCES Restaurants(id)"
            ");";

    rc = sqlite3_exec(db, sqlMenuItems.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Error creating MenuItems table: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }

    cout << "Database tables checked/created successfully!" << endl;
    return true;
}

void Database::close() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

Database::~Database() {
    close();
}

sqlite3 *Database::getConnection() {
    return db;
}