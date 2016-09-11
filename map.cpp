/**********************************************************************
* Filename: 	map.cpp
* Author:   	Austin Row
* Date:     	9/10/2016
* Description:	Contains implementations for Map class member
* 		functions.
**********************************************************************/
#include "./map.h"

/**********************************************************************
* Function:    	public Map::Map(); 
* Description:	Default constructor for Map class.		
**********************************************************************/
Map::Map(){
	map = NULL;
	row = 0;
	col = 0;
	border_thickness = 1;
}

/**********************************************************************
* Function:    	public Map::Map(int, int); 
* Description:	Non-default constructor for initializing Map object
* 		with number of rows and columns.		
**********************************************************************/
Map::Map(int rows, int columns, int border){
	row = rows;
	col = columns;
	border_thickness = border;

	map = new int*[row];
	for(int i = 0; i < row; i++)
		map[i] = new int[col];

	erase();
}

/**********************************************************************
* Function:    	public Map::Map(const Map&); 
* Description:	Copy constructor for map class.		
**********************************************************************/
Map::Map(const Map &other){
	row = other.rows();
	col = other.columns();
	border_thickness = other.border_size();

	map = new int*[row];
	for(int i = 0; i < row; i++){
		map[i] = new int[col];
		for(int j = 0; j < col; j++)
			map[i][j] = other.at(i,j);
	}
}

/**********************************************************************
* Function:    	public void Map::clone(int**, int, int, int); 
* Description:	Essentially provide functionality of second 
* 		more specific non-default constructor that isn't 
* 		called when object is constructed.  		
**********************************************************************/
void Map::clone(int** original, int rows, int cols, int border){
	row = rows;
	col = cols;
	border_thickness = border;

	map = new int*[rows];
	for(int i = 0 ; i < rows; i++){
		map[i] = new int[cols];
		for(int j = 0; j < cols; j++)
			map[i][j] = original[i][j];
	}	
}

/**********************************************************************
* Function:    	public void Map::resize(int, int); 
* Description:	Resizes map to have provided number of rows and 
* 		columns.		
**********************************************************************/
void Map::resize(int rows, int columns){
	if(map != NULL){
		for(int i = 0; i < row; i++)
			delete [] map[i];
		delete [] map;
	}
	
	row = rows;
	col = columns;

	map = new int*[row];
	for(int i = 0; i < row; i++)
		map[i] = new int[col];
}

/**********************************************************************
* Function:    	public void Map::resize_border(int); 
* Description:	Resizes map border.
* 		
* 		Note: The new border size only affects future 
* 		generated maps, not the current one. 
**********************************************************************/
void Map::resize_border(int new_size){
	border_thickness = new_size;
}

/**********************************************************************
* Function:    	public int Map::rows(); 
* Description:	Returns the number of rows the current map has.		
**********************************************************************/
int Map::rows() const{
	return row;
}

/**********************************************************************
* Function:    	public int Map::columns(); 
* Description:	Returns the number of columns the current map has.		
**********************************************************************/
int Map::columns() const{
	return col;
}

/**********************************************************************
* Function:    	public int Map::border_size(); 
* Description:	Returns border_thickness.		
**********************************************************************/
int Map::border_size() const{
	return border_thickness;
}

/**********************************************************************
* Function:    	public int Map::at(int, int); 
* Description:	Returns element at provided indices in map.		
**********************************************************************/
int Map::at(int row_index, int col_index) const{
	return map[row_index][col_index];
}

/**********************************************************************
* Function:    	public void Map::clear(); 
* Description:	Clears the map, leaving only its walls.		
**********************************************************************/
void Map::clear(){
	for(int i = 0; i < row; i++){
		for(int j = 0; j < col; j++)
			if(map[i][j] != 7) map[i][j] = 0;
	}
}

/**********************************************************************
* Function:    	public void Map::erase(); 
* Description:	Clears the entire map including the walls.		
**********************************************************************/
void Map::erase(){
	for(int i = 0; i < row; i++){
		for(int j = 0; j < col; j++)
			map[i][j] = 0;
	}
}

