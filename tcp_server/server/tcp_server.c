#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include "calculadora.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

void exit_prog();

int sockfd, connfd, len;
struct sockaddr_in servaddr, cli;


void func(int sockfd)
{
	char buff[MAX];
	bzero(buff, MAX);

	read(sockfd, buff, sizeof(buff)); // le a mensagem que vem do client

	char tempMessage[80];
	char *event;
	char operador[5];

	strcpy(tempMessage, buff);

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

	bzero(buff, MAX);

	// salva os resultados e manda para o client
	sprintf(buff, "%.2f", resultado);
	write(sockfd, buff, sizeof(buff));
}

int main()
{
	signal(SIGINT, exit_prog);

	// criando socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("Falha ao criar socket do servidor...\n");
		exit(0);
	}
	else
		printf("Socket do servidor criado com sucesso!\n");
	bzero(&servaddr, sizeof(servaddr));

	// passando os parametros de enderecamento do servidor
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	// realizando fowarding na porta 8080
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf("Falha ao realizar bind!\n");
		exit(0);
	}
	else
		printf("Bind realizado com sucesso\n");

	// escutando servidor e verificando se ha conexao
	if ((listen(sockfd, 5)) != 0) {
		printf("Falha ao escutar servidor\n");
		exit(0);
	}
	else
		printf("Escutando servidor\n");
	len = sizeof(cli);

	// verificando conexao com client
	connfd = accept(sockfd, (SA*)&cli, &len);
	if (connfd < 0) {
		printf("Não foi possivel conectar ao client!\n");
		exit(0);
	}
	else
		printf("Conexao com client realizada!\n");

	// funcao para realizar operacoes com os dados do client
	// espera sinal para ser cancelada
	while(1) {
		func(connfd);
	}
}

void exit_prog() {
	printf("\nFECHANDO SOCKET...\n");
	close(sockfd);
	exit(0);
}
