/**********************************************************************
* Filename: 	interface.cpp
* Author:   	Austin Row
* Date:     	May 2016 (retroactive documentation on 9/9/2016)
* Description:	Contains implementations for user interface functions. 
**********************************************************************/

#include "./interface.h"

/**********************************************************************
* Function:    	int get_search_type(WINDOW*); 
* Description:	Prompts user for search type (1 indicates
* 		breadth-first and 0 depth-first) and returns user's
* 		choice.		
**********************************************************************/
int get_search_type(WINDOW* header){
	char choice = '9';
	while(true){ 
		print_header(header, const_cast<char*>("Search Type (Depth-First-0, Breadth-First-1): "));
		echo();
		choice = wgetch(header);
		usleep(1000000);
		noecho();
		if(choice != '1' && choice != '0'){
			print_header(header, const_cast<char*>("Invalid Choice, Please Try Again."));
			usleep(1500000);
		}
		else
			break;
	}
	if(choice) print_header(header, const_cast<char*>("Breadth-First Search"));
	else		print_header(header, const_cast<char*>("Depth-First Search"));
	usleep(1000000);
	return choice-48;
}

/**********************************************************************
* Function:    	void print_header(WINDOW*, char*); 
* Description:	Displays the provided cstring centered in the window
* 		pointed to by the provided WINDOW*.		
**********************************************************************/
void print_header(WINDOW* header, char* title){
	int length = strlen(title), x,y;
	getmaxyx(header, y, x);
	wclear(header);
	wattron(header, COLOR_PAIR(6)| A_BOLD);
	for(int i = 0; i < y; i++){
		for(int j = 0; j < x; j++){
			wprintw(header, " ");
		}
	}	
	mvwprintw(header, 1, x/2-length/2, title);
	wattroff(header, COLOR_PAIR(6) | A_BOLD);
	wrefresh(header);
}

/**********************************************************************
* Function:    	void place_ending(Map); 
* Description:	Places the end position in a valid spot on the map.		
**********************************************************************/
void place_ending(Map& map){
	int end_y, end_x;
	do{
		end_y = rand() % (map.rows()-map.border_size()) + map.border_size();
		end_x = rand() % (map.columns()-map.border_size()) + map.border_size();
	}while(map[end_y][end_x] == 7);

	map[end_y][end_x] = 9;
}

/**********************************************************************
* Function:    	void place_start(Map); 
* Description:	Places the start position in a valid spot on the map.		
**********************************************************************/
void place_start(Map& map){
	int start_y, start_x;
	do{
		start_y = rand() % (map.rows()-map.border_size()) + map.border_size();
		start_x = rand() % (map.columns()-map.border_size()) + map.border_size();
	}while(map[start_y][start_x] == 7);

	map[start_y][start_x] = 8;
}

/**********************************************************************
* Function:    	void draw_border(WINDOW*, int); 
* Description:	Draws a border around a provided screen.
* 		
* 		Note: This function has been used primarily for 
* 		testing puporses and is not currently used in final
* 		implementation.		
**********************************************************************/
void draw_border(WINDOW* screen, int color_scheme){
	int x, y, i;
	attron(COLOR_PAIR(color_scheme));
	getmaxyx(screen, y, x);
	mvwprintw(screen, 0,0,"+");
	mvwprintw(screen, y-1,0,"+");
	mvwprintw(screen, 0,x-1,"+");
	mvwprintw(screen, y-1,x-1,"+");
	for(i = 1; i < (y-1); i++){
		mvwprintw(screen, i, 0, "|");
		mvwprintw(screen, i, x-1, "|");	
	}
	for(i = 1; i < (x-1); i++){
		mvwprintw(screen, 0, i,"-");
		mvwprintw(screen, (y-1), i, "-");
	}
	attroff(COLOR_PAIR(color_scheme));
}

/**********************************************************************
* Function:     void print_map(Map); 
* Description:	Prints the map with appropriate coloring.
**********************************************************************/
void print_map(Map map){
	move(0,0);
	for(int i = 0; i < map.rows(); i++){
		for(int j = 0; j < map.columns(); j++){
			if(map[i][j] == 9) 	color_print('E', 1);	//indicates end position
			else if(map[i][j] == 8) color_print('S', 2);	//indicates start position
			else if(map[i][j] == 1) color_print(' ', 3);	//indicates position currently in deque waiting to be checked by search algorithm
			else if(map[i][j] == 2) color_print(' ', 4);	//indicates position that has been checked by searching algorithm	
			else if(map[i][j] == 7) color_print(' ', 5);	//indicates wall
			else			printw(" ");
		}
	}
	refresh();
}

/**********************************************************************
* Function:     void print_space(int); 
* Description:	Prints space with indicated color pair.
**********************************************************************/
void color_print(char letter, int color){
	attron(COLOR_PAIR(color) | A_BOLD);
	printw("%c", letter);
	attroff(COLOR_PAIR(color) | A_BOLD);
}
