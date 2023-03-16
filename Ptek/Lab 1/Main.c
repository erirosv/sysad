
/*
* File: calender.c
* ---------------
*/

#include <stdio.h>
#include "genlib.h"
#include "simpio.h"

/*
* Constants:
* ----------
* Days of the week are represented by the integers 0-6.
* Months of the year are identified by the integers 1-12;
* because this numeric representation for months is in
* common use, no special constants are defined.
*/

#define Sunday     0
#define Monday     1
#define Tuesday    2
#define Wednesday  3
#define Thursday   4
#define Friday     5
#define Saturday   6

/* our own constant */
#define currentYear 2015

/* Function prototypes */

void GiveInstructions(void);
int GetYearFromUser(void);
void PrintCalendar(int year);
void PrintCalendarMonth(int month, int year);
void IndentFirstLine(int weekday);
int MonthDays(int month, int year);
int FirstDayOfMonth(int month, int year);
string MonthName(int month);
bool IsLeapYear(int year);

/* Here are our functions prototypes */
void displayMainMenu(void);
void displayYearCalendar(void);
void findWeekDay(void);
void birthday(void);
void holidayMenu(void);
void anyKey(void);
int GetMonthFromUser(void);
int GetDateFromUser(int year, int month);
int getWeekDay(int year, int month, int date);
string dayName(int dayNumber);
string zodiacMarks(int month, int date);

int christmasMonth(void);
int christmasDay(void);
void christmas(void);
int newYearMonth(void);
int newYearDay(void);
void newYear(void);
int midsummerMonth(void);
int midsummerDay(int year);
void midsummer(void);

void mothersAndFathersDayMenu(void);
void displayMothersAndFathersDayMenu(void);

int fathersDayMonth(void);
int fathersDayDay(int year);
void fathersDay(void);
int mothersDayMonth(void);
int mothersDayDay(int year);
void mothersDay(void);

void easter();
void whit();
int easterAlgorithm(int year);

int getEasterMonth(int easterDays);
int getEasterDate(int year, int easterDays);

void printEaster(int year, string MonthName, int date, string dayName);

int getWhitMonth(int year, int month, int date);
int getWhitDays(int year, int month, int date);

int printWhit(int year, int whitMonth, int whitDate, int weekDay);

/* Main program */

main(){
	int answer;

	while (TRUE){
		displayMainMenu();
		printf("Your choice? ");
		answer = GetInteger();

		if (answer == 0){
			break;
		}

		switch (answer){
		case 1:
			displayYearCalendar();
			break;

		case 2:
			findWeekDay();
			break;

		case 3:
			birthday();
			break;

		case 4:
			holidayMenu();
			break;

		default:
			system("cls");
			printf("Invalid input.\n");
			anyKey();
			break;
		}

	}

	anyKey();
}

//funktioner

/*
* Function: GiveInstructions
* Usage: GiveInstructions();
* --------------------------
* This procedure prints out instructions to the user.
*/

void GiveInstructions(void)
{
	printf("This program displays a calendar for a full\n");
	printf("year. The year must be at least 1500.\n");
}

/*
* Function: GetYearFromUser
* Usage: year = GetYearFromUser();
* --------------------------------
* This function reads in a year from the user and returns
* that value.  If the user enters a year before 1900, the
* function gives the user another chance.
*/

int GetYearFromUser(void)
{

	int year;

	while (TRUE) {
		printf("Which year? ");
		year = GetInteger();
		if (year >= 1500) return (year);
		printf("The year must be at least 1500.\n");
	}
}

/*
* Function: PrintCalendar
* Usage: PrintCalendar(year);
* ---------------------------
* This procedure prints a calendar for an entire year.
*/

void PrintCalendar(int year)
{
	int month;

	for (month = 1; month <= 12; month++) {
		PrintCalendarMonth(month, year);
		printf("\n");
	}

	anyKey();
}

/*
* Function: PrintCalendarMonth
* Usage: PrintCalendarMonth(month, year);
* ---------------------------------------
* This procedure prints a calendar for the given month
* and year.
*/

