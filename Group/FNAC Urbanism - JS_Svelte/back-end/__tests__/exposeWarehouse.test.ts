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
  expect,
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
  const createdProducts = await productModel.bulkCreate(productsData);
  const warehouseCatalogData = [
    {
      product_id: createdProducts[0].id,
      warehouse_id: 1,
      max_quantity: 100,
      min_quantity: 10,
      stock: 50,
    },
    {
      product_id: createdProducts[0].id,
      warehouse_id: 2,
      max_quantity: 200,
      min_quantity: 20,
      stock: 75,
    },
    {
      product_id: createdProducts[1].id,
      warehouse_id: 1,
      max_quantity: 150,
      min_quantity: 15,
      stock: 60,
    },
  ];
  await warehousecatalogModel.bulkCreate(warehouseCatalogData);
});

describe('POST /ask-warehouses', () => {
  it('should return 500 because wrong body', async () => {
    await request(app).post('/ask-warehouses').expect(400);
  });
});

describe('POST /ask-warehouses', () => {
  it('should return 200 if alive', async () => {
    const requestBody = {
      product_ids: [0, 1],
    };
    let productsRequest = await request(app)
      .post('/ask-warehouses')
      .send(requestBody)
      .expect(200);
    expect(productsRequest.body.products[0]).toEqual({
      id: 1,
      warehouses: [
        { id: 1, stock: 50 },
        { id: 2, stock: 75 },
      ],
    });
  });
});

afterEach(async () => {
  await warehousecatalogModel.destroy({ truncate: true });
  await productModel.destroy({ truncate: true });
});

afterAll(async () => {
  await sequelize.close();
});
