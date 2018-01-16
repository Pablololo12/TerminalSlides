/*
 * Pablololo12
 * pabloheralm@gmail.com
 */

#define MAX_BUFF 100

struct words
{
	int x;
	int y;
	char just;
	int lines;
	char *chars;
	struct words *next;
};

struct slide
{
	int number;
	struct words *words;
	struct slide *next;
	struct slide *prev;
};