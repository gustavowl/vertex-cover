#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "vertex.h"
#include "edge.h"
#include <algorithm>
#include <chrono>

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
	//std::cout << "-----EDGES-----" << std::endl;
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

		//std::cout << (*edges)[i] << ": " << (*vertices)[index_v1] << " " << (*vertices)[index_v2] << std::endl;
	}

	/*
	std::cout <<std::endl << "-----VERTICES-----";
	for (int i = 0; i < *num_vertices; i++) {
		std::cout << std::endl << (*vertices)[i] << ": ";
		for (int j = 0; j < (*vertices)[i]->degree; j++) {
			std::cout << (*vertices)[i]->edges[j] << " ";
		}
	}*/

	return max_degree;
}

//returns new max_degree after removing the vertex
int remove_vertex(std::vector<Vertex*>* vertices, int max_degree, Vertex** removed_vertex) {
	*removed_vertex = vertices[max_degree][0];
	vertices[max_degree].erase(vertices[max_degree].begin());

	//adjust degree of all neighbours
	for (int i = 0; i < (*removed_vertex)->edges.size(); i++) {
		Edge* e = (*removed_vertex)->edges[i];
		Vertex* neighbour = e->getNeighbourOf(*removed_vertex);

		//finds position of neighbour in vertices vector and removes it
		for (int j = 0; j < vertices[neighbour->degree].size(); j++) {
			if (vertices[neighbour->degree][j] == neighbour) {
				vertices[neighbour->degree].erase(vertices[neighbour->degree].begin() + j);
				break;
			}
		}

		neighbour->removeEdge(e);

		//update position in vertices vector
		if (neighbour->isFree()) {
			vertices[neighbour->degree].push_back(neighbour);
		}
	}

	//searches for vertex with largest degree
	for (; max_degree >= 0; max_degree--) {
		if (vertices[max_degree].size() > 0) {
			return max_degree;
		}
	}
	return -1;
}

//returns new max_degree after re-adding the vertex's edges do its neighbours
int add_vertex_edges(std::vector<Vertex*>* vertices, int max_degree, Vertex* removed_vertex) {
	//adjust degrees of all neighbours
	for (int i = 0; i < removed_vertex->edges.size(); i++) {
		Edge* e = removed_vertex->edges[i];
		Vertex* neighbour = e->getNeighbourOf(removed_vertex);

		//finds position of neighbour in vertices vector and removes it
		for (int j = 0; j < vertices[neighbour->degree].size(); j++) {
			if (vertices[neighbour->degree][j] == neighbour) {
				vertices[neighbour->degree].erase(vertices[neighbour->degree].begin() + j);
				break;
			}
		}

		neighbour->addEdge(e);

		//update position in vertices vector
		if (neighbour->isFree()) {
			vertices[neighbour->degree].push_back(neighbour);
			if (neighbour->degree > max_degree) {
				max_degree = neighbour->degree;
			}
		}
	}

	return max_degree;
}

bool min_cover(std::vector<Vertex*>* vertices, int vertices_size, int k, int uncovered_actual, int* uncovered_best, int* node_count) {
	(*node_count)++;
	//std::cout << "Node count: " << *node_count << std::endl;

	if (uncovered_actual == 0 || *uncovered_best == 0) { //prunes tree
		*uncovered_best = 0;
		return true;
	}

	if (k == 0) {
		if (uncovered_actual < *uncovered_best) {
			*uncovered_best = uncovered_actual;
			if (*uncovered_best > 0) {
				return false;
			}
			return true;
			//clear the set of stored configurations?
		}
		//store configuration
		return false;
	}

	//checks bound condition
	int bound = 0;
	int l = k;
	for (int i = vertices_size - 1; i > 0 && l > 0; i--) {
		if (vertices[i].size() > 0) {
			bound += std::min((int)vertices[i].size(), l) * i;
			l -= vertices[i].size();
		}
	}
	if (uncovered_actual - bound >= *uncovered_best) {
		return false;
	}

	//cover a new vertex
	Vertex* removed_vertex;
	vertices_size = remove_vertex(vertices, vertices_size - 1, &removed_vertex) + 1;
	removed_vertex->setCovered();
	uncovered_actual -= removed_vertex->degree;
	k--;
	if ( min_cover(vertices, vertices_size, k, uncovered_actual, uncovered_best, node_count) ){
		return true;
	}

	//readjust degrees of all neighbours
	vertices_size = add_vertex_edges(vertices, vertices_size - 1, removed_vertex) + 1;
	//if it has an uncovered neighbour, removed_vertex cannot be uncovered, otherwise
	//at least one edge would not be covered
	if (!removed_vertex->hasUncoveredNeighbour()) {
		//mark vertex as uncovered
		removed_vertex->setUncovered();
		uncovered_actual += removed_vertex->degree;
		k++;
		if ( min_cover(vertices, vertices_size, k, uncovered_actual, uncovered_best, node_count) ) {
			return true;
		}
	}

	//backtracks
	vertices[removed_vertex->degree].push_back(removed_vertex);
	if (vertices_size < removed_vertex->degree + 1) {
		vertices_size = removed_vertex->degree + 1;
	}
	removed_vertex->setFree();

	return false;
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

		std::chrono::high_resolution_clock::time_point start_time, end_time;
		std::chrono::duration<double> time_span;
		start_time = std::chrono::high_resolution_clock::now();
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
		end_time = std::chrono::high_resolution_clock::now();
		time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time);
		std::cout << "Time to pre-process (read file and create necessary vectors): " << 
			time_span.count() << " seconds" << std::endl;
		/*
		std::cout << "------DEGREES-----" << std::endl;
		for (int i = 0; i < max_degree + 1; i++) {
			std::cout << i << ": " << vertices_by_degree[i].size() << std::endl;
		}*/

		start_time = std::chrono::high_resolution_clock::now();
		int node_count = 0;
		bool result = min_cover(vertices_by_degree, max_degree + 1, k, num_edges, &num_edges, &node_count);
		end_time = std::chrono::high_resolution_clock::now();
		time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time);
		std::cout << "Time to compute exact answer: " << time_span.count() << " seconds" << std::endl;


		std::cout << std::endl << "OUTPUT: " << k << "-sized vertex cover? " << result << std::endl;
		std::cout << "Amount of tree nodes explored: " << node_count << std::endl;
	}
	else {
		std::cout << "Invalid number of arguments. Two arguments expected:" << std::endl <<
		"1 - filename" << std::endl << "2 - k (max number of covered vertices)" << std::endl;
	}

	return 0;
}