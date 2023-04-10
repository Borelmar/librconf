#include <stdio.h>
#include <string.h>

/*
 * function for get the char of string
 */
int _strcochr
	(char *str, char chr)
{
	int _c = 0;
	for(int i = 0; i < strlen(str); i++)
	{
		if(str[i] == chr)
		{
			_c += 1;
		}
	}
	return _c;
}

/*
 * function for get the char of file
 */
int _fcochr
	(const char* filename, char character)
{
	FILE *file_pointer;
	char file_character;
	int char_count = 0;
	file_pointer = fopen(filename, "r");
	if (file_pointer == NULL)
	{
		return -1;
	}
	while ((file_character = fgetc(file_pointer)) != EOF)
	{
		if (file_character == character)
		{
			char_count++;
		}
	}
	fclose(file_pointer);
	return char_count;
}

/*
 * is user character?
 * 0 - if yes
 */
int _isuchr
	(char chr)
{
	/* if digit */
	if(
		chr >= '0'
		&&
		chr <= '9' 
	)
	{
		return 0;
	}
	else if(
		chr >= 'A'
		&&
		chr <= 'z' 
	)
	{
		return 0;
	}
	/* if not user char */
	else
	{
		return -1;
	}
}

int _isustr
	(const char* str)
{
	int _len = strlen(str);
	for(int i = 0; i < _len; i++)
	{
		if(_isuchr(str[i]) != 0)
		{
			return -1;
		}
	}
	return 0;
}
