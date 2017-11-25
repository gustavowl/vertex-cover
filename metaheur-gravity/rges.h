#ifndef __RGES_H__
#define __RGES_H__

//t0 and beta will be used to compute the gravitational constant.
//constraints:
//	t0 > 0
//	0 < beta < 1
struct physicalConstants {
	int t0;
	float beta;
};

//returns found solution. min_vertex_cover = sum(return_vector)
float* rges_run(int **adj_matrix, int num_vertices, int init_solutions, int num_iterations,
	physicalConstants constants);

//returns array with fitness of each solution.
//fitness of a solution is defined as sum(solution[i])
//which means the sum of all vertices covered
float* rges_fitness(float** solutions, int solutions_size, int num_vertices);

//The gravitational constant determines the amount of interation
//between the objects.
//The gravitational constant changes with time, accordingly to
//the formula:
//G(t) = G(t0) * (t0/t) ^ beta. (with 0 < beta < 1)
//note that G(t0) always equals 1
//The original constraint is that beta < 1. We added 0 < beta
//since we need the gravitational constant to decrease with time.
//The value of t will be computed depending on the number of iterations,
//i.e. t0 + iteration. Hence it will assume the values (t0, t0 + 1, t0 + 2,
//t0 + 3, ..., t0 + number of iterations).
float rges_update_gravitational_constant(int iteration, physicalConstants constants);

float rges_min(float* fit, int size);

float rges_max(float* fit, int size);

//defined by mi(t) = (fit(t) - worst(t)) / (best(t) - worst(t))
void rges_compute_mi(float* mi, float* fit, int size, float best, float worst);

//Mi is defined as Mi = mi / summation(mi)
void rges_comput_Mi(float* mi, int size);

#endif