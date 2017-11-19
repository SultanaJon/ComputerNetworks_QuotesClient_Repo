//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		   Program: Quotes Client
//		Programmer: Jon Sultana
//			  Date: 10/23/2017
//
//  LTU Honor Code: "I have neither given nor received unauthorized aid in completing
//                   this work nor have I claimed someone else's work as my own."
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include "DnsServer.h"
#include "Server.h"
#include "Quotes.h"


using std::cout;
using std::cin;
using std::endl;
using std::string;







///////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Summery: Gathers the DNS Server information from the user.
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void getDnsInformation(string* _address, unsigned int* _port)
{
	cout << "Please enter the following DNS-Like Server information." << endl;
	cout << "[IP ADDRESS]: ";
	cin >> *_address;
	cout << "[PORT]: ";
	cin >> *_port;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Summery: Get the first and last name of the server to contact.
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void getTargetServerInformation(string* _firstName, string* _lastName)
{
	cout << "\n\nPlease provide information regarding the server.\n\n";
	cout << "[SERVER FIRST NAME]: ";
	cin >> *_firstName;
	cout << "[SERVER LAST NAME]: ";
	cin >> *_lastName;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Summery: Make requests to the target server for quotes.
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void makeRequests(Server* _server)
{
	cout << "Request quotes from the server. Enter '-1' to cancel requests.\n\n";
	cin.ignore();
	do
	{
		string request;
		cout << "[REQUEST MESSAGE]: ";
		std::getline(cin, request);
		cin.clear();
		if (request == "-1")
		{
			return;
		}
		else
		{
			request += "\r\n";
			_server->request(request);
			cout << endl;
		}
	} while (true);
}




int main(int argc, char **argv)
{
	#pragma region Local Members
		WSADATA wsaData;
		string ipAddress;
		unsigned int port;
		string firstName;
		string lastName;
		string userResponse;
	#pragma endregion



	// Initialize Winsock version 2.2
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	printf("Winsock DLL status is %s.\n", wsaData.szSystemStatus);

	do
	{

		// Gather the DNS information from the user.
		getDnsInformation(&ipAddress, &port);

		// Create a DNS Server Object
		DnsServer dnsServer(ipAddress, port);

		try
		{
			dnsServer.openSocket();
			dnsServer.setupServer();
			dnsServer.connectToServer();
			cout << "Quotes Client: Connected to DNS-Like Server at " << ipAddress << " on port " << port << endl;

			// Get the target server information
			getTargetServerInformation(&firstName, &lastName);

			// Request the target server IP address.
			string message = "GET " + lastName + "," + firstName + "\r\n";
			dnsServer.request(message);
			cout << lastName + "," + firstName + " IP address obtained!" << endl;
			dnsServer.closeSocket();
		}
		catch (std::exception& e)
		{
			cout << e.what() << endl;
			system("pause");
			WSACleanup();
			return -1;
		}



		// Create the target server instance.
		Server server(dnsServer.getServerAddr(), 21001);

		try
		{
			server.openSocket();
			server.setupServer();
			server.connectToServer();
			cout << "Connected to " + lastName + "," + firstName + " at " + dnsServer.getServerAddr() +"\n\n";
			// Make requests to the server;
			makeRequests(&server);

			// The user has stopped making requests.
			server.closeSocket();
		}
		catch (std::exception& e)
		{
			cout << e.what() << endl;
			system("pause");
			WSACleanup();
			return -1;
		}

		cout << "Would you like to contact another server?\n";
		cout << "Enter '-1' to close the application. Anthing else to contact another server.\n";
		cin >> userResponse;

		if (userResponse == "-1")
			break;

	} while (true);


	if (WSACleanup() != 0)
	{
		cout << "CLIENT: WSACleanup() failed!..." << endl;
		system("pause");
		return -1;
	}


	return 0;
}