import { productModel } from '../models/Product';
import { warehousecatalogModel } from '../models/Warehousecatalog';
import { shopcatalogModel } from '../models/Shopcatalog';
import { ApiError } from '../utils/ApiError';

export function generateUniqueRandomNumbers(
  count: number,
  min: number,
  max: number
) {
  if (count > max - min + 1 || max < min) {
    throw new Error('Erreur avec la plage de valeurs');
  }

  const uniqueNumbers = new Set(); // Use a Set for unique values

  while (uniqueNumbers.size < count) {
    const randomNumber = Math.floor(Math.random() * (max - min + 1)) + min;
    uniqueNumbers.add(randomNumber);
  }

  return Array.from(uniqueNumbers); // Return as Array of unique random values
}

// Generate a random number between min and max
export function getRandomArbitrary(min: number, max: number) {
  return Math.floor(Math.random() * (max - min)) + min;
}

export async function generateShopCatalog(shopId: number) {
  // Check if the given shopId already exists
  const existingShop = await shopcatalogModel.findAll({
    where: { shop_id: shopId },
    include: ['product'],
  });

  // If yes, send the corresponding catalog
  if (existingShop.length !== 0) {
    return existingShop.map((entry) => entry.product);
  }

  const productsQuantity = (await productModel.count()) - 1;
  const productIds = generateUniqueRandomNumbers(20, 1, productsQuantity);
  let max_quantity = getRandomArbitrary(100, 1000);
  let min_quantity = Math.floor(max_quantity / getRandomArbitrary(2, 10));
  // Create shopCatalog entries for each product
  const shopCatalogEntries = productIds.map((productId) => {
    return {
      shop_id: shopId,
      product_id: productId,
      max_quantity: max_quantity,
      min_quantity: min_quantity,
      stock: max_quantity,
    };
  });

  // Bulk insert shopCatalog entries into the database
  await shopcatalogModel.bulkCreate(shopCatalogEntries);

  // Fetch the products associated with the shop
  const updatedProducts = await shopcatalogModel.findAll({
    where: { shop_id: shopId },
    include: ['product'],
  });

  return updatedProducts.map((entry) => entry.product);
}

export async function generateWarehouseCatalog(warehouseId: number) {
  // Check if the given shopId already exists
  const existingWarehouse = await warehousecatalogModel.findAll({
    where: { warehouse_id: warehouseId },
    include: ['product'],
  });

  // If yes, send the corresponding catalog
  if (existingWarehouse.length !== 0) {
    return existingWarehouse.map((entry) => entry.product);
  }

  const productsQuantity = (await productModel.count()) - 1;
  const productIds = generateUniqueRandomNumbers(20, 1, productsQuantity);
  let max_quantity = getRandomArbitrary(100, 1000);
  let min_quantity = Math.floor(max_quantity / getRandomArbitrary(2, 10));
  // Create warehouseCatalog entries for each product
  const warehouseCatalogEntries = productIds.map((productId) => {
    return {
      warehouse_id: warehouseId,
      product_id: productId,
      max_quantity: max_quantity,
      min_quantity: min_quantity,
      stock: max_quantity,
    };
  });

  // Bulk insert warehouseCatalog entries into the database
  await warehousecatalogModel.bulkCreate(warehouseCatalogEntries);

  // Fetch the products associated with the warehouse
  const updatedProducts = await warehousecatalogModel.findAll({
    where: { warehouse_id: warehouseId },
    include: ['product'],
  });

  return updatedProducts.map((entry) => entry.product);
}
