///// UDP Server
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

int main() {

	//Initialize winsock
	WSADATA wsa;

	int error;
	error = WSAStartup(MAKEWORD(2, 2), &wsa);

	if (error != 0) {
		printf("Failed to initialize %d\n", error);
		return 1;
	}

	//Create a Server socket

	struct addrinfo* ptr = NULL, hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(NULL, "8888", &hints, &ptr) != 0) {
		printf("Getaddrinfo failed!! %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	//Create Socket
	SOCKET server_socket;

	server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (server_socket == INVALID_SOCKET) {
		printf("Failed creating a socket %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	// Bind socket

	if (bind(server_socket, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR) {
		printf("Bind failed: %d\n", WSAGetLastError());
		closesocket(server_socket);
		freeaddrinfo(ptr);
		WSACleanup();
		return 1;
	}

	printf("Waiting for Data...\n");

	// Receive msg from client
	const unsigned int BUF_LEN = 512;

	char recv_buf[BUF_LEN];
	char send_buf[BUF_LEN];

	// Struct that will hold the IP address of the client that sent the message (we don't have accept() anymore to learn the address)
	struct sockaddr_in fromAddr;
	int fromlen;
	fromlen = sizeof(fromAddr);
	//int test = 200;
	//setsockopt(server_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)& test, sizeof(test));
	for (;;) {

		printf("waiting for messages...\n");
		memset(recv_buf, 0, BUF_LEN);
		if (recvfrom(server_socket, recv_buf, sizeof(recv_buf), 0, (struct sockaddr*) & fromAddr, &fromlen) == SOCKET_ERROR) {
			printf("recvfrom() failed...%d\n", WSAGetLastError());
			return 1;
		}

		printf("Received: %s\n", recv_buf);

		char ipbuf[INET_ADDRSTRLEN];
		//		printf("Dest IP address: %s\n", inet_ntop(AF_INET, &fromAddr, ipbuf, sizeof(ipbuf)));
		//		printf("Source IP address: %s\n", inet_ntop(AF_INET, &fromAddr, ipbuf, sizeof(ipbuf)));

		//server can send messages too
		printf("Enter message: ");
		std::string line;
		std::getline(std::cin, line);
		line += "\n";
		char* message = (char*)line.c_str();

		if (sendto(server_socket, message, BUF_LEN, 0, (struct sockaddr*) & fromAddr, fromlen) == SOCKET_ERROR)
		{
			printf("sendto() failed %d\n", WSAGetLastError());
		}

		printf("Message sent...\n\n");

	}

		

		char ipbuf[INET_ADDRSTRLEN];
		//		printf("Dest IP address: %s\n", inet_ntop(AF_INET, &fromAddr, ipbuf, sizeof(ipbuf)));
		//		printf("Source IP address: %s\n", inet_ntop(AF_INET, &fromAddr, ipbuf, sizeof(ipbuf)));


	closesocket(server_socket);
	freeaddrinfo(ptr);
	WSACleanup();

	return 0;
}

bool AddUser(std::string nickname)
{
	//For each username in the index
	//If a username matches nickname
		//return false;
	//else
		//add nickname to index
	    //return true;
	return 0;
}