#include "rges.h"
#include "repair_operator.h"
#include <cstdlib>
#include <time.h>

float* rges_run(int **adj_matrix, int num_vertices, int init_solutions, int num_iterations) {
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
	double *solutions[init_solutions];

	for (int i = 0; i < init_solutions; i++) {
		double *row = new double[num_vertices];
		solutions[i] = row;
		for (int j = 0; j < num_vertices; j++) {
			double r = rand();
			solutions[i][j] = r / RAND_MAX; //range: [0, 1]
		}
	}

	//(c) Repair operator.
	for (int i = 0; i < init_solutions; i++) {
		//repair_operator(&(solutions[i]), adj_matrix, num_vertices);
	}

	//deallocates memory
	for (int i = 1; i < init_solutions; i++) {
		delete[] solutions[i];
	}
	//return solutions[0]; //does not deallocate solution
	float *k = new float;

	return k;
}

