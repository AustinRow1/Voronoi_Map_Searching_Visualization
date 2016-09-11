/**********************************************************************
* Filename: 	map.h
* Author:   	Austin Row
* Date:     	9/10/2016
* Description:	Header file that contains Map class definition. 
**********************************************************************/
#ifndef MAP_H
#define MAP_H

#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
#include <utility>
#include <queue>
#include <math.h>

using namespace std;

class Map{
	int** map;
	int row, col, border_thickness;
	int closest_center(Map, int, int);

	public:
		Map();
		Map(int, int, int);
		Map(const Map &);
		void clone(int**, int, int, int);
		void resize(int, int);
		void resize_border(int);
		int rows() const;
		int columns() const;
		int border_size() const;
		int at(int, int) const;
		void clear();
		void erase();
		void voronoi_generate();
		void operator=(const Map &);
		int* operator[](int);
		~Map();
};
#endif
