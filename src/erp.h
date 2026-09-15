#pragma once

#include <libpq-fe.h>

void showProducts(PGconn* conn);
void showRawMaterials(PGconn* conn);
void showStock(PGconn* conn);
void showProductionOrders(PGconn* conn);
void createProductionOrder(PGconn* conn);
void completeProductionOrder(PGconn* conn);