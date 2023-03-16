#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <sys/stat.h>


#include "htmllib.h"

void sendHtml(SOCKET connection, char filedir[]){
	char send_buffer[1000];
	int numsent, numread;
	FILE *sendFile = fopen(filedir, "r");
	if (sendFile == NULL){
		printf("Can't open file.\n");
		return;
	}
	while (!feof(sendFile))
	{
		numread = fread(send_buffer, sizeof(unsigned char), 1000, sendFile);
		if (numread < 1) break; // EOF or error

		char *send_buffer_ptr = send_buffer;
		do // Updating and sending the html code
		{
			numsent = send(connection, send_buffer_ptr, numread, 0);
			if (numsent < 1)  // The connection was interrupted
			{
				if (numsent < 0)
				{
					if (WSAGetLastError() == WSAEWOULDBLOCK)
					{
						fd_set wfd;
						FD_ZERO(&wfd);
						FD_SET(connection, &wfd);
					}
				}

				break; // timeout or error
			}

			send_buffer_ptr += numsent;
			numread -= numsent;
		} while (numread > 0);
	}
}

void sendBinData(SOCKET connection, char filedir[]){
	struct stat buf;
	int size;
	char *filename = filedir;
	//get file size
	if (stat(filename, &buf) != 0)
		perror("Problem getting information\n");
	else
		size = buf.st_size;

	char *send_buffer = malloc(size*sizeof(char));
	if (send_buffer == NULL){
		printf("Out of memory!");
		exit(1);
	}

	FILE *datafile = fopen(filedir, "rb");
	if (datafile == NULL){
		printf("Could not open requested file.\n");
		return;
	}
	size_t result;
	while ((result = fread(send_buffer, 1, 8096, datafile)) > 0){
		if ((send(connection, send_buffer, result, 0)) < 0){
			printf("Failed to send data. Error code: %d", WSAGetLastError());
			return;
		}
	}
	free(send_buffer);
	fclose(datafile);
}