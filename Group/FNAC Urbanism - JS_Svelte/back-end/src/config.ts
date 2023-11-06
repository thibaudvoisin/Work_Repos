import dotenv from 'dotenv';
dotenv.config();

export const config = {
  env: process.env.NODE_ENV as 'test' | 'development' | 'production',
  port: process.env.PORT as string,
  sequelize: {
    uri: process.env.DATABASE_URL as string,
  },
};
