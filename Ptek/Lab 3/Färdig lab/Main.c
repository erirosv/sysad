/* File: lab3_steg0.c
* ------------------
* Start file for Assignment #3, PTek, autumn 2014.
* Author: CES
* Last modified: 2014-12-16
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "genlib.h"
#include "simpio.h"
#include "strlib.h"

/*
* Constant: QUIT
* --------------
* The constant QUIT is to break the program
* from the main menu.
*/

#define QUIT -1

/*
* Constant: INITIAL_SIZE
* ----------------------
* The constant defines the initial
* size of the dynamic array holding
* the cat data.
*/

#define INITIAL_SIZE 10

/* Constant: MAX_COLOURS
* ---------------------
* The constant defines the maximum number
* of colours that one cat can have.
*/

#define MAX_COLOURS 5

/* Type definition: catT
* ---------------------
* The catT is a simple struct containing all the
* attributes of the cats. The colours attribute
* is an array, since a cat can have several colours.
*/

typedef struct {
	int id;
	string name;
	char gender;
	int birthYear;
	int numberOfColours;
	string colours[MAX_COLOURS];
} catT;

/* Type definition: DBT
* --------------------
* The DBT type is a pointer to a struct containing a dynamic
* array of catT and three integer counters: one to keep track of the
* current size of the catT vector, one to keep track of the
* current number of cats in the database and another to keep track
* of the next available id number.
*/

typedef struct{
	catT *cats;
	int currentMaxSize;
	int currentNumberOfCats;
	int nextId;
} *DBT;

//Prototypes
//________________________________________________________________

//steg 1
void printIntroMessage(void);
string getRegisterFileName(void);
DBT initializeDataBase(void);
void loadDataBase(string filename, DBT db);
void executeCommands(DBT db);
void saveDataBase(string filename, DBT db);
void printMenu(void);

//Steg 2
void createCat(DBT db);
string inputCatName(void);
char inputCatGender(void);
int inputCatBirthYear(void);
string inputCatColour(void);

//Steg 3
string getCatName(catT *cat);
void printAllCatsAndAttributes(DBT db);
void printAllCatsAndId(DBT db);
void printCatAttributes(DBT db);
void printCatAndAttributes(catT *cat);

//Steg 4
void removeCat(DBT db);
int findCatByIdOrName(DBT db, string lookup);
int findCatByName(DBT db, string lookup);
int findCatById(DBT db, int id);

//Steg 5
void changeAttributes(DBT db);

//Steg 6
void listStatistics(DBT db);

//Steg 7
void sortRegister(DBT db);
int compareCatNames(const void *cat1, const void *cat2);
int compareCatBirthYear(const void *cat1, const void *cat2);

//Steg 8
void increaseVector(DBT db);

//Main
void main(void){
	DBT database;
	string fileName;

	database = initializeDataBase();
	printIntroMessage();
	fileName = getRegisterFileName();
	loadDataBase(fileName, database);
	executeCommands(database);
	saveDataBase(fileName, database);
}

//----------------------------------------------------------------------

//Steg 1
void printIntroMessage(void){
	printf("\nThis program helps organize information about cats and their attributes.\n");
	printf("You may add and remove cats and their attributes,\n");
	printf("list the names of cats currently in the organizer,\n");
	printf("list the cats and their attributes,\n");
	printf("look up the attributes of an individual cat,\n");
	printf("get statistics from the register, or sort the register.\n\n");
}

void printMenu(void){
	printf("\n1. Add a cat and its attributes.\n");
	printf("2. Update the entry for a cat.\n");
	printf("3. Remove a cat and its attributes.\n");
	printf("4. List all cats.\n");
	printf("5. List all cats and their attributes.\n");
	printf("6. Look up the attributes of a cat.\n");
	printf("7. List statistics from register.\n");
	printf("8. Sort register.\n");
}

DBT initializeDataBase(void){
	DBT db;

	db = New(DBT);
	db->cats = NewArray(INITIAL_SIZE, catT);
	db->currentMaxSize = INITIAL_SIZE;
	db->currentNumberOfCats = 0;
	db->nextId = 1;

	return db;
}

