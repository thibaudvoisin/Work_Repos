if pgrep mysqld &>/dev/null; then

  sudo systemctl stop mysql
  echo "Le serveur MariaDB est arrêté avec succès."
  
else
  echo "Le serveur MariaDB n'est pas en cours d'exécution."
fi