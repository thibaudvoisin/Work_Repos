## STAGE 1: Build ###
FROM node:18-alpine AS build
WORKDIR /app
COPY package*.json ./
RUN npm ci
COPY tsconfig.json .
COPY src ./src
RUN npm run build



### STAGE 2: Production ###
FROM node:18-alpine

# Package to handle signals when running node as PID1
RUN apk add dumb-init

WORKDIR /app
COPY --from=build /app/package*.json ./
COPY ./sequelize/ ./
RUN npm ci --production
COPY --from=build /app/dist/ ./

# Switch from root to node user
USER node
CMD ["sh", "-c", "npx sequelize db:migrate --env production && npx sequelize db:seed:all --env production &&  node index.js"]
EXPOSE 80
