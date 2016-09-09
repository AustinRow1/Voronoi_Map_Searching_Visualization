/**********************************************************************
* Filename: 	voronoi.h
* Author:   	Austin Row
* Date:     	May 2016 (retroactive documentation on 9/9/2016)
* Description:	Header file that contains voronoi procedural map
* 		generation function signatures. Corresponding 
* 		implementation file is voronoi.cpp. 
**********************************************************************/
#ifndef VORONOI_H
#define VORONOI_H

#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>	//usleep()
#include <time.h>
#include<queue>

using std::queue;

int** voronoi_map_gen(int, int); 
int closest_center(int** , int, int, int, int);
int** copy_map(int**, int, int);
void delete_map(int**, int, int);
#endif
