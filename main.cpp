/***********************************************************************************
* Filename: 	main.cpp
* Author:   	Austin Row
* Date:     	May 2016 (retroactive documentation on 9/9/2016)
* Description:	Contains main() function to run entire program.
*
* 		Note: There will be a small memory leak in this
* 		program that is proportional to the terminal window
* 		size. This leak comes from the ncurses library.
* 		Relevant information is discussed here: 
* 		http://invisible-island.net/ncurses/ncurses.faq.html#config_leaks
**********************************************************************************/
#include <ncurses.h>
#include <time.h>
#include "./voronoi.h"
#include "./searching.h"
#include "./interface.h"

#include <iostream>

using namespace std;

/**********************************************************************
* Function:    	int main(); 
* Description:	Runs program.
**********************************************************************/
int main(){
	//General Initialization
	int row, col, success = 0, header_height = 3;
	char answer;
	srand(time(NULL));
	initscr();
	noecho();
	curs_set(0);
	start_color();

	//Color Pair Initialization
	init_pair(1, COLOR_RED, COLOR_BLUE); //for final destination
	init_pair(2, COLOR_RED, COLOR_YELLOW); //for current position
	init_pair(3, COLOR_RED, COLOR_GREEN); //for positions in stack/queue waiting to be checked
	init_pair(4, COLOR_RED, COLOR_CYAN); //for checked positions
	init_pair(5, COLOR_RED, COLOR_MAGENTA); //for walls
	init_pair(6, COLOR_BLACK, COLOR_WHITE); //for "found" message at end
	init_pair(7, COLOR_CYAN, COLOR_BLACK); //header printing

	//Header Initialization and Main Window Resize
	getmaxyx(stdscr, row, col);
	WINDOW* header = newwin(header_height, col, 0, 0); //win
	row = row-header_height;
	wresize(stdscr, row, col);
	mvwin(stdscr, header_height, 0);	
	
	//Program Loop
	int** grid, **copy;
	int search_type;
	do{
		print_header(header, const_cast<char*>("Voronoi Map Generation"));
		grid = voronoi_map_gen(row,col);
		
		#ifndef skip_map_gen	
		usleep(2000000);
		#endif

		do{
			copy = copy_map(grid, row, col); 
			place_ending(copy, row, col);
			place_start(copy, row, col);
			print_map(copy, row, col);
			search_type = get_search_type(header);
			if(search_type){
			       	print_header(header, const_cast<char*>("Breadth-First Search"));
				usleep(1000000);
				success = breadth_first_search(copy, row, col);
			}
			else{
			       	print_header(header, const_cast<char*>("Depth-First Search"));
				usleep(1000000);
				success = depth_first_search(copy, row, col);
			}
		
			attron(COLOR_PAIR(6)| A_BOLD);
			if(success) print_header(header, const_cast<char*>("Found!"));
			else print_header(header, const_cast<char*>("Not Found."));
			usleep(1500000);
			print_header(header, const_cast<char*>("0-Quit, 1-New Map, 2-Same Map"));
			attroff(COLOR_PAIR(6) | A_BOLD);
			
			answer = (char)getch();
			delete_map(copy, row, col);
		}while(answer > '1');
		delete_map(grid, row, col);
	}while(answer > '0');
	delwin(stdscr);	
	delwin(header);
	endwin();
	return 0;
}
