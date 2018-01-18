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

	if ( has_colors() && can_change_color() && COLOR_PAIRS >= 13 ) {
		init_pair(1, COLOR_RED, COLOR_BLACK);
		init_pair(2, COLOR_GREEN, COLOR_BLACK);
		init_pair(3, COLOR_BLUE, COLOR_BLACK);
		init_pair(4, COLOR_WHITE, COLOR_BLACK);
		init_pair(5, COLOR_RED, COLOR_WHITE);
		init_pair(6, COLOR_GREEN, COLOR_WHITE);
		init_pair(7, COLOR_BLUE, COLOR_WHITE);
		init_pair(8, COLOR_BLACK, COLOR_WHITE);
	} else {
		printf("Pantalla no soportada\n");
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
	struct words* aux_w = NULL;
	char *buff;
	char discard[5];
	int x,y,lines,i,color;
	char opt;
	char just;


	confFile = fopen(fileName, "r");
	if (confFile == NULL) {
		return 0;
	}

	while(!feof(confFile)) {
		opt = fgetc(confFile);
		if (opt == '#') {
			color = fgetc(confFile);
			if (start == NULL) {
				start = (struct slide*) malloc(sizeof(struct slide));
				start->number = ++num;
				start->color = color == 'W' ? 1 : 0;
				current = start;
				current->words = NULL;
				current->prev = NULL;
			} else {
				current->next = (struct slide*) malloc(sizeof(struct slide));
				aux = current;
				current = current->next;
				current->prev = aux;
				current->number = ++num;
				current->color = color == 'W' ? 1 : 0;
				current->words = NULL;
			}
		} else if (opt == '%') {
			buff = (char *) malloc(sizeof(char)*MAX_BUFF);
			fscanf(confFile, "%d:%d:%c:%d:", &x, &y, &just, &color);
			fgets(buff,MAX_BUFF,confFile);
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
			aux_w->lines = 0;
			aux_w->just = just;
			aux_w->chars = buff;
			aux_w->color = color;
		} else if (opt == '&') {
			fscanf(confFile, "%d:%d:%c:%d:%d", &x, &y, &just,&color, &lines);
			fgets(discard,5,confFile); // To read the rest of the line
			for(i=0;i<lines;i++) {
				buff = (char *) malloc(sizeof(char)*MAX_BUFF);
				fgets(buff,MAX_BUFF,confFile);
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
				aux_w->lines = lines;
				aux_w->just = just;
				aux_w->chars = buff;
				aux_w->color = color;
			}
		}
	}

	current = start;
	fclose(confFile);
	return 1;
}

int draw()
{
	int x,y,pos_y,pos_x,len,i,bg;
	struct words* aux_w;

	erase(); //Clean up window

	// get size of windows
	getmaxyx(mainwin, y, x);

	bg = current->color;
	if (bg == 1) { //white
		wbkgd(mainwin, COLOR_PAIR(8));
	} else {
		wbkgd(mainwin, COLOR_PAIR(4));
	}

	aux_w = current->words;
	while (aux_w != NULL) {
		color_set(aux_w->color+4*bg, NULL);
		pos_y = y*aux_w->y/100;
		pos_x = x*aux_w->x/100;
		len = strlen(aux_w->chars);
		if (aux_w->just == 'C') {
			pos_x = pos_x - len/2;
		} else if (aux_w->just == 'R') {
			pos_x = pos_x - len;
		}
		if (aux_w->lines==0) {
			mvwprintw(mainwin, pos_y, pos_x, "%s", aux_w->chars);
			aux_w = aux_w->next;
		} else {
			i = 0;
			len = aux_w->lines;
			while(aux_w != NULL && i<len) {
				mvwprintw(mainwin, pos_y+i, pos_x, "%s", aux_w->chars);
				i++;
				aux_w = aux_w->next;
			}
		}
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

	if (read_file(argv[1]) != 1) {
		printf("Error reading file\n");
		return 1;
	}

	initialize_win();

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