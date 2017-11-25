#include "rges.h"
#include "repair_operator.h"
#include <cstdlib>
#include <time.h>
#include <iostream> //FIXME: remove me
#include <cmath>

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

		//(e) Update G(t)
		float grav_const = rges_update_gravitational_constant(iteration, constants);

		//(e) Update best(t)
		float best = rges_min(fit, init_solutions);

		//(e) Update worst(t)
		float worst = rges_max(fit, init_solutions);

		//(e)Update Mi(t) for i = 1, 2, ..., N.
		float mi[init_solutions];
		rges_compute_mi(mi, fit, init_solutions, best, worst);
		rges_comput_Mi(mi, init_solutions);

		//(f) Calculation of the total force in different directions.


		iteration++;
	}

	//deallocates memory
	if (fit != 0) {
		delete[] fit;
	}

	//assert(init_solutions == 1)
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

float rges_update_gravitational_constant(int iteration, physicalConstants constants) {
	if (iteration > 0) {
		return pow((float)constants.t0 / (constants.t0 + iteration), constants.beta);
	}
	return 1.0;
}

float rges_min(float* fit, int size) {
	float min = fit[0];
	for (int i = 1; i < size; i++) {
		if (fit[i] < min) {
			min = fit[i];
		}
	}
	return min;
}

float rges_max(float* fit, int size) {
	float max = fit[0];
	for (int i = 1; i < size; i++) {
		if (fit[i] > max) {
			max = fit[i];
		}
	}
	return max;
}

void rges_compute_mi(float* mi, float* fit, int size, float best, float worst) {
	for (int i = 0; i < size; i++) {
		mi[i] = (fit[i] - worst) / (best - worst);
	}
}

void rges_comput_Mi(float* mi, int size) {
	float sum_mi = 0;
	for (int i = 0; i < size; i++) {
		sum_mi += mi[i];
	}

	for (int i = 0; i < size; i++) {
		mi[i] /= sum_mi;
	}
}

void rges_kbest(float** solutions, float* mi, int* size, int iteration, int max_iterations) {
	int k = 10;
	if (k < *size) {
		//increase exploitation
		rges_quick_sort(mi, 0, *size, solutions);
		for (int i = *size - 1; i > k ; i--) {
			regs_swap(mi, i, i - k);
			regs_swap(solutions, i, i - k);
		}
		for (int i = k; i < *size; i++) {
			delete[] solutions[i];
		}
		*size = k;
	}
}

//TODO: Implement general version (using template)
//used internally. Based on
//https://github.com/gustavowl/Algorithms-Cormen/blob/master/Chapter02/quick_sort_recursive.py
//implementation of Cormen version
void rges_quick_sort(float* vector, int start, int end, float** bijec_vector) {
	if (start < end) {
		int pivot_pos = regs_order_pivot(vector, start, end, bijec_vector);
		regs_quick_sort(vector, start, pivot_pos, bijec_vector);
		regs_quick_sort(vector, pivot_pos + 1, end, bijec_vector);
	}
}

//TODO: Implement general version (using template)
//used internally. Based on
//https://github.com/gustavowl/Algorithms-Cormen/blob/master/Chapter02/quick_sort_recursive.py
//implementation of Cormen version
int rges_order_pivot(float* vector, int start, int end, float** bijec_vector) {
	if (start < end) {
		int pivot_pos = start;
		for (int i = start + 1; i < end; i++) {
			if (vector[i] < vector[pivot_pos]) {
				if (i > pivot_pos + 1) {
					regs_swap(vector, i, pivot_pos + 1);
					regs_swap(bijec_vector, i, pivot_pos + 1);
				}
				regs_swap(vector, pivot_pos, pivot_pos + 1);
				regs_swap(bijec_vector, pivot_pos, pivot_pos + 1);
				pivot_pos++;
			}
		}
		return pivot_pos;
	}
	return start;
}

//TODO: Implement general version (using template)
//used internally
void rges_swap(float* vector, int index1, int index2) {
	float aux = vector[index1];
	vector[index1] = vector[index2];
	vector[index2] = aux;
}

//TODO: Implement general version (using template)
//used internally
void rges_swap(float** vector, int index1, int index2) {
	float* aux = vector[index1];
	vector[index1] = vector[index2];
	vector[index2] = aux;
}