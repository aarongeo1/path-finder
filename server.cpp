
// Name : Aaron Geo Binoy

#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <string>
#include "dijkstra.h"
#include "digraph.h"
#include "wdigraph.h"

using namespace std;

// the datatype which stores the latitudes and longitudes
struct Point {
	long long lat;
	// latitude of the point
	long long lon;
	// longitude of the point
};

/*
	Description : creates the cost of the edge
	Arguments:
		Point pt1 - the Point dataype of the first vertex we take
		Point Pt2 - the Point datatype of the second vectex we take
	Returns:
		long long sum - the cost
*/
long long manhattan(const Point& pt1, const Point& pt2) {
	long long sum = abs(pt1.lat - pt2.lat) + abs(pt1.lon - pt2.lon);
	return sum;
}

// a new alias for datatype pair
typedef pair<int, int> vpil;

/*
	Description : it reads the text file and reads each line to create the
		vertexes and edges to connect directly
	Arguments:
		string filename - the name of the file from which we get the inputs
		WDigraph& graph - a reference to the graph to build from the inputs
		unordered_map<int, Point>& points - the unordered map where we store the
			latitude and longitude for each vertex.
	Returns:
		void - none
*/
void readGraph(string filename, WDigraph& graph,
	unordered_map<int, Point>& points) {
	string line;
	ifstream file(filename);
	while (getline(file, line)) {
		if (line[0] == 'V') {
			// checking if it is a vertex
			int size = line.find(',', 2);
			// using the index to convert string to integer
			int id = stoi(line.substr(2, size - 2));
			int size1 = line.find(",", size + 1);
			// getting the coordinates and storing it in as double
			double coord2 = stod(line.substr(size1 + 1));
			double coord1 = stod(line.substr(size + 1, size1 - size - 1));
			Point coord;
			// storing the coordinates in Point
			coord.lat = static_cast<long long>(coord1 * 100000);
			coord.lon = static_cast<long long>(coord2 * 100000);
			points[id] = coord;
			// adding each vertex
			graph.addVertex(id);
		}
		if (line[0] == 'E') {
			int firstid = line.find(',', 2);
			// using the index to convert string to integer
			int fv = stoi(line.substr(2, firstid - 2));
			int secondid = line.find(',', firstid + 1);
			// using the index to convert string to integer
			int sv = stoi(line.substr(firstid + 1, secondid - firstid - 1));
			long long cost = manhattan(points[fv], points[sv]);
			// since its not undirectly, we only add one side
			graph.addEdge(fv, sv, cost);
		}
	}
}

/*
	Description :gets the closest vertex from the coordinates given in the input
	Arguments:
		Point x - the Point dataype of the first vertex we take
		Point y - the Point datatype of the second vectex we take
		unordered_map<int, Point> &points - the unordered map where we store the
			latitude and longitude for each vertex.
	Returns:
		vpil ver - the new alias of pair which contains the start and end vertex
*/
vpil getvertex(Point x, Point y, unordered_map<int, Point> &points) {
	vpil ver;
	// keeping the first vertex for comparison
	ver = {points.begin()->first, points.begin()->first};
	string str = "";
	for (auto it : points) {
	// for finding the closest vertexes to that start and end
		if (manhattan(it.second, x) < manhattan(points[ver.first], x)) {
		ver.first = it.first;
		str = "success";
		}
		if (manhattan(it.second, y) < manhattan(points[ver.second], y)) {
		ver.second = it.first;
		}
	}
	return ver;
}

/*
	Description : To create the vertexes in order to output in main
	Arguments:
		Point x - the Point dataype of the first vertex we take
		Point y - the Point datatype of the second vectex we take
		unordered_map<int, Point> &points - the unordered map where we store the
			latitude and longitude for each vertex.
	Returns:
		vpil ver - the new alias of pair which contains the start and end vertex
*/
vector<int> rever(unordered_map<int, PIL>& tree, int start, int end) {
	vector<int> cod;
	int current = end;
	while (current != start) {
		cod.push_back(current);
		current = tree[current].first;
	}
	cod.push_back(start);
	return cod;
}

/*
	Description : gets the input, calls all the functions and outputs
	Arguments:
		None
	Returns:
		int 0
*/
int main() {
	// initializing all the variables
	unordered_map<int, Point> points;
	unordered_map<int, PIL> tree;
	vpil ver;
	vector<int> cod;
	WDigraph graph = WDigraph();
	Point x, y;
	// gets the graph build from the text file
	readGraph("edmonton-roads-2.0.1.txt", graph, points);
	string check;
	cin >> check;
	// condition to check if to start reading
	if (check == "R") {
		cin >> check;
		x.lat = static_cast<long long>(stod(check));
		cin >> check;
		x.lon = static_cast<long long>(stod(check));
		cin >> check;
		y.lat = static_cast<long long>(stod(check));
		cin >> check;
		y.lon = static_cast<long long>(stod(check));
		// gets the closest vertex
		ver = getvertex(x, y, points);
		// algorithm to find the shortest path
		dijkstra(graph, ver.first, tree);
	}
	//  checking if the path exists
	if (tree.find(ver.second) == tree.end()) {
		cout << "N 0" << endl;
	// else condition if it does indeed exists
	} else {
		// building the vertexs in order of the shortest path
		cod = rever(tree, ver.first, ver.second);
		// starts the output
		cout << "N " << cod.size() << endl;
		int i = cod.size() - 1;
		while (i >= 0) {
			// conditions to print only if the user sends "A"
			cin >> check;
			if(check == "A") {
				cout << "W " << points[cod[i]].lat << " ";
				cout << points[cod[i]].lon << endl;
				i--;
			} else {
				break;
			}
		}
		// end of outputting
		cout << "E" << endl;
		return 0;
	}
}
