/**********************************************************************
* Filename: 	interface.h
* Author:   	Austin Row
* Date:     	May 2016 (retroactive documentation on 9/9/2016)
* Description:	Header file that contains function signatures for 
* 		functions used in user interface. Corresponding 
* 		implementation file is interface.cpp. 
**********************************************************************/
#ifndef INTERFACE_H
#define INTERFACE_H

#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>	//usleep()
#include "./map.h"

using namespace std;

void place_ending(Map&);
void place_start(Map&);
void print_header(WINDOW*, char*);
int get_search_type(WINDOW*);
void print_map(Map);
void color_print(char, int);
void draw_border(WINDOW*, int);
#endif
