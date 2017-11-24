#include "read_file.h"

int read_graph(FILE* file, int ***adjacency_matrix) {
	char buffer[127];
	fgets(buffer, 127, (FILE*)file);

	int num_vertices = 0;
	int num_edges = 0;

	
	char* substring = strtok(buffer, " ");
	for (int i = 0; i < 4; i++, substring = strtok(nullptr, " ")) {
		if (i == 2) {
			num_vertices = atoi(substring);
		}
		else if (i == 3) {
			num_edges = atoi(substring);
		}
	}

	//initializes matrix
	*adjacency_matrix = new int*[num_vertices];
	for (int i = 0; i < num_vertices; i++) {
		(*adjacency_matrix)[i] = new int[num_vertices];
		for (int j = 0; j < num_vertices; j++) {
			(*adjacency_matrix)[i][j] = 0;
		}
	}

	//Fills matrix with valus from file
	for (int i = 0; i < num_edges; i++) {
		fgets(buffer, 127, (FILE*)file);
		substring = strtok(buffer, " ");

		substring = strtok(nullptr, " ");
		int index_v1 = atoi(substring);
		index_v1--;

		substring = strtok(nullptr, " ");
		int index_v2 = atoi(substring);
		index_v2--;

		(*adjacency_matrix)[index_v1][index_v2] = 1;
		(*adjacency_matrix)[index_v2][index_v1] = 1;
	}

	return num_vertices;
}