#ifndef __REPAIR_OPERATOR_H__
#define __REPAIR_OPERATOR_H__

//arg1: solution vector for which the repair operator will be applied
//arg2: the transpose of the adjacency matrix
void repair_operator(float **solution_vector, int **transp_adj_matrix);

//used internally
void rpr_oprtr_add_phase(float **solution_vector, int **transp_adj_matrix);

//used internally
void rpr_oprtr_drop_phase(float **solution_vector, int **transp_adj_matrix);

#endif