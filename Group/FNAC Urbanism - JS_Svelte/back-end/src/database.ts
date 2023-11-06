import { Sequelize } from 'sequelize-typescript';
import { config } from './config';
import path from 'path';
import logger from './logger';

export const sequelize =
  process.env.NODE_ENV === 'test'
    ? new Sequelize('sqlite::memory:', {
        logging: false,
        models: [path.join(__dirname, 'models')],
        define: {
          timestamps: false,
        },
        modelMatch: (filename, member) => {
          return filename.toLowerCase() + 'model' === member.toLowerCase();
        },
      })
    : new Sequelize(config.sequelize.uri, {
        models: [path.join(__dirname, 'models')],
        modelMatch: (filename, member) => {
          return filename.toLowerCase() + 'model' === member.toLowerCase();
        },
        logging:
          process.env.NODE_ENV === 'development'
            ? (msg) => logger.debug(msg)
            : false,
      });

export async function connectDB(): Promise<void> {
  if (process.env.NODE_ENV === 'development') {
    await sequelize
      .sync()
      .then(() => {
        logger.info(
          'Connected to PostgreSQL database: ' + sequelize.getDatabaseName()
        );
        logger.info('Tables synchronized');
      })
      .catch((err) => {
        logger.error('Error while synchronizing DB', err);
      });
  } else {
    await sequelize.sync().catch((err) => {
      logger.error('Error while synchronizing DB', err);
    });
  }
}
