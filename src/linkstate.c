#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;


class Graph {
private:
	class Node { 
	private:
		vector<Node*> neighbors;
		vector<size_t> costs; 
		int num_neighbors;
		Graph *graph;
	public:
		Node(Graph *g) : graph(g) {}
		void add_neighbor(string, int);
	};

	// mapping name of node -> node
	map<string, Node*> nodes;

	// mapping node -> its id
	map<Node*, int> ids;

	// index nodes by int
	int maxnode = 0;

	Node* get_or_create(string);
public:
	// add an edge based on a description
	void add_edge(string, string, int);
	void parse_edge(string); 
};

Graph::Node *Graph::get_or_create(string src)
{
	Node *n = nodes[src];
	if (n == NULL) {
		n = new Node(this); 
		nodes[src] = n;
		ids[n] = maxnode++;
	}
	return n; 
}

void Graph::Node::add_neighbor(string neighbor, int cost)
{
	Node *node = graph->nodes[neighbor];
	int nodeidx = graph->ids[node];
	if (nodeidx >= num_neighbors++) {
		neighbors.resize(num_neighbors);
		costs.resize(num_neighbors);
	} 
	neighbors[nodeidx] = node;
	costs[nodeidx] = cost; 
}

void Graph::add_edge(string src, string dest, int cost)
{
	Node *srcnode = get_or_create(src); 
	srcnode->add_neighbor(dest, cost);
}

void Graph::parse_edge(string line)
{
	line += " ";
	int begin = 0, end = 0;
	const char *p = line.c_str();
	string columns[3];
	int i = 0;
	while (end < line.length()) {
		char c = p[end];
		//cout << "[" << c << "]";
		if (!isdigit(c)) { 
			columns[i++] = line.substr(begin, end-begin);
			while (end < line.length() and !isdigit(p[end])) end++;
			begin = end;
		} else {
			end++;
		}
	} 
	string src = columns[0];
	string dest = columns[1];
	int cost = atoi(columns[2].c_str()); 
	add_edge(src, dest, cost);
}

Graph *read_file(string filename)
{
	string line;
	ifstream topo(filename.c_str());
	Graph *graph = new Graph;
	if (topo.is_open()) {
		while(getline(topo, line)) {
			graph->parse_edge(line);
		}
		topo.close();
	} else {
		cout << "Unable to open file";
		exit(1);
	}

	return graph; 
}


int main(int argc, char *argv[])
{
	if (argc != 4) {
		fprintf(stderr,"usage: distvec topofile messagefile changesfile\n");
		exit(1);
	}

	// Get the topofile
	string topofile  = argv[1];

	// Read topofile, and parse topofile	
	Graph *graph = read_file(topofile);
}
