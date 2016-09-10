full_visualization = full_visualization.out
search_only_visualization = search_only.out
CC = g++

all: $(full_visualization) $(search_only_visualization)

$(full_visualization): main.cpp voronoi.cpp searching.cpp interface.cpp
	$(CC) main.cpp voronoi.cpp searching.cpp interface.cpp -lncurses -o $(full_visualization)

$(search_only_visualization): main.cpp voronoi.cpp searching.cpp interface.cpp
	$(CC) main.cpp voronoi.cpp searching.cpp interface.cpp -lncurses -o $(search_only_visualization) -D skip_map_gen
clean:
	rm *.out
