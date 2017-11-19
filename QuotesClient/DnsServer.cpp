#include "DnsServer.h"


////////////////////////////////////////////////////////////////////////////////////////
//
//	Custom constructor: Initializes DNS_Like Server object
//          Parameters: _address - The address of the server we want to connect to
//		                _port    - The port we will use to connect to the server.
//
////////////////////////////////////////////////////////////////////////////////////////
DnsServer::DnsServer(string _address, unsigned int _port)
{
	ipAddress = _address;
	port = _port;
}


DnsServer::~DnsServer()
{

}


////////////////////////////////////////////////////////////////////////////////////////
//
//	   Summery: Strips the server response and returns the ip address of the server
//              we want to communicate with.
//
//  Parameters: _message - The message returned from the DNS-Like Server
//		         _length - The length of the message.
//
////////////////////////////////////////////////////////////////////////////////////////
string DnsServer::getIpAddress(char* _message, int _length)
{
	char* rn = strstr(_message, "\r\n");
	char* temp = strchr(_message, '.');

	while (*temp != ' ')
	{
		temp--;
	}
	temp++; // point to the first part of the IP address

	int ipLength = (int)(rn - temp);
	char ipAddress[16];
	strncpy_s(ipAddress, temp, ipLength);
	ipAddress[ipLength] = '\0';
	return ipAddress;
}



////////////////////////////////////////////////////////////////////////////////////////
//
//	   Summery: Receives the response from the DNS-Like Server
//
////////////////////////////////////////////////////////////////////////////////////////
void DnsServer::receive()
{
	char tempRecBuff[buffsize];
	unsigned int bytesRec = recv(Socket, tempRecBuff, sizeof(tempRecBuff), 0);
	if (bytesRec == SOCKET_ERROR)
	{
		string message = "Quote Client: Receive error! Error Code: " + WSAGetLastError();
		throw std::exception(message.c_str());
	}
	else
	{
		tempRecBuff[bytesRec] = '\0';
		recBuffer = tempRecBuff;
		size_t pos = recBuffer.find("ERR");
		if (pos != string::npos)
		{
			string message = "Quote Client: Invalid request! Server returned error.";
			throw std::exception(message.c_str());
		}
		int len = recBuffer.length();
		serverAddr = getIpAddress(tempRecBuff, len);
	}
}


////////////////////////////////////////////////////////////////////////////////////////
//
//	   Summery: Opens the socket to communicate with the server.
//
////////////////////////////////////////////////////////////////////////////////////////
void DnsServer::openSocket()
{
	Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (Socket == INVALID_SOCKET)
	{
		string message = "Quote Client: Socket error! Error Code: " + WSAGetLastError();
		throw std::exception(message.c_str());
	}
}



////////////////////////////////////////////////////////////////////////////////////////
//
//	   Summery: Closes the communication socket between the client and the DNS-Like 
//              Server
//
////////////////////////////////////////////////////////////////////////////////////////
void DnsServer::closeSocket()
{
	if (shutdown(Socket, SD_SEND) != 0)
	{
		string message = "Quote Client: Shutdown error! Error Code: " + WSAGetLastError();
		throw std::exception(message.c_str());
	}
}


////////////////////////////////////////////////////////////////////////////////////////
//
//	   Summery: Sets up the dns server address.
//
////////////////////////////////////////////////////////////////////////////////////////
void DnsServer::setupServer()
{
	try
	{
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_port = htons(port);
		serverAddress.sin_addr.s_addr = inet_addr(ipAddress.c_str());
	}
	catch (std::exception)
	{
		string message = "Quote Client: Server setup error! Error Code: " + WSAGetLastError();
		throw std::exception(message.c_str());
	}

}


////////////////////////////////////////////////////////////////////////////////////////
//
//	   Summery: Connects the client to the DNS-Like Server.
//
////////////////////////////////////////////////////////////////////////////////////////
void DnsServer::connectToServer()
{
	int RetCode = connect(Socket, (SOCKADDR*)&serverAddress, sizeof(serverAddress));
	if (RetCode != 0)
	{
		string message = "Quote Client: Connect error! Error Code: " + WSAGetLastError();
		throw std::exception(message.c_str());
	}
}


////////////////////////////////////////////////////////////////////////////////////////
//
//	   Summery: Requests the ip address of the server based on the given message
//              we sent.
//
//  Parameters: _message - The message to request the address of the server.
//
////////////////////////////////////////////////////////////////////////////////////////
void DnsServer::request(string _message)
{
	sendBuffer = _message;
	unsigned int bytesSent = send(Socket, sendBuffer.c_str(), sizeof(sendBuffer), 0);
	if (bytesSent == SOCKET_ERROR)
	{
		string message = "Quote Client: Request error! Error Code: " + WSAGetLastError();
		throw std::exception(message.c_str());
	}
	else
	{
		receive();
	}
}



////////////////////////////////////////////////////////////////////////////////////////
//
//	   Summery: Returns the address of the server we want to communicate with.
//
////////////////////////////////////////////////////////////////////////////////////////
string DnsServer::getServerAddr()
{
	return serverAddr;
}
