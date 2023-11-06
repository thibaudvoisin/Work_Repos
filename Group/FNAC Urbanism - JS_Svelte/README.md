# Blank Application

## Getting started

To make it easy for you to get started with the application, here's a list of recommended next steps.

## Set-up database
The chosen database is postgres. The database is run in a docker container for development purposes.
Its volume will be saved in [./database/postgres-data/](./database/postgres-data/).

In order to create the database, you can run the following command:
```bash
cd database
docker-compose up -d
```
Your postgres instance is now available on port 5432.

From now on you can start the database as a normal docker container.

## Install dependencies
To install the dependencies for the backend and the front-end, run the following command:
```bash
npm install
```

## Start the application
To start the application (front-end and back-end), run the following command:
```bash
npm start
```

## For more details
### Backend
You can find the backend README [here](./back-end/README.md).

### Frontend
You can find the frontend README [here](./front-end/README.md).

## Authors
Your favorite DevOps team <3
