#include "edge.h"
#include "vertex.h"
#include <cstddef>

Vertex::Vertex() {
	*this = Vertex(true, false, 0);
}

Vertex::Vertex(bool free, bool covered, int degree) {
	this->free = free;
	this->covered = covered;
	this->degree = degree;
}

void Vertex::removeEdge(Edge* e) {
	for (int i = 0; i < degree; i++) {
		if (edges[i] == e) {
			edges.erase(edges.begin() + i);
			degree--;
		}
	}
}