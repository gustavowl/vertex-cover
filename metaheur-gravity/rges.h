#ifndef __RGES_H__
#define __RGES_H__

//returns found solution. min_vertex_cover = sum(return_vector)
float* rges_run(int **adj_matrix, int num_vertices, int init_solutions, int num_iterations);

#endif