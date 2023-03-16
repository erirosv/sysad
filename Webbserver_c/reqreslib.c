
/*Include libraries*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
#include <WinSock2.h>
#include <ws2tcpip.h>

#include "cnfglib.h"
#include "reqreslib.h"
#include "htmllib.h"


/*Constant definitions*/

#define SERVER_NAME "Server: localserver/0.1.0\r\n"
#define ARRAY_SIZE 55
#define BUFFER_SIZE 2049
#define FILETYPE_SIZE 5
#define HTML_FILE "catSite.html"

/*Import global variable*/

extern configT configInfo;


/*Export entries*/

/*Section 1 - get header data*/

void getModDate(char modDate[], char *filedir){
	struct stat buf;
	struct tm *gt;
	char time[50];

	/* Get File Statistics for stat.c. */
	if (stat(filedir, &buf) != 0)
		perror("Problem getting information");
	else{ /* get the last-modified-date in RFC 1123 standard*/
		gt = gmtime(&buf.st_mtime);
		strftime(time, 50, "%a, %d %b %Y %X", gt);
		strcpy(modDate, time);
	}
}

void getResponseDate(char buffer[]){
	time_t currTime;
	struct tm *gt;
	char respDate[50];

	time(&currTime);
	gt = gmtime(&currTime);
	strftime(respDate, 50, "%a, %d %b %Y %X", gt); /* get the current date and time in RFC 1123 standard*/
	strcpy(buffer, respDate);

}

void getContentLength(char buffer[], char *filedir){
	struct stat buf;
	int size;

	if (stat(filedir, &buf) != 0)
		perror("Problem getting information");
	else{ /*Get size of file*/
		size = buf.st_size;
		sprintf(buffer, "%d", size);
	}
}

void getContentType(char buffer[], char fileName[]){
	char fileType[FILETYPE_SIZE];
	int i = 0, j = 0;
	while (!ispunct(fileName[i]) && (i < ARRAY_SIZE - 1))
		i++;
	i++;
	while ((fileName[i] != '\0') && (j < FILETYPE_SIZE-1)){
		fileType[j] = fileName[i];
		i++;
		j++;
	}
	fileType[j] = '\0'; //filename extension read into fileType
	// extension controlled for appropriate MIME
	if (strcmp(fileType, "html") == 0)
		strcpy(buffer, "Content-Type: text/html\r\n");
	else if (strcmp(fileType, "jpg") == 0)
		strcpy(buffer, "Content-type: image/jpg\r\n");
	else if (strcmp(fileType, "gif") == 0)
		strcpy(buffer, "Content-type: image/gif\r\n");
	else if (strcmp(fileType, "png") == 0)
		strcpy(buffer, "Content-type: image/png\r\n");
	else if (strcmp(fileType, "ico") == 0)
		strcpy(buffer, "Content-type: image/x-icon\r\n");
	else if (strcmp(fileType, "zip") == 0)
		strcpy(buffer, "Content-type: application/zip\r\n");
	else
		strcpy(buffer, "application/octet-stream\r\n");
}

/*Section 2 - response headers*/

void sendNotMod(SOCKET S){
	char buffer[1024], message[55];

	strcpy(buffer, "HTTP/1.1 304 Not Modified\r\nDate: ");
	getResponseDate(message);
	strcat(buffer, message);
	strcat(buffer, " GMT\r\n");
	strcat(buffer, SERVER_NAME);
	strcat(buffer, "\r\n");
	if ((send(S, buffer, strlen(buffer), 0)) < 0){
		printf("Could not send response in sendNotMod. Error code: %d", WSAGetLastError());
		return;
	}
}

void sendHeader(SOCKET S, char *filedir, char fileName[]){
	char buffer[1024], message[255];

	strcpy(buffer, "HTTP/1.1 200 OK\r\nDate: ");
	getResponseDate(message);
	strcat(buffer, message);
	strcat(buffer, " GMT\r\n");
	strcat(buffer, SERVER_NAME);
	strcat(buffer, "Last-Modified: ");
	getModDate(message, filedir);
	strcat(buffer, message);
	strcat(buffer, " GMT\r\n");
	getContentType(message, fileName);
	strcat(buffer, message);
	strcat(buffer, "\r\n");
	if ((send(S, buffer, strlen(buffer), 0)) < 0){
		printf("Could not send response in snedHeader. Error code: %d", WSAGetLastError());
		return;
	}
}

