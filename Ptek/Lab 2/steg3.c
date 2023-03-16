#include <stdio.h>
#include <ctype.h>
#include "simpio.h"
#include "scanner.h"
#include "bigint.h"
#include "memory.h"
#include "filereader.h"


void processInput(string input);

main(){
	string input, cmd;
	initMemory();

	while (TRUE){
		printf("\n>");
		input = GetLine();
		processInput(input);
	}
}

void processInput(string input){
	string a, b, c, op, token, s, cmd;

	InitScanner(input);

	token = GetNextToken();

	if (StringEqual(token, "%")){ // User wants to quit
		printf("ok, bye\n");
		printf("Press any key to leave");
		a = GetLine();
		exit(0);
	}

	if (StringEqual(token, "#")){ // Dump memory to screen
		dumpMemory();
		return;
	}

	if (StringEqual(token, "?")){  // A print statement
		token = GetNextToken(); //The space
		token = GetNextToken(); //The variable
		printBigInt(lookUp(token));
		return;
	}

	if (StringEqual(token, "&")){  // A print statement

		printf("Enter file: ");
		s = GetLine();

		initializeReadFromFile(s);
		while (cmd = getCommandFromFile())
			processInput(cmd);

		return;
	}



	// If we get to here the first token must be the name of a variable!
	a = CopyString(token);  //Keep the name of the variable for future use 
	token = GetNextToken(); //The space
	token = GetNextToken(); //This must be an operator i.e one of  = + - * += -= *=
	if (StringEqual(token, "=")){ //Assignment     var = ??  ?? is either a number or another variable
		token = GetNextToken(); //The space
		token = GetNextToken();
		if (isdigit(IthChar(token, 0))) // number   var = number
			enter(a, newBigInt(token)); //Store in memory
		else {
			b = token;
			if (!AtEndOfLine()){
				token = GetNextToken(); //The space
				op = GetNextToken();
				token = GetNextToken(); //The space
				c = GetNextToken();
				if (StringEqual(op, "+")){
					enter(a, add(lookUp(b), lookUp(c)));
				}
				else if (StringEqual(op, "-")){
					enter(a, sub(lookUp(b), lookUp(c)));
				}
				else if(StringEqual(op, "*")){
					enter(a, mul(lookUp(b), lookUp(c)));
				}
			}
			else {
				enter(a, lookUp(b));
			}
		}
	}
	else if (StringEqual(token, "+=")){
		token = GetNextToken();
		token = GetNextToken();
		addTo(lookUp(a), lookUp(token));
	}
	else if (StringEqual(token, "-=")){
		token = GetNextToken();
		token = GetNextToken();
		subFrom(lookUp(a), lookUp(token));
	}
	else if (StringEqual(token, "*=")){
		token = GetNextToken();
		token = GetNextToken();
		mulWith(lookUp(a), lookUp(token));
	}
}