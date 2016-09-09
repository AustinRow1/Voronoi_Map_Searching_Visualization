/**********************************************************************
* Filename: 	searching.cpp
* Author:   	Austin Row
* Date:     	May 2016 (retroactive documentation on 9/9/2016)
* Description:	Contains implementations for breadth and depth first
* 		searches. The corresponding header file is 
* 		searching.h.
*
* 		Note: The only difference between the functions
* 		lies in what end of the deque that the next position
* 		to check is taken from. I chose to write to seperate
* 		functions rather than one with a search-choice 
* 		indicator for the sake of clarity.
**********************************************************************/

#include "./searching.h"
#include "./interface.h"

/**********************************************************************
* Function:    	int breadth_first_search(int**, int, int); 
* Description: 	Finds start position and then performs breadth-first-
* 	       	search for end position from start. Returns 1 if end
* 	       	position found, else returns 0 (if end position is in
* 	       	part of map inaccessible from start).	
**********************************************************************/
int breadth_first_search(int** grid, int row, int col){
	deque<int> r_pos;
	deque<int> c_pos;

	int cur_r, cur_c;
	//Find start position (denoted by 8 in the integer grid).
	for(int i = 2; i < row-2; i++){
		for(int j = 2; j < col-2; j++){
			if(grid[i][j] == 8){
				cur_r = i;
				cur_c = j;
				break;		
			}
			if(grid[i][j] == 8) break;
		}
	}
	do{
		move(cur_r, cur_c);
		grid[cur_r][cur_c] = 8; //8 indicates current position
		print_map(grid, row, col);	
		for(int i = cur_r-1; i <= cur_r+1; i++){
			for(int j = cur_c-1; j <= cur_c+1; j++){
				//if position hasn't been added to queue for checking, add it
				if(i < row && i > 0 && j < col && j > 0 && (grid[i][j] == 0 || grid[i][j] == 9)){	
					r_pos.push_back(i);
					c_pos.push_back(j);		
					//if statement keeps from overwriting our end position	
					if(grid[i][j] != 9)
						grid[i][j] = 1; //1 indicates position is currently in queue waiting to be checked
					print_map(grid, row, col);
				}
			}
		}
		grid[cur_r][cur_c] = 2;	//indicate that this position has now been checked
		print_map(grid, row, col);	
		
		//get next position to check
		if(!r_pos.empty() && !c_pos.empty()){
			cur_r = r_pos.front();
			cur_c = c_pos.front();
			r_pos.pop_front();
			c_pos.pop_front();
		}
		else break;

	}while(grid[cur_r][cur_c] != 9);

	if(grid[cur_r][cur_c] == 9) return 1;
	else return 0;
}

/**********************************************************************
* Function:    	int depth_first_search(int**, int, int); 
* Description: 	Finds start position and then performs depth-first-
* 	       	search for end position from start. Returns 1 if end
* 	       	position found, else returns 0 (if end position is in
* 	       	part of map inaccessible from start).	
**********************************************************************/
int depth_first_search(int** grid, int row, int col){
	deque<int> r_pos;
	deque<int> c_pos;

	int cur_r, cur_c;
	for(int i = 2; i < row-2; i++){
		for(int j = 2; j < col-2; j++){
			if(grid[i][j] == 8){
				cur_r = i;
				cur_c = j;
				break;		
			}
			if(grid[i][j] == 8) break;
		}
	}
	do{
		move(cur_r, cur_c);
		grid[cur_r][cur_c] = 8; //8 indicates current position
		print_map(grid, row, col);	
		for(int i = cur_r-1; i <= cur_r+1; i++){
			for(int j = cur_c-1; j <= cur_c+1; j++){
				//if position hasn't been added to queue for checking, add it
				if(i < row && i > 0 && j < col && j > 0 && (grid[i][j] == 0 || grid[i][j] == 9)){	
					r_pos.push_back(i);
					c_pos.push_back(j);		
					//if statement keeps from overwriting our end position	
					if(grid[i][j] != 9)
						grid[i][j] = 1; //1 indicates position is currently in queue waiting to be checked
					print_map(grid, row, col);
				}
			}
		}
		grid[cur_r][cur_c] = 2;	//indicate that this position has now been checked
		print_map(grid, row, col);	
		
		//get next position for checking
		if(!r_pos.empty() && !c_pos.empty()){
			cur_r = r_pos.back();
			cur_c = c_pos.back();
			r_pos.pop_back();
			c_pos.pop_back();
		}
		else break;

	}while(grid[cur_r][cur_c] != 9);

	if(grid[cur_r][cur_c] == 9) return 1;
	else return 0;
}