void sendNotImpl(SOCKET S){ //405
	char buffer[1024], message[55];

	strcpy(buffer, "HTTP/1.1 405 Method Not Allowed\r\nDate: ");
	getResponseDate(message);
	strcat(buffer, message);
	strcat(buffer, " GMT\r\n");
	strcat(buffer, SERVER_NAME);
	strcat(buffer, "Content-Type: text/html\r\n");
	strcat(buffer, "\r\n");
	strcat(buffer, "<HTML><HEAD><TITLE>405 - Method Not Allowed\r\n");
	strcat(buffer, "</TITLE></HEAD>\r\n");
	strcat(buffer, "<BODY><P>HTTP request method not supported\r\n");
	strcat(buffer, "</BODY></HTML>\r\n");
	if ((send(S, buffer, strlen(buffer), 0)) < 0){
		printf("Could not send response in sendNotImpl. Error code: %d", WSAGetLastError());
		return;
	}
}

void sendNotFound(SOCKET S){ // 404
	char buffer[1024], message[55];

	strcpy(buffer, "HTTP/1.1 404 Not Found\r\nDate: ");
	getResponseDate(message);
	strcat(buffer, message);
	strcat(buffer, " GMT\r\n");
	strcat(buffer, SERVER_NAME);
	strcat(buffer, "Content-Type: text/html\r\n");
	strcat(buffer, "\r\n");
	strcat(buffer, "<HTML><HEAD><TITLE>404 - Not Found\r\n");
	strcat(buffer, "</TITLE></HEAD>\r\n");
	strcat(buffer, "<BODY><P>HTTP Requested URL could not be found,\r\n");
	strcat(buffer, "it is either unavailable or nonexistent.\r\n");
	strcat(buffer, "Please control if the input address is correct.\r\n");
	strcat(buffer, "</BODY></HTML>\r\n");
	if ((send(S, buffer, strlen(buffer), 0)) < 0){
		printf("Could not send response in sendNotFound. Error code: %d", WSAGetLastError());
		return;
	}
}

void sendBadRequest(SOCKET S){ // 400
	char buffer[1024], message[55];

	strcpy(buffer, "HTTP/1.1 400 Bad Request\r\nDate: ");
	getResponseDate(message);
	strcat(buffer, message);
	strcat(buffer, " GMT\r\n");
	strcat(buffer, SERVER_NAME);
	strcat(buffer, "Content-Type: text/html\r\n");
	strcat(buffer, "\r\n");
	strcat(buffer, "<HTML><HEAD><TITLE>400 - Bad Request\r\n");
	strcat(buffer, "</TITLE></HEAD>\r\n");
	strcat(buffer, "<BODY><P>HTTP-request written with bad syntax or \r\n");
	strcat(buffer, "it has wrong http-version. \r\n");
	strcat(buffer, "Please control request.\r\n");
	strcat(buffer, "</BODY></HTML>\r\n");
	if ((send(S, buffer, strlen(buffer), 0)) < 0){
		printf("Could not send response in sendBadRequest. Error code: %d", WSAGetLastError());
		return;
	}
}

void sendMTNotSupp(SOCKET S){ //415
	char buffer[1024], message[55];

	strcpy(buffer, "HTTP/1.1 415 Unsupported Media Type\r\nDate: ");
	getResponseDate(message);
	strcat(buffer, message);
	strcat(buffer, " GMT\r\n");
	strcat(buffer, SERVER_NAME);
	strcat(buffer, "Content-Type: text/html\r\n");
	strcat(buffer, "\r\n");
	strcat(buffer, "<HTML><HEAD><TITLE>415 - Unsupported Media Type\r\n");
	strcat(buffer, "</TITLE></HEAD>\r\n");
	strcat(buffer, "<BODY><P>HTTP requested media type is not \r\n");
	strcat(buffer, "supported by the server.\r\n");
	strcat(buffer, "</BODY></HTML>\r\n");
	int s = (send(S, buffer, strlen(buffer), 0));
	if (s < 0){
		printf("Could not send response in sendMTNotSupp. Error code: %d", WSAGetLastError());
		return;
	}
}


/*Section 3 - handle and interpret requests*/

