#ifndef __REPAIR_OPERATOR_H__
#define __REPAIR_OPERATOR_H__

//arg1: solution vector for which the repair operator will be applied
//arg2: the transpose of the adjacency matrix
void repair_operator(float **solution_vector, int **transp_adj_matrix, int num_vertices);

//used internally
int rpr_oprtr_add_phase(float **solution_vector, int **transp_adj_matrix, int num_vertices,
	int** zero_entries);

//used internally
void rpr_oprtr_drop_phase(float **solution_vector, int **transp_adj_matrix, int num_vertices,
	int** zero_entries, int zero_entries_size);

//used internally
float* rpr_oprtr_multiply(float **vector, int **transp_adj_matrix, int num_vertices);

//used internally
int rpr_oprtr_cost_of_column(int col, int **transp_adj_matrix, int num_vertices);

//used internally
int rpr_oprtr_find_zero_entries(float* vector, int **transp_adj_matrix, int num_vertices,
	int** j_col_index);

//used internally
bool rpr_oprtr_vector_has_zero_entry(float* vector, int num_vertices);

//TODO: Implement general version (using template)
//used internally. Based on
//https://github.com/gustavowl/Algorithms-Cormen/blob/master/Chapter02/quick_sort_recursive.py
//implementation of Cormen version
void quick_sort(int* vector, int start, int end, int* bijec_vector);

//TODO: Implement general version (using template)
//used internally. Based on
//https://github.com/gustavowl/Algorithms-Cormen/blob/master/Chapter02/quick_sort_recursive.py
//implementation of Cormen version
int order_pivot(int* vector, int start, int end, int* bijec_vector);

//TODO: Implement general version (using template)
//used internally
void swap(int* vector, int index1, int index2);

#endif