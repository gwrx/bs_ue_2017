#!/bin/bash

# run inside the directory where the executables are

mkfifo database
mkfifo middle-ware
mkfifo web

function clean {
	rm database middle-ware web
}

trap clean EXIT

./task1_service database "ERROR 1064 (42000): You have an error in your SQL syntax; check the manual that corresponds to your MariaDB server version for the right syntax to use near ',' at line 1" 'InnoDB: Error: Column last_update in table "mysql"."innodb_table_stats" is INT UNSIGNED NOT NULL but should be BINARY(4) NOT NULL (type mismatch).' "[Note] Plugin 'FEEDBACK' is disabled."&
./task1_service middle-ware "Everything is ok" "Warning: xyz" "Updating information database"&
./task1_service web "1.2.3.4: GET / 200" "5.6.7.8: GET /fail 404" "9.10.11.12: GET /robots.txt 200"&

./task1_logger database middle-ware web
