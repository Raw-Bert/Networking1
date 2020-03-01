#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <thread>

#pragma comment(lib, "Ws2_32.lib")

// - Takes in Username that will be tracked in server so clients know who sends each message - //
// If username is taken already, server will return false and client will be promted to choose a new username
void createNickname()
{
	

}

void sendMessage(SOCKET cli_socket)
{
	std::string msg;

	while (true)
	{
		std::getline(std::cin, msg);
		int size = ((int)msg.size() + 1);
		if (send(cli_socket, (char*)&size, 4, 0) == SOCKET_ERROR) {
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
		if (recv(cli_socket,(char*)msg.c_str(), size, 0) > 0);
			//printf("Received from server: %s\n", recv_buf);
		printf("Message Recieved: %s\n", msg.c_str());
	}
}


int main() {
	//Client prompted to input server IP
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

	printf("Connected to the server\n");

	bool nameAccepted = false;
	const unsigned int BUF_LEN = 512;

	//u_long mode = 0;
	//ioctlsocket(cli_socket, FIONBIO, &mode);

	char recv_buf[BUF_LEN];
	memset(recv_buf, 0, BUF_LEN);

	do {
		//takes in nickname
		std::string name;
		printf("Please Enter Your Nickname: ");
		//name = "" + name;
		std::cin >> name;
		name.resize(BUF_LEN);
		//Sends nickname to server
		//myclient->sendsting(name);
		//myclient->getBool(nameAccepted);
		
		if (send(cli_socket, name.c_str(), BUF_LEN, 0) == SOCKET_ERROR) {
			printf("Failed to send msg to client %d\n", WSAGetLastError());
			closesocket(cli_socket);
			freeaddrinfo(ptr);
			WSACleanup();
			return 1;
		}
		bool temp;
		if (recv(cli_socket, (char*)&temp, sizeof(bool), 0) > 0)
		{
			if (temp)
			{
				printf("Name Accepted\n");
				nameAccepted = true;
			}
		}

		if (!nameAccepted)
		{
			printf("Name Taken, Please Choose A Different Name\n");
		}
	} while (!nameAccepted); //repeats if name not accepted

	std::thread sende(sendMessage,cli_socket);
	std::thread recieve(recvMessege, cli_socket);
	while (true)
	{

	}


	if (recv(cli_socket, recv_buf, BUF_LEN, 0) > 0)
		printf("Received from server: %s\n", recv_buf);
	else printf("recv() error: %d\n", WSAGetLastError());

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