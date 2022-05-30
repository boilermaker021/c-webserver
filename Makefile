server: server.c types.h
	gcc -o server server.c -lpthread


client: client.c types.h
	gcc -o client client.c