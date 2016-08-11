all:ServerApp ClientApp HostApp
ServerApp:server/TcpEcho.c
	gcc $^ -o $@
ClientApp:client/TcpRead.c
	gcc $^ -o $@
HostApp:client/gethostbyname.c
	gcc $^ -o $@

clean:
	rm ServerApp
	rm ClientApp
	rm HostApp
