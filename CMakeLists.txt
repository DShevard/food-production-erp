cmake_minimum_required(VERSION 3.16)

project(food_production_erp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PostgreSQL REQUIRED)

add_executable(food-erp
    src/main.cpp
    src/database.cpp
    src/erp.cpp
)

target_include_directories(food-erp PRIVATE
    ${PostgreSQL_INCLUDE_DIRS}
)

target_link_libraries(food-erp PRIVATE
    ${PostgreSQL_LIBRARIES}
)