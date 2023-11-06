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

describe('GET /catalog/global', () => {
  it('should return 200 if working', async () => {
    await request(app).get('/catalog/global').expect(200);
  });
});

afterAll(async () => {
  await sequelize.close();
});
