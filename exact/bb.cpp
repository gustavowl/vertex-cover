#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "vertex.h"
#include "edge.h"
#include <algorithm>

int read_graph(FILE* file, Vertex ***vertices, int* num_vertices, Edge ***edges, int* num_edges) {
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

	*vertices = new Vertex*[*num_vertices];
	for (int i = 0; i < *num_vertices; i++) {
		(*vertices)[i] = new Vertex();
	}

	*edges = new Edge*[*num_edges];
	int index_v1, index_v2;
	index_v1 = index_v2 = 0;
	int max_degree = 0;
	std::cout << "-----EDGES-----" << std::endl;
	for (int i = 0; i < *num_edges; i++) {
		fgets(buffer, 127, (FILE*)file);
		substring = strtok(buffer, " ");

		substring = strtok(nullptr, " ");
		index_v1 = atoi(substring);
		index_v1--;

		substring = strtok(nullptr, " ");
		index_v2 = atoi(substring);
		index_v2--;

		(*edges)[i] = new Edge((*vertices)[index_v1], (*vertices)[index_v2]);

		(*vertices)[index_v1]->edges.push_back((*edges)[i]);
		(*vertices)[index_v1]->degree++;
		if ((*vertices)[index_v1]->degree > max_degree) {
			max_degree = (*vertices)[index_v1]->degree;
		}
		(*vertices)[index_v2]->edges.push_back((*edges)[i]);
		(*vertices)[index_v2]->degree++;
		if ((*vertices)[index_v2]->degree > max_degree) {
			max_degree = (*vertices)[index_v2]->degree;
		}

		std::cout << (*edges)[i] << ": " << (*vertices)[index_v1] << " " << (*vertices)[index_v2] << std::endl;
	}

	std::cout <<std::endl << "-----VERTICES-----";
	for (int i = 0; i < *num_vertices; i++) {
		std::cout << std::endl << (*vertices)[i] << ": ";
		for (int j = 0; j < (*vertices)[i]->degree; j++) {
			std::cout << (*vertices)[i]->edges[j] << " ";
		}
	}

	return max_degree;
}

//returns new max_degree after removing the vertex
int remove_vertex(std::vector<Vertex*>* vertices, int max_degree, Vertex** removed_vertex) {
	*removed_vertex = vertices[max_degree].back();
	vertices[max_degree].pop_back();

	//adjust degree of all neighbours
	for (int i = 0; i < (*removed_vertex)->edges.size(); i++) {
		Edge* e = (*removed_vertex)->edges[i];
		Vertex* neighbour = e->getNeighbourOf(*removed_vertex);

		//find position of neighbour in vertices vector and removes it
		for (int j = 0; j < vertices[neighbour->degree].size(); j++) {
			if (vertices[neighbour->degree][j] == neighbour) {
				vertices[neighbour->degree].erase(vertices[neighbour->degree].begin());
				break;
			}
		}


		neighbour->removeEdge(e);

		//update position in vertices vector
		vertices[neighbour->degree].push_back(neighbour);
	}

	//searches for vertex with largest degree
	for (; max_degree >= 0; max_degree--) {
		if (vertices[max_degree] > 0) {
			return max_degree;
		}
	}
	return -1;
}

bool min_cover(std::vector<Vertex*>* vertices, int vertices_size, int k, int uncovered_actual, int uncovered_best) {

	if (uncovered_best == 0) { //prunes tree
		return true;
	}

	if (k == 0) {
		if (uncovered_actual < uncovered_best) {
			uncovered_best = uncovered_actual;
			if (uncovered_best > 0) {
				return false;
			}
			return true;
			//clear the set of stored configurations?
		}
		//store configuration
	}

	//checks bound condition
	int bound = 0;
	int l = k;
	for (int i = vertices_size - 1; i > 0 && l > 0; i--) {
		if (vertices[i].size() > 0) {
			bound += std::min(vertices[i].size(), l) * i;
			l -= vertices.size();
			/*
			if (vertices[i].size() <= l) {
				bound += vertices[i].size() * i;
				l -= vertices.size();
			}
			else {
				bound += i * l;
				l -= vertices.size();
			}*/
		}
	}
	if (uncovered_actual - bound > uncovered_best) {
		return false;
	}

	//get new node
	int max_degree = vertices_size;
	//remove_vertex
}

int main(int argc, char* argv[]) {

	if (argc == 3) {
		printf("File: %s\n", argv[1]);
		printf("k: %s\n", argv[2]);

		FILE *file;
		int k = atoi(argv[2]);
		file = fopen(argv[1], "r");

		if (file == NULL) {
			printf("Could not open specified file\n");
			return 1;
		}

		Vertex **vertices;
		Edge **edges;
		int num_vertices, num_edges, max_degree;
		max_degree = read_graph(file, &vertices, &num_vertices, &edges, &num_edges);

		printf("\nexit success:\n%i %i %i\n", num_vertices, num_edges, max_degree);
		fclose(file);

		//creates a list of vertices ordered by degree
		std::vector<Vertex*>* vertices_by_degree = new std::vector<Vertex*>[max_degree + 1];

		for (int i = 0; i < num_vertices; i++) {
			vertices_by_degree[vertices[i]->degree].push_back(vertices[i]);
		}

		std::cout << "------DEGREES-----" << std::endl;
		for (int i = 0; i < max_degree + 1; i++) {
			std::cout << i << ": " << vertices_by_degree[i].size() << std::endl;
		}

		min_cover(vertices_by_degree, max_degree + 1);
	}
	else {
		std::cout << "Invalid number of arguments. Two arguments expected:" << std::endl <<
		"1 - filename" << std::endl << "2 - k (max number of covered vertices)" << std::endl;
	}

	return 0;
}