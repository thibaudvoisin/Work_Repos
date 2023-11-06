// Catalog types
const catalogType = {
  Warehouse: 'warehouse',
  Shop: 'shop',
  Eshop: 'eshop',
};

// Generate a random number between min and max
function getRandomArbitrary(min, max) {
  return Math.floor(Math.random() * (max - min)) + min;
}

// Generate an array of unique random numbers between min and max
function generateUniqueRandomNumbers(count, min, max) {
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

// Generate a catalog of products
function generateCatalog(type, shop_warehouse_id) {
  let catalog = [];
  let max_quantity;
  let min_quantity;

  generateUniqueRandomNumbers(20, 1, 50).forEach((e) => {
    switch (type) {
      case catalogType.Warehouse:
        catalog.push({
          product_id: e,
          warehouse_id: shop_warehouse_id,
          createdAt: new Date().toISOString(),
          updatedAt: new Date().toISOString(),
        });
        break;

      case catalogType.Shop:
        catalog.push({
          product_id: e,
          shop_id: shop_warehouse_id,
          createdAt: new Date().toISOString(),
          updatedAt: new Date().toISOString(),
        });
        break;

      case catalogType.Eshop:
        catalog.push({
          product_id: e,
          createdAt: new Date().toISOString(),
          updatedAt: new Date().toISOString(),
        });
        break;
    }
  });
  return catalog;
}

module.exports = { generateCatalog, catalogType, getRandomArbitrary };
