#ifndef __RGES_H__
#define __RGES_H__

//returns found solution. min_vertex_cover = sum(return_vector)
float* rges_run(int **adj_matrix, int num_vertices, int init_solutions, int num_iterations);

//returns array with fitness of each solution.
//fitness of a solution is defined as sum(solution[i])
//which means the sum of all vertices covered
float* rges_fitness(float** solutions, int solutions_size, int num_vertices);

#endif