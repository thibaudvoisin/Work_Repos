# Back-end

Welcome to the backend of our exciting project! This document will guide you through setting up, configuring, and maintaining the backend using the Express.js and Sequelize stack.

## Getting Started

Prerequisites
Before you start, make sure you have the following tools and software installed:

- Node.js (Recommended LTS version)
- npm (Node Package Manager)

### Configuration

Create a .env file in the root directory based on the .env.default provided. Adjust the variables to match your local environment if needed.

## Project Structure

### Overview

Our project follows a modular structure to keep the codebase organized and maintainable. The main components include:

Controllers: Handle HTTP requests, process data, and interact with services.  
Models: Represent database tables and define their relationships.  
Routes: Define API endpoints and route handlers.  
Services: Contain the core business logic of the application.  
Utilities: Contain helper functions used throughout the application.

### Directory Structure

```
back-end/
├── index.ts
├── app.js
├── controllers/
├── models/
├── routes/
├── services/
├── utils/
└── ...
```

## Development Workflow

### Installing Dependencies

Install the project dependencies by running the command:

```bash
npm install
```

**npm install** can result in minor version upgrades. This can still be a "breaking change" for our app, making it somewhat less secure.

Or

```bash
npm ci
```

On the other hand, **npm ci** installs dependencies by following the package-lock.json file and ensures that the production version matches the local one.  
This is why we will use **npm ci** in our CI/CD pipelines and encourage you to do the same.

### Running the Server

Run the server locally by executing the command:

```bash
npm run dev
```

You can also access the swagger at
Run the server locally by executing the command:

```bash
localhost:8000/docs
```

### Running the documentation

Run the documentation locally by executing the command:

```bash
npm run docs
```

You can then acces it by opening the index.html in the docs folder created in your navigator

### Using the environnement variables

Copy the .env.default file as a .env file

```bash
cp .env.default .env
```

## Keeping the Codebase Clean

Les commandes décrites ci-dessous sont également éxécutables depuis la root du projet afin de les appliquer à l'ensemble du projet.

### Linting

Linting is the process of analyzing code to flag potential errors, bugs, stylistic errors, and suspicious constructs.  
We use **ESLint** to lint our code.  
You can run the linter by executing the command:

```bash
npm run lint # this will tell you what's wrong
npm run lint:fix # this will fix what can be fixed automatically
```

### Formatting

Formatting is the process of modifying code to meet a consistent style.  
We use **Prettier** to format our code.  
You can run the formatter by executing the command:

```bash
npm run format # this will tell you what's wrong
npm run format:fix # this will fix what can be fixed automatically
```
