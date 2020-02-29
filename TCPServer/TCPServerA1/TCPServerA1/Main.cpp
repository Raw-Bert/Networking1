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

int main(int arg0,char* arg1[]) {
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
	int fromlen;

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

	// Accept a connection (multiple clients --> threads)

	SOCKET client_socket;
	

	client_socket = accept(server_socket, NULL, NULL);
	BOOL j = TRUE;
	setsockopt(client_socket, IPPROTO_TCP, TCP_NODELAY, (const char*)& j, sizeof(BOOL));

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
	strcpy_s(send_buf, "Connected to Server!\r\n");

	if (send(client_socket, send_buf, BUF_LEN, 0) == SOCKET_ERROR) {
		printf("Failed to send msg to client %d\n", WSAGetLastError());
		closesocket(client_socket);
		freeaddrinfo(ptr);
		WSACleanup();
		return 1;
	}

	printf("Connection confirmation sent to client!\n");

	std::thread message(setString);
	message.detach();
	for (;;)
	{
		//Client can recieve messages 
		printf("Waiting for messages...\n");

		memset(send_buf, 0, BUF_LEN);
		if (recv(client_socket, send_buf, sizeof(send_buf), 0) == SOCKET_ERROR) {
			printf("recvfrom() failed...%d\n", WSAGetLastError());
			SetLastError("", WSAGetLastError());
			printf(GetLastNetworkError().c_str());
			return 1;
		}

		printf("Received: %s\n", send_buf);

		printf("Enter message: ");
		

		//Send msg to server
		if(messageStr.size()>0)
		if (send(client_socket, messageStr.c_str(), BUF_LEN, 0) == SOCKET_ERROR)
		{
			printf("sendto() failed %d\n", WSAGetLastError());
			SetLastError("", WSAGetLastError());
			printf(GetLastNetworkError().c_str());
			//return 1;
		}
		//messageStr.clear();
		
		printf("Message sent...\n");
	}

	//Shutdown the socket

	if (shutdown(server_socket, SD_BOTH) == SOCKET_ERROR) {
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