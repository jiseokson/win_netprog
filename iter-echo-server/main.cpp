#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 1024

void ErrorHandling(const char* message)
{
	fprintf(stderr, "%s\n", message);
	exit(1);
}

int main(int ac, char** av)
{
	WSADATA wsaData;
	SOCKET servSock, clntSock;
	SOCKADDR_IN servAddr, clntAddr;
	char buffer[BUFFER_SIZE];
	int readed;
	int clntAddrSize;

	if (ac != 2) {
		fprintf(stderr, "");
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error");

	servSock = socket(PF_INET, SOCK_STREAM, 0);
	if (servSock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(atoi(av[1]));
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(servSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

	if (listen(servSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");

	for (int i = 0; i < 5; ++i) {
		clntAddrSize = sizeof(clntAddr);
		clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &clntAddrSize);
		if (clntSock == INVALID_SOCKET)
			ErrorHandling("accpet() error");
		else
			fprintf(stdout, "Connect to #%d\n", i);
		while ((readed = recv(clntSock, buffer, BUFFER_SIZE, 0)) != 0)
			send(clntSock, buffer, readed, 0);
		closesocket(clntSock);
	}

	closesocket(servSock);
	return 0;
}