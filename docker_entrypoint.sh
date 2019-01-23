#!/bin/sh
#Start Mysql via the ubuntu_dev script
/start.sh
#Start sshd server
service ssh start

#Start My Game Server and never quit
# Others can only access this docker via ssh
/SocketExample/ServerApp


