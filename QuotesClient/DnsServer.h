#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

class DnsServer
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
	string serverAddr;

	// PRIVATE METHODS
	void receive();
	string getIpAddress(char* _recBuffer, int _length);

public:
	DnsServer(string _address, unsigned int _port);
	~DnsServer();
	void openSocket();
	void closeSocket();
	void setupServer();
	void connectToServer();
	void request(string _message);
	string getServerAddr();
};

