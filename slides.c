/*
 * Pablololo12
 * pabloheralm@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include "slides.h"

static WINDOW * mainwin;

struct slide *start;
struct slide *current;
/*
 * Initialize the screen
 */
int initialize_win()
{
	if ( (mainwin = initscr()) == NULL ) {
		fprintf(stderr, "Error initialising ncurses.\n");
		exit(EXIT_FAILURE);
	}

	cbreak(); // One key at time
	noecho(); // Not print any key
	keypad(stdscr, TRUE); // To get arrow keys

	start_color();

	if ( has_colors() && COLOR_PAIRS >= 13 ) {

		init_pair(1,  COLOR_RED,     COLOR_BLACK);
		init_pair(2,  COLOR_WHITE,   COLOR_BLACK);
	}

	refresh();

	return 1;
}

/*
 * Call to erase screen
 */
int destroy()
{
	erase();
	delwin(mainwin);
	endwin();
	refresh();

	return 0;
}

int read_file(char *fileName)
{
	FILE *confFile;
	int num = 0;
	struct slide* aux;
	struct words* aux_w;
	char *buff;
	int x,y;
	char opt;
	char just;


	confFile = fopen(fileName, "r");
	if (confFile == NULL) {
		return 0;
	}

	while(!feof(confFile)) {
		opt = fgetc(confFile);
		if (opt == '#') {
			if (start == NULL) {
				start = (struct slide*) malloc(sizeof(struct slide));
				start->number = ++num;
				current = start;
			} else {
				current->next = (struct slide*) malloc(sizeof(struct slide));
				aux = current;
				current = current->next;
				current->prev = aux;
				current->number = ++num;
				aux_w = NULL;
			}
		} else if (opt == '%') {
			buff = (char *) malloc(sizeof(char)*MAX_BUFF);
			fscanf(confFile, "%d:%d:%c:%[^\n]", &x, &y, &just, buff);
			if (current->words == NULL) {
				current->words = (struct words*) malloc(sizeof(struct words));
				aux_w = current->words;
			} else {
				#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
				aux_w->next = (struct words*) malloc(sizeof(struct words));
				aux_w = aux_w->next;
			}
			aux_w->x = x;
			aux_w->y = y;
			aux_w->just = just;
			aux_w->chars = buff;
		}
	}

	current = start;
	fclose(confFile);
	return 1;
}

int draw()
{
	int x,y,pos_y,pos_x,len;
	struct words* aux_w;

	erase(); //Clean up window

	// get size of windows
	getmaxyx(mainwin, y, x);

	color_set(2, NULL);

	aux_w = current->words;
	while (aux_w != NULL) {
		pos_y = y*aux_w->y/100;
		pos_x = x*aux_w->x/100;
		len = strlen(aux_w->chars);
		if (aux_w->just == 'C') {
			pos_x = pos_x - len/2;
		} else if (aux_w->just == 'R') {
			pos_x = pos_x - len;
		}
		mvwprintw(mainwin, pos_y, pos_x, "%s", aux_w->chars);
		aux_w = aux_w->next;
	}

	mvwprintw(mainwin, y-1, x-1, "%d", current->number);

	refresh();
	return 1;
}


int main(int argc, char **argv)
{
	int ch;

	if (argc < 2) {
		printf("Error not enough arguments\n");
		printf("Usage: ./slides [slides file]\n");
		return 1;
	}

	initialize_win();
	if (read_file(argv[1]) != 1) {
		printf("Error reading file\n");
		return 1;
	}

	draw();
	while (1) {
		ch = getch();
		if(ch == KEY_DOWN || ch == KEY_RIGHT || ch == ' ' || ch == '\n') {
			if(current->next!=NULL) {
				current = current->next;
				draw();
			}
		} else if (ch == KEY_UP || ch == KEY_LEFT) {
			if(current->prev!=NULL) {
				current = current->prev;
				draw();
			}
		} else if (ch == 27 || ch == 'q') {
			break;
		}
	}
	destroy();
	return 0;
}