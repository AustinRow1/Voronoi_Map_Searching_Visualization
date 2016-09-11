#include "./map.h"

Map::Map(){
	map = NULL;
	row = 0;
	col = 0;
	border_thickness = 1;
}

Map::Map(int rows, int columns){
	row = rows;
	col = columns;
	border_thickness = 1;

	map = new int*[row];
	for(int i = 0; i < row; i++)
		map[i] = new int[col];

	erase();
}

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

void Map::voronoi_generate(){
	//srand(time(NULL));
	int** copy;
	
	int num_centers = int (ceil((row-border_thickness*2)*(col-border_thickness*2)*0.05)), placed_centers = 0, center_r, center_c, closest_val;
	for(int i = 0; i < row; i++){	
		//Create outer boundary/border.
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

	copy = clone(map);

	//Set each position in the map to either a wall or not a wall depending on what type of site/center it is closest to.
	for(int i = border_thickness; i < row-border_thickness; i++){
		for(int j = border_thickness; j < col-border_thickness; j++){
			if(map[i][j] != -2 && map[i][j] != -3){
				closest_val = closest_center(copy, i, j); 
				if(closest_val == -3)  map[i][j] = 0;	//CHANGE THIS WHEN PROGRAM IS STABLE TO IF(CLOSEST_VAL != -3) MAP[I][J] = 7;
				else		       map[i][j] = 7;
				//getchar(); //for demonstrating map gen slowly (waits to put next piece in place until a key is pressed)
			}
			
			#ifndef skip_map_gen
			//Visualizes creation of walls based on what site/center they are closest to.
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
	delete_clone(copy);
}

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

void Map::resize_border(int new_size){
	border_thickness = new_size;
}

int Map::rows() const{
	return row;
}

int Map::columns() const{
	return col;
}

int Map::border_size() const{
	return border_thickness;
}

int Map::at(int row_index, int col_index) const{
	return map[row_index][col_index];
}

void Map::clear(){
	for(int i = 0; i < row; i++){
		for(int j = 0; j < col; j++)
			if(map[i][j] != 7) map[i][j] = 0;
	}
}

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

int* Map::operator[](int row_index){
	return map[row_index];	
}

Map::~Map(){
	for(int i = 0; i < row; i++)
		delete [] map[i];
	delete [] map;

	map = NULL;
}

int Map::closest_center(int** map_copy, int row_position, int col_position){
	int** copy = clone(map_copy);	//don't want to make changes for the purposes of searching to map_copy as that would change the copy variable back in voronoi_generation()
	queue<pair<int, int> > positions;
	pair<int, int> temp;
	int current_row = row_position, current_col = col_position;
	int* current = &copy[row_position][col_position];

	while(*current != -2 && *current != -3){
		*current = 1;	//don't think this line is necessary, should already have been set to 1 when it was stored in the queue
		
		for(int i = current_row-1; i <= current_row+1; i++){
			for(int j = current_col-1; j <= current_col+1; j++){
				if(copy[i][j] != 1 && copy[i][j] != 7){  //1 indicates position has already been checked, 7 indicates wall
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

int** Map::clone(int** original){
	int** clone = new int*[row];
	for(int i = 0 ; i < row; i++){
		clone[i] = new int[col];
		for(int j = 0; j < col; j++)
			clone[i][j] = original[i][j];
	}	
	
	return clone;	
}

void Map::delete_clone(int** clone){
	for(int i = 0; i < row; i++)
		delete [] clone[i];
	delete [] clone;
}

void Map::erase(){
	for(int i = 0; i < row; i++){
		for(int j = 0; j < col; j++)
			map[i][j] = 0;
	}
}
