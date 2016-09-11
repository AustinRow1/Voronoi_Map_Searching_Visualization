/**********************************************************************
* Filename: 	searching.h
* Author:   	Austin Row
* Date:     	May 2016 (retroactive documentation on 9/9/2016)
* Description:	Header file that contains searching function 
* 		signatures. Corresponding implementation file is
* 		searching.cpp. 
**********************************************************************/
#ifndef SEARCHING_H
#define SEARCHING_H

#include <ncurses.h>
#include <deque>
#include "./map.h"

using namespace std;

int breadth_first_search(Map);
int depth_first_search(Map);
void find_start(Map, int&, int&);
#endif
