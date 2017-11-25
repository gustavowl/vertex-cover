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

	float *vdi[init_solutions];
	for (int i = 0; i < init_solutions; i++) {
		float *row = new float[num_vertices];
		vdi[i] = row;
		for (int j = 0; j < num_vertices; j++) {
			vdi[i][j] = 0.0;
		}
	}

	int iteration = 0;
	float* fit = 0;
	float** fdi = 0;
	while (iteration < num_iterations) {
		srand(time(NULL));

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
		//deallocates memory
		if (fit != 0) {
			delete[] fit;
		}
		fit = rges_fitness(solutions, init_solutions, num_vertices);
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
		rges_compute_Mi(mi, init_solutions);

		//(f) Calculation of the total force in different directions.
		//deallocates memory
		if (fdi != 0) {
			for (int i = 0; i < init_solutions; i++) {
				delete[] fdi[i];
			}
			delete[] fdi;
		}
		fdi = rges_compute_fdi(grav_const, mi, solutions, init_solutions,
			num_vertices, iteration, num_iterations);

		//(g) Calculation of acceleration and velocity.
		rges_compute_acceleration(fdi, mi, init_solutions, num_vertices, fdi);
		rges_compute_velocity(vdi, fdi, init_solutions, num_vertices);


		iteration++;
	}

	//deallocates memory
	if (fit != 0) {
		delete[] fit;
	}

	//deallocates memory
	if (fdi != 0) {
		for (int i = 0; i < init_solutions; i++) {
			delete[] fdi[i];
		}
		delete[] fdi;
	}

	for (int i = 1; i < init_solutions; i++) {
		delete[] solutions[i];
	}

	for (int i = 0; i < init_solutions; i++) {
		delete[] vdi[i];
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

void rges_compute_Mi(float* mi, int size) {
	float sum_mi = 0;
	for (int i = 0; i < size; i++) {
		sum_mi += mi[i];
	}

	for (int i = 0; i < size; i++) {
		mi[i] /= sum_mi;
	}
}

//used internally
//returns the fdi matrix
float** rges_compute_fdi(float gravitational_constant, float* mi, float** solutions,
	int sols_size, int num_vertices, int iteration, int max_iterations) {
	//computes k-best
	int k = rges_kbest(solutions, mi, sols_size, iteration, max_iterations);

	//initializes fdi
	float** fdi = new float*[sols_size];
	for (int i = 0; i < sols_size; i++) {
		fdi[i] = new float[num_vertices];
		for (int j = 0; j < num_vertices; j++) {
			fdi[i][j] = 0.0;
		}
	}

	//computes fdi
	double r;

	for (int j = 0; j < k; j++) {
		for (int i = 0; i < sols_size; i++) {
			if (j != i) {
				float euclid = rges_compute_euclidian(solutions[i], solutions[j],
					num_vertices);

				for (int d = 0; d < num_vertices; d++) {
					float force = rges_compute_fdij(gravitational_constant, mi, i,
					 	j, euclid, solutions, d);

					r = rand();
					r /= RAND_MAX; //range: [0, 1]
					fdi[i][d] += force * (float)r;
				}
			}
		}
	}

	return fdi;
}

int rges_kbest(float** solutions, float* mi, int size, int iteration, int max_iterations) {
	//Computes k linearly
	float m = (float)(1 - size) / (max_iterations/* - 0*/);
	//equation of line is given by
	float y = m * iteration + (size);
	int k = (int)ceil(y);

	//Determines de k-best solutions
	if (k < size) {
		//increase exploitation
		rges_quick_sort(mi, 0, size, solutions);
		for (int i = size - 1; i > k ; i--) {
			rges_swap(mi, i, i - k);
			rges_swap(solutions, i, i - k);
		}
		/*for (int i = k; i < *size; i++) {
			delete[] solutions[i];
		}
		*size = k;*/
		return k;
	}
	return size;
}

//TODO: Implement general version (using template)
//used internally. Based on
//https://github.com/gustavowl/Algorithms-Cormen/blob/master/Chapter02/quick_sort_recursive.py
//implementation of Cormen version
void rges_quick_sort(float* vector, int start, int end, float** bijec_vector) {
	if (start < end) {
		int pivot_pos = rges_order_pivot(vector, start, end, bijec_vector);
		rges_quick_sort(vector, start, pivot_pos, bijec_vector);
		rges_quick_sort(vector, pivot_pos + 1, end, bijec_vector);
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
					rges_swap(vector, i, pivot_pos + 1);
					rges_swap(bijec_vector, i, pivot_pos + 1);
				}
				rges_swap(vector, pivot_pos, pivot_pos + 1);
				rges_swap(bijec_vector, pivot_pos, pivot_pos + 1);
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

//used internally
//note: on the article, fdij is defined the force from "j" acting on "i" as:
//gravitational_constant * (mi * mj) / (euclid(i, j)) * (xid - xid)
//where xid is the position (x) of the solution (i) regarding the dimension (d)
//I believe there's a typo. Hence, the following formula is used:
//gravitational_constant * (mi * mj) / (euclid(i, j)) * (xjd - xid)
float rges_compute_fdij(float gravitational_constant, float* mi, int i,
	int j, float euclid, float** solutions, int d) {
	
	return gravitational_constant * mi[i] * mi[j] / euclid *
		(solutions[j][d] - solutions[i][d]);
}

float rges_compute_euclidian(float* vector1, float* vector2, int size) {
	float euclid = 0;
	for (int i = 0; i < size; i++) {
		euclid += pow( (vector1 - vector2), 2 );
	}
	return sqrt(euclid);
}

void rges_compute_acceleration(float** fdi, float* mi, int sols_size, int num_vertices,
	float** adi) {

	for (int i = 0; i < sols_size; i++) {
		for (int d = 0; d < num_vertices; d++) {
			adi[i][d] = fdi[i][d] / mi[i];
		}
	}
}

void rges_compute_velocity(float** vdi, float** adi, int sols_size, int num_vertices) {
	double r;
	for (int i = 0; i < sols_size; i++) {
		for (int d = 0; d < num_vertices; d++) {
			r = rand();
			r /= RAND_MAX; //range: [0, 1]
			vdi[i][d] = r * vdi[i][d] + adi[i][d];
		}
	}
}