string getRegisterFileName(void){
	string fileName;

	printf("Enter file: ");
	fileName = GetLine();

	return fileName;
}

void loadDataBase(string fileName, DBT db){
	FILE *infile;
	string line;
	int i, j;

	infile = fopen(fileName, "r");
	if (infile != NULL){
		line = ReadLine(infile);
		if (line != NULL){
			db->currentNumberOfCats = StringToInteger(line);

			for (i = 0; i < db->currentNumberOfCats; i++){
				db->cats[i].id = StringToInteger(ReadLine(infile));
				if (db->cats[i].id >= db->nextId) db->nextId = db->cats[i].id + 1;
				db->cats[i].name = ReadLine(infile);
				db->cats[i].gender = IthChar(ReadLine(infile), 0);
				db->cats[i].birthYear = StringToInteger(ReadLine(infile));
				db->cats[i].numberOfColours = StringToInteger(ReadLine(infile));

				for (j = 0; j < db->cats[i].numberOfColours; j++)
					db->cats[i].colours[j] = ReadLine(infile);
			}
		}
	}
	else{
		infile = fopen(fileName, "w");
	}
	fclose(infile);
}

void executeCommands(DBT db){
	while (TRUE){
		int answer;

		system("CLS");
		printMenu();
		printf("\nEnter your selection (0 to quit): ");
		answer = GetInteger();

		if (answer == 0)
			break;

		if (answer < 1 || answer > 8)
			printf("Invalid selection. Please try again.\n");

		switch (answer){
			case 1:
				createCat(db);
				break;
			case 2:
				changeAttributes(db);
				break;
			case 3:
				removeCat(db);
				break;
			case 4:
				printAllCatsAndId(db);
				break;
			case 5:
				printAllCatsAndAttributes(db);
				break;
			case 6:
				printCatAttributes(db);
				break;
			case 7:
				listStatistics(db);
				break;
			case 8:
				sortRegister(db);
				break;
		}
		printf("Press Enter to continue: ");
		GetLine();
	}
}

void saveDataBase(string filename, DBT db){
	FILE *infile;
	int i, j;

	infile = fopen(filename, "w");
	if (db->currentNumberOfCats != 0){
		fprintf(infile, "%d\n", db->currentNumberOfCats);

		for (i = 0; i < db->currentNumberOfCats; i++){
			fprintf(infile, "%d\n", db->cats[i].id);
			fprintf(infile, "%s\n", getCatName(&db->cats[i]));
			fprintf(infile, "%s\n", ConvertToUpperCase(CharToString(db->cats[i].gender)));
			fprintf(infile, "%d\n", db->cats[i].birthYear);
			fprintf(infile, "%d\n", db->cats[i].numberOfColours);

			for (j = 0; j < db->cats[i].numberOfColours; j++)
				fprintf(infile, "%s\n", ConvertToUpperCase(db->cats[i].colours[j]));
		}
		fclose(infile);
	}
}

//Steg 2

void createCat(DBT db){
	string name, colour;
	char gender;
	int birthYear, i;
	int colourCounter = 0;
	int colours[MAX_COLOURS];

	name = inputCatName();
	gender = inputCatGender();
	birthYear = inputCatBirthYear();

	for (i = 0; i < MAX_COLOURS; i++){
		colour = inputCatColour();
		if (StringEqual(colour, "\0")) break;

		colours[i] = colour;

		colourCounter++;
	}

	if (db->currentNumberOfCats == db->currentMaxSize)
		increaseVector(db);

	db->cats[db->currentNumberOfCats].id = db->nextId;
	db->cats[db->currentNumberOfCats].name = name;
	db->cats[db->currentNumberOfCats].gender = gender;
	db->cats[db->currentNumberOfCats].birthYear = birthYear;
	db->cats[db->currentNumberOfCats].numberOfColours = colourCounter;

	for (i = 0; i < colourCounter; i++)
		db->cats[db->currentNumberOfCats].colours[i] = colours[i];

	db->currentNumberOfCats++;
	db->nextId++;

	printf("Cat added.\n");
}

