#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>

using namespace std;

struct Node { 
	vector<Node*> neighbors;
	vector<size_t> costs; 
};

// mapping name of node -> node
map<string, Node*> nodes;
// index nodes by int
int maxnode = 0;

Node *get_or_create(string src)
{
	Node *n = nodes[src];
	if (n == NULL) {
		n = new Node; 
		nodes[src] = n;
		maxnode++;
	}
	return n;
	
}

void parse_line (string line) {
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
	string cost = columns[2];
	Node *srcnode = get_or_create(src);
	srcnode[
}

void read_file(string filename)
{
	string line;
	std::string src;
	std::string dest;
	std::string weight;
	ifstream topo (filename.c_str());
	if(topo.is_open())
	{
		while(getline(topo, line))
		{
			 stringstream linestream(line);
			 parse_line(line);
		}
		topo.close();
	}
	else
	{
		cout << "Unable to open file";
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
//	map<string, string, string> topo_map;
	read_file(topofile);
	//topo_map = read_file(topofile);
	// std::cout << topo_map << std::endl;	
}
