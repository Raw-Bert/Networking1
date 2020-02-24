// UDP Client
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")


int main() {
	printf("Please Enter Your IP Address: ");
	char ipAd[50];
	std::cin >> ipAd;
	//(PCSTR)ipAd;
	//Initialize winsock
	WSADATA wsa;

	int error;
	error = WSAStartup(MAKEWORD(2, 2), &wsa);

	if (error != 0) {
		printf("Failed to initialize %d\n", error);
		return 1;
	}

	//Create a client socket

	struct addrinfo* ptr = NULL, hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;

	if (getaddrinfo(ipAd, "8888", &hints, &ptr) != 0) {
		printf("Getaddrinfo failed!! %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	SOCKET cli_socket;

	cli_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (cli_socket == INVALID_SOCKET) {
		printf("Failed creating a socket %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	const unsigned int BUF_LEN = 512;
	// Receive msg from client
	//const unsigned int BUF_LEN = 512;

	char recv_buf[BUF_LEN];

	char send_buf[BUF_LEN];
	memset(send_buf, 0, BUF_LEN);

	struct sockaddr_in fromAddr;
	int fromlen;
	fromlen = sizeof(fromAddr);

	for (;;) {
		printf("Enter message: ");
		std::string line;
		std::getline(std::cin, line);
		line += "\n";
		char* message = (char*)line.c_str();

		// send msg to server

		if (sendto(cli_socket, message, BUF_LEN, 0,
			ptr->ai_addr, ptr->ai_addrlen) == SOCKET_ERROR) {
			printf("sendto() failed %d\n", WSAGetLastError());
			return 1;
		}

		printf("Message sent...\n");

		//memset(recv_buf, 0, BUF_LEN);
		//if (recvfrom(cli_socket, recv_buf, sizeof(recv_buf), 0, (struct sockaddr*) & fromAddr, &fromlen) == SOCKET_ERROR) {
		//	printf("recvfrom() failed...%d\n", WSAGetLastError());
		//	return 1;
		//}

		//printf("Received: %s\n", recv_buf);

		//char ipbuf[INET_ADDRSTRLEN];
		//		printf("Dest IP address: %s\n", inet_ntop(AF_INET, &fromAddr, ipbuf, sizeof(ipbuf)));
		//		printf("Source IP address: %s\n", inet_ntop(AF_INET, &fromAddr, ipbuf, sizeof(ipbuf)));
	}
	

		


	//Shutdown the socket

	if (shutdown(cli_socket, SD_BOTH) == SOCKET_ERROR) {
		printf("Shutdown failed!  %d\n", WSAGetLastError());
		closesocket(cli_socket);
		WSACleanup();
		return 1;
	}

	closesocket(cli_socket);
	freeaddrinfo(ptr);
	WSACleanup();

	return 0;








}