string inputCatName(void){
	int i;
	string name;

	while (TRUE){
		while (TRUE){
			printf("Enter the cats name (can only contain letters): ");
			name = GetLine();
			if (StringLength(name) > 0)
				break;
			printf("Cats name must be atleast 1 letter.\n");
		}
		for (i = 0; i < StringLength(name); i++){
			if (!(name[i] >= 'a' && name[i] <= 'z' || name[i] >= 'A' && name[i] >= 'Z')){
				printf("Invalid input.\n");
				name = NULL;
				break;
			}
		}
		if (name != NULL)break;
	}
	return name;
}

char inputCatGender(void){
	char gender;

	while (TRUE){
		printf("Enter the cats gender(M or F): ");
		gender = getchar();
		getchar();

		if (gender == 'm' || gender == 'M' || gender == 'f' || gender == 'F')
			break;
		printf("Invalid input.\n");
	}
	return gender;
}

int inputCatBirthYear(void){
	int birthYear;
	printf("Enter the cats birthyear: ");
	birthYear = GetInteger();

	return birthYear;
}

string inputCatColour(void){
	string colour;
	printf("Enter colour (Press Enter when you are done): ");
	colour = GetLine();

	return colour;
}

// Steg 3
string getCatName(catT *cat){
	return ConvertToUpperCase(cat->name);
}

void printAllCatsAndId(DBT db){
	int i;

	for (i = 0; i < db->currentNumberOfCats; i++){
		printf("%d\n", db->cats[i].id);
		printf("%s\n", ConvertToUpperCase(db->cats[i].name));
	}
}

void printAllCatsAndAttributes(DBT db){
	int i;

	for (i = 0; i < db->currentNumberOfCats; i++){
		printCatAndAttributes(&db->cats[i]);
	}
}

void printCatAndAttributes(catT* cat){
	int i;
	printf("%d\n", cat->id);
	printf("%s\n", ConvertToUpperCase(cat->name));
	printf("%s\n", ConvertToUpperCase(CharToString(cat->gender)));
	printf("%d\n", cat->birthYear);
	printf("%d\n", cat->numberOfColours);

	for (i = 0; i < cat->numberOfColours; i++)
		printf("%s\n", ConvertToUpperCase(cat->colours[i]));
}

void printCatAttributes(DBT db){
	string answer;
	int selectedCatIndex;

	printf("Enter a cats name or id: ");
	answer = GetLine();
	answer = ConvertToUpperCase(answer);

	if (StringLength(answer) == 0){
		printf("Invalid input");
		return;
	}

	selectedCatIndex = findCatByIdOrName(db, answer);

	if (selectedCatIndex < 0 || selectedCatIndex >= db->currentNumberOfCats){
		printf("No cat found with the specified ID or name");
	}
	else{
		printCatAndAttributes(&db->cats[selectedCatIndex]);
	}
}

//Steg 4
void removeCat(DBT db){
	int i, j;
	string answer;

	printf("Enter the cats name or id: ");
	answer = GetLine();
	answer = ConvertToUpperCase(answer);

	i = findCatByIdOrName(db, answer);
	if (i < 0 || i >= db->currentNumberOfCats){
		printf("No cat found with the specified ID or name");
		return;
	}
	for (; i < db->currentNumberOfCats - 1; i++){
		db->cats[i] = db->cats[i + 1];
	}
	db->currentNumberOfCats--;
}

int findCatByIdOrName(DBT db, string lookup){
	int i;
	char c;

	if (StringLength(lookup) == 0){
		return -1;
	}

	for (i = 0; i < StringLength(lookup); i++){
		c = IthChar(lookup, i);
		if (c > '9' || c < '0'){
			return findCatByName(db, lookup);
		}
	}

	return findCatById(db, StringToInteger(lookup));
}

