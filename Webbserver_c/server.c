#include <stdio.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <Windows.h>
#include <process.h>

//Our own lib
#include "cnfglib.h"
#include "reqreslib.h"
#include "telnetLib.h"


/*global variables*/
configT configInfo; //Contains important data from config-file
int activationKey = 0;	//Flag to set server on and off
int authStatus = 0; //Used to control if telnet-client is authorized with admininstrator-status



main(){
	//Initializing the config file
	configInfo = configInit();
	readConfigFile(configInfo);

	//variables used to unblock accept-function
	struct timeval tv;
	fd_set readfds;
	tv.tv_sec = 1;
	tv.tv_usec = 500000;

	//Initialize winsock
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);
	if (WSAStartup(wVersionRequested, &wsaData) != 0){
		printf("WSAStartup failed. Error code: %d", WSAGetLastError());
		return;
	}
	printf("Server initialized.\n");

	//Start telnetThread
	telnetThreadStartup();
	activationKey = 1;

	//Create variables and initialize WSA
	while (1){
		int iResult;
		if (activationKey == 1){
			SOCKET serverSock;
			if ((serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET){
				printf("Creating server socket failed. Error code: %d", WSAGetLastError);
				WSACleanup();
				return;
			}
			printf("Socket created.\n");

			//Get adressinformation of server
			struct addrinfo *info;
			int adInfo = getaddrinfo("localhost", configInfo->incomPort, NULL, &info);
			if (adInfo != 0){
				WCHAR * error = gai_strerror(adInfo);
				printf("%s", error);
				closesocket(serverSock);
				WSACleanup();
				return;
			}
			else{
				while (info->ai_family != AF_INET && info->ai_next != NULL)
					info = info->ai_next;
			}

			//bind socket to adress
			if ((adInfo = bind(serverSock, info->ai_addr, info->ai_addrlen)) == SOCKET_ERROR){
				printf("Failed to bind. Error code: %d", WSAGetLastError);
				closesocket(serverSock);
				WSACleanup();
				return;
			}
			printf("Bind done.\n");

			//Start listening for incoming connections
			if ((adInfo = listen(serverSock, SOMAXCONN)) == SOCKET_ERROR){
				printf("Failed to listen. Error code: %d", WSAGetLastError);
				closesocket(serverSock);
				WSACleanup();
				return;
			}
			printf("Listening for incoming connections.\n\n");

			struct sockaddr_in client; //Create struct that recieves IP- and portnumber from connecting client
			int clientAddrLen = sizeof(struct sockaddr_in);
			while (activationKey == 1){
				FD_ZERO(&readfds);
				FD_SET(serverSock, &readfds);
				argT threadArg;
				HANDLE thread;
				DWORD threadID;

				iResult = select(serverSock + 1, &readfds, NULL, NULL, &tv); //sets a timout for non-blocking of accept()-function
				if (iResult == 0 || iResult == SOCKET_ERROR)
					continue;
				else{
					SOCKET s1 = accept(serverSock, (struct sockaddr *)&client, &clientAddrLen);
					if (s1 == INVALID_SOCKET){
						printf("Error trying to accept connection. Error code: %d\n", WSAGetLastError);
						continue;
					}
					else{
						char *client_ip = inet_ntoa(client.sin_addr);
						int client_port = ntohs(client.sin_port);
						if (client_ip != NULL){
							printf("Accept incoming from: %s at port: %d\n", client_ip, client_port);
							threadArg.IP = client_ip;
							threadArg.S = s1;
							thread = CreateThread(NULL, 0, recieveRequest, &threadArg, 0, &threadID);
							if (thread == NULL)
								printf("Could not create thread. Error message: %s", GetLastError());
							Sleep(30);
						}
					}
				}
			}
			closesocket(serverSock);
		}
	}
	freeAllMem(configInfo);
	WSACleanup();
}
