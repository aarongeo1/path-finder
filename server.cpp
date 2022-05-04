// ---------------------------------------------------
// Name : Aaron Geo Binoy
// ---------------------------------------------------
#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include <list>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sstream>

#include "wdigraph.h"
#include "dijkstra.h"

struct Point {
    long long lat, lon;
};

// returns the manhattan distance between two points
long long manhattan(const Point& pt1, const Point& pt2) {
  long long dLat = pt1.lat - pt2.lat, dLon = pt1.lon - pt2.lon;
  return abs(dLat) + abs(dLon);
}

// finds the id of the point that is closest to the given point "pt"
int findClosest(const Point& pt, const unordered_map<int, Point>& points) {
  pair<int, Point> best = *points.begin();

  for (const auto& check : points) {
    if (manhattan(pt, check.second) < manhattan(pt, best.second)) {
      best = check;
    }
  }
  return best.first;
}

// read the graph from the file that has the same format as the "Edmonton graph" file
void readGraph(const string& filename, WDigraph& g, unordered_map<int, Point>& points) {
  ifstream fin(filename);
  string line;

  while (getline(fin, line)) {
    // split the string around the commas, there will be 4 substrings either way
    string p[4];
    int at = 0;
    for (auto c : line) {
      if (c == ',') {
        // start new string
        ++at;
      }
      else {
        // append character to the string we are building
        p[at] += c;
      }
    }

    if (at != 3) {
      // empty line
      break;
    }

    if (p[0] == "V") {
      // new Point
      int id = stoi(p[1]);
      assert(id == stoll(p[1])); // sanity check: asserts if some id is not 32-bit
      points[id].lat = static_cast<long long>(stod(p[2])*100000);
      points[id].lon = static_cast<long long>(stod(p[3])*100000);
      g.addVertex(id);
    }
    else {
      // new directed edge
      int u = stoi(p[1]), v = stoi(p[2]);
      g.addEdge(u, v, manhattan(points[u], points[v]));
    }
  }
}

int create_and_open_fifo(const char * pname, int mode) {
  // creating a fifo special file in the current working directory
  // with read-write permissions for communication with the plotter
  // both proecsses must open the fifo before they can perform
  // read and write operations on it
  if (mkfifo(pname, 0666) == -1) {
    cout << "Unable to make a fifo. Ensure that this pipe does not exist already!" << endl;
    exit(-1);
  }

  // opening the fifo for read-only or write-only access
  // a file descriptor that refers to the open file description is
  // returned
  int fd = open(pname, mode);

  if (fd == -1) {
    cout << "Error: failed on opening named pipe." << endl;
    exit(-1);
  }

  return fd;
}

// keep in mind that in part 1, the program should only handle 1 request
// in part 2, you need to listen for a new request the moment you are done
// handling one request
int main() {
  WDigraph graph;
  unordered_map<int, Point> points;

  const char *inpipe = "inpipe";
  const char *outpipe = "outpipe";

  // Open the two pipes
  int in = create_and_open_fifo(inpipe, O_RDONLY);
  cout << "inpipe opened..." << endl;
  int out = create_and_open_fifo(outpipe, O_WRONLY);
  cout << "outpipe opened..." << endl;  

  // build the graph
  readGraph("server/edmonton-roads-2.0.1.txt", graph, points);
  int max1 = 1024;
  while (true) {
    string s = "";
    Point sPoint, ePoint;
    char *buffer = new char[max1];
    double sla, slo, ela, elo;
    read(in, buffer, max1);
    // allocating buffer
    for (int i = 0; i < max1; i++) {
      s += buffer[i];
    }
    // ending if there is no path
    if (s.find('Q') != string::npos) {
      s = "";
      // deallocating buffer
      delete[] buffer;
      break;
    }
    stringstream fin(s);
    fin >> sla >> slo >> ela >> elo;
    // getting coordinates
    sPoint = {static_cast<long long>(sla * 100000), static_cast<long long>(slo * 100000)};
    ePoint = {static_cast<long long>(ela * 100000), static_cast<long long>(elo * 100000)};
    int start = findClosest(sPoint, points);
    int end = findClosest(ePoint, points);
    // run dijkstra's algorithm, this is the unoptimized version that
    // does not stop when the end is reached but it is still fast enough
    unordered_map<int, PIL> tree;
    dijkstra(graph, start, tree);
    // NOTE: in Part II you will use a different communication protocol than Part I
    // So edit the code below to implement this protocol
    if (tree.find(end) == tree.end()) {
      // writing E to the output pipe
      // to indicate that there is no path
      write(out, "E\n", 16);
    } else {
      vector<int> cod;
      while (end != start) {
        cod.push_back(end);
        end = tree[end].first;
      }
      cod.push_back(start);
      // Declaring the ouput string.
      string str = "";
      int i = cod.size() - 1;
      // getiing the coordinates backwards
      while (i >= 0) {
        // converting coordinates
        string Lat = to_string(points[cod[i]].lat);
        string Long = to_string(points[cod[i]].lon);
        // converting the coordinates into decimals
        Lat.insert(2, ".");
        Long.insert(4, ".");
        str += Lat + ' ' + Long + '\n';
        i--;
      }
      str += "E\n";
      write(out, str.c_str(), str.size());
    }
  }
  // closing and deleting pipes
  close(in);
  close(out);
  unlink(inpipe);
  unlink(outpipe);
  return 0;
}
