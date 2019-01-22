MYSQL_LIB+=$(shell /usr/local/mysql/bin/mysql_config --libs)
MYSQL_HEAD+=$(shell /usr/local/mysql/bin/mysql_config --include)

ServerApp:server/TcpForkMysql.cpp
	g++ $^ -o $@ $(MYSQL_LIB) $(MYSQL_HEAD)

run:ServerApp
	DYLD_LIBRARY_PATH=/usr/local/mysql/lib ./ServerApp

all:ServerApp ClientApp HostApp


ClientApp:client/UdpSendRecv.c
	gcc $^ -o $@
HostApp:client/gethostbyname.c
	gcc $^ -o $@

clean:
	rm ServerApp
	rm ClientApp
	rm HostApp
