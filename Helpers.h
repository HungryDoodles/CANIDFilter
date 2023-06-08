#ifndef _HELPERS_H_
#define _HELPERS_H_
#include <stdint.h>

// Find column begin and end indices taking parenthesis into account. Return column length in chars.
int SeekCol(const char* str, int colIndex, int& colBegOut, int& colEndOut) 
{
	int i = 0;

	int parenth = 0;
	int currentCol = 0;
	bool spaces = false;

	while (str[i] == ' ') ++i;

	do
	{
		if (str[i] == '(') ++parenth;
		else if (str[i] == ')') --parenth;

		if (parenth < 0) parenth = 0; // Error mitigation

		if (parenth == 0)
			if (spaces && str[i] != ' ')
			{
				spaces = false;
				currentCol++;
				if (currentCol == colIndex)
					colBegOut = i;
			}
			else if (!spaces && str[i] == ' ')
			{
				spaces = true;
				if (currentCol == colIndex)
				{
					colEndOut = i;
					return colEndOut - colBegOut;
				}
			}

	} while (str[++i] != 0);

	if (currentCol == colIndex) 
	{
		colEndOut = i;
		return colEndOut - colBegOut;
	}
	return 0;
}

inline uint32_t SubInt(uint32_t val, int firstBit, int bits)
{
	uint32_t mask = (1 << bits) - 1;
	return (val >> firstBit) & mask;
}

#endif
