-- Products
INSERT INTO products (name, unit) VALUES
('White Bread', 'pcs'),
('Croissant', 'pcs'),
('Baguette', 'pcs');

-- Raw materials
INSERT INTO raw_materials (name, unit, price) VALUES
('Flour', 'kg', 60.00),
('Yeast', 'kg', 250.00),
('Salt', 'kg', 30.00),
('Butter', 'kg', 500.00),
('Sugar', 'kg', 90.00);

-- Recipes
INSERT INTO recipes (product_id, name) VALUES
(1, 'White Bread Recipe'),
(2, 'Croissant Recipe'),
(3, 'Baguette Recipe');

-- Recipe items
INSERT INTO recipe_items (recipe_id, raw_material_id, quantity) VALUES
-- White Bread
(1, 1, 0.500),
(1, 2, 0.010),
(1, 3, 0.010),

-- Croissant
(2, 1, 0.100),
(2, 2, 0.005),
(2, 4, 0.050),
(2, 5, 0.020),

-- Baguette
(3, 1, 0.300),
(3, 2, 0.008),
(3, 3, 0.006);

-- Raw material stock
INSERT INTO raw_material_stock (raw_material_id, quantity) VALUES
(1, 500.000),
(2, 20.000),
(3, 50.000),
(4, 30.000),
(5, 40.000);

-- Product stock
INSERT INTO product_stock (product_id, quantity) VALUES
(1, 100.000),
(2, 50.000),
(3, 30.000);

-- Production orders
INSERT INTO production_orders (product_id, quantity, status) VALUES
(1, 100.000, 'COMPLETED'),
(2, 50.000, 'COMPLETED'),
(3, 30.000, 'COMPLETED');