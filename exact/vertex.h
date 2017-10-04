#ifndef _VERTEX_H_
#define _VERTEX_H_
#include <vector>

class Edge;

class Vertex {
	public:
		bool free;
		bool covered;
		int degree;
		std::vector<Edge*> edges;

		Vertex();
		Vertex(bool free, bool covered, int degree);

		void addEdge(Edge* e);
		void removeEdge(Edge* e);

		void setFree();
		void setCovered();
		void setUncovered();

		bool isFree();
		bool isCovered();
};

#include "edge.h"
#endif