#ifndef __REPAIR_OPERATOR_H__
#define __REPAIR_OPERATOR_H__

//arg1: solution vector for which the repair operator will be applied
//arg2: the transpose of the adjacency matrix
void repair_operator(float **solution_vector, int **transp_adj_matrix, int num_vertices);

//used internally
void rpr_oprtr_add_phase(float **solution_vector, int **transp_adj_matrix, int num_vertices);

//used internally
void rpr_oprtr_drop_phase(float **solution_vector, int **transp_adj_matrix, int num_vertices);

//used internally
void rpr_oprtr_cost_of_column(int col, int **transp_adj_matrix, int num_vertices);

float** rpr_oprtr_multiply(float **vector, int **transp_adj_matrix, int num_vertices);

#endif