// Server side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "calculadora.h"

#define PORT 8080
#define MAXLINE 1024

void calculadora_parser();

int sockfd;
char buffer[MAXLINE];
char message[MAXLINE]; // resultado da operacao que ira ser enviada para o client
struct sockaddr_in servaddr, cliaddr;

int main() {
	// criando socket
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));

	// setUp do servidor
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);

	// Bind na porta do servidor
	if ( bind(sockfd, (const struct sockaddr *)&servaddr,
			sizeof(servaddr)) < 0 )
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	int len, clientMessage;

	len = sizeof(cliaddr);

	// recebendo mensagem do client
	clientMessage = recvfrom(sockfd, (char *)buffer, MAXLINE,
				MSG_WAITALL, ( struct sockaddr *) &cliaddr,
				&len);


	buffer[clientMessage] = '\0';
	printf("Client : %s\n", buffer); // printa o que vem do client

	// envia para o client a mensagem
	calculadora_parser();
	sendto(sockfd, (const char *)message, strlen(message),
		MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
			len);
	printf("Mensagem do servidor enviada!\n");
	return 0;
}

void calculadora_parser() {
	char tempMessage[MAXLINE];
	char *event;
	char operador[5];

	strcpy(tempMessage, buffer);

	// realizando parser da mensagem para calcular a operacao
	event = strtok(tempMessage, " ");

	operador[0] = *(event); // pega o operador
	int x = atoi(strtok(NULL, " ")); // pega o primeiro numero
	int y = atoi(strtok(NULL, " ")); // pega o segundo numero

	// calcula a operacao desejada e retorna um valor no formato float
	float resultado = calculadora(operador[0], (float)x, (float)y);

	// printa os resultados
	printf("OPERAÇÃO: %c\n", operador[0]);
	printf("X: %d\n", x);
	printf("Y: %d\n", y);
	printf("RESULTADO: %.2lf\n", resultado);

	bzero(buffer, MAXLINE);
	bzero(message, MAXLINE);

	// salva os resultados e manda para o client
	sprintf(message, "%.2f\n", resultado);
}
