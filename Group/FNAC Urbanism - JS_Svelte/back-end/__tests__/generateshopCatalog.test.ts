import { type Application } from 'express';
import request from 'supertest';
import dotenv from 'dotenv';
import { init } from '../src/app';
import { sequelize } from '../src/database';
import { productModel } from '../src/models/Product';
import { shopcatalogModel } from '../src/models/Shopcatalog';
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
  let createdProducts: any[] = [];

  for (let i = 1; i <= 50; i++) {
    createdProducts.push({
      id: i,
      name: `Produit ${i}`,
      description: `Description du produit ${i}`,
      image: `image${i}.jpg`,
      price: 19.99 + i, // Prix différent pour chaque produit
      warranty_duration: i % 2 === 0 ? 12 : 24, // Durée de garantie alternée
    });
  }
  await productModel.bulkCreate(createdProducts);
});

describe('POST /send-stores', () => {
  it('should return 400 because wrong body', async () => {
    await request(app).post('/send-stores').expect(400);
  });
});

describe('POST /send-stores', () => {
  it('should return 400 because wrong body (invalid id)', async () => {
    const requestBody = {
      stores: [
        {
          id: -1,
          name: 'Fnac KB',
          location: '12 rue KB',
          createdAt: '2023-10-08T10:31:57.221Z',
          updatedAt: '2023-10-08T10:31:57.221Z',
        },
        {
          id: -2,
          name: 'Fnac Montparnasse',
          location: '34 rue Montparnasse',
          createdAt: '2023-10-08T10:31:57.221Z',
          updatedAt: '2023-10-08T10:31:57.221Z',
        },
      ],
    };
    await request(app).post('/send-stores').send(requestBody).expect(400);
  });
});

describe('POST /send-stores', () => {
  it('should return 200 if alive', async () => {
    const requestBody = {
      stores: [
        {
          id: 3,
          name: 'Fnac KB',
          location: '12 rue KB',
          createdAt: '2023-10-08T10:31:57.221Z',
          updatedAt: '2023-10-08T10:31:57.221Z',
        },
        {
          id: 4,
          name: 'Fnac Montparnasse',
          location: '34 rue Montparnasse',
          createdAt: '2023-10-08T10:31:57.221Z',
          updatedAt: '2023-10-08T10:31:57.221Z',
        },
      ],
    };
    await request(app).post('/send-stores').send(requestBody).expect(200);
  });
});

describe('POST /send-stores', () => {
  it('should return 200 if alive', async () => {
    const shopcatalogData = [
      {
        product_id: 1,
        shop_id: 1,
        max_quantity: 844,
        min_quantity: 422,
        stock: 844,
        createdAt: '2023-10-18 09:55:22.283+00',
        updatedAt: '2023-10-18 09:55:22.283+00',
      },
    ];
    await shopcatalogModel.bulkCreate(shopcatalogData);

    const requestBody = {
      stores: [
        {
          id: 1,
          name: 'Fnac KB',
          location: '12 rue KB',
          createdAt: '2023-10-08T10:31:57.221Z',
          updatedAt: '2023-10-08T10:31:57.221Z',
        },
      ],
    };
    await request(app).post('/send-stores').send(requestBody).expect(200);
    await shopcatalogModel.destroy({ truncate: true });
  });
});

afterEach(async () => {
  await productModel.destroy({ truncate: true });
});

afterAll(async () => {
  await sequelize.close();
});
