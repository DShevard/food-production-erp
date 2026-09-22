#include "erp.h"

#include <iostream>
#include <string>

void showProducts(PGconn* conn)
{
    PGresult* result = PQexec(conn, "SELECT id, name, unit FROM products ORDER BY id;");

    if (PQresultStatus(result) != PGRES_TUPLES_OK)
    {
        std::cerr << "Query failed: " << PQerrorMessage(conn);
        PQclear(result);
        return;
    }

    std::cout << "\n=== Products ===\n";

    int rows = PQntuples(result);

    for (int row = 0; row < rows; ++row)
    {
        std::cout << PQgetvalue(result, row, 0) << ". " << PQgetvalue(result, row, 1) << " ("
                  << PQgetvalue(result, row, 2) << ")\n";
    }

    PQclear(result);
}

void showRawMaterials(PGconn* conn)
{
    PGresult* result = PQexec(conn, "SELECT id, name, unit, price "
                                    "FROM raw_materials "
                                    "ORDER BY id;");

    if (PQresultStatus(result) != PGRES_TUPLES_OK)
    {
        std::cerr << "Query failed: " << PQerrorMessage(conn);
        PQclear(result);
        return;
    }

    std::cout << "\n=== Raw Materials ===\n";

    int rows = PQntuples(result);

    for (int row = 0; row < rows; ++row)
    {
        std::cout << PQgetvalue(result, row, 0) << ". " << PQgetvalue(result, row, 1) << " | "
                  << PQgetvalue(result, row, 2) << " | price: " << PQgetvalue(result, row, 3) << '\n';
    }

    PQclear(result);
}

void showStock(PGconn* conn)
{
    PGresult* result = PQexec(conn, "SELECT "
                                    "rm.name, "
                                    "rms.quantity, "
                                    "rm.unit "
                                    "FROM raw_material_stock rms "
                                    "JOIN raw_materials rm "
                                    "ON rm.id = rms.raw_material_id "
                                    "ORDER BY rm.id;");

    if (PQresultStatus(result) != PGRES_TUPLES_OK)
    {
        std::cerr << "Query failed: " << PQerrorMessage(conn);
        PQclear(result);
        return;
    }

    std::cout << "\n=== Raw Material Stock ===\n";

    int rows = PQntuples(result);

    for (int row = 0; row < rows; ++row)
    {
        std::cout << PQgetvalue(result, row, 0) << ": " << PQgetvalue(result, row, 1) << " "
                  << PQgetvalue(result, row, 2) << '\n';
    }

    PQclear(result);

    PGresult* productResult = PQexec(conn, "SELECT p.name, ps.quantity, p.unit "
                                           "FROM product_stock ps "
                                           "JOIN products p "
                                           "ON p.id = ps.product_id "
                                           "ORDER BY p.id;");

    if (PQresultStatus(productResult) != PGRES_TUPLES_OK)
    {
        std::cerr << "Query failed: " << PQerrorMessage(conn);
        PQclear(productResult);
        return;
    }

    std::cout << "\n=== Finished Product Stock ===\n";

    int productRows = PQntuples(productResult);

    for (int row = 0; row < productRows; ++row)
    {
        std::cout << PQgetvalue(productResult, row, 0) << ": " << PQgetvalue(productResult, row, 1) << " "
                  << PQgetvalue(productResult, row, 2) << '\n';
    }

    PQclear(productResult);
}

void showProductionOrders(PGconn* conn)
{
    PGresult* result = PQexec(conn, "SELECT po.id, p.name, po.quantity, po.status, po.created_at "
                                    "FROM production_orders po "
                                    "JOIN products p ON p.id = po.product_id "
                                    "ORDER BY po.id;");

    if (PQresultStatus(result) != PGRES_TUPLES_OK)
    {
        std::cerr << "Query failed: " << PQerrorMessage(conn);
        PQclear(result);
        return;
    }

    std::cout << "\n=== Production Orders ===\n";

    int rows = PQntuples(result);

    for (int row = 0; row < rows; ++row)
    {
        std::cout << "#" << PQgetvalue(result, row, 0) << " | " << PQgetvalue(result, row, 1)
                  << " | quantity: " << PQgetvalue(result, row, 2) << " | status: " << PQgetvalue(result, row, 3)
                  << " | " << PQgetvalue(result, row, 4) << '\n';
    }

    PQclear(result);
}

void createProductionOrder(PGconn* conn)
{
    int productId;
    double quantity;

    std::cout << "\n=== Create Production Order ===\n";

    showProducts(conn);

    std::cout << "\nProduct ID: ";
    std::cin >> productId;

    if (std::cin.fail())
    {
        std::cin.clear();
        std::cin.ignore(1000, '\n');

        std::cout << "Please enter a number.\n";
        return;
    }

    // Check that the product exists
    std::string productQuery = "SELECT id FROM products WHERE id = " + std::to_string(productId) + ";";

    PGresult* checkResult = PQexec(conn, productQuery.c_str());

    if (PQresultStatus(checkResult) != PGRES_TUPLES_OK)
    {
        std::cerr << "Failed to check product.\n";
        PQclear(checkResult);
        return;
    }

    if (PQntuples(checkResult) == 0)
    {
        std::cout << "Product with ID " << productId << " does not exist.\n";

        PQclear(checkResult);
        return;
    }

    PQclear(checkResult);

    std::cout << "Quantity: ";
    std::cin >> quantity;

    if (std::cin.fail())
    {
        std::cin.clear();
        std::cin.ignore(1000, '\n');

        std::cout << "Please enter a number.\n";
        return;
    }

    std::string insertQuery = "INSERT INTO production_orders (product_id, quantity) "
                              "VALUES (" +
                              std::to_string(productId) + ", " + std::to_string(quantity) +
                              ") "
                              "RETURNING id;";

    PGresult* result = PQexec(conn, insertQuery.c_str());

    if (PQresultStatus(result) != PGRES_TUPLES_OK)
    {
        std::cerr << "Failed to create production order.\n";
        PQclear(result);
        return;
    }

    std::cout << "Production order created. ID: " << PQgetvalue(result, 0, 0) << '\n';

    PQclear(result);
}

