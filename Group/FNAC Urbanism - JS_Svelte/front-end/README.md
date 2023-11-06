# create-svelte

Everything you need to build a Svelte project, powered by [`create-svelte`](https://github.com/sveltejs/kit/tree/master/packages/create-svelte).

## Creating a project

If you're seeing this, you've probably already done this step. Congrats!

```bash
# create a new project in the current directory
npm create svelte@latest

# create a new project in my-app
npm create svelte@latest my-app
```

## Developing

Once you've created a project and installed dependencies with `npm install` (or `pnpm install` or `yarn`), start a development server:

```bash
npm run dev

# or start the server and open the app in a new browser tab
npm run dev -- --open
```

## Building

To create a production version of your app:

```bash
npm run build
```

You can preview the production build with `npm run preview`.

> To deploy your app, you may need to install an [adapter](https://kit.svelte.dev/docs/adapters) for your target environment.

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
