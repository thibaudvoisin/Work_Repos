# Web API (backend) template

A simple web API template for [SOCRA SIGL's web-api workshop](https://github.com/SOCRA-EPITA-SIGL-2024/web-api).t

You will use:

- [`express`](https://expressjs.com/) a NodeJS module to implement your web service
- [`nodemon`](https://github.com/remy/nodemon) a NodeJS module as CLI to have hot-reload while you're developping your web services
- Docker to package your application

## Run it locally

To run it:

- Make sure you are on correct version of NodeJS

```sh
nvm use
```

- Install NodeJS modules

```sh
npm install
```

- Run it locally (with hot reload)

```sh
npm start
```

> Note: `npm start` runs the `"start": "nodemon src/server.mjs"` script define in your [package.json](package.json) file

## Run it with Docker

NodeJS process inside docker are a bit capricious, so you need to add specific flags to 
run them locally and be able to kill them with `CTRL + C` shortcut like any other containers.

Here is the command to build and run your container on [localhost:4000](localhost:4000) (4000 not to clash with your localhost:3000 process):

```sh
docker build -t web-api .
docker run --rm --init -t -i -p 4000:3000 web-api:latest
```

- `-t` makes sure your can interact with your process from your terminal (t for TTY)
- `-i` makes sure you can interact with the process
- `--init` wrap the main process in a "kill-friendly" process. See [this known issue from docker + node together](https://github.com/nodejs/docker-node/blob/main/docs/BestPractices.md#handling-kernel-signals)
- `--rm` (optional) just remove the container once stop (for cleanup)
- `-p` to bind host port to container port (to enable access from your host's localhost)

## Template's web services

This template expose already three services:

- a healtcheck
- a distance from eiffel tower service
- a products service that is not complete (challenge of your web-api workshop)

### Healthcheck service

To consume the healthcheck routes:

```sh
curl -XGET http://localhost:3000/
```

This should return:

```json
{"status": "healthy"}
```

### Distance from eiffel tower service

To consume the distance from eiffel tower service, let's consider you want to know the fly distance from the eiffel tower to Clermond-Ferrand:

- position of clermont ferrand is (approx) lat: 45.7785324 and lgn: 3.0850938
>
> Feel free to look it up on [maps.google.com](http://maps.google.com), by typing the coma-separated lat,lng coordinates in the maps search `45.7785324,3.0850938`

```sh
curl -XPOST -H "Content-Type: application/json" -d '{"position": {"lat": 45.7785324, "lng": 3.0850938 }}' http://localhost:3000/v1/distance
```

This should return:

```json
{ "distance": 348 }
```

Meaning, flying distance from Clermond-Ferrand to Eiffel tower is 348km.

### The products service

This service is *work-in-progress* and it's your job to make it work!

We did prepare for you the expected payload to consume the service but not the expected response.

In this template, the product service simply echos the consumer inputs and return the two first gardens from the 2000 available gardens
in France.

To consume this work-in-progress product service:

```sh
curl -XPOST -H "Content-Type: application/json" -d '{"position": {"lat": 45.7785324, "lng": 3.0850938 }}' http://localhost:3000/v1/product\?radius=15
```

> Note: you have to escape the `?` using the `\` char

This should return (without the pretty formatting):

```json
{
  "inputRadius": 15,
  "inputPosition": {
    "lat": 48.856614,
    "lng": 2.3522219
  },
  "gardens": [
    {
      "title": "Potager 1",
      "id": "u1",
      "position": {
        "lat": 48.476542,
        "lng": 0.892455
      },
      "products": {
        "vegetable": [
          {
            "name": "Salade",
            "price": 2.39
          },
          {
            "name": "Cerise",
            "price": 2.44
          }
        ],
        "red_meat": [],
        "wine": [],
        "juice": [
          {
            "name": "Fuze Tea",
            "price": 5.6
          }
        ],
        "chicken": [
          {
            "name": "Poule la Flèche",
            "description": "Pour la chair (grande finesse), mixte (chair et ponte)",
            "price": 10.71
          },
          {
            "name": "Poule estaires",
            "description": "Bonne pondeuse, bonne couveuse, chair fine, variétés",
            "price": 11.05
          }
        ]
      }
    },
    {
      "title": "Potager 2",
      "id": "u2",
      "position": {
        "lat": 47.954351,
        "lng": 1.700814
      },
      "products": {
        "vegetable": [
          {
            "name": "Kiwi",
            "price": 2.2
          },
          {
            "name": "Courgette",
            "price": 0.81
          },
          {
            "name": "Blette",
            "price": 1.71
          }
        ],
        "red_meat": [
          {
            "beef": {
              "name": "Aure-et-saint-girons"
            },
            "parts": [
              {
                "name": "Hampe",
                "price": 12.65
              },
              {
                "name": "Jumeau à pot-au-feu",
                "price": 12.72
              },
              {
                "name": "Bavette d'aloyau",
                "price": 11.85
              },
              {
                "name": "Aiguillette baronne",
                "price": 13.73
              },
              {
                "name": "Rumsteck",
                "price": 10.09
              },
              {
                "name": "Paleron",
                "price": 14.27
              }
            ]
          }
        ],
        "wine": [],
        "juice": [],
        "chicken": [
          {
            "name": "Poule crèvecoeur",
            "description": "Pour la chair (grande finesse), mixte (chair et ponte)",
            "price": 15.69
          },
          {
            "name": "Poule landaise",
            "description": "",
            "price": 13.26
          }
        ]
      }
    }
  ]
}
```
