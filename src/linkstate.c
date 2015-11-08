#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

struct Edge
{
	int destination;
	int weight;
};

// Create graph
vector<Edge> graph;

void parse_edge(string line)
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
	int src = atoi(columns[0].c_str());
	int dest = atoi(columns[1].c_str());
	int cost = atoi(columns[2].c_str()); 
	//Edge newEdge;
	graph[src].destination = dest;
	graph[src].weight = cost;
}

void read_file(string filename)
{
	string line;
	ifstream topo(filename.c_str());
	if (topo.is_open()) {
		while(getline(topo, line)) {
			parse_edge(line);
		}
		topo.close();
	} else {
		cout << "Unable to open file";
		exit(1);
	}
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
	read_file(topofile);

	// Print out graph
	for(int i =0; i < graph.size(); i++)
	{
		cout << graph[i].destination << endl;
		cout << graph[i].weight << endl;
	}
}
