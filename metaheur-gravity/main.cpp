#include "read_file.h"
#include "rges.h"
#include <iostream>
/*#include <stdlib.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include <chrono>*/

int main(int argc, char* argv[]) {

	if (argc == 6) {
		printf("File: %s\n", argv[1]);

		FILE *file;
		file = fopen(argv[1], "r");

		if (file == NULL) {
			printf("Could not open specified file\n");
			return 1;
		}

		int** adj_matrix; //adjacency Matrix
		int num_vertices = read_graph(file, &adj_matrix);

		std::cout << "Adjacency matrix:" << std::endl;
		for (int i = 0; i < num_vertices; i++) {
			for (int j = 0; j < num_vertices; j++) {
				std::cout << adj_matrix[i][j] << " ";
			}
			std::cout << std::endl;
		}

		int init_solutions = atoi(argv[2]);
		if (init_solutions <= 0) {
			std::cout << "Number of initial solutions must be a positive value" <<
			std::endl;
			return 0;
		}
		std::cout << "Number of initial solutions: " << init_solutions << std::endl;

		int num_iterations = atoi(argv[3]);
		if (init_solutions <= 0) {
			std::cout << "Number of iterations must be a positive value" << std::endl;
			return 0;
		}
		std::cout << "Number of iterations: " << num_iterations << std::endl;

		int t0 = atoi(argv[4]);
		if (t0 <= 0) {
			std::cout << "Initial time (t0) must be a positive value" << std::endl;
			return 0;
		}

		float beta = std::stof(argv[5]);
		if (beta <= 0 || beta >= 1) {
			std::cout << "Beta must be greater than 0 and smaller than 1" << std::endl;
			return 0;
		}

		physicalConstants pc;
		pc.t0 = t0;
		pc.beta = beta;

		//starts metaheuristic
		float *solution = rges_run(adj_matrix, num_vertices, init_solutions, num_iterations, pc);

		std::cout << std::endl << std::endl << "SOLUTION: ";
		for (int i = 0; i < num_vertices; i++) {
			std::cout << solution[i] << " ";
		}
		std::cout << std::endl;

		std::cout << "iterations needed: " << num_iterations << std::endl;

		delete[] solution;
	}
	else {
		std::cout << "Invalid number of arguments. Two arguments expected:" << std::endl <<
		"1 - filename" << std::endl << "2 - number of initial solutions" << std::endl <<
		"3 - number of iterations" << std::endl << "4 - t0 (Initial time of the universe)" <<
		"5 - beta (range: (0, 1))";
	}

	return 0;
}