# Documentation

## Documentation des différents outils utilisé

### Frontend

Pour le front

### Prisma ###

Pour la database, on va utiliser une image docker de mariadb.
Pour pull l’image :

    docker pull mariadb:10.4

Pour créer le conteneur :

    docker run --name mariadbtest -e MYSQL_ROOT_PASSWORD=mypass -e MYSQL_DATABASE=mariadb -p 3306:3306 -d docker.io/library/mariadb:10.4

Pour lancer le docker :

    docker start mariadbtest

Pour arrêter le docker :

    docker stop mariadbtest

### Elysia ###

Pour le backend
Tuto pour les deux frameworks https://www.youtube.com/watch?v=cpOKHEX9pxY