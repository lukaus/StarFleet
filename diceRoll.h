#include <string>
#include <cstdlib>
#include <ctime>

#ifndef DICE_H
#define DICE_H

int roll(int num, int type, bool sign, int mod)
{
	int result, i;
	result = 0;
	for (i=0;i<num;i++) {
		result += (rand()%type);
	}
	if (sign == 0) {
		result += mod;
	}
	else {
		result -= mod;
	}

	return result+1;
}
#endif
