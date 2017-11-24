#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include <chrono>

int read_graph(FILE* file, int ***adjacency_matrix) {
	char buffer[127];
	fgets(buffer, 127, (FILE*)file);

	int num_vertices = 0;
	int num_edges = 0;

	
	char* substring = strtok(buffer, " ");
	for (int i = 0; i < 4; i++, substring = strtok(nullptr, " ")) {
		if (i == 2) {
			num_vertices = atoi(substring);
		}
		else if (i == 3) {
			num_edges = atoi(substring);
		}
	}

	//initializes matrix
	*adjacency_matrix = new int*[num_vertices];
	for (int i = 0; i < num_vertices; i++) {
		(*adjacency_matrix)[i] = new int[num_vertices];
		for (int j = 0; j < num_vertices; j++) {
			(*adjacency_matrix)[i][j] = 0;
		}
	}

	//Fills matrix with valus from file
	for (int i = 0; i < num_edges; i++) {
		fgets(buffer, 127, (FILE*)file);
		substring = strtok(buffer, " ");

		substring = strtok(nullptr, " ");
		int index_v1 = atoi(substring);
		index_v1--;

		substring = strtok(nullptr, " ");
		int index_v2 = atoi(substring);
		index_v2--;

		(*adjacency_matrix)[index_v1][index_v2] = 1;
		(*adjacency_matrix)[index_v2][index_v1] = 1;
	}

	return num_vertices;
}

int main(int argc, char* argv[]) {

	if (argc == 2) {
		printf("File: %s\n", argv[1]);

		FILE *file;
		file = fopen(argv[1], "r");

		if (file == NULL) {
			printf("Could not open specified file\n");
			return 1;
		}

		int** adj_matrix; //adjacency Matrix
		int num_vertices = read_graph(file, &adj_matrix);

		for (int i = 0; i < num_vertices; i++) {
			for (int j = 0; j < num_vertices; j++) {
				std::cout << adj_matrix[i][j] << " ";
			}
			std::cout << std::endl;
		}
	}
	else {
		std::cout << "Invalid number of arguments. Two arguments expected:" << std::endl <<
		"1 - filename" << std::endl << std::endl;
	}

	return 0;
}