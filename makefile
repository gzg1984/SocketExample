all:ServerApp ClientApp HostApp
#ServerApp:server/TcpForkEchoOnce.c
ServerApp:server/TcpHello.c
	gcc $^ -o $@
ClientApp:client/UdpSendRecv.c
	gcc $^ -o $@
HostApp:client/gethostbyname.c
	gcc $^ -o $@

clean:
	rm ServerApp
	rm ClientApp
	rm HostApp
