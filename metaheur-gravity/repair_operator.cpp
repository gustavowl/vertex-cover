#include "repair_operator.h"

//arg1: solution vector for which the repair operator will be applied
//arg2: the transpose of the adjacency matrix
void repair_operator(float **solution_vector, int **transp_adj_matrix, int num_vertices) {
	rpr_oprtr_add_phase(solution_vector, transp_adj_matrix, num_vertices);
	rpr_oprtr_drop_phase(solution_vector, transp_adj_matrix, num_vertices);
}

//used internally
void rpr_oprtr_add_phase(float **solution_vector, int **transp_adj_matrix, int num_vertices) {
	/*
	(i)C = S × B ( matrix multiplication)
	(ii) ADD Phase
	(a) For each 0 entry in C , find the first column j( cost of j is
	in increasing order)
	(b) Add j to S ie., S(j) = 1.
	(c) D = S ( temporary )
	*/

	//(i)
	float* c = rpr_oprtr_multiply(solution_vector, transp_adj_matrix, num_vertices);
	
	//(ii)(a)
	int* zero_entries;
	int zero_entries_size = rpr_oprtr_find_zero_entries(c, transp_adj_matrix,
		num_vertices, &zero_entries);
	
	//(b)(c)
	for (int i = 0; i < zero_entries_size; i++) {
		(*solution_vector)[zero_entries[i]] = 1;
	}

	if (zero_entries_size > 0) {
		delete[] zero_entries;
	}
	delete[] c;
}

//used internally
void rpr_oprtr_drop_phase(float **solution_vector, int **transp_adj_matrix, int num_vertices) {
/*
(iii)DROP Phase
(a) Identify the column j ( cost in the decreasing order)
(b) Remove j from D, if C = D × B have no zero entry, ie.,
D(j) = 0.
(c) S=D is a feasible solution for SCP that contains no
redundant columns.
*/
}

float* rpr_oprtr_multiply(float **vector, int **transp_adj_matrix, int num_vertices) {
	//initializes vector
	float* c = new float[num_vertices];
	for (int i = 0; i < num_vertices; i++) {
		c[i] = 0;
	}

	for (int row = 0; row < num_vertices; row++) {
		for (int col = 0; col < num_vertices; col++) {
			c[col] += (*vector)[row] * transp_adj_matrix[row][col];
		}
	}
	return c;
}


//used internally
//The cost of a column j is not defined by the article.
//Here, it is defined as "num_vertices - sum(col)", since it is supposed
//we are working only with unweighted undirected graphs. "Greedly defined"
int rpr_oprtr_cost_of_column(int col, int **transp_adj_matrix, int num_vertices) {
	int sum = 0;
	for (int i = 0; i < num_vertices; i++) {
		sum += transp_adj_matrix[i][col];
	}
	return num_vertices - sum;
}

//returns the size of (j_col_index) index of columns j,
//depending on the cost. Increasing order
int rpr_oprtr_find_zero_entries(float* vector, int **transp_adj_matrix, int num_vertices,
	int** j_col_index) {

	//counts the number of zero entries
	int num_zeros = 0;
	for (int i = 0; i < num_vertices; i++) {
		if (vector[i] == 0.0) {
			num_zeros++;
		}
	}

	if (num_zeros > 0) {
		//stores the index of columns j
		*j_col_index = new int[num_zeros];
		int count = 0;
		for (int i = 0; count < num_zeros && i < num_vertices; i++) {
			if (vector[i] == 0.0) {
				(*j_col_index)[count++] = i;
			}
		}

		//computes and stores cost of column j
		int j_col_cost[num_zeros];
		for (int i = 0; i < num_zeros; i++) {
			j_col_cost[i] = rpr_oprtr_cost_of_column((*j_col_index)[i], transp_adj_matrix,
				num_vertices);
		}

		//sorts columns
		quick_sort(j_col_cost, 0, num_zeros, *j_col_index);
	}

	return num_zeros;
}

//"vector" will be the one used for sorting. The bijec_vector
//will be sorted as well, by it may be left unordered, since it
//is a bijection with vector
//based on https://github.com/gustavowl/Algorithms-Cormen/blob/master/Chapter02/quick_sort_recursive.py
void quick_sort(int* vector, int start, int end, int* bijec_vector) {
	if (start < end) {
		int pivot_pos = order_pivot(vector, start, end, bijec_vector);
		quick_sort(vector, start, pivot_pos, bijec_vector);
		quick_sort(vector, pivot_pos + 1, end, bijec_vector);
	}
}

//based on https://github.com/gustavowl/Algorithms-Cormen/blob/master/Chapter02/quick_sort_recursive.py
int order_pivot(int* vector, int start, int end, int* bijec_vector) {
	if (start < end) {
		int pivot_pos = start;
		for (int i = start + 1; i < end; i++) {
			if (vector[i] < vector[pivot_pos]) {
				if (i > pivot_pos + 1) {
					swap(vector, i, pivot_pos + 1);
					swap(bijec_vector, i, pivot_pos + 1);
				}
				swap(vector, pivot_pos, pivot_pos + 1);
				swap(bijec_vector, pivot_pos, pivot_pos + 1);
				pivot_pos++;
			}
		}
		return pivot_pos;
	}
	return start;
}

void swap(int* vector, int index1, int index2) {
	int aux = vector[index1];
	vector[index1] = vector[index2];
	vector[index2] = aux;
}