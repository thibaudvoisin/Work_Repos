## STAGE 1: Build ###
FROM node:18-alpine AS build
WORKDIR /app
COPY package*.json ./
COPY *.config.* ./
RUN npm ci
COPY tsconfig.json .
COPY src ./src
COPY static ./static
ARG PUBLIC_API_URL
RUN npm run build



### STAGE 2: Production ###
FROM nginx:alpine
COPY --from=build /app/build/ /usr/share/nginx/html/
CMD ["nginx", "-g", "daemon off;"]
EXPOSE 80
