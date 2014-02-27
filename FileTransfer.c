/*
	Eduardo Angeles Cabrera
	RT688375
	Programación de Sockets
*/

#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "ServerClient.h"

int main(int args, char *argv[]){
	u_int port;
	int server1;
	int client1;
	int localerror;
	struct sockaddr_in *server_addr;
	struct sockaddr_in cliente_addr;
	socklen_t clienteLen;
	int status;

	char archivo [] = "archivo.txt";
	int file;
	char *readBuffer = NULL;
	int totalReadBytes = 0;	
	int totalWrotenBytes = 0;
	int readBytes = 0;
	int wrotenBytes = 0;

	server_addr = malloc(sizeof(*server_addr));

	//Validamos los argumentos

	if(args < 2){
		fprintf(stderr, "Error: Faltan argumentos\n");
		fprintf(stderr, "\tUSE: %s [s/c] [PUERTO] [DIRECCION]\n", argv[0]);
		fprintf(stderr, "\tUSE: s para Servidor\n");
		fprintf(stderr, "\tUSE: c para Cliente\n");
		fprintf(stderr, "\tUSE: [DIRECCION] solo para el cliente\n");
		return 1;
	}

	port = atoi(argv[2]);
	if(port < 1 || port > 65535) {
		fprintf(stderr,"Puerto %i Fuera del rango (1-65535)\n",port);
		return 1;
	}

	//Llamamos a la función que crea al servidor/cliente
	if(strcmp(argv[1], "s") == 0){
	//Llamamos a la funcion que crea al servidor
		server1 = start_server(args, argv, server_addr);
		
		//Esperamos una conexión
		while(1){

			bzero(&cliente_addr,sizeof(cliente_addr));
			client1 = accept(server1,(struct sockaddr*)&cliente_addr, &clienteLen);

			if(client1 == -1){
				localerror = errno;
				fprintf(stderr, "Error aceptando la conexión (%s)\n", strerror(localerror));
				close(server1);
				return 1;
			}

			//Inicia el protocolo

			if((file = open(archivo, O_RDONLY)) ==-1){
			
				localerror = errno;
				fprintf(stderr, "No se pudo abrir el archivo para mandar (%s)\n", strerror(localerror));
				return 0;
			}

			readBuffer = (char *) calloc(1, 1024);

			while((readBytes = read(file,readBuffer,1024)) > 0){
				totalWrotenBytes = 0;
				while(totalWrotenBytes < readBytes){
					wrotenBytes = write(client1,readBuffer+totalWrotenBytes, readBytes-totalWrotenBytes);
					totalWrotenBytes += wrotenBytes;
					printf("Se mandaron %d Bytes\n",wrotenBytes);
				}
				totalWrotenBytes += readBytes;
			}
			free(readBuffer);
			close(file);
			close(client1);
		}
	
		return 0;
	}
	
	else if(strcmp(argv[1], "c") == 0){
		//Llamamos a la funcion que crea al cliente
		server1 = start_client(args, argv, server_addr);
		readBuffer = (char *) calloc(1,1024);

		if((file = open("recivido.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)) ==-1){
			
			localerror = errno;
			fprintf(stderr, "No se pudo abrir el archivo para mandar (%s)\n", strerror(localerror));
			return 0;
		}

		while((readBytes = read(server1,readBuffer,1024)) > 0) {
			totalWrotenBytes = 0;
			printf("leyendo...\n");
			while(totalWrotenBytes < readBytes) {
				wrotenBytes = write(file,readBuffer+totalWrotenBytes,readBytes-totalWrotenBytes);
				totalWrotenBytes += wrotenBytes;
				printf("Escribiendo...\n");
			}
			totalReadBytes += readBytes;
		}

		close(server1);
		return 0;
	}

	else{
		fprintf(stderr, "\tEl segundo argumento debe ser: [s/c]\n");
		return 1;
	}

	return 0;
}
