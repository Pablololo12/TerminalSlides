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
	int color;
	char *chars;
	struct words *next;
};

struct slide
{
	int number;
	int color;
	struct words *words;
	struct slide *next;
	struct slide *prev;
};