void getMethod(char buffer[], char method[], int metSize){
	int  i = 0, j = 0;

	while (!isspace(buffer[i]) && (j < metSize - 1)){ //reads first word in request to get method
		method[j] = buffer[i];
		i++;
		j++;
	}
	method[j] = '\0';
}

int checkURL(char *fileName){
	char *OKchars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz"
		"0123456789"
		"/._"; //allowed characters in localServer.
	int i = 0;

	while (fileName[i] != '\0'){ // URL characters compared with allowed characters
		if (strchr(OKchars, fileName[i]) == NULL)
			return(0);
		i++;
	}
	return(1);
}

int checkMT(char *fileName){
	char *mediaTypes[] = { "html", "jpg", "gif", "png", "zip", "ico"}; //Supported file extensions
	char mt[25];
	int i = 0, j = 0;
	while (fileName[i] != '.'){
		if (fileName[i] == '\0')
			return(0);
		i++;
	}
	i++;
	while (fileName[i] != '\0'){
		mt[j] = fileName[i];
		i++;
		j++;
	}
	mt[j] = '\0';
	for (i = 0; i < 6; i++){
		if (strcmp(mediaTypes[i], mt) == 0) //if file extension matches one of the supported
			return(1);
	}
	return(0);
}

char *checkConditional(char buffer[], char reqDate[], int bufSize, int rdSize){
	char *lastMod = NULL;
	int i, j = 0;
	lastMod = strstr(buffer, "If-Modified-Since:");
	if (lastMod != NULL){ //If request is conditional get
		i = lastMod - buffer;
		while (!isspace(buffer[i]) && (i < bufSize - 1))
			i++;
		while (isspace(buffer[i]) && (i < bufSize - 1))
			i++;
		while ((buffer[i] != '\r') && (j < rdSize - 1)){
			reqDate[j] = buffer[i];
			i++;
			j++;
		}
		reqDate[j] = '\0';
	}
	return(lastMod);
}

void getReqFile(char buffer[], char filename[], int bufSize){
	int j = 0, i = 0;
	while (!isspace(buffer[i]) && (i < bufSize - 1))
		i++;
	while (isspace(buffer[i]) && (i < bufSize - 1))
		i++;
	while (ispunct(buffer[i]) && (i < bufSize - 1))
		i++;
	while (!isspace(buffer[i]) && (i < bufSize - 1)){
		filename[j] = buffer[i];
		i++;
		j++;
	}
	filename[j] = '\0';
	if (filename[0] == '\0')
		strcpy(filename, HTML_FILE);
}

char *createFilePath(char *directory, char fileName[]){
	char *filePath;
	int sLength;
	sLength = strlen(directory);
	sLength += strlen(fileName);
	sLength++;
	filePath = malloc(sLength*sizeof(char));
	if (filePath == NULL){
		printf("Error allocating memory in createFilePath, out of memory. Exiting program.\n");
		system("Pause");
		exit(1);
	}
	filePath = strcpy(filePath, directory);
	filePath = strcat(filePath, fileName);
	return(filePath);
}

void writeToLogFile(logT logInfo){
	FILE *infile;
	int count = 0;

	while (1){
		infile = fopen(configInfo->logDir, "a");
		if (infile == NULL && count <= 5){ //If file i used by other thread, wait and try again
			Sleep(25);
		}
		else if (count > 5){ //if file could not be opened in five tries, skip writing to log-file.
			printf("Error! Could not open logfile to record client-request\n");
			break;
		}
		else{ //log-file opened, write data to it
			fprintf(infile, "%s %s\r\n", logInfo.IP_addr, logInfo.date);
			fprintf(infile, "\"%s\" %s %s\r\n\r\n", logInfo.URL, logInfo.respCode, logInfo.bytes);
			fclose(infile);
			break;
		}
	}
}

