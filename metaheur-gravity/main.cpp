#include "read_file.h"
#include <stdio.h>
/*#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include <chrono>*/

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