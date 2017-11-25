#include "read_file.h"
#include "repair_operator.h"
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

		//starts metaheuristic
		float *solution = new float[7]; //{0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0}
		for (int i = 0; i < 7; i++) {
			if (i == 1 || i == 2) {
				solution[i] = 1.0;
				continue;
			}
			solution[i] = 0.0;
		}

		repair_operator(&solution, adj_matrix, num_vertices);

		std::cout << "SOLUTION: ";
		for (int i = 0; i < num_vertices; i++) {
			std::cout << solution[i] << " ";
		}
	}
	else {
		std::cout << "Invalid number of arguments. Two arguments expected:" << std::endl <<
		"1 - filename" << std::endl << std::endl;
	}

	return 0;
}