all:ServerApp ClientApp HostApp
ServerApp:server/UdpEcho.c
	gcc $^ -o $@
ClientApp:client/TcpHello.c
	gcc $^ -o $@
HostApp:client/gethostbyname.c
	gcc $^ -o $@

clean:
	rm ServerApp
	rm ClientApp
	rm HostApp
