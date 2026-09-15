#include <iostream>

#include "database.h"
#include "erp.h"

int main()
{
    Database db;

    if (!db.isConnected())
    {
        std::cerr << "Connection failed.\n";
        return 1;
    }

    PGconn *conn = db.connection();

    std::cout << "Connected to PostgreSQL successfully!\n";

    int choice;

    do
    {
        std::cout << "\n=== Food Production ERP ===\n";
        std::cout << "1. Show products\n";
        std::cout << "2. Show raw materials\n";
        std::cout << "3. Show stock\n";
        std::cout << "4. Show production orders\n";
        std::cout << "5. Create production order\n";
        std::cout << "6. Complete production order\n";
        std::cout << "0. Exit\n";
        std::cout << "Choose: ";

        std::cin >> choice;

        switch (choice)
        {
        case 1:
            showProducts(conn);
            break;

        case 2:
            showRawMaterials(conn);
            break;

        case 3:
            showStock(conn);
            break;

        case 4:
            showProductionOrders(conn);
            break;

        case 5:
            createProductionOrder(conn);
            break;

        case 6:
            completeProductionOrder(conn);
            break;

        case 0:
            std::cout << "Goodbye!\n";
            break;

        default:
            std::cout << "Unknown option.\n";
        }

    } while (choice != 0);

    PQfinish(conn);

    return 0;
}