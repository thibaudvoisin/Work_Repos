import { warehousecatalogModel } from '../models/Warehousecatalog';

export async function getProductsFromWarehouses(product_ids: []) {
  const products = await warehousecatalogModel.findAll({
    attributes: ['product_id', 'warehouse_id', 'stock'],
    where: { product_id: product_ids },
  });

  const regroupedProducts: { [key: number]: any } = {};
  products.forEach((product) => {
    const { product_id, warehouse_id, stock } = product.toJSON();

    if (!regroupedProducts[product_id]) {
      regroupedProducts[product_id] = {
        id: product_id,
        warehouses: [],
      };
    }

    regroupedProducts[product_id].warehouses.push({ id: warehouse_id, stock });
  });

  const finalProducts = Object.values(regroupedProducts);

  return finalProducts;
}
