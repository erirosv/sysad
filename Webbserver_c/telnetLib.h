/*
* File: telnetLib.h
* -----------------------------------------------------
* This interface provides access to functions that runs 
* an administration-connection to the server through a 
* telnet-connection. The functions are written to run on
* an own thread in the program.
*/

#ifndef _telnetLib_h
#define _telnetLib_h

/*include libraries*/

#include <WinSock2.h>
#include <ws2tcpip.h>
#include <Windows.h>


/*Function prototypes*/

/*
* Function: recieveStrToUp
* Usage: recieveStrToUp(Sock, buffer);
* ------------------------
* recieveStrToUp recieves data sent from client and reads
* the characters into a buffer-string.
*/

void recieveStrToUp(SOCKET S, char *buffer);

/*
* Function: adminMenu
* Usage: adminMenu(buffer);
* ------------------------
* adminMenu reads the menu of the administration-login
* into a buffer-string, which is later sent to client.
*/

void adminMenu(char *buffer);

/*
* Function: checkCommand
* Usage: checkCommand(Sock, buffer);
* ------------------------
* checkCommand is called only when a client has entered a correct
* password for admin authorisation. checkCommand then lets client
* use the server command options.
*/

void checkCommand(SOCKET S, char *buffer);

/*
* Function: readPassword
* Usage: readPassword(password);
* ------------------------
* readPassword reads the password from configT variable and
* creates a comparable string variable with following format:
* AUTHORIZE <PASSWORD>. The password is converted to capital 
* letters since the telnet-administration function only reads
* capital letters.
*/

void readPassword(char *password);

/*
* Function: adminMode
* Usage: adminMode(Sock);
* ------------------------
* adminMode controls inputstring from client and decides
* if input-string is a valid option, otherwise the client 
* is noticed. The main purpose of this function is to control
* so that client enters and sends a correct password to get
* authorisation to use server commands. 
*/

void adminMode(SOCKET S);

/*
* Function: telnetHandler
* Usage: thread = CreateThread(Null, 0, telnetHandler, 0, 0, &threadID);
* ------------------------
* telnetHandler the threadfunction used to start listening
* administration connection through telnet.
*/

DWORD WINAPI telnetHandler(LPVOID lpParam);

/*
* Function: telnetThreadStartup
* Usage: telnetThreadStartup();
* ------------------------
* telnetThreadStartup is used in the beginning of the main-program
* to start a thread on which a socket is listening for connection
* to the telnet administration connection.
*/

void telnetThreadStartup(void);

#endif