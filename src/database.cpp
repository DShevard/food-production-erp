#include "database.h"

Database::Database() {
    conn = PQconnectdb("dbname=shop user=DShevard");
}

Database::~Database() {
    PQfinish(conn);
}

PGconn* Database::connection() {
    return conn;
}

bool Database::isConnected() const {
    return PQstatus(conn) == CONNECTION_OK;
}