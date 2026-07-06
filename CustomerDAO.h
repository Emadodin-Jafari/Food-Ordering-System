#ifndef CUSTOMERDAO_H
#define CUSTOMERDAO_H

#include "Database.h"
#include "Customer.h"
#include <vector>

class CustomerDAO {
private:
    Database& db;

public:
    CustomerDAO(Database& database);

    bool addCustomer(const Customer& customer);

    Customer* getCustomerById(int customerId);

    bool updateCustomerPoints(int customerId, int points);
};

#endif