void PrintCalendarMonth(int month, int year)
{
	int weekday, nDays, day;

	printf("    %s %d\n", MonthName(month), year);
	printf(" Su Mo Tu We Th Fr Sa\n");
	nDays = MonthDays(month, year);
	weekday = FirstDayOfMonth(month, year);
	IndentFirstLine(weekday);
	for (day = 1; day <= nDays; day++) {
		printf(" %2d", day);
		if (weekday == Saturday) printf("\n");
		weekday = (weekday + 1) % 7;
	}
	if (weekday != Sunday) printf("\n");
}

/*
* Function: IndentFirstLine
* Usage: IndentFirstLine(weekday);
* --------------------------------
* This procedure indents the first line of the calendar
* by printing enough blank spaces to get to the position
* on the line corresponding to weekday.
*/

void IndentFirstLine(int weekday)
{
	int i;

	for (i = 0; i < weekday; i++) {
		printf("   ");
	}
}

/*
* Function: MonthDays
* Usage: ndays = MonthDays(month, year);
* --------------------------------------
* MonthDays returns the number of days in the indicated
* month and year.  The year is required to handle leap years.
*/

int MonthDays(int month, int year){

	switch (month) {
	case 2:
		if (year == 1712) return 30;
		if (year == 1753) return 17;
		if (IsLeapYear(year)) return (29);
		return (28);
	case 4: case 6: case 9: case 11:
		return (30);
	default:
		return (31);
	}
}

/*
* Function: FirstDayOfMonth
* Usage: weekday = FirstDayOfMonth(month, year);
* ----------------------------------------------
* This function returns the day of the week on which the
* indicated month begins.  This program simply counts
* forward or backward from January 1, 1900, which was a Monday.
* In the backward count, care is taken to avoid negative values
* for weekday, which would result in miscalculations.
*/

int FirstDayOfMonth(int month, int year){
	int weekday, i;

	weekday = Wednesday;
	for (i = 1500; i < year; i++) {
		if (i == 1712){
			weekday = (weekday + 365) % 7;
		}
		if (i == 1753){
			weekday = (weekday + 354) % 7;
		}
		else{
			weekday = (weekday + 365) % 7;
			if (IsLeapYear(i)) weekday = (weekday + 1) % 7;
		}
	}
	for (i = 1; i < month; i++) {
		weekday = (weekday + MonthDays(i, year)) % 7;
	}
	return (weekday);
}

/*
* Function: MonthName
* Usage: name = MonthName(month);
* -------------------------------
* MonthName converts a numeric month in the range 1-12
* into the string name for that month.
*/

string MonthName(int month){
	switch (month) {
	case  1: return ("January");
	case  2: return ("February");
	case  3: return ("March");
	case  4: return ("April");
	case  5: return ("May");
	case  6: return ("June");
	case  7: return ("July");
	case  8: return ("August");
	case  9: return ("September");
	case 10: return ("October");
	case 11: return ("November");
	case 12: return ("December");
	default: return ("Illegal month");
	}
}

/*
* Function: IsLeapYear
* Usage: if (IsLeapYear(year)) . . .
* ----------------------------------
* This function returns TRUE if year is a leap year.
*/

bool IsLeapYear(int year){

	if (year == 1700)
		return FALSE;

	if ((year >= 1500 && year <= 1699) || (year >= 1712 && year <= 1752)){
		return (year % 4 == 0);
	}
	else{
		return (((year % 4 == 0) && (year % 100 != 0))
			|| (year % 400 == 0));
	}
}

/* Our functions */

/*
* Function: displayMainMenu
* Usage: displayMainMenu();
* -------------------------
* This function first clears the screen and
* prints out the mainmenu for the user.
*/

void displayMainMenu(){

	system("cls");

	printf("Which calendar function would you like to use?\n");
	printf("Enter 1 for Printing a whole year’s calendar\n");
	printf("Enter 2 for Finding the weekday of a specific date\n");
	printf("Enter 3 for Birthday data\n");
	printf("Enter 4 for Holidays\n");
	printf("Enter 0 to Quit\n");
}

/*
* Function: anyKey
* Usage: anyKey();
* ----------------
* This function ask the user for a single key as input
* before proceeding the program.
*/

void anyKey(){
	printf("Press any key to continue");
	GetLine();
}


/* Stage 1 */

/*
* Function: displayYearCalendar
* Usage: displayYearCalendar();
* -----------------------------
* This ask the user for a year and
* prints calendar for that year.
*/

