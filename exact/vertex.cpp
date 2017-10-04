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

void Vertex::addEdge(Edge* e) {
	if (e->isVertexAttached(this)) {
		edges.push_back(e);
		degree++;
	}
}

void Vertex::removeEdge(Edge* e) {
	for (int i = 0; i < degree; i++) {
		if (edges[i] == e) {
			edges.erase(edges.begin() + i);
			degree--;
		}
	}
}

void Vertex::setFree() {
	free = true;
	covered = false;
}

void Vertex::setCovered() {
	free = false;
	covered = true;
}

void Vertex::setUncovered() {
	free = false;
	covered = false;
}

bool Vertex::isFree() {
	return free;
}

bool Vertex::isCovered() {
	return !free && covered;
}