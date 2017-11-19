#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

class Server
{
private:
	// PRIVATE MEMBERS
	SOCKET Socket;
	SOCKADDR_IN serverAddress;
	unsigned int port;
	string ipAddress;
	static const int buffsize = 10240;
	string sendBuffer;
	string recBuffer;

	// PRIVATE METHODS
	void receive();
	string getQuote(char* _message, int _length);

public:
	Server(string _address, unsigned int _port);
	~Server();
	void openSocket();
	void closeSocket();
	void setupServer();
	void connectToServer();
	void request(string _message);
};
