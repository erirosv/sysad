
/*
* File: reqreslib.h
* -----------------------------------------------------
* This interface provides access to functions that handles requests
* and responses to and from an HTTP 1.1 server. The functions reads
* and interprets requests from clients, gets response data, and 
* creates and sends response messages.
*/

#ifndef _reqreslib_h
#define _reqreslib_h

/*include libraries*/

#include <WinSock2.h>
#include <ws2tcpip.h>
#include "cnfglib.h"
#include "cnfglib.h"


/*Function prototypes*/

/*struct definition*/

typedef struct{
	SOCKET S;
	char *IP;
}argT;


/*Section 1 - get header data*/

/*
* Function: getModDate
* Usage: getModDate(modDate);
* ------------------------
* getModDate gets the date of last modification of a 
* file and assigns it to the modDate variable.
*/

void getModDate(char modDate[], char *filedir);

/*
* Function: getResponseDate
* Usage: getResponseDate(buffer);
* ------------------------
* getResponseDate gets the actual date and time in GMT when the
* procedure is called. Typically to be used in an HTTP-response message. 
*/

void getResponseDate(char buffer[]);

/*
* Function: getContentLength
* Usage: getContentLength(buffer);
* ------------------------
* getContentLength reads the size of a file. Typically to
* be used as content-length in an HTTP-response message.
*/

void getContentLength(char buffer[], char *filedir);

/*
* Function: getContentType
* Usage: getContentType(buffer, fileName);
* ------------------------
* getContentType reads the type of the requested file and 
* creates a string representing Content-type in a response
* header, with correct MIME for that type of file. The
* string is assigned to the buffer char-array.
*/

void getContentType(char buffer[], char fileName[]);


/*Section 2 - response headers*/

/*
* Function: sendNotMod
* Usage: sendNotMod(S);
* ------------------------
* sendNotMod creates and sends a "304 Not Modified"-response,
* with appropriate header-lines, to a client through an open
* socket.
*/

void sendNotMod(SOCKET S);

/*
* Function: sendHeader
* Usage: sendHeader(S);
* ------------------------
* sendHeader creates and sends a "200 OK"-response, with
* appropriate header-lines, to a client through an open
* socket.
*/

void sendHeader(SOCKET S, char *filedir, char fileName[]);

/*
* Function: sendNotImpl
* Usage: sendNotImpl(S);
* ------------------------
* sendNotImpl sends a 501-response message to client when 
* requsted method is not supported by the server.
*/

void sendNotImpl(SOCKET S);

/*
* Function: sendNotFound
* Usage: sendNotFound(S);
* ------------------------
* sendNotFound sends a 404-response message to client when
* the requested URL does not exist.
*/

void sendNotFound(SOCKET S);

/*
* Function: sendBadRequest
* Usage: sendBadRequest(S);
* ------------------------
* sendBadRequest sends a 400-response message due to bad syntax
* in request from client or if the HTTP-version is not supported.
*/

void sendBadRequest(SOCKET S);

/*
* Function: sendMTNotSupp
* Usage: sendMTNotSupp(S);
* ------------------------
* sendMTNotSupp sends a 415-response message to client when
* the requested mediatype is not supported by the server.
*/

void sendMTNotSupp(SOCKET S);

/*Section 3 - handle and interpret requests*/

/*
* Function: getMethod
* Usage: index = getMethod(char buffer[], char method[], int metSize);
* ------------------------
* getMethod reads the request-method from client and assigns it to the 
* method char-array. It also returns the index after method-string in
* the requset-buffer.
*/

void getMethod(char buffer[], char method[], int metSize);

/*
* Function: checkURL
* Usage: if(checkURL(fileName))...
* ------------------------
* checkURL is used as a boolean function to determine if requested URL 
* contains invalid characters.
*/

int checkURL(char *fileName);

/*
* Function: checkMT
* Usage: if(checkMT(ileName))...
* ------------------------
* checkMT is used as a boolean function to determine if requested 
* media type is supported by server.
*/

int checkMT(char *fileName);

/*
* Function: checkConditional
* Usage: ptr = checkConditional(buffer, reqDate, bufSize, rdSize);
* ------------------------
* checkConditional checks if the request sent from client is a conditional
* request. If it finds the "If-modified-since"-statements the function
* a pointer with the memory location of that statement, which is later
* returned by the function, and reads the subsequent date, which is
* assigned to reqDate char-array.
*/

char *checkConditional(char buffer[], char reqDate[], int bufSize, int rdSize);

/*
* Function: getReqFile
* Usage: getReqFile(buffer, filename, startindex, bufSize);
* ------------------------
* getReqFile reads which file is requested from client and assigns
* the fileName to the filename char-array
*/

void getReqFile(char buffer[], char filename[], int bufSize);

/*
* Function: createFilePath
* Usage: ptr = createFilePath(directory, fileName);
* ------------------------
* createFilePath append a filename with the directory where the file
* is stored to create a full path of that file, which is returned from
* the function.
*/

char *createFilePath(char *directory, char fileName[]);

/*
* Function: writeToLogFile
* Usage: writeToLogFile(logInfo);
* ------------------------
* writeToLogFile writes the information in a logT-struct
* to a textfile
*/

void writeToLogFile(logT logInfo);

/*
* Function: handleRequest
* Usage: ptr = createFilePath(directory, fileName);
* ------------------------
* handleRequest append a filename with the directory where the file
* is stored to create a full path of that file, which is returned from
* the function.
*/

void handleRequest(SOCKET S, char *buffer, char *fileName, char *filePath, char *method, logT logInfo);

/*
* Function: recieveRequest
* Usage: recieve(S);
* ------------------------
* recieveRequest uses functions to intepret incoming request
* and controls which request-method is sent from client
* and then decides what actions to take.
*/

DWORD WINAPI recieveRequest(LPVOID pParam);

#endif