int findCatByName(DBT db, string lookup){
	int i;
	int *catChoices = NewArray(db->currentNumberOfCats, int);
	int nextIndex = 0;
	int input;

	for (i = 0; i < db->currentNumberOfCats; i++){
		if (StringEqual(lookup, db->cats[i].name)){
			catChoices[nextIndex++] = i;
		}
	}
	if (nextIndex == 0) return -1;
	if (nextIndex == 1) return catChoices[0];

	for (i = 0; i < nextIndex; i++){
		printf("%d. %d %s\n", i + 1, db->cats[catChoices[i]].id, db->cats[catChoices[i]].name);
	}
	while (TRUE){
		printf("Choose a cat (1-%d): ", nextIndex);
		input = GetInteger() - 1;
		if (input >= 0 && input < nextIndex){
			return catChoices[input];
		}
		printf("Invalid Input\n");
	}
}

int findCatById(DBT db, int id){
	int i;

	for (i = 0; i < db->currentNumberOfCats; i++){
		if (db->cats[i].id == id){
			return i;
		}
	}
	return -1;
}

//Steg 5
void changeAttributes(DBT db){
	int id, birthyear, i;
	string answer, name, colour;

	printf("Enter the cats name or id: ");
	answer = GetLine();
	answer = ConvertToUpperCase(answer);

	id = findCatByIdOrName(db, answer);
	if (id < 0 || id >= db->currentNumberOfCats){
		printf("No cat found with the specified ID or name");
		return;
	}

	name = inputCatName();
	db->cats[id].name = name;

	birthyear = inputCatBirthYear();
	db->cats[id].birthYear = birthyear;

	for (i = 0; i < MAX_COLOURS; i++){
		colour = inputCatColour();
		if (StringEqual(colour, "\0")) break;
		db->cats[id].colours[i] = colour;
	}
	db->cats[id].numberOfColours = i;
	printf("Cat updated.\n");
}

//Steg 6
void listStatistics(DBT db){

	int i;
	int m = 0;
	int f = 0;

	int youngest;
	int oldest;

	if (db->currentNumberOfCats == 0){
		printf("No cats in register.\n");
		return;
	}

	for (i = 0; i < db->currentNumberOfCats; i++){
		if (StringEqual(CharToString(db->cats[i].gender), "M"))
			m++;
		else
			f++;
	}

	oldest = INT_MAX;
	youngest = INT_MIN;

	for (i = 0; i < db->currentNumberOfCats; i++){

		if (youngest < db->cats[i].birthYear)
			youngest = db->cats[i].birthYear;

		if (db->cats[i].birthYear < oldest)
			oldest = db->cats[i].birthYear;
	}

	printf("Number of cats: %d\n", db->currentNumberOfCats);
	printf("Number of males: %d\n", m);
	printf("Number of females: %d\n", f);
	printf("Oldest cats birthyear: %d\n", oldest);
	printf("youngest cats birthyear: %d\n", youngest);
}

//Steg 7
void sortRegister(DBT db){
	int input;
	printf("Enter sorting by name(1) or birthyear(2): ");
	input = GetInteger();

	if (input == 1){
		qsort(db->cats, db->currentNumberOfCats, sizeof(catT), compareCatNames);
	}
	else if (input == 2){
		qsort(db->cats, db->currentNumberOfCats, sizeof(catT), compareCatBirthYear);
	}
	else
		printf("Invalid input.\n");
}

int compareCatNames(const void *cat1, const void *cat2){
	return StringCompare(((catT*)cat1)->name, ((catT*)cat2)->name);
}

int compareCatBirthYear(const void *cat1, const void *cat2){
	return ((catT*)cat1)->birthYear - ((catT*)cat2)->birthYear;
}
//Steg 8

void increaseVector(DBT db){
	int i;
	catT *v;
	v = NewArray(db->currentMaxSize * 2, catT);

	for (i = 0; i < db->currentNumberOfCats; i++){
		v[i] = db->cats[i];
	}
	free(db->cats);
	db->cats = v;
}