void completeProductionOrder(PGconn* conn)
{
    int orderId;

    std::cout << "\n=== Complete Production Order ===\n";
    std::cout << "Order ID: ";
    std::cin >> orderId;

    if (std::cin.fail())
    {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "Please enter a number.\n";
        return;
    }

    // Start transaction
    PGresult* beginResult = PQexec(conn, "BEGIN");

    if (PQresultStatus(beginResult) != PGRES_COMMAND_OK)
    {
        std::cerr << "Failed to start transaction: " << PQerrorMessage(conn);
        PQclear(beginResult);
        return;
    }

    PQclear(beginResult);

    // Check that the order exists and is pending
    std::string orderQuery = "SELECT product_id, quantity "
                             "FROM production_orders "
                             "WHERE id = " +
                             std::to_string(orderId) + " AND status = 'PENDING';";

    PGresult* orderResult = PQexec(conn, orderQuery.c_str());

    if (PQresultStatus(orderResult) != PGRES_TUPLES_OK || PQntuples(orderResult) == 0)
    {

        std::cerr << "Order not found or already completed.\n";
        PQclear(orderResult);
        PQexec(conn, "ROLLBACK");
        return;
    }

    // to this moment

    int productId = std::stoi(PQgetvalue(orderResult, 0, 0));
    int productionQuantity = std::stoi(PQgetvalue(orderResult, 0, 1));

    PQclear(orderResult);

    // Check raw materials
    std::string checkQuery = "SELECT rm.name, rms.quantity, ri.quantity"
                             "FROM recipe_items ri "
                             "JOIN recipes r ON r.id = ri.recipe_id "
                             "JOIN raw_materials rm ON rm.id = ri.raw_material_id "
                             "JOIN raw_material_stock rms "
                             "ON rms.raw_material_id = ri.raw_material_id "
                             "WHERE r.product_id = " +
                             std::to_string(productId) + ";";

    PGresult* checkResult = PQexec(conn, checkQuery.c_str());

    if (PQresultStatus(checkResult) != PGRES_TUPLES_OK)
    {
        std::cerr << "Failed to check stock: " << PQerrorMessage(conn);
        PQclear(checkResult);
        PQexec(conn, "ROLLBACK");
        return;
    }

    int rows = PQntuples(checkResult);

    for (int row = 0; row < rows; ++row)
    {
        double stock = std::stod(PQgetvalue(checkResult, row, 1));
        double recipeQuantity = std::stod(PQgetvalue(checkResult, row, 2));

        double required = recipeQuantity * productionQuantity;

        if (stock < required)
        {
            std::cout << "Not enough " << PQgetvalue(checkResult, row, 0) << ". Required: " << required
                      << ", stock: " << stock << '\n';

            PQclear(checkResult);
            PQexec(conn, "ROLLBACK");
            return;
        }
    }

    PQclear(checkResult);

    // up to this 2 point I think I get it
    
    // Consume raw materials
    std::string updateStockQuery = "UPDATE raw_material_stock rms "
                                   "SET quantity = rms.quantity - "
                                   "(ri.quantity * " +
                                   std::to_string(productionQuantity) +
                                   ") "
                                   "FROM recipe_items ri "
                                   "JOIN recipes r ON r.id = ri.recipe_id "
                                   "WHERE r.product_id = " +
                                   std::to_string(productId) +
                                   " "
                                   "AND rms.raw_material_id = ri.raw_material_id;";

    PGresult* stockResult = PQexec(conn, updateStockQuery.c_str());

    if (PQresultStatus(stockResult) != PGRES_COMMAND_OK)
    {
        std::cerr << "Failed to update raw material stock: " << PQerrorMessage(conn);
        PQclear(stockResult);
        PQexec(conn, "ROLLBACK");
        return;
    }

    PQclear(stockResult);

    // Add finished products to stock
    std::string productStockQuery = "UPDATE product_stock "
                                    "SET quantity = quantity + " +
                                    std::to_string(productionQuantity) +
                                    " WHERE product_id = " + std::to_string(productId) + ";";

    PGresult* productResult = PQexec(conn, productStockQuery.c_str());

    if (PQresultStatus(productResult) != PGRES_COMMAND_OK)
    {
        std::cerr << "Failed to update product stock: " << PQerrorMessage(conn);
        PQclear(productResult);
        PQexec(conn, "ROLLBACK");
        return;
    }

    PQclear(productResult);

    // Mark order as completed
    std::string completeQuery = "UPDATE production_orders "
                                "SET status = 'COMPLETED' "
                                "WHERE id = " +
                                std::to_string(orderId) + ";";

    PGresult* completeResult = PQexec(conn, completeQuery.c_str());

    if (PQresultStatus(completeResult) != PGRES_COMMAND_OK)
    {
        std::cerr << "Failed to complete order: " << PQerrorMessage(conn);
        PQclear(completeResult);
        PQexec(conn, "ROLLBACK");
        return;
    }

    PQclear(completeResult);

    // Everything succeeded
    PQexec(conn, "COMMIT");

    std::cout << "Production order completed successfully!\n";
}
