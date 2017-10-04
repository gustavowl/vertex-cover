#ifndef _EDGE_H_
#define _EDGE_H_

#include "vertex.h"

class Edge {
	public:
		bool covered;
		Vertex* vertices[2];

		Edge();
		Edge(Vertex* v1, Vertex* v2);
		Edge(bool covered, Vertex* v1, Vertex* v2);

		Vertex* getNeighbourOf(Vertex* v);
		bool isVertexAttached(Vertex* v);
};

#endif