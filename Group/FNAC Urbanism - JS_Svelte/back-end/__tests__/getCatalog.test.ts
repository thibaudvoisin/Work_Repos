import { type Application } from 'express';
import dotenv from 'dotenv';
import { init } from '../src/app';
import { sequelize } from '../src/database';
import { productModel } from '../src/models/Product';
import { eshopcatalogModel } from '../src/models/Eshopcatalog';
import { warehousecatalogModel } from '../src/models/Warehousecatalog';
import { shopcatalogModel } from '../src/models/Shopcatalog';
import {
  afterEach,
  beforeEach,
  describe,
  it,
  afterAll,
  beforeAll,
  expect,
} from '@jest/globals';

import {
  getGlobalCatalog,
  getEshopCatalog,
  getWarehouseCatalogs,
  getShopCatalogs,
} from '../src/services/getCatalogs';

dotenv.config();
process.env.NODE_ENV = 'test';

let app: Application;
beforeAll(async () => {
  app = await init();
});

beforeEach(async () => {
  const productsData = [
    {
      id: 1,
      name: 'Produit 1',
      description: 'Description du produit 1',
      image: 'image1.jpg',
      price: 19.99,
      warranty_duration: 12,
    },
    {
      id: 2,
      name: 'Produit 2',
      description: 'Description du produit 2',
      image: 'image2.jpg',
      price: 29.99,
      warranty_duration: 24,
    },
  ];
  await productModel.bulkCreate(productsData);

  const eshopData = [
    {
      id: 1,
      name: 'Produit 1',
      description: 'Description du produit 1',
      image: 'image1.jpg',
      price: 19.99,
      warranty_duration: 12,
    },
    {
      id: 2,
      name: 'Produit 2',
      description: 'Description du produit 2',
      image: 'image2.jpg',
      price: 29.99,
      warranty_duration: 24,
    },
  ];
  await eshopcatalogModel.bulkCreate(eshopData);

  const warehousecatalogData = [
    {
      id: 1,
      product_id: 1,
      warehouse_id: 1,
      max_quantity: 844,
      min_quantity: 422,
      stock: 844,
      createdAt: '2023-10-18 09:55:22.283+00',
      updatedAt: '2023-10-18 09:55:22.283+00',
    },
    {
      id: 2,
      product_id: 2,
      warehouse_id: 1,
      max_quantity: 844,
      min_quantity: 422,
      stock: 844,
      createdAt: '2023-10-18 09:55:22.283+00',
      updatedAt: '2023-10-18 09:55:22.283+00',
    },
  ];
  await warehousecatalogModel.bulkCreate(warehousecatalogData);

  const shopcatalogData = [
    {
      id: 1,
      product_id: 1,
      shop_id: 1,
      max_quantity: 844,
      min_quantity: 422,
      stock: 844,
      createdAt: '2023-10-18 09:55:22.283+00',
      updatedAt: '2023-10-18 09:55:22.283+00',
    },
    {
      id: 2,
      product_id: 2,
      shop_id: 1,
      max_quantity: 844,
      min_quantity: 422,
      stock: 844,
      createdAt: '2023-10-18 09:55:22.283+00',
      updatedAt: '2023-10-18 09:55:22.283+00',
    },
  ];
  await shopcatalogModel.bulkCreate(shopcatalogData);
});

describe('getGlobalCatalog', () => {
  it('should return an array of products', async () => {
    const products = await getGlobalCatalog();
    expect(Array.isArray(products)).toBe(true);
    expect(products.length).toBeGreaterThan(0);
  });
});

describe('getEshopCatalog', () => {
  it('should return an array of products', async () => {
    const products = await getEshopCatalog();
    expect(Array.isArray(products)).toBe(true);
    expect(products.length).toBeGreaterThan(0);
  });
});

describe('getWarehouseCatalogs', () => {
  it('should return al warehouses products', async () => {
    const products = await getWarehouseCatalogs();
    expect(Array.isArray(products)).toBe(true);
    expect(products.length).toBeGreaterThan(0);
  });
});

describe('getShopCatalogs', () => {
  it('should return all shops products', async () => {
    const products = await getShopCatalogs();
    expect(Array.isArray(products)).toBe(true);
    expect(products.length).toBeGreaterThan(0);
  });
});

afterEach(async () => {
  await productModel.destroy({ truncate: true });
  await eshopcatalogModel.destroy({ truncate: true });
  await warehousecatalogModel.destroy({ truncate: true });
  await shopcatalogModel.destroy({ truncate: true });
});

afterAll(async () => {
  await sequelize.close();
});
