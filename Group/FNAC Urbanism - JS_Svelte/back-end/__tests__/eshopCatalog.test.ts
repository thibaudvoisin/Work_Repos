import { type Application } from 'express';
import request from 'supertest';
import dotenv from 'dotenv';
import { init } from '../src/app';
import { sequelize } from '../src/database';
import { describe, it, afterAll, beforeAll } from '@jest/globals';

dotenv.config();
process.env.NODE_ENV = 'test';

let app: Application;
beforeAll(async () => {
  app = await init();
});

describe('GET /catalog/eshop', () => {
  it('should return 200 if alive', async () => {
    await request(app).get('/catalog/eshop').expect(200);
  });
});

afterAll(async () => {
  await sequelize.close();
});
