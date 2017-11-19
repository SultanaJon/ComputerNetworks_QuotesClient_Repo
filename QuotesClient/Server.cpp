#include "Server.h"


////////////////////////////////////////////////////////////////////////////////////////
//
//	Custom constructor: Initializes Server object
//          Parameters: _address - The address of the server we want to connect to
//		                _port    - The port we will use to connect to the server.
//
////////////////////////////////////////////////////////////////////////////////////////
Server::Server(string _address, unsigned int _port)
{
	ipAddress = _address;
	port = _port;
}


Server::~Server()
{
}



////////////////////////////////////////////////////////////////////////////////////////
//
//	   Summery: Strips the server response and returns the quote.
//
//  Parameters: _message - The message returned from the server.
//		         _length - The length of the message.
//
////////////////////////////////////////////////////////////////////////////////////////
string Server::getQuote(char* _message, int _length)
{
	char* rn = strstr(_message, "\r\n");
	char* temp = strchr(_message, ' '); // get the first space
	temp++;
	while (*temp != ' ') // we should now be over the number
	{
		temp++;
	}

	while (*temp == ' ')
		temp++;


	int quoteLength = (int)(rn - temp);
	char quote[buffsize];
	strncpy_s(quote, temp, quoteLength);
	quote[quoteLength] = '\0';
	return quote;
}



////////////////////////////////////////////////////////////////////////////////////////
//
//	   Summery: Receives the message returned from the server.
//
////////////////////////////////////////////////////////////////////////////////////////
void Server::receive()
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
			string message = "Quote Client: Invalid request! Server returned error.: ";
			throw std::exception(message.c_str());
		}
		int len = recBuffer.length();
		string quote = getQuote(tempRecBuff, len);
		len = quote.length();
		cout << quote << endl;
	}
}


////////////////////////////////////////////////////////////////////////////////////////
//
//	   Summery: Open the socket used to communicate with the server.
//
////////////////////////////////////////////////////////////////////////////////////////
void Server::openSocket()
{
	Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (Socket == INVALID_SOCKET)
	{
		string message = "Quote Client: Open socket error! Error Code: " + WSAGetLastError();
		throw std::exception(message.c_str());
	}
}


////////////////////////////////////////////////////////////////////////////////////////
//
//	   Summery: Close the socket used to communicate with the server
//
////////////////////////////////////////////////////////////////////////////////////////
void Server::closeSocket()
{
	if (shutdown(Socket, SD_SEND) != 0)
	{
		string message = "Quote Client: Close Socket error! Error Code: " + WSAGetLastError();
		throw std::exception(message.c_str());
	}
}


////////////////////////////////////////////////////////////////////////////////////////
//
//	   Summery: Setup the server address
//
////////////////////////////////////////////////////////////////////////////////////////
void Server::setupServer()
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
//	   Summery: Establishes a connection with the server
//
////////////////////////////////////////////////////////////////////////////////////////
void Server::connectToServer()
{
	int RetCode = connect(Socket, (SOCKADDR*)&serverAddress, sizeof(serverAddress));
	if (RetCode != 0)
	{
		string message = "Quote Client: Server connect error! Error Code: " + WSAGetLastError();
		throw std::exception(message.c_str());
	}
}


////////////////////////////////////////////////////////////////////////////////////////
//
//	   Summery: Request a quote from the server.
//
//  Parameters: _message - The message we want to send to the server.
//
////////////////////////////////////////////////////////////////////////////////////////
void Server::request(string _message)
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
