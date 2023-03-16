
/*
* File: htmllib.h
* -----------------------------------------------------
* This interface provides access to functions that sends data
* from webserver to the client-browser.
*/


#ifndef _htmllib_h
#define _htmllib_h


#include <WinSock2.h>
#include <ws2tcpip.h>

/*
* Function: sendHtml
* Usage: sendHtml(connection, filedir);
* ------------------------
* sendHtml sends a text-file through a socket-connection.
*/

void sendHtml(SOCKET connection, char filedir[]);

/*
* Function: sendBinData
* Usage: sendBinData(connection, filedir);
* ------------------------
* sendBinData sends non-text data through a socket-connection.
*/

void sendBinData(SOCKET connection, char filedir[]);

#endif