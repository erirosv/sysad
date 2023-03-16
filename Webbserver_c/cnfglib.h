/*
* File: cnfglib.h
* -----------------------------------------------------
* This interface provides access to two structs. The configT 
* struct keeps configuration information for the server and the
* logT struct keeps information to be printed into the server
* logfile. The library also exports functions to initialise the
* structs. 
*/

#ifndef _cnfglib_h
#define _cnfglib_h

/*Constant definitions*/

#define MAX_NUM 6
#define MAX_PASS 13
#define IP_LEN 16
#define DATE_LEN 30
#define CODE_LEN 4
#define URL_LEN 255
#define BYTES_LEN 11

/*type definitions*/

typedef struct{
	char incomPort[MAX_NUM];
	char telnetPort[MAX_NUM];
	char password[MAX_PASS];
	char *webDir;
	char *logDir;
}*configT;

typedef struct{
	char IP_addr[IP_LEN];
	char date[DATE_LEN];
	char URL[URL_LEN];
	char respCode[CODE_LEN];
	char bytes[BYTES_LEN];
}logT;


/*Function prototypes*/

/*Section 1 - configT*/
/*
* Function: configInit
* Usage: configVar = configInit();
* ------------------------
* configInit initialises a variable of configT-type.
* It allocates memory to the two dynamic char-vectors and
* sets the values of the static vectors to an empty string.
*/

configT configInit(void);

/*
* Function: readConfigFile
* Usage: readConfigFile(configVar);
* ------------------------
* readConfigFile reads a a configuration file with a
* certain format. It reads every other line and assigns
* the strings to the attributes of configT according to
* following format:
*	incoming port - line 2
*	telnet port - line 4
*	password (for telnet admin) - line 6
*	directory of website-file - line 8
*	directory of logfile - line 10
* it is upp to the user to make sure that the configuration
* file read is correct since there are no error checking of
* the strings assigned to the struct attributes.
*/

void readConfigFile(configT configVar);

/*
* Function: expandWebArray
* Usage: expandWebArray(configVar);
* ------------------------
* expandWebArray expands the dynamic vector for the directory
* of the website-file if the string is bigger than the currently
* allocated memory
*/

void expandWebArray(configT configVar);

/*
* Function: expandLogArray
* Usage: expandLogArray(configT configVar);
* ------------------------
* expandLogArray expands the dynamic vector for the directory
* of the logfile if the string is bigger than the currently
* allocated memory
*/

void expandLogArray(configT configVar);

/*
* Function: setEndArrays
* Usage: setEndArrays(configVar, sizeWeb, sizeLog);
* ------------------------
* When string is read from configuration file setEndArrays 
* sets the dynamic vectors to the size of the read strings, 
* making sure no space is wasted.
*/

void setEndArrays(configT configVar, int sizeWeb, int sizeLog);

/*
* Function: printConfigInfo
* Usage: printConfigInfo(configVar);
* ------------------------
* printConfigInfo prints all the attributes in a configT
* variable
*/

void printConfigInfo(configT configVar);

/*
* Function: freeAllMem
* Usage: freeAllMem(configInfo);
* ------------------------
* freeAllMem frees the memory space dynamically allocated
* for a configT variable.
*/

void freeAllMem(configT configVar);


/*Section 2 - logT*/

/*
* Function: getLogClientIP
* Usage: getLogClientIP(IP, logIP);
* ------------------------
* getLogClientIP assigns the connecting IP adress to the
* appropriate logT attribute.
*/

void getLogClientIP(char *IP, char *logIP);

/*
* Function: getLogDate
* Usage: getLogDate(logDate);
* ------------------------
* getLogDate assigns the date and time of current connection
* to the appropriate logT attribute.
*/

void getLogDate(char *logDate);

/*
* Function: getLogSize
* Usage: getLogSize(bytes, logBytes);
* ------------------------
* getLogSize assigns the content length of requestet file
* to appropriate logT attribute.
*/

void getLogSize(char *bytes, char *logBytes);

/*
* Function: getLogURL
* Usage: getLogURL(method, fileName, logURL);
* ------------------------
* getLogURL assigns the request-line from current client-
* request to appropriate logT attribute
*/

void getLogURL(char *method, char *filename, char *URL);

/*
* Function: getLogRespCode
* Usage: getLogRespCode(respCode, logRespCode);
* ------------------------
* getLogRespCode assigns response-code sent to current
* request
*/

void getLogRespCode(char *respCode, char *logCode);

#endif