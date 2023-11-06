import { productModel } from '../models/Product';
import { eshopcatalogModel } from '../models/Eshopcatalog';
import { warehousecatalogModel } from '../models/Warehousecatalog';
import { shopcatalogModel } from '../models/Shopcatalog';
import internal from 'stream';

export async function getGlobalCatalog() {
  const products: productModel[] = await productModel.findAll();
  return products;
}

export async function getEshopCatalog() {
  const products = await eshopcatalogModel.findAll({ include: ['product'] });
  return products.map((entry) => entry.product);
}

export async function getWarehouseCatalogs() {
  const products = await warehousecatalogModel.findAll({
    include: ['product'],
  });

  const warehouseProductMap = new Map();

  for (const product of products) {
    const warehouseId = product.warehouse_id;
    const productInfo = {
      ...product.product.get(),
      max_quantity: product.max_quantity,
    };

    if (warehouseProductMap.has(warehouseId)) {
      warehouseProductMap.get(warehouseId).push(productInfo);
    } else {
      warehouseProductMap.set(warehouseId, [productInfo]);
    }
  }

  return Array.from(warehouseProductMap, ([warehouseId, products]) => ({
    warehouseId,
    products,
  }));
}

export async function getShopCatalogs() {
  const products = await shopcatalogModel.findAll({ include: ['product'] });

  const shopProductMap = new Map();

  for (const product of products) {
    const shopId = product.shop_id;
    const productInfo = {
      ...product.product.get(),
      max_quantity: product.max_quantity,
    };

    if (shopProductMap.has(shopId)) {
      shopProductMap.get(shopId).push(productInfo);
    } else {
      shopProductMap.set(shopId, [productInfo]);
    }
  }

  return Array.from(shopProductMap, ([shopId, products]) => ({
    shopId,
    products,
  }));
}
