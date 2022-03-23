#ifndef _DIJKSTRA_H_
#define _DIJKSTRA_H_

#include "wdigraph.h"
#include <utility> // for std::pair
#include <unordered_map>
#include <iostream>

using namespace std;

// typedef creates an alias for the specified type
// PIL is the value type of our searchTree 
typedef pair<int, long long> PIL;

// PIPIL is used to insert a key-value pair in our searchTree
// if we declare a variable 'x' as follows:  PIPIL x;
// x.first gives the start vertex of the edge, 
// x.second.first gives the end vertex of the edge, 
// x.second.second gives the cost of the edge
typedef pair<int, PIL> PIPIL;

// NOTE: you are not required to use PIPIL in your solution if you would prefer
// to implement Dijkstra's algorithm differently, this is here simply because
// it was used in the lecture for the slower version of Dijkstra's algorithm.

void dijkstra(const WDigraph& graph, int startVertex,
              unordered_map<int, PIL>& tree);

#endif
