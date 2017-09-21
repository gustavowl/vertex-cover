#include "edge.h"
#include <cstddef>

Edge::Edge() {
	*this = Edge(false, nullptr, nullptr);
}

Edge::Edge(Vertex* v1, Vertex* v2) {
	*this = Edge(false, v1, v2);
}

Edge::Edge(bool covered, Vertex* v1, Vertex* v2) {
	this->covered = covered;
	this->vertices[0] = v1;
	this->vertices[1] = v2;
}