void displayYearCalendar(){

	system("cls");

	int year;

	year = GetYearFromUser();
	PrintCalendar(year);
}

/* Stage 2 */

/*
* Function: findWeekDay
* Usage: findWeekDay();
* ----------------------
* This function take input for year, month and date
* and prints out its weekday.
*/

void findWeekDay(){

	system("cls");

	int year, month, date, resultWeekday;

	year = GetYearFromUser();
	month = GetMonthFromUser();
	date = GetDateFromUser(month, year);

	resultWeekday = getWeekDay(year, month, date);

	printf("%s %d %d was a %s\n", MonthName(month), date, year, dayName(resultWeekday));

	anyKey();

}

/*
* Function: GetMonthFromUser
* Usage: month = GetMonthFromUser();
* ----------------------------------
* This asks the user for a month beetween 1 to 12.
* It returns a month represented as an integer
* when user enter valid input.
*/

int GetMonthFromUser(){

	int month;

	while (TRUE) {
		printf("Which month? ");
		month = GetInteger();
		if (month >= 1 && month <= 12) return (month);
		printf("Enter 1 - 12\n");

	}
}

/*
* Function: GetDateFromUser
* Usage: date = GetDateFromUser(int month, int year);
* ----------------------------------
* This function takes month and year as arguments and
* returns an integer represented as date for that year and month.
*/


int GetDateFromUser(int month, int year){

	int date;

	while (TRUE) {
		printf("Which date? ");
		date = GetInteger();
		if (date >= 1 && date <= MonthDays(month, year)) return (date);
		printf("Enter 1 - %d\n", MonthDays(month, year));
	}
}

/*
* Function: getWeekDay
* Usage: weekDay = getWeekDay(year, month, date);
* ----------------------------------
* This function returns an integer beetween 0 - 6 that represent the weekdays.
*/

int getWeekDay(int year, int month, int date){

	int weekday;

	weekday = FirstDayOfMonth(month, year);
	weekday = (weekday + date - 1) % 7;

	return weekday;
}

/*
* Function: dayName
* Usage: weekDay = dayName(int);
* ----------------------------------
* It returns a weekday as a stirng.
*/

string dayName(int dayNumber){

	switch (dayNumber){

	case  0: return ("Sunday");
	case  1: return ("Monday");
	case  2: return ("Tuesday");
	case  3: return ("Wednesday");
	case  4: return ("Thursday");
	case  5: return ("Friday");
	case  6: return ("Saturday");
	default: return ("Error");
	}
}

/* Stage 3 */

/*
* Function: birthday
* Usage: birthday();
* ----------------------------------
* This asks the user to enter its name and birthday information.
* It then prints out which day the user is born and what days their
* birthdays will be on 10 year from now. It also prints out what zodiac sign the user has.
*/

void birthday(){

	system("cls");

	int year, month, date, i;
	string name, weekdayResult;

	printf("Enter name: ");
	name = GetLine();

	year = GetYearFromUser();
	month = GetMonthFromUser();
	date = GetDateFromUser(month, year);

	weekdayResult = dayName(getWeekDay(year, month, date));

	printf("%s was born on a %s \n", name, weekdayResult);

	for (year = currentYear; year <= currentYear + 9; year++){

		int weekNr;
		string weekdayResult;

		weekNr = getWeekDay(year, month, date);

		weekdayResult = dayName(weekNr);

		printf("In %d %s's birthday is on a %s\n", year, name, weekdayResult);
	}

	printf("%s %s\n", name, zodiacMarks(month, date));

	anyKey();
}

/*
* Function: zodiacMarks
* Usage: zodiac = zodiacMarks(month, date);
* -----------------------------------------
* This takes month and date as arguments and returns a string for the zodiac sign of that day.
*/

string zodiacMarks(int month, int date){

	if (month == 12 && date >= 22 || month == 1 && date <= 19) return "is a Capricorn";
	if (month == 1 && date >= 20 || month == 2 && date <= 18) return " is an Aquarius";
	if (month == 2 && date >= 19 || month == 3 && date <= 20) return "is a Pisces";
	if (month == 3 && date >= 21 || month == 4 && date <= 19) return "is an Aries";
	if (month == 4 && date >= 20 || month == 5 && date <= 20) return "is a Taurus";
	if (month == 5 && date >= 21 || month == 6 && date <= 20) return "is a Gemini";
	if (month == 6 && date >= 21 || month == 7 && date <= 22) return "is a Cancer";
	if (month == 7 && date >= 23 || month == 8 && date <= 22) return "is a Leo";
	if (month == 8 && date >= 23 || month == 9 && date <= 23) return "is a Virgo";
	if (month == 9 && date >= 23 || month == 10 && date <= 22) return "is a Libra";
	if (month == 10 && date >= 23 || month == 11 && date <= 21) return "is a Scorpio";
	if (month == 11 && date >= 22 || month == 12 && date <= 21) return "is a Sagittarius";
}

