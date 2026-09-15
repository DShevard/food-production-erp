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

## Database Setup

Create the database:

```bash
createdb shop
```

Apply the database schema:

```bash
psql shop -f DB/schema.sql
```

Insert test data:

```bash
psql shop -f DB/seed.sql
```

## Build

From the project root:

```bash
cmake -S . -B build
cmake --build build
```

## Run

```bash
./build/food-erp
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

Production processing is performed inside a PostgreSQL transaction to ensure that related database changes are committed atomically.

## Purpose

This project was created as a practical C++ and PostgreSQL project focused on database-driven business logic and ERP workflows in the food production domain.
