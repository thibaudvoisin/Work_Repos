-- Login to MariaDB as the root user (you will be prompted for the root password)
mysql -u root -p

-- Create a new database
CREATE DATABASE mydb;

-- Create a new user and grant privileges to the database
CREATE USER 'myuser'@'localhost' IDENTIFIED BY 'mypassword';
GRANT ALL PRIVILEGES ON mydb.* TO 'myuser'@'localhost';
FLUSH PRIVILEGES;

-- Use the newly created database
USE mydb;

-- Create a sample table
CREATE TABLE users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50),
    email VARCHAR(100)
);

-- Exit the MySQL shell
EXIT;