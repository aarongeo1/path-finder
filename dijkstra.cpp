#include "dijkstra.h"
#include <iostream>
#include <queue>
using namespace std;

typedef pair<long long, PIPIL> minq;

/*
Compute least cost paths that start from a given vertex.
Use a binary heap to efficiently retrieve an unexplored
vertex that has the minimum distance from the start vertex
at every iteration.
NOTE: PIL is an alias for "pair<int, long long>" type as discussed in class
PARAMETERS:
WDigraph: an instance of the weighted directed graph (WDigraph) class
startVertex: a vertex in this graph which serves as the root of the search tree
tree: a search tree to construct the least cost path from startVertex to some vertex
*/


void dijkstra(const WDigraph& graph, int startVertex, unordered_map<int, PIL>& tree) {

	priority_queue<minq, vector<minq>, greater<minq>> events; 
	events.push({0,{startVertex,{startVertex,0}}});

	while (events.empty() != true) {
		int u = events.top().second.first;
		int v = events.top().second.second.first;
		long long time = events.top().first;
		events.pop();
		if (tree.find(v) == tree.end()) {
			tree[v] = PIL(u, time); // add v to tree
			for (auto it = graph.neighbours(v); it != graph.endIterator(v); it++) {
				// insert point into heap with new cost
				events.push({time + graph.getCost(v, *it),{v, {*it, time + graph.getCost(v, *it)}}});
			}
      	}
	}
}
