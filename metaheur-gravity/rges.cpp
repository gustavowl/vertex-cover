#include "rges.h"
#include "repair_operator.h"
#include <cstdlib>
#include <time.h>
#include <iostream> //FIXME: remove me

float* rges_run(int **adj_matrix, int num_vertices, int init_solutions, int num_iterations,
	physicalConstants constants) {
	/*
	(a) Search space identification.
	(b) Randomized initialization.
	(c) Repair operator.
	(d) Fitness evaluation of agents
	(e) Update G(t), best(t), worst(t) and Mi(t) for i = 1, 2, ..., N.
	(f) Calculation of the total force in different directions.
	(g) Calculation of acceleration and velocity.
	(h) Updating agents’ position.
	(i) Repeat steps c to h until the stop criteria is reached.
	(j) End
	*/

	//(a) ??????
	//(b) Randomized initialization.
	srand(time(NULL));
	float *solutions[init_solutions];

	std::cout << "solutions: " << std::endl;
	for (int i = 0; i < init_solutions; i++) {
		float *row = new float[num_vertices];
		solutions[i] = row;
		for (int j = 0; j < num_vertices; j++) {
			double r = rand();
			solutions[i][j] = (float)(r / RAND_MAX); //range: [0, 1]
			std::cout << solutions[i][j] << " ";
		}
		std::cout << std::endl;
	}

	int iteration = 0;
	float* fit = 0;
	while (iteration < num_iterations) {
		//(c) Repair operator.
		for (int i = 0; i < init_solutions; i++) {
			repair_operator(&(solutions[i]), adj_matrix, num_vertices);
		}

		std::cout << std::endl << "repaired:" << std::endl;
		for (int i = 0; i < init_solutions; i++) {
			for (int j = 0; j < num_vertices; j++) {
				std::cout << solutions[i][j] << " ";
			}
			std::cout << std::endl;
		}

		//(d) Fitness evaluation of agents
		float* fit = rges_fitness(solutions, init_solutions, num_vertices);
		std::cout << std::endl << "fit:" << init_solutions << std::endl;
		for (int i = 0; i < init_solutions; i++) {
			std::cout << fit[i] << " ";
		}
		std::cout << std::endl;
	}

	//deallocates memory
	if (fit != 0) {
		delete[] fit;
	}

	for (int i = 1; i < init_solutions; i++) {
		delete[] solutions[i];
	}
	
	return solutions[0]; //does not deallocate solution
}

//returns array with fitness of each solution.
//fitness of a solution is defined as sum(solution[i])
//which means the sum of all vertices covered
float* rges_fitness(float** solutions, int solutions_size, int num_vertices) {
	//initializes fit array
	float* fit = new float[solutions_size];
	for (int i = 0; i < solutions_size; i++) {
		fit[i] = 0.0;
	}

	//computes fit
	for (int i = 0; i < solutions_size; i++) {
		for (int j = 0; j < num_vertices; j++) {
			fit[i] += solutions[i][j];
		}
	}

	return fit;
}