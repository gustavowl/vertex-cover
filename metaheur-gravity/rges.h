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
void rges_compute_Mi(float* mi, int size);

//used internally
//returns the fdi matrix
float** rges_compute_fdi(float gravitational_constant, float* mi, float** solutions,
	int sols_size, int num_vertices, int iteration, int max_iterations);

//the kbest are defined in terms of the objects with the best fit and greatest
//mass. Since mi depends on fitness, we just use it to compute the kbest.
//the value "k" will be computed accordingly to the iteration number.
//"k" is computed in such a way that only after the last iteration k = 1.
//Returns the value of k.
int rges_kbest(float** solutions, float* mi, int size, int iteration, int max_iterations);

//TODO: Implement general version (using template)
//used internally. Based on
//https://github.com/gustavowl/Algorithms-Cormen/blob/master/Chapter02/quick_sort_recursive.py
//implementation of Cormen version
void rges_quick_sort(float* vector, int start, int end, float** bijec_vector);

//TODO: Implement general version (using template)
//used internally. Based on
//https://github.com/gustavowl/Algorithms-Cormen/blob/master/Chapter02/quick_sort_recursive.py
//implementation of Cormen version
int rges_order_pivot(float* vector, int start, int end, float** bijec_vector);

//TODO: Implement general version (using template)
//used internally
void rges_swap(float* vector, int index1, int index2);

//TODO: Implement general version (using template)
//used internally
void rges_swap(float** vector, int index1, int index2);

//used internally
//note: on the article, fdij is defined the force from "j" acting on "i" as:
//gravitational_constant * (mi * mj) / (euclid(i, j)) * (xid - xid)
//where xid is the position (x) of the solution (i) regarding the dimension (d)
//I believe there's a typo. Hence, the following formula is used:
//gravitational_constant * (mi * mj) / (euclid(i, j)) * (xjd - xid)
float rges_compute_fdij(float gravitational_constant, float* mi, int i,
	int j, float euclid, float** solutions, int d);

//used internally
float rges_compute_euclidian(float* vector1, float* vector2, int size);

//used internally
void rges_compute_acceleration(float** fdi, float* mi, int sols_size, int num_vertices,
	float** adi);

//used internally
void rges_compute_velocity(float** vdi, float** adi, int sols_size, int num_vertices);
#endif