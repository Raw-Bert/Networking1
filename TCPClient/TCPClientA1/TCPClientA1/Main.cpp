#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <thread>

#pragma comment(lib, "Ws2_32.lib")

std::string last;

std::string GetLastNetworkError()
{
	static std::string tmp; tmp = last;
	return last.clear(), tmp;
}

void SetLastError(const char* head, int code)
{
	void* lpMsgBuf = nullptr;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		(DWORD)code,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)& lpMsgBuf,
		0, NULL);

	last = std::string(head) + (const char*)lpMsgBuf;
}
std::string messageStr;
void setString()
{
	for (;;)
	{
		std::cin >> messageStr;
	}
}
int main() {

	// - Prompts user to enter the IP address of the server - //
	printf("Please Enter Server IP Address: ");
	char ipAd[50];
	std::cin >> ipAd;

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
	int fromlen;
	//fromlen = sizeof(fromAddr);

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo(ipAd, "5000", &hints, &ptr) != 0) {
		printf("Getaddrinfo failed!! %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	SOCKET cli_socket;

	cli_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	BOOL j = TRUE;

	if (cli_socket == INVALID_SOCKET) {
		printf("Failed creating a socket %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	//Connect to the server

	if (connect(cli_socket, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR) {
		printf("Unable to connect to server: %d\n", WSAGetLastError());
		closesocket(cli_socket);
		freeaddrinfo(ptr);
		WSACleanup();
		return 1;
	}

	// Change to non-blocking mode
	u_long mode = 1; // 0 for blocking mode
	ioctlsocket(cli_socket, FIONBIO, &mode);
	printf("Connected to the server\n");

	const unsigned int BUF_LEN = 512;

	char recv_buf[BUF_LEN];
	memset(recv_buf, 0, BUF_LEN);

	if (recv(cli_socket, recv_buf, BUF_LEN, 0) > 0)
		printf("Received from server: %s\n", recv_buf);
	else printf("recv() error: %d\n", WSAGetLastError());
	std::thread message(setString);
	message.detach();
	for (;;)
	{
		//printf("Enter message: ");


		//Send msg to server
		if (messageStr.size() > 0)
			if (send(cli_socket, messageStr.c_str(), BUF_LEN, 0) == SOCKET_ERROR)
			{
				//printf("sendto() failed %d\n", WSAGetLastError());
				//SetLastError("", WSAGetLastError());
				//printf(GetLastNetworkError().c_str());
				//return 1;
			}
		//messageStr.clear();
		//printf("Message sent...\n");

		//Client can recieve messages 
		//printf("Waiting for messages...\n");

		memset(recv_buf, 0, BUF_LEN);
		if (recv(cli_socket, recv_buf, sizeof(recv_buf), 0) == SOCKET_ERROR) {
		//	printf("recvfrom() failed...%d\n", WSAGetLastError());
		//	SetLastError("", WSAGetLastError());
		//	printf(GetLastNetworkError().c_str());
			//return 1;
		}

		//printf("Received: %s\n", recv_buf);

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