/**********************************************************************
* Filename: 	voronoi.cpp
* Author:   	Austin Row
* Date:     	May 2016 (retroactive documentation on 9/9/2016)
* Description:	Contains implementations for functions used in 
* 		voronoi procedural map generation. 
**********************************************************************/
#include "./voronoi.h"

/**********************************************************************
* Function:    	int** voronoi_map_gen(int, int); 
* Description: 	Generates and returns a 2D integer array that 
* 		represents a map created procedurally using principles
* 		of voronoi diagrams. Visualizes process as it occurs
* 		if the skip_map_gen macro is not defined at compile-
* 		time.
**********************************************************************/
int** voronoi_map_gen(int row, int col){
	int** grid = new int*[row];
	int num_centers = (row-2)*(col-2)/20, placed_centers = 0, center_r, center_c, closest_val;
	for(int i = 0; i < row; i++){
		grid[i] = new int[col];
		
		//Create outer boundary/border.
		for(int j = 0; j < col; j++){
			if(i < 2 || i > row-3 || j < 2 || j > col-3) grid[i][j] = 7; //7 indicates wall.
			else	grid[i][j] = -1; //-1 means not set yet.
		}
	}
	int** copy = copy_map(grid, row, col);	
	
	//Randomly place sites/centers for voronoi map generation.
	for(placed_centers = 0; placed_centers < num_centers;){
		center_r = rand() % (row-2) + 2;
		center_c = rand() % (col-2) + 2;
	
		//Place center only if there is nothing already taking up randomly generated position.
		if(grid[center_r][center_c] == -1){
			if(placed_centers < num_centers/3){  
				grid[center_r][center_c] = -2;
				copy[center_r][center_c] = -2;
			}
			else{
				grid[center_r][center_c] = -3;
				copy[center_r][center_c] = -3;
			}
			placed_centers++;
		}
	}
	
	//Set each position in the map to either a wall or not a wall depending on what type of site/center it is closest to.
	for(int i = 2; i < row-2; i++){
		for(int j = 2; j < col-2; j++){	
			if(grid[i][j] != -2 && grid[i][j] != -3){
				closest_val = closest_center(grid,row,col,i,j); 
				if(closest_val == -3)  copy[i][j] = 0;
				else		       copy[i][j] = 7;
				//getchar(); //for demonstrating map gen slowly (waits to put next piece in place until a key is pressed)
			}
			
			#ifndef skip_map_gen
			//Visualizes creation of walls based on what site/center they are closest to.
			move(0,0);
			for(int x = 0; x < row; x++){
				for(int y = 0; y < col; y++){
					if(copy[x][y] == 7) printw("#");
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

	for(int i = 2; i < row-2; i++){
		for(int j = 2; j < col-2; j++){
			if(grid[i][j] == -2) copy[i][j] = 7;
			else if(grid[i][j] == -3) copy[i][j] = 0;				
		}
	}
	
	#ifndef skip_map_gen
	//Used in visualization to convert Wall sites/centers and Not-Wall sites/centers from W's and N's to walls and spaces respectively.	
	move(0,0);
	for(int x = 0; x < row; x++){
		for(int y = 0; y < col; y++){
			if(copy[x][y] == 7) printw("#");
			else printw(" ");
		}
	}
	refresh();
	usleep(5000);	
	#endif
	delete_map(grid, row, col);
	return copy;
}

/**********************************************************************
* Function:    	int closest_center(int**, int, int, int, int); 
* Description:	Uses a breadth-first search to find the value of 
* 		site/center closest to the current location in the 
* 		grid.	
**********************************************************************/
int closest_center(int** grid, int row, int col, int start_r, int start_c){
	int** copy = copy_map(grid, row, col);
	int cur_r = start_r, cur_c = start_c;
	queue<int> r_pos;
	queue<int> c_pos;
	while(copy[cur_r][cur_c] != -2 && copy[cur_r][cur_c] != -3){
		copy[cur_r][cur_c] = 1;
		for(int i = cur_r-1; i <= cur_r+1; i++){
			for(int j = cur_c-1; j <= cur_c+1; j++){
				if(copy[i][j] != 7 && copy[i][j] != 1){	
					r_pos.push(i);
					c_pos.push(j);
					if(copy[i][j] != -2 && copy[i][j] != -3)	
						copy[i][j] = 1;
				}
			}					
		}
		cur_r = r_pos.front();
		cur_c = c_pos.front();
		r_pos.pop();
		c_pos.pop();	
	}	
	delete_map(copy, row, col);
	return grid[cur_r][cur_c];	
}

/**********************************************************************
* Function:    	int** copy_map(int**, int, int); 
* Description:	Returns a copy of the passed in map.
**********************************************************************/
int** copy_map(int** grid, int row, int col){
	int** copy = new int*[row];

	for(int i = 0; i < row; i++){
		copy[i] = new int[col];
		for(int j = 0; j < col; j++)
			copy[i][j] = grid[i][j];
	}
	return copy;
}

/**********************************************************************
* Function:    	void delete_map(int**, int, int); 
* Description:	Deallocates the memory used for the passed in map.
**********************************************************************/
void delete_map(int** grid, int row, int col){
	for(int i = 0; i < row; i++)
		delete [] grid[i];
	delete [] grid;

	grid = NULL;
}
