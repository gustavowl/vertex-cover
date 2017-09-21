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