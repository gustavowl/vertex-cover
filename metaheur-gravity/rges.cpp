#include "rges.h"
#include "repair_operator.h"

float* rges_run(int **adj_matrix, int num_vertices) {
	float *solution = new float[7]; //{0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0}
	for (int i = 0; i < 7; i++) {
		if (i == 1 || i == 2) {
			solution[i] = 1.0;
			continue;
		}
		solution[i] = 0.0;
	}

	repair_operator(&solution, adj_matrix, num_vertices);
	return solution;
}

