#pragma once

#include <libpq-fe.h>

class Database
{
  public:
    Database();
    ~Database();

    PGconn* connection();

    bool isConnected() const;

  private:
    PGconn* conn;
};