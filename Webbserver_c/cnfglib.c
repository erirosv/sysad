/*Include libraries*/
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <time.h>


#include "cnfglib.h"


/*
 * Global variables
 *---------------------------
 * These variables are used set the sizes of the
 * dynamic vectors used in a configT variable.
 */

static int arraySizeWeb = 100;
static int arraySizeLog = 100;


/*Export Entries*/

/*Section 1 - configT*/

configT configInit(void){
	configT configVar;
	int i = 0;
	configVar = malloc(sizeof(*configVar));
	configVar->webDir = malloc(sizeof(char)*arraySizeWeb);
	configVar->logDir = malloc(sizeof(char)*arraySizeLog);
	if (configVar == NULL || configVar->webDir == NULL || configVar->logDir == NULL){
		printf("Error allocating memory, out of memory. Exiting program.\n");
		system("Pause");
		exit(1);
	}
	configVar->incomPort[i] = '\0';
	configVar->telnetPort[i] = '\0';
	configVar->password[i] = '\0';
	return(configVar);
}

void readConfigFile(configT configVar){
	FILE *infile = fopen("Config\\config.txt", "r");
	if (infile == NULL){
		printf("Error reading config file. Exiting program.\n");
		system("Pause");
		exit(1);
	}
	int counter = 1, i, countWeb = 0, countLog = 0, countIPIn = 0, countIPT = 0, countPass = 0;
	char ch;
	while (1){
		ch = getc(infile);
		if (ch == EOF) break;
		if (ch == '\n'){
			counter++;
			i = 0;
			ch = getc(infile);
			if (ch == EOF) break;
		}
		switch (counter){ // counter counts line read in config-file. Lines with even numbers contain variable-data to read to struct.
		case 2:
			configVar->incomPort[i] = ch;
			i++;
			countIPIn++;
			break;
		case 4:
			configVar->telnetPort[i] = ch;
			i++;
			countIPT++;
			break;
		case 6:
			if (i > 11){
				printf("Config file Error. Password too long. Exiting program.\n");
				system("Pause");
				exit(0);
			}
			configVar->password[i] = ch;
			i++;
			countPass++;
			break;
		case 8:
			if (i == arraySizeWeb)
				expandWebArray(configVar);
			configVar->webDir[i] = ch;
			i++;
			countWeb++;
			break;
		case 10:
			if (i == arraySizeLog)
				expandLogArray(configVar);
			configVar->logDir[i] = ch;
			i++;
			countLog++;
			break;
		default:
			break;
		}
	}
	configVar->incomPort[countIPIn] = '\0';
	configVar->telnetPort[countIPT] = '\0';
	configVar->password[countPass] = '\0';
	setEndArrays(configVar, countWeb, countLog);
	printf("Config file loaded.\n");
}

void expandWebArray(configT configVar){
	int interval = arraySizeWeb;
	arraySizeWeb *= 2;
	char *tempChar = malloc(sizeof(char)*arraySizeWeb);
	if (tempChar == NULL){
		printf("Error allocating memory, out of memory. Exiting program.\n");
		system("Pause");
		exit(1);
	}
	int i;
	for (i = 0; i < interval; i++)
		tempChar[i] = configVar->webDir[i];
	free(configVar->webDir);
	configVar->webDir = tempChar;
}

void expandLogArray(configT configVar){
	int interval = arraySizeLog;
	arraySizeLog *= 2;
	char *tempChar = malloc(sizeof(char)*arraySizeLog);
	if (tempChar == NULL){
		printf("Error allocating memory, out of memory. Exiting program.\n");
		system("Pause");
		exit(1);
	}
	int i;
	for (i = 0; i < interval; i++)
		tempChar[i] = configVar->logDir[i];
	free(configVar->logDir);
	configVar->logDir = tempChar;
}

void setEndArrays(configT configVar, int sizeWeb, int sizeLog){
	char *tempArrayWeb = malloc(sizeof(char)*(sizeWeb + 1));
	if (tempArrayWeb == NULL){
		printf("Error allocating memory, out of memory. Exiting program.\n");
		system("Pause");
		exit(1);
	}
	char *tempArrayLog = malloc(sizeof(char)*(sizeLog + 1));
	if (tempArrayLog == NULL){
		printf("Error allocating memory, out of memory. Exiting program.\n");
		system("Pause");
		exit(1);
	}
	int i;
	for (i = 0; i < sizeWeb; i++)
		tempArrayWeb[i] = configVar->webDir[i];
	for (i = 0; i < sizeLog; i++)
		tempArrayLog[i] = configVar->logDir[i];
	free(configVar->webDir);
	free(configVar->logDir);
	configVar->webDir = tempArrayWeb; //sets array-size to the exact string-size
	configVar->webDir[sizeWeb] = '\0';
	configVar->logDir = tempArrayLog; //sets array-size to the exact string-size
	configVar->logDir[sizeLog] = '\0';
}

void printConfigInfo(configT configVar){
	printf("%s\n", configVar->incomPort);
	printf("%s\n", configVar->telnetPort);
	printf("%s\n", configVar->password);
	printf("%s\n", configVar->webDir);
	printf("%s\n", configVar->logDir);
}

void freeAllMem(configT configVar){
	free(configVar->logDir);
	free(configVar->webDir);
	free(configVar);
}

/*Section 2 - logT*/

void getLogClientIP(char *IP, char *logIP){
	int i = 0, j = 0;
	while ((IP[i] != '\0') && (j < IP_LEN-1)){
		logIP[j] = IP[i];
		i++;
		j++;
	}
	logIP[j] = '\0';
}

void getLogDate(char *logDate){
	time_t currTime;
	struct tm *gt;
	char respDate[30];

	time(&currTime);
	gt = localtime(&currTime);
	strftime(respDate, 50, "%a, %d %b %Y %X", gt);
	strcpy(logDate, "[");
	strcat(logDate, respDate);
	strcat(logDate, "]");
}

void getLogSize(char *bytes, char *logBytes){
	int i = 0, j = 0;

	while (bytes[i] != '\0'){
		logBytes[j] = bytes[i];
		i++;
		j++;
	}
	logBytes[j] = '\0';
}


void getLogURL(char *method, char *filename, char *URL){
	int i = 0, j = 0;
	while ((method[j] != '\0') && (i < URL_LEN - 1)){
		URL[i] = method[j];
		i++;
		j++;
	}
	URL[i] = ' ';
	i++;
	URL[i] = '/';
	i++;
	j = 0;
	while ((filename[j] != '\0') && (i < URL_LEN - 1)){
		URL[i] = filename[j];
		i++;
		j++;
	}
	URL[i] = '\0';
	strcat(URL, " HTTP/1.1");
}

void getLogRespCode(char *respCode, char *logCode){
	strcpy(logCode, respCode);
}
