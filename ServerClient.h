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
#include <time.h>

#ifndef _SERVERCLIENT_H
#define _SERVERCLIENT_H


//prototypes
int start_server(int args,char *argv[],struct sockaddr_in *server_addr);
int start_client(int args,char *argv[],struct sockaddr_in *server_addr);

#endif
