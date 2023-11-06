import express, {
  type Request,
  type Application,
  type Response,
  type NextFunction,
} from 'express';
import swaggerJSDoc from 'swagger-jsdoc';
import swaggerUi from 'swagger-ui-express';
import cors from 'cors';
import health from './routes/health';

import eshopCatalog from './routes/eshopCatalog';
import generalCatalog from './routes/globalCatalog';
import shopCatalog from './routes/shopCatalog';
import warehouseCatalog from './routes/warehouseCatalog';
import exposeWarehouse from './routes/exposeWarehouse';
import generateshopCatalog from './routes/generateshopCatalog';
import generatewarehouseCatalog from './routes/generatewarehouseCatalog';

import { connectDB } from './database';
import { ApiError } from './utils/ApiError';
import { config } from './config';
import logger, { initMorgan } from './logger';
const PORT = process.env.PORT ?? 8000;

export async function init(): Promise<express.Application> {
  const app: Application = express();

  if (config.env === 'development') {
    // use cors for api calls
    app.use(cors());
  }

  // handle JSON format
  app.use(express.json());

  // ****************************** SETUP DOCUMENTATION **************************
  if (config.env !== 'production') {
    const options = {
      definition: {
        openapi: '3.0.0',
        info: {
          title: 'Sales Management API',
          version: '1.0.0',
        },
        components: {
          securitySchemes: {
            Authorization: {
              type: 'http',
              scheme: 'bearer',
              bearerFormat: 'JWT',
              value: 'Bearer <JWT token here>',
            },
          },
        },
        servers: [
          {
            url: `http://localhost:${PORT}`,
            description: 'Local server',
          },
          {
            url: `https://sales-management.dev.ubsi.fr/api`,
            description: 'Development server',
          },
        ],
      },
      apis: ['./routes/*', './src/routes/*'],
    };
    const swaggerSpec = swaggerJSDoc(options);
    app.use('/docs', swaggerUi.serve, swaggerUi.setup(swaggerSpec));
  }

  await connectDB();

  initMorgan(app);

  app.use(health);
  app.use(eshopCatalog);
  app.use(generalCatalog);
  app.use(shopCatalog);
  app.use(warehouseCatalog);
  app.use(generateshopCatalog);
  app.use(generatewarehouseCatalog);
  app.use(exposeWarehouse);
  app.use(generateshopCatalog);
  app.use(generatewarehouseCatalog);

  // Error Handler : must be after route declaration
  app.use(function (
    err: unknown,
    _req: Request,
    res: Response,
    _next: NextFunction
  ) {
    if (err instanceof ApiError) {
      res.status(err.statusCode).send(err);
    } else {
      logger.error(err);
      res.status(500).send('API Error');
    }
  });
  return app;
}
