/***********************************************************************************
* Filename: 	main.cpp
* Author:   	Austin Row
* Date:     	May 2016 (retroactive documentation on 9/9/2016)
* Description:	Contains main() function to run entire program along with helper
* 		functions to help complete the task.
*
* 		Note: There will be a small memory leak in this
* 		program that is proportional to the terminal window
* 		size. This leak comes from the ncurses library.
* 		Relevant information is discussed here: 
* 		http://invisible-island.net/ncurses/ncurses.faq.html#config_leaks
**********************************************************************************/
#include <ncurses.h>
#include <time.h>
#include "./searching.h"
#include "./interface.h"
#include "./map.h"

using namespace std;

void initialize_colors();
void initialize_windows(WINDOW* &, int&, int&);
int search(Map, int);
void print_result(WINDOW*, int);
char end_choice(WINDOW*);

/**********************************************************************
* Function:    	int main(); 
* Description:	Runs program.
**********************************************************************/
int main(){
	//General Initialization
	int row, col, success = 0;
	char answer;
	WINDOW* header;
	Map map;

	srand(time(NULL));
	initscr();
	noecho();
	curs_set(0);

	initialize_colors();	
	initialize_windows(header, row, col);		
	
	map.resize(row, col);
	
	//Program Loop
	do{
		print_header(header, const_cast<char*>("Voronoi Map Generation"));
		map.voronoi_generate();
		
		#ifndef skip_map_gen	
		usleep(2000000);
		#endif
		
		do{
			map.clear();
			place_ending(map);
			place_start(map);
			print_map(map);
				
			int search_type = get_search_type(header);
			success = search(map, search_type);

			attron(COLOR_PAIR(6)| A_BOLD);
			print_result(header, success);	
			answer = end_choice(header);
			attroff(COLOR_PAIR(6) | A_BOLD);
			
		}while(answer > '1');
	}while(answer > '0');
	
	delwin(stdscr);	
	delwin(header);
	endwin();
	return 0;
}

/**********************************************************************
* Function:    	void initialize_colors(); 
* Description:	Initializes the color pairs that will be used.
**********************************************************************/
void initialize_colors(){
	start_color();

	init_pair(1, COLOR_GREEN, COLOR_BLUE); //for final destination
	init_pair(2, COLOR_BLUE, COLOR_GREEN); //for current position
	init_pair(3, COLOR_RED, COLOR_GREEN); //for positions in stack/queue waiting to be checked
	init_pair(4, COLOR_RED, COLOR_CYAN); //for checked positions
	init_pair(5, COLOR_RED, COLOR_MAGENTA); //for walls
	init_pair(6, COLOR_BLACK, COLOR_WHITE); //for "found" message at end
	init_pair(7, COLOR_CYAN, COLOR_BLACK); //header printing
}

/**********************************************************************
* Function:    	void initialize_windows(); 
* Description:	Sizes the header and stdscr windows to the proper
* 		size and sets row and col.
**********************************************************************/
void initialize_windows(WINDOW* &header, int& row, int& col){
	int header_height = 3;

	getmaxyx(stdscr, row, col);
	header = newwin(header_height, col, 0, 0); 
	row = row-header_height;
	wresize(stdscr, row, col);
	mvwin(stdscr, header_height, 0);
}

/**********************************************************************
* Function:    	int search(); 
* Description:	Gets type of search user wants to perform, performs
* 		search, and returns result.
**********************************************************************/
int search(Map map, int search_type){
	if(search_type)	return breadth_first_search(map);
	else		return depth_first_search(map);
}

/**********************************************************************
* Function:    	void print_result(); 
* Description:	Prints the provided (search) result to the header 
* 		window.
**********************************************************************/
void print_result(WINDOW* header, int success){
	if(success) print_header(header, const_cast<char*>("Found!"));
	else print_header(header, const_cast<char*>("Not Found."));
	usleep(1500000);

}

/**********************************************************************
* Function:    	char end_choice(); 
* Description:	Prompts user and returns their choice for what they
* 		want to do after search is complete.
**********************************************************************/
char end_choice(WINDOW* header){
	print_header(header, const_cast<char*>("0-Quit, 1-New Map, 2-Same Map"));
	return (char)getch();
}