/* Stage 4 */

/*
* Function: displayHolidayMenu
* Usage: displayHolidayMen();
* ----------------------------------
* This first clears the screen and prints out the holidaymenu.
*/

void displayHolidayMenu(){

	system("cls");

	printf("What holiday date would you like to calculate?\n");
	printf("Enter 1 for Christmas Day\n");
	printf("Enter 2 for New Year’s Eve\n");
	printf("Enter 3 for Midsummer’s Day\n");
	printf("Enter 4 for Mother’s and Father’s Day\n");
	printf("Enter 5 for Easter\n");
	printf("Enter 6 for Whit\n");
	printf("Enter 0 to Return to Main menu\n");
}

/*
* function: holidayMenu
* usage: holidayMenu();
* ----------------------------------
* This asks the user for which function to call.
*/

void holidayMenu(){

	int answer;

	while (TRUE){
		displayHolidayMenu();
		printf("Your choice? ");
		answer = GetInteger();

		if (answer == 0){
			break;
		}

		switch (answer){
		case 1:
			christmas();
			break;

		case 2:
			newYear();
			break;

		case 3:
			midsummer();
			break;

		case 4:
			mothersAndFathersDayMenu();
			break;

		case 5:
			easter();
			break;

		case 6:
			whit();
			break;

		default:
			system("cls");
			printf("Invalid input.\n");
			anyKey();
			break;
		}
	}

	anyKey();
}

/*
* function: christmasMonth
* usage: christmasMonth();
* ----------------------------------
* This returns the month for christmas.
*/

int christmasMonth(){
	return 12;
}

/*
* function: christmasDay
* usage: christmasDay();
* ----------------------------------
* This returns the date for christasday.
*/

int christmasDay(){
	return 25;
}

/*
* function: christmas
* usage: christmas();
* ----------------------------------
* This asks the user for year and prints
* out the christmasday for that year.
*/

void christmas(){
	int year, month, day, weekday;
	year = GetYearFromUser();
	month = christmasMonth();
	day = christmasDay();
	weekday = getWeekDay(year, month, day);
	printf("Christmas day in %d is on %s %d which is a %s\n", year,
		MonthName(month), day, dayName(weekday));
	anyKey();
}

/*
fuction: newYearMonth
usage: newYearMonth();
--------------------------------
This fuction returns the number of the month for new year.
*/

int newYearMonth(void){
	return 12;
}

/*
function: newYearDay
usage: newYearDay();
-----------------------------
This fuction returns the date of the new year.
*/

int newYearDay(void){
	return 31;
}

/*
* function: newYear
* usage: newYear();
* -----------------------------
* This function asks user for year, month and date
* it then prints out information about the new year.
*/

void newYear(){
	int year, month, day, weekday;
	year = GetYearFromUser();
	month = newYearMonth();
	day = newYearDay();
	weekday = getWeekDay(year, month, day);
	printf("New Year’s Eve in %d is on %s %d which is a %s\n", year,
		MonthName(month), day, dayName(weekday));
	anyKey();
}

/*
* function: midsummerMonth
* usage: midsummermonth();
* -----------------------------
* This fuction returns the month for midsummer.
*/

int midsummerMonth(){
	return 6;
}

/*
* function: midsummerDay
* usage: mDay = midsummerDay(year);
* -----------------------------
* This fuction returns midsummer day;
*/

int midsummerDay(int year){

	int day;

	for (day = 20; day <= 26; day++){
		if (getWeekDay(year, midsummerMonth(), day) == Saturday){
			return day;
		}
	}
}

/*
* function: midsummer
* usage: midsummer();
* -----------------------------
* This fuction asks for year, month date.
* It then prints out infor
*/

