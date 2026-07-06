#include "CustomerDAO.h"
#include <iostream>
#include "sqlite3.h"

using namespace std;

CustomerDAO::CustomerDAO(Database& database) : db(database) {}

int customerReadCallback(void* data, int argc, char** argv, char** azColName) {
    auto* container = static_cast<vector<Customer>*>(data);

    Customer cust;
    if (argv[0] != nullptr) cust.setCustomerID(stoi(argv[0]));
    if (argv[1] != nullptr) cust.setCustomerName(string(argv[1]));
    if (argv[2] != nullptr) cust.setCustomerPhoneNumber(string(argv[2]));

    if (argv[3] != nullptr) cust.setCustomerPoints(stoi(argv[3]));

    container->push_back(cust);
    return 0;
}

bool CustomerDAO::addCustomer(const Customer& customer) {
    string sql = "INSERT INTO Customers (id, name, phoneNumber, points, current_level) VALUES (" +
                 to_string(customer.getCustomerID()) + ", '" +
                 customer.getCustomerName() + "', '" +
                 customer.getCustomerPhoneNumber() + "', " +
                 to_string(customer.getCustomerPoints()) + ", '" +
                 customer.getCustomerLevel() + "');";

    char* errorMessage = nullptr;
    int exit = sqlite3_exec(db.getConnection(), sql.c_str(), nullptr, nullptr, &errorMessage);

    if (exit != SQLITE_OK) {
        cerr << "addCustomer error: " << errorMessage << endl;
        sqlite3_free(errorMessage);
        return false;
    }
    return true;
}

Customer* CustomerDAO::getCustomerById(int customerId) {
    vector<Customer> tempContainer;
    string sql = "SELECT id, name, phoneNumber, points, current_level FROM Customers WHERE id = " + to_string(customerId) + ";";
    char* errorMessage = nullptr;

    sqlite3_exec(db.getConnection(), sql.c_str(), customerReadCallback, &tempContainer, &errorMessage);

    if (errorMessage) {
        cerr << "getCustomerById Error: " << errorMessage << endl;
        sqlite3_free(errorMessage);
        return nullptr;
    }

    if (!tempContainer.empty()) {
        return new Customer(tempContainer[0]);
    }
    return nullptr;
}

bool CustomerDAO::updateCustomerPoints(int customerId, int points) {
    Customer temp;
    temp.setCustomerPoints(points);
    string calculatedLevel = temp.getCustomerLevel();

    string sql = "UPDATE Customers SET points = " + to_string(points) +
                 ", current_level = '" + calculatedLevel +
                 "' WHERE id = " + to_string(customerId) + ";";

    char* errorMessage = nullptr;
    int exit = sqlite3_exec(db.getConnection(), sql.c_str(), nullptr, nullptr, &errorMessage);

    if (exit != SQLITE_OK) {
        cerr << "updateCustomerPoints error: " << errorMessage << endl;
        sqlite3_free(errorMessage);
        return false;
    }
    return true;
}