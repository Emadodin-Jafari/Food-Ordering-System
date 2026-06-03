#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <iostream>
#include "sqlite3.h"

class Database {
private:
    sqlite3* db;

public:
    Database();
    bool open(const std::string& filename);
    bool createTables();
    void close();
    ~Database();
    sqlite3* getConnection();
};

#endif
