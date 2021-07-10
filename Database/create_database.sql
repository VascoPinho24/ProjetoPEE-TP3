  
CREATE USER 'noderedUser'@'localhost' IDENTIFIED BY 'nodered123';
CREATE DATABASE hubDB;
CREATE DATABASE uploaded_data;
GRANT ALL PRIVILEGES ON hubDB.* TO 'noderedUser'@'localhost';
GRANT ALL PRIVILEGES ON uploaded_data.* TO 'noderedUser'@'localhost';
USE hubDB;
CREATE TABLE sensor_data(id int primary key not null auto_increment,peripheralID int not null, parameter varchar(255) not null, value_type varchar(255) not null, value varchar(255) not null, date int not null);
CREATE TABLE rules(ruleID int primary key not null auto_increment, peripheralID_sensor int not null, parameter varchar(255) not null, value_type varchar(255) not null, type varchar(255) not null, value varchar(255) not null, peripheralID_actuator int not null, message varchar(255) not null);
CREATE TABLE peripherals(peripheralID INT NOT NULL AUTO_INCREMENT PRIMARY KEY,type VARCHAR(255) NOT NULL,parameters VARCHAR(255) NOT NULL, mac_address VARCHAR(255) NOT NULL, is_mesh TINYINT(1) DEFAULT 0);
CREATE TABLE credentials(network VARCHAR(255) NOT NULL PRIMARY KEY, value VARCHAR(255) NOT NULL);