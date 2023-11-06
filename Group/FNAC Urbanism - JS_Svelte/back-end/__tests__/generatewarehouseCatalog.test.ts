import { type Application } from 'express';
import request from 'supertest';
import dotenv from 'dotenv';
import { init } from '../src/app';
import { sequelize } from '../src/database';
import { productModel } from '../src/models/Product';
import { warehousecatalogModel } from '../src/models/Warehousecatalog';
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
  let productsData: any[] = [];

  for (let i = 1; i <= 50; i++) {
    productsData.push({
      id: i,
      name: `Produit ${i}`,
      description: `Description du produit ${i}`,
      image: `image${i}.jpg`,
      price: 19.99 + i, // Prix différent pour chaque produit
      warranty_duration: i % 2 === 0 ? 12 : 24, // Durée de garantie alternée
    });
  }
  await productModel.bulkCreate(productsData);
});

describe('POST /send-warehouses', () => {
  it('should return 400 because wrong body', async () => {
    await request(app).post('/send-warehouses').expect(400);
  });
});

describe('POST /send-warehouses', () => {
  it('should return 400 because wrong body (invalid id)', async () => {
    const requestBody = {
      warehouse_list: [
        {
          id: -1,
          address: '48 rue Marcel Prout',
        },
        {
          id: 2,
          address: '37 rue Zambla',
        },
      ],
    };
    await request(app).post('/send-warehouses').send(requestBody).expect(400);
  });
});

describe('POST /send-warehouses', () => {
  it('should return 200 if alive', async () => {
    const requestBody = {
      warehouse_list: [
        {
          id: 1,
          address: '48 rue Marcel Prout',
        },
        {
          id: 2,
          address: '37 rue Zambla',
        },
      ],
    };
    await request(app).post('/send-warehouses').send(requestBody).expect(200);
  });
});

describe('POST /send-warehouses', () => {
  it('should return 200 if alive', async () => {
    const warehouseData = [
      {
        product_id: 1,
        warehouse_id: 1,
        max_quantity: 844,
        min_quantity: 422,
        stock: 844,
        createdAt: '2023-10-18 09:55:22.283+00',
        updatedAt: '2023-10-18 09:55:22.283+00',
      },
    ];
    await warehousecatalogModel.bulkCreate(warehouseData);

    const requestBody = {
      warehouse_list: [
        {
          id: 1,
          address: '13 avenue Oui',
        },
      ],
    };
    await request(app).post('/send-warehouses').send(requestBody).expect(200);
    await warehousecatalogModel.destroy({ truncate: true });
  });
});

afterEach(async () => {
  await productModel.destroy({ truncate: true });
});

afterAll(async () => {
  await sequelize.close();
});
