import { init } from './app';
import { config } from './config';
import logger from './logger';
async function main(): Promise<void> {
  const app = await init();
  app.listen(config.port, () => {
    logger.info(`Server is running on port: ${config.port}`);
  });
}

void main();
