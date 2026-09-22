CREATE TABLE raw_materials (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    unit VARCHAR(20) NOT NULL,
    price NUMERIC(10, 2) NOT NULL CHECK (price >= 0)
);

CREATE TABLE products (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    unit VARCHAR(20) NOT NULL
);

CREATE TABLE recipes (
    id SERIAL PRIMARY KEY,
    product_id INT NOT NULL,
    name VARCHAR(100) NOT NULL,
    FOREIGN KEY (product_id) REFERENCES products(id)
);

CREATE TABLE recipe_items (
    recipe_id INT NOT NULL,
    raw_material_id INT NOT NULL,
    quantity NUMERIC(10, 3) NOT NULL CHECK (quantity > 0),

    PRIMARY KEY (recipe_id, raw_material_id),

    FOREIGN KEY (recipe_id)
        REFERENCES recipes(id)
        ON DELETE CASCADE,

    FOREIGN KEY (raw_material_id)
        REFERENCES raw_materials(id)
);

CREATE TABLE raw_material_stock (
    raw_material_id INT PRIMARY KEY,
    quantity NUMERIC(12, 3) NOT NULL CHECK (quantity >= 0),

    FOREIGN KEY (raw_material_id)
        REFERENCES raw_materials(id)
);

CREATE TABLE product_stock (
    product_id INT PRIMARY KEY,
    quantity INT NOT NULL CHECK (quantity >= 0),

    FOREIGN KEY (product_id)
        REFERENCES products(id)
);

CREATE TABLE production_orders (
    id SERIAL PRIMARY KEY,
    product_id INT NOT NULL,
    quantity INT NOT NULL CHECK (quantity > 0),
    status VARCHAR(20) NOT NULL DEFAULT 'PENDING',
    created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,

    FOREIGN KEY (product_id)
        REFERENCES products(id),

    CHECK (status IN ('PENDING', 'COMPLETED', 'CANCELLED'))
);