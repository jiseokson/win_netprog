#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable : 4996)

#define BUFFER_SIZE 1024

void ErrorHandling(const char* message)
{
	fprintf(stderr, "%s\n", message);
	exit(1);
}

// iter echo client
int main(int ac, char** av)
{
	WSADATA wsaData;
	SOCKET sock;
	SOCKADDR_IN servAddr;

	if (ac != 3) {
		fprintf(stderr, "Usage: %s <IP> <port>\n", av[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error");

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(atoi(av[2]));
	servAddr.sin_addr.s_addr = inet_addr(av[1]);

	if (connect(sock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() error");
	else
		fprintf(stdout, "Connect...");

	while (1) {
		char buffer[BUFFER_SIZE];
		int readed;

		fprintf(stdout, "Input(\'Q\' to quit): ");
		fgets(buffer, BUFFER_SIZE, stdin);

		if (!strcmp(buffer, "q\n") || !strcmp(buffer, "Q\n"))
			break;

		send(sock, buffer, (int)strlen(buffer), 0);
		readed = recv(sock, buffer, BUFFER_SIZE - 1, 0);
		buffer[readed] = '\0';
		fprintf(stdout, "echo from server: %s", buffer);
	}

	closesocket(sock);
	return 0;
}