BEGIN;

-- Проверяем, хватает ли всех материалов для заказа №1
DO $$
DECLARE
    insufficient_count INT;
BEGIN
    SELECT COUNT(*)
    INTO insufficient_count
    FROM recipe_items ri
    JOIN recipes r
        ON r.id = ri.recipe_id
    JOIN production_orders po
        ON po.product_id = r.product_id
    JOIN raw_material_stock rms
        ON rms.raw_material_id = ri.raw_material_id
    WHERE po.id = 1
      AND rms.quantity < ri.quantity * po.quantity;

    IF insufficient_count > 0 THEN
        RAISE EXCEPTION 'Not enough raw materials for production order %', 1;
    END IF;
END $$;


-- Списываем сырьё
UPDATE raw_material_stock rms
SET quantity = rms.quantity - (ri.quantity * po.quantity)
FROM production_orders po
JOIN recipes r
    ON r.product_id = po.product_id
JOIN recipe_items ri
    ON ri.recipe_id = r.id
WHERE po.id = 1
  AND rms.raw_material_id = ri.raw_material_id;


-- Добавляем готовую продукцию на склад
UPDATE product_stock ps
SET quantity = ps.quantity + po.quantity
FROM production_orders po
WHERE po.id = 1
  AND ps.product_id = po.product_id;


-- Завершаем заказ
UPDATE production_orders
SET status = 'COMPLETED'
WHERE id = 1;

COMMIT;