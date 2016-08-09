all:ServerApp ClientApp HostApp
ServerApp:server/TcpForkHello.c
	gcc $^ -o $@
ClientApp:client/TcpWriteRead.c
	gcc $^ -o $@
HostApp:client/gethostbyname.c
	gcc $^ -o $@

clean:
	rm ServerApp
	rm ClientApp
	rm HostApp
