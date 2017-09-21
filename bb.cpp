#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "vertex.h"
#include "edge.h"

void read_graph(FILE* file, int* num_vertices, int* num_edges) {
	char buffer[127];
	fgets(buffer, 127, (FILE*)file);

	
	char* substring = strtok(buffer, " ");
	for (int i = 0; i < 4; i++, substring = strtok(nullptr, " ")) {
		if (i == 2) {
			*num_vertices = atoi(substring);
		}
		else if (i == 3) {
			*num_edges = atoi(substring);
		}
	}

	Vertex *vertices[*num_vertices];
	for (int i = 0; i < *num_vertices; i++) {
		vertices[i] = new Vertex();
	}

	Edge *edges[*num_edges];
	int index_v1, index_v2;
	index_v1 = index_v2 = 0;
	for (int i = 0; i < *num_edges; i++) {
		fgets(buffer, 127, (FILE*)file);
		substring = strtok(buffer, " ");

		substring = strtok(nullptr, " ");
		index_v1 = atoi(substring);
		index_v1--;

		substring = strtok(nullptr, " ");
		index_v2 = atoi(substring);
		index_v2--;

		edges[i] = new Edge(vertices[index_v1], vertices[index_v2]);

		vertices[index_v1]->edges.push_back(edges[i]);
		vertices[index_v1]->degree++;
		vertices[index_v2]->edges.push_back(edges[i]);
		vertices[index_v2]->degree++;
	}
}

int main(int argc, char* argv[]) {

	if (argc == 2) {
		printf("File: %s\n", argv[1]);

		FILE *file;
		file = fopen(argv[1], "r");

		if (file == NULL) {
			printf("Could not open specified file\n");
			return 1;
		}

		int num_vertices, num_edges;
		read_graph(file, &num_vertices, &num_edges);

		printf("exit success:\n%i %i\n", num_vertices, num_edges);
		return fclose(file);
	}
	else {
		printf("Invalid number of arguments. Only one expected: filename\n");
	}

	return 0;
}