/**********************************************************************
* Function:    	public void Map::voronoi_generate(); 
* Description:	Fills int** map in the Map class using voronoi diagram
* 		principles for procedural map generation.
* Precondition: rand() has been seeded.		
**********************************************************************/
void Map::voronoi_generate(){
	Map copy;
	int num_centers = int (ceil((row-border_thickness*2)*(col-border_thickness*2)*0.05)), placed_centers = 0, center_r, center_c, closest_val;
	erase();

	//Fill outer boundary/border.
	for(int i = 0; i < row; i++){	
		for(int j = 0; j < col; j++){
			if(i < border_thickness || i > row -(border_thickness+1) || j < border_thickness || j > col - (border_thickness+1)) map[i][j] = 7; //7 indicates wall.
			else	map[i][j] = 0; //0 means empty 
		}
	}	
		
	//Randomly place sites/centers for voronoi map generation.
	for(placed_centers = 0; placed_centers < num_centers;){
		center_r = rand() % (row-border_thickness) + border_thickness;
		center_c = rand() % (col-border_thickness) + border_thickness;
		
		//Place center only if there is nothing already taking up randomly generated position.
		if(map[center_r][center_c] == 0){
			if(placed_centers <= num_centers/3) map[center_r][center_c] = -2;
			else				   map[center_r][center_c] = -3;
			
			placed_centers++;
		}
	}
	copy.clone(map, row, col, border_thickness);	//A copy is needed so that the searching for the nearest site/center can be done on a map containing only sites/centers and no walls.
	
	//Set each position in the map to either a wall or not a wall depending on what type of site/center it is closest to.
	for(int i = border_thickness; i < row-border_thickness; i++){
		for(int j = border_thickness; j < col-border_thickness; j++){
			if(map[i][j] != -2 && map[i][j] != -3){
				closest_val = closest_center(copy, i, j); 
				if(closest_val != -3) map[i][j] = 7;
			}
			
			#ifndef skip_map_gen
			//Visualizes creation of walls based on what site/center they are closest to.
			/*if(closest_val == -2)
				getchar(); //for demonstrating map gen slowly (waits to put next piece in place until a key is pressed)*/
			move(0,0);
			for(int x = 0; x < row; x++){
				for(int y = 0; y < col; y++){
					if(map[x][y] == 7) printw("#");
					else if(copy[x][y] == -2) printw("W");
					else if(copy[x][y] == -3) printw("N");
					else printw(" ");
				}
			}
			refresh();
			usleep(5000000/(row*col));	
			#endif
		}
	}
	
	//Covert sites/centers to corresponding wall/not wall.
	for(int i = border_thickness; i < row-border_thickness; i++){
		for(int j = border_thickness; j < col-border_thickness; j++){
			if(copy[i][j] == -2) map[i][j] = 7;
			else if(copy[i][j] == -3) map[i][j] = 0;				
		}
	}
	
	#ifndef skip_map_gen
	//Used in visualization to convert Wall sites/centers and Not-Wall sites/centers from W's and N's to walls and spaces respectively.	
	move(0,0);
	for(int x = 0; x < row; x++){
		for(int y = 0; y < col; y++){
			if(map[x][y] == 7) printw("#");
			else printw(" ");
		}
	}
	refresh();
	usleep(5000);	
	#endif
}

/**********************************************************************
* Function:    	public void Map::operator=(const Map&); 
* Description:	Assignment operator overload for deep copying of Map
* 		objects.		
**********************************************************************/
void Map::operator=(const Map &other){
	if(map != NULL){
		for(int i = 0; i < row; i++)
			delete [] map[i];
		delete [] map;
	}
	
	row = other.rows();
	col = other.columns();
	border_thickness = other.border_size();

	map = new int*[row];
	for(int i = 0; i < row; i++)
		map[i] = new int[col];
}

/**********************************************************************
* Function:    	public void Map::operator[](); 
* Description:	Access operator overload for enabling 2D-array-styled
* 		access to map contents.		
**********************************************************************/
int* Map::operator[](int row_index){
	return map[row_index];	
}

/**********************************************************************
* Function:    	public Map::~Map(); 
* Description:	Map class destructor.		
**********************************************************************/
Map::~Map(){
	for(int i = 0; i < row; i++)
		delete [] map[i];
	delete [] map;

	map = NULL;
}

/**********************************************************************
* Function:    	private int Map::closest_center(); 
* Description:	Returns the closest site/center type (-2/wall or 
* 		-3/not wall) for voronoi map generation.		
**********************************************************************/
int Map::closest_center(Map copy, int row_position, int col_position){
	queue<pair<int, int> > positions;
	pair<int, int> temp;
	int current_row = row_position, current_col = col_position;
	int* current = &copy[row_position][col_position];
	*current = 1;	//1 indicates position is in line to be checked 
	while(*current != -2 && *current != -3){
		for(int i = current_row-1; i <= current_row+1; i++){
			for(int j = current_col-1; j <= current_col+1; j++){
				if(copy[i][j] != 1 && copy[i][j] != 7){ //7 indicates wall 
					temp.first = i;
					temp.second = j;
					positions.push(temp);
					if(copy[i][j] != -2 && copy[i][j] != -3)
						copy[i][j] = 1;
				}
			}
		}
		current_row = positions.front().first;
		current_col = positions.front().second;
		current = &copy[current_row][current_col];
		positions.pop();
	}
	return *current;	
}