void handleRequest(SOCKET S, char *buffer, char *fileName, char *filePath, char *method, logT logInfo){
	char reqFileDate[ARRAY_SIZE], modDate[ARRAY_SIZE], *lastMod = NULL;

	lastMod = checkConditional(buffer, reqFileDate, BUFFER_SIZE, ARRAY_SIZE); //returns value to pointer if conditional get
	getModDate(modDate, filePath);
	
	if (strcmp(method, "GET") == 0){
		if (lastMod != NULL){ //If conditional GET
			if (strstr(reqFileDate, modDate) != NULL){ //if file not modified
				getLogRespCode("304", logInfo.respCode);
				sendNotMod(S);
				printf("304-response sent on thread %d.\n", GetCurrentThreadId());
			}
			else{ //If modified after "If-modified-since"-date
				getLogRespCode("200", logInfo.respCode);
				sendHeader(S, filePath, fileName);
				if (strcmp(fileName, "catSite.html") == NULL)
					sendHtml(S, filePath);
				else
					sendBinData(S, filePath);
				printf("200-response sent on thread %d.\n", GetCurrentThreadId());
			}
		}
		else{ //If GET-request
			getLogRespCode("200", logInfo.respCode);
			sendHeader(S, filePath, fileName);
			if (strcmp(fileName, "catSite.html") == 0)
				sendHtml(S, filePath);
			else
				sendBinData(S, filePath);
			printf("200-response sent on thread %d.\n", GetCurrentThreadId());
		}
	}
	else if (strstr(method, "HEAD") != NULL){ //HEAD-request
		getLogRespCode("200", logInfo.respCode);
		sendHeader(S, filePath, fileName);
		printf("200-response sent on thread %d.\n", GetCurrentThreadId());
	}
	else{ //If method is not matched with any allowed in localServer
		getLogRespCode("405", logInfo.respCode);	
		sendNotImpl(S);
		printf("405-response sent on thread %d.\n", GetCurrentThreadId());
	}
	writeToLogFile(logInfo);
}

DWORD WINAPI recieveRequest(LPVOID pParam){

	//Assign data from thread-argument to seperate variables
	argT *temp = (argT*)pParam;
	argT threadArg = *temp;
	SOCKET S = threadArg.S;
	logT logInfo;
	getLogClientIP(threadArg.IP, logInfo.IP_addr);


	//Create variables to parse request
	char buffer[BUFFER_SIZE], method[ARRAY_SIZE], fileName[ARRAY_SIZE], message[ARRAY_SIZE], *filePath;
	int iResult;
	FILE *infile;

	//recieve request from client
	printf("Connection accepted on thread %d\n", GetCurrentThreadId());
	iResult = recv(S, buffer, 2048, 0);
	if (iResult < 1){
		printf("No data sent from client.\n");
		return(1);
	}
	else
		printf("Client request recieved.\n");
	buffer[iResult] = '\0';

	//parse client-request
	getMethod(buffer, method, ARRAY_SIZE); //reads request-method
	getReqFile(buffer, fileName, BUFFER_SIZE); //reads requested file
	filePath = createFilePath(configInfo->webDir, fileName); //Gets the whole path for the requested file

	//Assign logfile-information to log-struct
	getLogURL(method, fileName, logInfo.URL);
	getLogDate(logInfo.date);
	getLogSize("0", logInfo.bytes); //set default size of file to 0, which is printed to logfile if there is an error in client request.

	if (strstr(buffer, "HTTP/1.1") == NULL || !checkURL(fileName)){ //check if bad request syntax
		getLogRespCode("400", logInfo.respCode);
		writeToLogFile(logInfo);
		sendBadRequest(S); //bad syntax, 400-response sent
		printf("400-response sent on thread %d.\n", GetCurrentThreadId());
		free(filePath);
		closesocket(S);
		return(1);
	}
	if (!checkMT(fileName)){ //check if mediatype is supported
		getLogRespCode("415", logInfo.respCode);
		writeToLogFile(logInfo);
		sendMTNotSupp(S); // mediatype not supported, 415-response sent
		printf("415-response sent on thread %d.\n", GetCurrentThreadId());
		free(filePath);
		closesocket(S);
		return(1);
	}
	infile = fopen(filePath, "r"); //Check if requested file exists
	if (infile == NULL){
		getLogRespCode("404", logInfo.respCode);
		writeToLogFile(logInfo);
		sendNotFound(S); //nonexistent file, sends 404-response sent
		printf("404-response sent on thread %d.\n", GetCurrentThreadId());
		free(filePath);
		closesocket(S);
		return(1);
	}
	else{ //requested file exists and no other request-errors occurred                        
		fclose(infile);
		getContentLength(message, filePath); //if no client request-error, size of file can be calculated.
		getLogSize(message, logInfo.bytes);
		handleRequest(S, buffer, fileName, filePath, method, logInfo); 
	}
	free(filePath);
	closesocket(S);
	return(0);
}
