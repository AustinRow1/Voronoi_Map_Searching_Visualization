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

	int closest_center(int**, int, int);
	int** clone(int**);
	void delete_clone(int**);

	public:
		Map();
		Map(int, int);
		Map(const Map &);
		void voronoi_generate();
		void resize(int, int);
		int rows() const;
		int columns() const;
		int border_size() const;
		int at(int, int) const;
		void resize_border(int);
		void clear();
		void erase();
		void operator=(const Map &);
		int* operator[](int);
		~Map();
};
/*
class Map_Proxy{
	
}
*/
#endif
