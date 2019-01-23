#!/bin/sh
#Start Mysql via the ubuntu_dev script
chown -R mysql:mysql /var/lib/mysql /var/run/mysqld
service mysql start

sleep 20

#Start sshd server
service ssh start

#Start My Game Server and never quit
# Others can only access this docker via ssh
/SocketExample/ServerApp


