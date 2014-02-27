/*
	Eduardo Angeles Cabrera
	RT688375
	Programación de Sockets
*/

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int start_server(int args,char *argv[],struct sockaddr_in *server_addr) {

	u_int port;
	int server;
	int localerror;
	int status;

	port = atoi(argv[2]);
	if(port < 1 || port > 65535) {
		fprintf(stderr,"Puerto %i fuera del rango (1-65535)\n",port);
		return 1;
	}

	//Iniciamos la apertura del Socket
	server = socket(PF_INET,SOCK_STREAM,0);
	if(server == -1) {
		localerror = errno;
		fprintf(stderr, "Error: %s\n",strerror(localerror));
		return 1;
	}

	//Nos adjudicamos el Puerto.
	bzero(server_addr,sizeof(*server_addr));
	server_addr->sin_family = AF_INET;
	server_addr->sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr->sin_port = htons(port);

	status = bind(server, (struct sockaddr *) server_addr, sizeof(*server_addr));
	if(status != 0) {
		localerror = errno;
		fprintf(stderr,"No se puede enlazar con el puerto: %s\n",strerror(localerror));
		close(server);
		return 1;
	}

	//Nos ponemos el modo de Escucha
	status = listen(server,5);
	if(status == -1) {
		localerror = errno;
		fprintf(stderr,"No se puede escuchar al socket (%s)\n",strerror(localerror));
		close(server);
		return 1;
	}	

	return server;
}



int start_client(int args,char *argv[],struct sockaddr_in *server_addr) {

	u_int port;
	int server;
	int localerror;
	int status;

	//Iniciamos la apertura del Socket
	server = socket(PF_INET,SOCK_STREAM,0);
	if(server == -1) {
		localerror = errno;
		fprintf(stderr, "Error: %s\n",strerror(localerror));
		return 1;
	}

	port = atoi(argv[2]);
	printf("Puerto: %d\nIP: %s \n",port,argv[3]);
	bzero(server_addr,sizeof(*server_addr));
	server_addr->sin_family = AF_INET;		
	status = inet_pton(AF_INET,argv[3],&(server_addr->sin_addr.s_addr));
	server_addr->sin_port = htons(port);
	printf("Status inet_pton: %d \n",status);

	status = connect(server,(struct sockaddr *)server_addr,sizeof(*server_addr));
	printf("Status connect: %d \n",status);
	if(status != 0) {
		localerror = errno;
		printf("Error al conectar (%s)\n",strerror(localerror));
		return 1;
	}

	return server;
}
