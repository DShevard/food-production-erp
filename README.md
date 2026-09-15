# Food Production ERP

A console-based ERP system for managing food production, recipes, raw materials, warehouse stock, and production orders.

## Tech Stack

* C++17
* PostgreSQL
* libpq
* CMake
* Linux
* Git / GitHub

## Features

* View products
* View raw materials
* View warehouse stock
* View production orders
* Create production orders
* Complete production orders
* Calculate required raw materials
* Check raw material availability
* Automatically consume raw materials
* Automatically add finished products to stock
* Transaction-based production processing
* Configurable database name through environment variables

## Architecture

```text
Console
   ↓
C++
   ↓
libpq
   ↓
PostgreSQL
```

## Project Structure

```text
ProjectERP/
├── CMakeLists.txt
├── README.md
├── .gitignore
├── DB/
│   ├── schema.sql
│   ├── seed.sql
│   └── production.sql
└── src/
    ├── main.cpp
    ├── database.cpp
    ├── database.h
    ├── erp.cpp
    └── erp.h
```

## Database Structure

The database contains the following main entities:

* Products
* Raw materials
* Recipes
* Recipe items
* Raw material stock
* Product stock
* Production orders

The system models the relationship between finished products and the raw materials required for production.

## Production Workflow

```text
Production Order
       ↓
Find product recipe
       ↓
Calculate required raw materials
       ↓
Check warehouse stock
       ↓
If materials are available
       ↓
Consume raw materials
       ↓
Add finished products to stock
       ↓
Mark order as COMPLETED
```

Production processing is performed inside a PostgreSQL transaction so that related database changes are committed atomically.

## Database Setup

Create a PostgreSQL database:

```bash
createdb food_erp_test
```

Apply the database schema:

```bash
psql food_erp_test -f DB/schema.sql
```

Insert test data:

```bash
psql food_erp_test -f DB/seed.sql
```

## Build

From the project root:

```bash
cmake -S . -B build
cmake --build build
```

## Run

The application uses the `DB_NAME` environment variable to determine which PostgreSQL database to connect to.

Set the database name and start the application:

```bash
export DB_NAME=food_erp_test
./build/food-erp
```

You can also provide the variable only for a single run:

```bash
DB_NAME=food_erp_test ./build/food-erp
```

If `DB_NAME` is not set, the application will ask for the database name when it starts:

```text
Database name:
```

## Usage

After starting the application, the following menu is available:

```text
=== Food Production ERP ===
1. Show products
2. Show raw materials
3. Show stock
4. Show production orders
5. Create production order
6. Complete production order
0. Exit
```

## Example

Create a production order:

```text
Choose: 5

=== Create Production Order ===
Product ID: 3
Quantity: 200

Production order created. ID: 3
```

Complete the production order:

```text
Choose: 6

=== Complete Production Order ===
Order ID: 3

Production order completed successfully!
```

## Development Environment

The project is designed to be built and run on Linux with PostgreSQL and CMake.

The database connection is configurable through the `DB_NAME` environment variable, allowing the application to work with different PostgreSQL databases without modifying the source code.

## Purpose

This project was created as a practical C++ and PostgreSQL project focused on database-driven business logic and ERP workflows in the food production domain.
