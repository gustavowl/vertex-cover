#include "read_file.h"
#include "rges.h"
#include <iostream>
#include <cstdlib>
#include <time.h>
/*#include <string.h>
#include <vector>
#include <algorithm>*/
#include <chrono>

int main(int argc, char* argv[]) {

	if (argc == 6) {
		printf("File: %s\n", argv[1]);

		FILE *file;
		file = fopen(argv[1], "r");

		if (file == NULL) {
			printf("Could not open specified file\n");
			return 1;
		}

		std::chrono::high_resolution_clock::time_point start_time, end_time;
		std::chrono::duration<double> time_span;
		start_time = std::chrono::high_resolution_clock::now();
		int** adj_matrix; //adjacency Matrix
		int num_vertices = read_graph(file, &adj_matrix);
		end_time = std::chrono::high_resolution_clock::now();
		time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time);
		std::cout << "Time to pre-process (read file and create adjacency matrix): " << 
			time_span.count() << " seconds" << std::endl;

		std::cout << "Adjacency matrix:" << std::endl;
		for (int i = 0; i < num_vertices; i++) {
			for (int j = 0; j < num_vertices; j++) {
				std::cout << adj_matrix[i][j] << " ";
			}
			std::cout << "// " << i << std::endl;
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
		start_time = std::chrono::high_resolution_clock::now();
		float *solution = rges_run(adj_matrix, num_vertices, init_solutions, num_iterations, pc);
		end_time = std::chrono::high_resolution_clock::now();

		float prob = 0.0;
		int cover = 0;
		double r;
		srand(time(NULL));
		std::cout << std::endl << std::endl << "SOLUTION: ";
		for (int i = 0; i < num_vertices; i++) {
			std::cout << solution[i] << " ";
			prob += solution[i];
			r = rand();
			r /= RAND_MAX; //[0, 1]
			if ((float)r <= solution[i]) {
				cover++;
			}
		}
		std::cout << std::endl;

		std::cout << "Probabilities sum: " << prob << std::endl;
		std::cout << "Possible min cover: " << cover << std::endl;
		//std::cout << "iterations needed: " << num_iterations << std::endl;
		std::cout << "Time running metaheuristic: " << time_span.count() << " seconds" << std::endl;

		delete[] solution;
	}
	else {
		std::cout << "Invalid number of arguments. Five arguments expected:" << std::endl <<
		"1 - filename" << std::endl << "2 - number of initial solutions" << std::endl <<
		"3 - number of iterations" << std::endl << "4 - t0 (Initial time of the universe)" <<
		std::endl << "5 - beta (range: (0, 1))" << std::endl;
	}

	return 0;
}