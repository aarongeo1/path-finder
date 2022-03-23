#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <string>
#include "dijkstra.h"
#include "digraph.h"
#include "wdigraph.h"
using namespace std;

struct Point {
	long long lat; // latitude of the point
	long long lon; // longitude of the point
};

long long manhattan(const Point& pt1, const Point& pt2) {
	long long sum = abs(pt1.lat - pt2.lat) + abs(pt1.lon - pt2.lon);
	return sum;
}

typedef pair<int, int> vpil;

/*
    Description : it reads the text file and reads each line to create the
    	vertexes and edges to connect directly
    Arguments:
            
    Returns:
    		
            
*/
void readGraph(string filename, WDigraph& graph, unordered_map<int, Point>& points) {
  string line;
  ifstream file(filename);
  while (getline(file, line)) {
    if (line[0] == 'V') {
      // checking if it is a vertex
      int size = line.find(',', 2);
      // using the index to convert string to integer
      int id = stoi(line.substr(2, size - 2));

      int size1 = line.find(",",size + 1);
      double coord2 = stod(line.substr(size1 + 1));
      double coord1 = stod(line.substr(size + 1, size1 - size - 1));

      Point coord;
      coord.lat = static_cast<long long>(coord1*100000);
      coord.lon = static_cast<long long>(coord2*100000);

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
      long long cost = manhattan(points[fv],points[sv]);
      graph.addEdge(fv, sv, cost);
    }
  }
}

vpil getvertex(Point x, Point y, unordered_map<int, Point> &points){
	vpil ver;
	ver = {points.begin()->first, points.begin()->first};
	string str = "";
	for (auto it : points) {
	// the closest point
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


int main() {
	unordered_map<int, Point> points;
	unordered_map<int, PIL> tree;
	vpil ver;
	vector<int> cod;
	WDigraph graph = WDigraph();
	readGraph("edmonton-roads-2.0.1.txt", graph, points);
	// checking closest
	Point x;
	Point y;
	string check;
	cin >> check;
	if (check == "R") {
		cin >> check;
		x.lat = static_cast<long long>(stod(check));
		cin >> check;
		x.lon = static_cast<long long>(stod(check));
		cin >> check;
		y.lat = static_cast<long long>(stod(check));
		cin >> check;
		y.lon = static_cast<long long>(stod(check));
		ver = getvertex(x, y, points);
		dijkstra(graph, ver.first, tree);
	}
	if (tree.find(ver.second) == tree.end()) {
        cout << "N 0" << endl;
    }
    cod = rever(tree, ver.first, ver.second);
    cout << "N " << cod.size() << endl;
    int i = cod.size() - 1;
	while (i >= 0) {
		cin >> check;
		if(check == "A") {
			cout << "W " << points[cod[i]].lat << " ";
            cout << points[cod[i]].lon << endl;
            i--;
		} else {
			break;
		}
	}
	cout << "E" << endl;
	return 0;
}