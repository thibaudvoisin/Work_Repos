import { type Application } from 'express';
import winston from 'winston';
import { ElasticsearchTransport } from 'winston-elasticsearch';
import morgan from 'morgan';

const transports = [];
if (process.env.NODE_ENV === 'development' || process.env.NODE_ENV === 'test') {
  transports.push(new winston.transports.Console({ level: 'debug' }));
} else {
  transports.push(
    new ElasticsearchTransport({
      level: 'info',
      source: 'ubsi-app-sales-management',
      clientOpts: {
        node: 'http://ubsi.fr:1337',
      },
    })
  );
}
const logger = winston.createLogger({
  transports: transports,
});

const morganJSONFormat = () =>
  JSON.stringify({
    method: ':method',
    url: ':url',
    http_version: ':http-version',
    remote_addr: ':remote-addr',
    response_time: ':response-time',
    status: ':status',
    content_length: ':res[content-length]',
    user_agent: ':user-agent',
  });

export function initMorgan(app: Application) {
  if (process.env.NODE_ENV === 'test') return;
  app.use(
    morgan(morganJSONFormat(), {
      stream: {
        write: (message) => {
          const data = JSON.parse(message);
          return logger.info('accesslog', data);
        },
      },
    })
  );
}

export default logger;
