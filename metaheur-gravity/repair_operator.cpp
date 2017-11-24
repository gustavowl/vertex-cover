#include "repair_operator.h"

//arg1: solution vector for which the repair operator will be applied
//arg2: the transpose of the adjacency matrix
void repair_operator(float **solution_vector, int **transp_adj_matrix) {
	rpr_oprtr_add_phase(solution_vector, transp_adj_matrix);
	rpr_oprtr_drop_phase(solution_vector, transp_adj_matrix);
}

//used internally
void rpr_oprtr_add_phase(float **solution_vector, int **transp_adj_matrix) {
/*
(i)C = S × B ( matrix multiplication)
(ii) ADD Phase
(a) For each 0 entry in C , find the first column j( cost of j is
in increasing order)
(b) Add j to S ie., S(j) = 1.
(c) D = S ( temporary )
*/
}

//used internally
void rpr_oprtr_drop_phase(float **solution_vector, int **transp_adj_matrix) {
/*
(iii)DROP Phase
(a) Identify the column j ( cost in the decreasing order)
(b) Remove j from D, if C = D × B have no zero entry, ie.,
D(j) = 0.
(c) S=D is a feasible solution for SCP that contains no
redundant columns.
*/
}

//used internally
void rpr_oprtr_cost_of_column(int col, int **transp_adj_matrix, int num_vertices);

float** rpr_oprtr_multiply(float **vector, int **transp_adj_matrix, int num_vertices);