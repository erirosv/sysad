#include "bigInt.h"
#include "genlib.h"
#include "strlib.h"


bigIntT newBigInt(string s){
	int i, j, len;
	bigIntT a;
	a = NewArray(NUMBEROFDIGITS, int);
	len = StringLength(s);
	for (i = 0; i < len; i++)
		a[i] = StringToInteger(CharToString(IthChar(s, StringLength(s) - i - 1)));
	for (j = len; j<NUMBEROFDIGITS; j++)
		a[j] = 0;
	return a;
}

bigIntT copyBigInt(bigIntT b){
	int i;
	bigIntT c = newBigInt("0");
	for (i = 0; i<NUMBEROFDIGITS; i++)
		c[i] = b[i];
	return c;
}

static bool isZero(bigIntT b){
	int i;
	for (i = 0; i<NUMBEROFDIGITS; i++)
	if (b[i] != 0) return FALSE;
	return TRUE;
}

void printBigInt(bigIntT b){
	int i = NUMBEROFDIGITS - 1;
	while (b[i] == 0 && i>0) i--; //don't print initial zeroes
	while (i > 0)
		printf("%d", b[i--]);
	printf("%d\n", b[0]);
}


bigIntT add(bigIntT a, bigIntT b){
	int i, carry = 0;
	int digitsum;
	bigIntT result = newBigInt("0");

	for (i = 0; i<NUMBEROFDIGITS; i++){
		digitsum = a[i] + b[i] + carry;
		result[i] = digitsum % 10;
		carry = digitsum / 10;
	}
	return result;
}

//------------------------------------

void addTo(bigIntT a, bigIntT b){
	int i;
	bigIntT result;
	result = add(a, b);
	for (i = 0; i<NUMBEROFDIGITS; i++)
		a[i] = result[i];
}

bigIntT sub(bigIntT a, bigIntT b){
	int i, carry = 0;
	int digitsum;
	bigIntT result = newBigInt("0");

	for (i = 0; i<NUMBEROFDIGITS; i++){
		digitsum = a[i] - b[i] + carry;
		carry = 0;
		if (digitsum < 0){
			digitsum += 10;
			carry = -1;
		}
		result[i] = digitsum;
	}
	return result;
}
void subFrom(bigIntT a, bigIntT b){
	int i;
	bigIntT result;
	result = sub(a, b);
	for (i = 0; i<NUMBEROFDIGITS; i++)
		a[i] = result[i];
}

bigIntT mul(bigIntT a, bigIntT b){
	int i, j, carry = 0;
	int digitsum;
	bigIntT result = newBigInt("0");

	for (i = 0; i<NUMBEROFDIGITS; i++){
		digitsum = carry;
		for (j = 0; j < i + 1; j++)
			digitsum += a[j] * b[i - j];

		result[i] = digitsum % 10;
		carry = digitsum / 10;
	}
	return result;
}

void mulWith(bigIntT a, bigIntT b){
	int i;
	bigIntT result;
	result = mul(a, b);
	for (i = 0; i<NUMBEROFDIGITS; i++)
		a[i] = result[i];
}