void midsummer(){

	int year, month, day, weekday;

	year = GetYearFromUser();
	month = midsummerMonth();
	day = midsummerDay(year);
	weekday = getWeekDay(year, month, day);
	printf("Midsummer’s Day in %d is on %s %d which is a %s\n", year,
		MonthName(month), day, dayName(weekday));
	anyKey();
}


/*
* function: monthersAndFathersDayMenu
* usage: monthersAndFathersDayMenu();
* -----------------------------
* This prints out the menu for mother or fathersday.
* Then it asks user to select mothersday or fathersday function.
*/

void mothersAndFathersDayMenu(){
	int answer;

	while (TRUE){
		displayMothersAndFathersDayMenu();
		printf("Your choice? ");
		answer = GetInteger();

		if (answer == 0){
			break;
		}

		switch (answer){
		case 1:
			mothersDay();
			break;

		case 2:
			fathersDay();
			break;

		default:
			system("cls");
			printf("Invalid input.\n");
			anyKey();
			break;
		}
	}

	anyKey();
}

/*
* function: displayMothersAndFathersDayMenu
* usage: displayMothersAndFathersDayMenu();
* ------------------------------------------
* First it clears the screen and then
* prints out information to user.
*/

void displayMothersAndFathersDayMenu(){

	system("cls");

	printf("Enter 1 for Mother's Day\n");
	printf("Enter 2 for Father's Day\n");
	printf("Enter 0 to go back\n");
}

/*
* function: fathersDayMonth
* usage: fMonth = fathersDayMonth();
* ----------------------------------
* This returns month of fathersday.
*/

int fathersDayMonth(){
	return 11;
}

/*
* function: fathersDayDay
* usage: fDate = fathersDayDay(year)
* ----------------------------------
* Returns the date for fathersday.
*/

int fathersDayDay(int year){

	int day;

	for (day = 8; day < 15; day++){
		if (getWeekDay(year, fathersDayMonth(), day) == Sunday){
			return day;
		}
	}
}

/*
* function: fathersDay
* usage: fathersDay();
* ----------------------------------
* This function asks user for year and prints -
* out the fathers day and its weekday.
*/

void fathersDay(void){

	int year, month, day, weekday;

	year = GetYearFromUser();
	month = fathersDayMonth();
	day = fathersDayDay(year, month);
	weekday = getWeekDay(year, month, day);
	printf("Father's Day in %d is on %s %d which is a %s\n", year,
		MonthName(month), day, dayName(weekday));
	anyKey();
}

/*
* function: mothersDayMonth
* usage: mMonth = mothersDayMonth();
* ----------------------------------
* This returns month of mothersday.
*/

int mothersDayMonth(void){
	return 5;
}

/*
* function: mothersDayDay
* usage: mDate = mothersDayDay(year)
* ----------------------------------
* Returns the date for mothersday.
*/

int mothersDayDay(int year){

	int day;

	for (day = 25; day <= 31; day++){
		if (getWeekDay(year, mothersDayMonth(), day) == Sunday){
			return day;
		}
	}
}

/*
* function: mothersDay
* usage: mothersDay();
* ----------------------------------
* This function asks user for year and prints -
* out the mothers day and its weekday.
*/

void mothersDay(){

	int year, month, day, weekday;

	year = GetYearFromUser();
	month = mothersDayMonth();
	day = mothersDayDay(year, month);
	weekday = getWeekDay(year, month, day);
	printf("Mother’s Day in %d is on %s %d which is a %s\n", year,
		MonthName(month), day, dayName(weekday));
	anyKey();
}

/* Stage 6 */

/*
* function: easter
* usage: easter();
* ----------------------------------
* This asks for year and prints out easter for that year.
*/

void easter(){

	int year, easterDays, date, month;

	year = GetYearFromUser();
	easterDays = easterAlgorithm(year);

	date = getEasterDate(year, easterDays);
	month = getEasterMonth(easterDays);

	printEaster(year, MonthName(month), date, dayName(getWeekDay(year, month, date)));

	anyKey();
}

/*
* function: whit
* usage: whit();
* ----------------------------------
* This asks for year and prints out whit for that year.
*/

