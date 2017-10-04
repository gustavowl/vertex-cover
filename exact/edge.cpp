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

Vertex* Edge::getNeighbourOf(Vertex* v) {
	if (vertices[0] == v)
		return vertices[1];
	else if (vertices[1] == v)
		return vertices[0];
	return nullptr;
}

bool isVertexAttached(Vertex* v) {
	if (vertices[0] == v || vertices[1] == v) {
		return true;
	}
	return false;
}