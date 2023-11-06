#!/bin/bash

# Vérifie que le serveur MariaDB est installé
if ! [ -f /usr/sbin/mysqld ]; then

    # Installe MariaDB
    sudo apt install mariadb-server

fi

# Lance le serveur MariaDB
if pgrep mysqld &>/dev/null; then
  echo "Le serveur MariaDB est déjà en cours d'exécution."
else
  # Démarrer le serveur MariaDB
  sudo service mysql start  # Utilisez 'mariadb' à la place de 'mysql' si nécessaire

  # Vérifier si le serveur MariaDB a démarré avec succès
  if [ $? -eq 0 ]; then
    echo "Le serveur MariaDB a démarré avec succès."
  else
    echo "Erreur : Impossible de démarrer le serveur MariaDB."
  fi
fi
