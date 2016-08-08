all:ServerApp ClientApp HostApp
#ServerApp:server/socket_SOCK_STREAM_bind_listen_accept.c
ServerApp:server/server.c
	gcc $^ -o $@
#ClientApp:client/socket_SOCK_DGRAM_sendto.c
ClientApp:client/client.c 
	gcc $^ -o $@
HostApp:client/gethostbyname.c
	gcc $^ -o $@

clean:
	rm ServerApp
	rm ClientApp
	rm HostApp
