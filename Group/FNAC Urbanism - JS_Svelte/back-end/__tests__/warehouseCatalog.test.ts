import { type Application } from 'express';
import request from 'supertest';
import dotenv from 'dotenv';
import { init } from '../src/app';
import { sequelize } from '../src/database';
import { warehousecatalogModel } from '../src/models/Warehousecatalog';
import { productModel } from '../src/models/Product';
import {
  afterEach,
  beforeEach,
  describe,
  it,
  afterAll,
  beforeAll,
} from '@jest/globals';
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
    }];
    const createdProducts = await productModel.bulkCreate(productsData);
    const warehouseCatalogData = [
      {
        product_id: createdProducts[0].id,
        warehouse_id: 1,
        max_quantity: 100,
        min_quantity: 10,
        stock: 50,
      },
    ];
    await warehousecatalogModel.bulkCreate(warehouseCatalogData);
  });

  describe('GET /catalog/warehouses', () => { // Change the path
    it('should return 200 if alive', async () => {
      await request(app).get('/catalog/warehouses').expect(200); // Change the path
    });
  });

  afterEach(async () => {
    await warehousecatalogModel.destroy({ truncate: true });
    await productModel.destroy({ truncate: true });
  });

  afterAll(async () => {
    await sequelize.close();
  });
