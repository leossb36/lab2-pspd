#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

void exit_prog();

int sockfd, connfd;
struct sockaddr_in servaddr, cli;

void func(int sockfd)
{
	// funcao que tem objetivo de coletar os dados do terminal e passar para o servidor
	char buff[MAX];
	int n;
	for (;;) {
		bzero(buff, sizeof(buff));
		printf("\nEntre com string : ");
		n = 0;
		while ((buff[n++] = getchar()) != '\n')
			;
		write(sockfd, buff, sizeof(buff));
		bzero(buff, sizeof(buff));
		read(sockfd, buff, sizeof(buff));
		printf("\nResposta do servidor : %s\n", buff);
	}
}

int main()
{
	signal(SIGINT, exit_prog);

	// criacao do socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("Falha ao criar socket do client...\n");
		exit(0);
	}
	else
		printf("Socket criado com sucesso!\n");
	bzero(&servaddr, sizeof(servaddr));

	// settando o enderecamento do servidor
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	// realizar conexao com servidor
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("Nao foi possivel conectar ao servidor...\n");
		exit(0);
	}
	else
		printf("Conexao estabelecida com servidor..\n");

	func(sockfd);
}

void exit_prog() {
	printf("\nFINALIZANDO SOCKET DE CLIENT...\n");
	close(sockfd);
	exit(0);
}
