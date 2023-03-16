/*Include libraries*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <Windows.h>

#include "cnfglib.h"
#include "htmllib.h"

/*Import global variables for main-file*/
extern int activationKey;
extern int authStatus;
extern configT configInfo;

/*Export functions*/

void recieveStrToUp(SOCKET S, char *buffer){
	int iResult, i = 0;
	char buf[2];
	while (1){
		if ((iResult = recv(S, buf, 1, 0)) == SOCKET_ERROR){
			printf("Failed to recieve data. Error code: %d", WSAGetLastError());
		}
		else{
			if (i < 0)
				i = 0;
			if (buf[0] == '\r'){
				buffer[i] = '\0';
				do{
					iResult = recv(S, buf, 1, 0);
				} while (buf[0] != '\n'); //clears the buffer from \n following the \r
				break;
			}
			else if (buf[0] == '\b') //allows backspace if input needs changes
				i--;
			else{
				buffer[i] = toupper(buf[0]);
				i++;
			}
		}
	}
}

void adminMenu(char *buffer){

	strcpy(buffer, "Possible commands:\r\n\r\n");
	strcat(buffer, "AUTHORIZE <PASSWORD>	- Enter password to be able to use server commands\r\n");
	strcat(buffer, "SERVER START		- activates server request listening\r\n");
	strcat(buffer, "SERVER STOP		- deactivates server request listening\r\n");
	strcat(buffer, "ISACTIVE		- responds if server is listening for requests\r\n");
	strcat(buffer, "GETLOGFILE		- returns the server logfile\r\n");
	strcat(buffer, "QUIT			- exits admin mode\r\n\r\n");
}

void checkCommand(SOCKET S, char *buffer){
	char message[255];
	if (strcmp(buffer, "SERVER START") == 0){
		activationKey = 1;
		strcpy(message, "Server activated\r\n\r\n");
		printf("Server activated\r\n\r\n");
	}
	else if (strcmp(buffer, "SERVER STOP") == 0){
		activationKey = 0;
		strcpy(message, "Server deactivated\r\n\r\n");
		printf("Server deactivated\r\n\r\n");
	}
	else if (strcmp(buffer, "ISACTIVE") == 0){
		if (activationKey == 1)
			strcpy(message, "Responding to incoming requests.\r\n\r\n");
		else
			strcpy(message, "Ignoring incoming requests.\r\n\r\n");
	}
	else if (strcmp(buffer, "GETLOGFILE") == 0){
		sendHtml(S, configInfo->logDir);
		strcpy(message, "\r\n");
	}
	else
		strcpy(message, "Invalid command. ");
	if ((send(S, message, strlen(message), 0)) < 0)
		printf("Failed to send data in checkCommand. Error code: %d", WSAGetLastError());
}

void readPassword(char *password){
	strcpy(password, "AUTHORIZE ");
	int i = strlen(password), j = 0;
	while (configInfo->password[j] != '\0'){
		password[i] = toupper(configInfo->password[j]);
		i++;
		j++;
	}
	password[i] = '\0';

}

void adminMode(SOCKET S){
	char buffer[1024], message[255]; 

	strcpy(buffer, "\r\n");
	strcat(buffer, "Welcome to localhost-server admin mode\r\n\r\n");
	if ((send(S, buffer, strlen(buffer), 0)) < 0){
		printf("Failed to send data in adminMode. Error code: %d", WSAGetLastError());
		return;
	}
	adminMenu(buffer);
	if ((send(S, buffer, strlen(buffer), 0)) < 0){
		printf("Failed to send data in adminMode. Error code: %d", WSAGetLastError());
		return;
	}
	while (strcmp(buffer, "QUIT") != 0){
		strcpy(buffer, "Enter command (HELP to display menu): ");
		if ((send(S, buffer, strlen(buffer), 0)) < 0){
			printf("Failed to send data in adminMode. Error code: %d", WSAGetLastError());
			return;
		}
		recieveStrToUp(S, buffer);
		if (strstr(buffer, "AUTHORIZE") != NULL){
			readPassword(message); //sets password comparison string by getting password from config-struct
			if (strcmp(buffer, message) == 0){ 
				authStatus = 1;
				strcpy(message, "Password correct, you now have administrators access\r\n\r\n");
			}
			else{
				authStatus = 0;
				strcpy(message, "Password incorrect.\r\n\r\n");
			}
			if ((send(S, message, strlen(message), 0)) < 0){
				printf("Failed to send data in adminMode. Error code: %d", WSAGetLastError());
				return;
			}
		}
		else if (strcmp(buffer, "HELP") == 0){
			adminMenu(buffer);
			if ((send(S, buffer, strlen(buffer), 0)) < 0){
				printf("Failed to send data in adminMode. Error code: %d", WSAGetLastError());
				return;
			}
		}
		else if (strcmp(buffer, "QUIT") == 0){
			authStatus = 0;
			continue;
		}
		else if (authStatus == 0){
			strcpy(message, "You must enter password for authorisation before you can use server commands\r\n\r\n");
			if ((send(S, message, strlen(message), 0)) < 0){
				printf("Failed to send data in adminMode. Error code: %d", WSAGetLastError());
				return;
			}
		}
		else
			checkCommand(S, buffer);
	}
}


DWORD WINAPI telnetHandler(LPVOID lpParam){
	SOCKET s;
 
	if ((s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET){
		printf("Creating telnet socket failed. Error code: %d", WSAGetLastError);
		WSACleanup();
		return(1);
	}

	struct addrinfo *info;
	int ok = getaddrinfo("localhost", configInfo->telnetPort, NULL, &info);
		if (ok != 0) {
		WCHAR * error = gai_strerror(ok);
		printf("%S\n", error);
	}
	else while (info->ai_family != AF_INET && info->ai_next != NULL)
		info = info->ai_next;

	if ((ok = bind(s, info->ai_addr, info->ai_addrlen)) == SOCKET_ERROR){
		printf("Failed to bind telnet-sock. Error code: %d", WSAGetLastError);
		closesocket(s);
		WSACleanup();
		return(1);
	}

	if ((ok = listen(s, SOMAXCONN)) == SOCKET_ERROR){
		printf("Failed to listen. Error code: %d", WSAGetLastError);
		closesocket(s);
		WSACleanup();
		return(1);
	}


	struct sockaddr_in clienttelnet; 
	int clientAddrLen = sizeof(struct sockaddr_in);
	while (1){
		SOCKET s1 = accept(s, (struct sockaddr *)&clienttelnet, &clientAddrLen);
		if (s1 != INVALID_SOCKET) {
			char *client_telnet = inet_ntoa(clienttelnet.sin_addr);
			int client_port_telnet = ntohs(clienttelnet.sin_port);
			if (client_telnet != NULL){
				printf("Accept incoming telnet-call from: %s at port %d\n", client_telnet, client_port_telnet);
				adminMode(s1);
				closesocket(s1);
			}
			else{
				printf("Error trying to accept telnet connection. Error code: %d\n", WSAGetLastError);
				continue;
			}
		}
	}
	closesocket(s);
	return(0);
}

void telnetThreadStartup(void){
	HANDLE thread;
	DWORD threadID;

	thread = CreateThread(NULL, 0, telnetHandler, 0, 0, &threadID);
	if (thread == NULL){
		perror("Could not create thread.");
		return;
	}
	return;
}