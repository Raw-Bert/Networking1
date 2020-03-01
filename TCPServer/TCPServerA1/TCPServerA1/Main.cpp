#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <thread>

#pragma comment(lib, "Ws2_32.lib")

fd_set all_sockets;


struct Client
{
	Client(SOCKET _clientSocket, std::string _clientName) : _cs(_clientSocket), _cName(_clientName) {};
	SOCKET _cs;
	std::string _cName;
};



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
void sendMessage(SOCKET cli_socket)
{
	std::string msg;

	while (true)
	{
		std::getline(std::cin, msg);
		int size = ((int)msg.size() + 1);
		if (send(cli_socket, (char*)& size, 4, 0) == SOCKET_ERROR) {
			//printf("Failed to send msg to client %d\n", WSAGetLastError());
			//closesocket(cli_socket);
			//freeaddrinfo(ptr);
			//WSACleanup();
			//return 1;
		}
		if (send(cli_socket, (char*)msg.c_str(), size, 0) == SOCKET_ERROR) {
			//printf("Failed to send msg to client %d\n", WSAGetLastError());
			//closesocket(cli_socket);
			//freeaddrinfo(ptr);
			//WSACleanup();
			//return 1;
		}
	}
}

void recvMessege(SOCKET cli_socket)
{
	std::string msg;
	while (true)
	{
		int size = 0;
		if (recv(cli_socket, (char*)& size, 4, 0) > 0);
		msg.resize(size);
		if (recv(cli_socket, (char*)msg.c_str(), size, 0) > 0);
		//printf("Received from server: %s\n", recv_buf);
		printf("Message Received: %s\n", msg.c_str());
	}
}
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
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo(NULL, "5000", &hints, &ptr) != 0) {
		printf("Getaddrinfo failed!! %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	SOCKET server_socket;

	server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

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

	// Listen on socket

	if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
		printf("Listen failed: %d\n", WSAGetLastError());
		closesocket(server_socket);
		freeaddrinfo(ptr);
		WSACleanup();
		return 1;
	}

	printf("Waiting for connections...\n");

	FD_ZERO(&all_sockets);
	FD_SET(server_socket, &all_sockets);

	// Accept a connection (multiple clients --> threads)

	SOCKET client_socket;

	client_socket = accept(server_socket, NULL, NULL);

	if (client_socket == INVALID_SOCKET) {
		printf("Accept() failed %d\n", WSAGetLastError());
		closesocket(server_socket);
		freeaddrinfo(ptr);
		WSACleanup();
		return 1;

	}

	printf("Client connected!!\n");

	// Send a msg to client
	const unsigned int BUF_LEN = 512;

	char send_buf[BUF_LEN];
	memset(send_buf, 0, BUF_LEN);
	strcpy_s(send_buf, "Welcome to INFR3830 server!!!\r\n");

	if (send(client_socket, send_buf, BUF_LEN, 0) == SOCKET_ERROR) {
		printf("Failed to send msg to client %d\n", WSAGetLastError());
		closesocket(client_socket);
		freeaddrinfo(ptr);
		WSACleanup();
		return 1;
	}

	printf("Message sent to client\n");

	//Shutdown the socket

	if (shutdown(client_socket, SD_BOTH) == SOCKET_ERROR) {
		printf("Shutdown failed!  %d\n", WSAGetLastError());
		closesocket(server_socket);
		WSACleanup();
		return 1;
	}

	closesocket(server_socket);
	freeaddrinfo(ptr);
	WSACleanup();

	return 0;








}
// - Takes in Username that will be tracked in server so clients know who sends each message - //
// If username is taken already, server will return false and client will be promted to choose a new username
void createNickname()
{
	bool nameAccepted = false;

	do {
		//takes in nickname
		std::string name;
		printf("Please Enter Your Nickname: ");

		std::cin >> name;

		//Sends nickname to server
		//myclient->sendsting(name);
		//myclient->getBool(nameAccepted);


		if (!nameAccepted)
		{
			printf("Name Taken, Please Choose A Different Name");
		}
	} while (!nameAccepted); //repeats if name not accepted

}