void whit(){

	int year, easterDays, whitMonth, whitDate, easterDate, easterMonth;

	year = GetYearFromUser();
	easterDays = easterAlgorithm(year);

	easterDate = getEasterDate(year, easterDays);
	easterMonth = getEasterMonth(year, easterDays);

	whitMonth = getWhitMonth(year, easterMonth, easterDate);
	whitDate = getWhitDate(year, easterDate, easterDate);

	printWhit(year, whitMonth, whitDate, getWeekDay(year, whitMonth, whitDate));


	anyKey();
}

/*
* function: easterAlgorithm
* usage: easterAlgorithm(year);
* ----------------------------------
* This asks for a year and returns the
* number of days after 21 march.
*/

int easterAlgorithm(int year){

	int golden, solar, lunar, pfm, dom, tmp, easter;

	/* the Golden number */
	golden = (year % 19) + 1;

	if (year <= 1738)
	{
		/* JULIAN CALENDAR */
		/* the "Dominical number" - finding a Sunday */
		dom = (year + (year / 4) + 5) % 7;
		if (dom < 0) dom += 7;

		/* uncorrected date of the Paschal full moon */
		pfm = (3 - (11 * golden) - 7) % 30;
		if (pfm < 0) pfm += 30;
	}
	else
	{
		/* GREGORIAN CALENDAR */
		/* the "Dominical number" - finding a Sunday */
		dom = (year + (year / 4) - (year / 100) + (year / 400)) % 7;
		if (dom < 0) dom += 7;

		/* the solar and lunar corrections */
		solar = (year - 1600) / 100 - (year - 1600) / 400;
		lunar = (((year - 1400) / 100) * 8) / 25;

		/* uncorrected date of the Paschal full moon */
		pfm = (3 - (11 * golden) + solar - lunar) % 30;
		if (pfm < 0) pfm += 30;
	}

	/* corrected date of the Paschal full moon - days after 21st March */
	if ((pfm == 29) || (pfm == 28 && golden > 11))
		pfm--;

	tmp = (4 - pfm - dom) % 7;
	if (tmp < 0) tmp += 7;

	/* Easter as the number of days after 21st March */
	easter = pfm + tmp + 1;

	if (year >= 1700 && year <= 1711) return (easter + 1);
	if (year >= 1739 && year <= 1752) return (easter - 11);

	return easter;
}

/*
* function: getEasterMonth
* usage: eMonth = getEasterMonth(easterDays);
* ----------------------------------
* This returns an integer that represent the month for easter.
*/

int getEasterMonth(int easterDays){

	if (easterDays < 11)
		return (3);
	else
		return (4);
}

/*
* function: getEasterDate
* usage: eDate = getEasterDate(year, easterDays);
* ----------------------------------
* This returns an integer that represent the date for easter.
*/

int getEasterDate(int year, int easterDays){

	if (year == 1744) return 18;
	if (year == 1818) return 29;

	if (easterDays < 11) return (21 + easterDays);
	else
		return (easterDays - 10);
}

/*
* function: printEaster
* usage: printEaster(year, monthName, date,  dayName);
* ----------------------------------
* This prints out the easter from the arguments in the function.
*/

void printEaster(int year, string MonthName, int date, string dayName){
	printf("Easter day in %d is on %s %d which is a %s\n", year, MonthName, date, dayName);
}

/*
* function: getWhitMonth
* usage: eMonth = getEasterMonth(easterDays);
* ----------------------------------
* This returns an integer that represent the month for whit.
*/

int getWhitMonth(int year, int month, int date){

	int i;

	for (i = 0; i <= 49; i++){
		if (date > MonthDays(month, year)){
			month++;
			date = 1;
		}
		date++;
	}
	return month;
}

/*
* function: getWhitDate
* usage: eDate = getWhitDate(year, month, date);
* ----------------------------------
* This returns an integer that represent the date for whit.
*/

int getWhitDate(int year, int month, int date){

	int i;

	for (i = 0; i <= 49; i++){
		if (date > MonthDays(month, year)){
			month++;
			date = 1;
		}
		date++;
	}
	return date;
}

/*
* function: printWhit
* usage: printWhit(year, whitName, whitDate,  dayName);
* ----------------------------------
* This prints out the whit from the arguments in the function.
*/

printWhit(int year, int whitMonth, int whitDate, int weekDay){
	printf("Whitday in year %d is in %s %d which is on a %s \n", year, MonthName(whitMonth), whitDate, dayName(weekDay));
}