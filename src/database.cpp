#include "database.h"

#include <cstdlib>
#include <iostream>
#include <string>

Database::Database()
    : conn(nullptr)
{
    std::string dbName;

    const char* envDbName = std::getenv("DB_NAME");

    if (envDbName != nullptr && std::string(envDbName) != "")
    {
        dbName = envDbName;
    }
    else
    {
        std::cout << "Database name: ";
        std::cin >> dbName;
    }

    std::string connectionString = "dbname=" + dbName + " user=DShevard";

    conn = PQconnectdb(connectionString.c_str());
}

Database::~Database()
{
    PQfinish(conn);
}

PGconn* Database::connection()
{
    return conn;
}

bool Database::isConnected() const
{
    return PQstatus(conn) == CONNECTION_OK;
}