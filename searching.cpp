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
#include "./map.h"

/**********************************************************************
* Function:    	int breadth_first_search(Map); 
* Description: 	Finds start position and then performs breadth-first-
* 	       	search for end position from start. Returns 1 if end
* 	       	position found, else returns 0 (if end position is in
* 	       	part of map inaccessible from start).	
**********************************************************************/
int breadth_first_search(Map map){
	deque<int> r_pos;
	deque<int> c_pos;

	int cur_r, cur_c;
	find_start(map, cur_r, cur_c);

	do{
		move(cur_r, cur_c);
		map[cur_r][cur_c] = 8; //8 indicates current position
		print_map(map);	
		for(int i = cur_r-1; i <= cur_r+1; i++){
			for(int j = cur_c-1; j <= cur_c+1; j++){
				//if position hasn't been added to queue for checking, add it
				if(i < map.rows() && i > 0 && j < map.columns() && j > 0 && (map[i][j] == 0 || map[i][j] == 9)){	
					r_pos.push_back(i);
					c_pos.push_back(j);		
					//if statement keeps from overwriting our end position	
					if(map[i][j] != 9)
						map[i][j] = 1; //1 indicates position is currently in queue waiting to be checked
					print_map(map);
				}
			}
		}
		map[cur_r][cur_c] = 2;	//indicate that this position has now been checked
		print_map(map);	
		
		//get next position to check
		if(!r_pos.empty() && !c_pos.empty()){
			cur_r = r_pos.front();
			cur_c = c_pos.front();
			r_pos.pop_front();
			c_pos.pop_front();
		}
		else break;

	}while(map[cur_r][cur_c] != 9);

	if(map[cur_r][cur_c] == 9) return 1;
	else return 0;
}

/**********************************************************************
* Function:    	int depth_first_search(Map); 
* Description: 	Finds start position and then performs depth-first-
* 	       	search for end position from start. Returns 1 if end
* 	       	position found, else returns 0 (if end position is in
* 	       	part of map inaccessible from start).	
**********************************************************************/
int depth_first_search(Map map){
	deque<int> r_pos;
	deque<int> c_pos;

	int cur_r, cur_c;
	find_start(map, cur_r, cur_c);

	do{
		move(cur_r, cur_c);
		map[cur_r][cur_c] = 8; //8 indicates current position
		print_map(map);	
		for(int i = cur_r-1; i <= cur_r+1; i++){
			for(int j = cur_c-1; j <= cur_c+1; j++){
				//if position hasn't been added to queue for checking, add it
				if(i < map.rows() && i > 0 && j < map.columns() && j > 0 && (map[i][j] == 0 || map[i][j] == 9)){	
					r_pos.push_back(i);
					c_pos.push_back(j);		
					//if statement keeps from overwriting our end position	
					if(map[i][j] != 9)
						map[i][j] = 1; //1 indicates position is currently in queue waiting to be checked
					print_map(map);
				}
			}
		}
		map[cur_r][cur_c] = 2;	//indicate that this position has now been checked
		print_map(map);	
		
		//get next position for checking
		if(!r_pos.empty() && !c_pos.empty()){
			cur_r = r_pos.back();
			cur_c = c_pos.back();
			r_pos.pop_back();
			c_pos.pop_back();
		}
		else break;

	}while(map[cur_r][cur_c] != 9);

	if(map[cur_r][cur_c] == 9) return 1;
	else return 0;
}

void find_start(Map map, int& cur_r, int& cur_c){
	for(int i = map.border_size(); i < map.rows()-map.border_size(); i++){
		for(int j = map.border_size(); j < map.columns()-map.border_size(); j++){
			if(map[i][j] == 8){
				cur_r = i;
				cur_c = j;
				break;		
			}
			if(map[i][j] == 8) break;
		}
	}
}
