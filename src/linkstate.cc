#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <utility>
#include <set>
#include <string.h> 
#include <map>

#include "array.h"
#include "common.h"

using namespace std;

void parse_edge(std::string line, util::array<int,2> & amat, std::set<int> & node_list)
{
		line += " ";
		unsigned begin = 0, end = 0;
		const char *p = line.c_str();
		std::string columns[3];
		int i = 0;
		while (end < line.length()) {
				char c = p[end];
				//cout << "[" << c << "]";
				if (!isalnum(c)) {
						columns[i++] = line.substr(begin, end-begin);
						while (end < line.length() and !isalnum(p[end])) end++;
						begin = end;
				} else {
						end++;
				}
		}
		int src = atoi(columns[0].c_str());
		int dest = atoi(columns[1].c_str());
		int cost = atoi(columns[2].c_str());
		amat(src,dest) = cost;
		amat(dest,src) = cost;
		node_list.insert(src);
		node_list.insert(dest);
}

void read_file(std::string filename, util::array<int,2> & amat, std::set<int> & node_list)
{
		std::string line;
		std::ifstream topo(filename.c_str());
		if (topo.is_open()) {
				while(getline(topo, line)) {
						parse_edge(line, amat, node_list);
				}
				topo.close();
		} else {
				std::cout << "Unable to open file" << std::endl;
				exit(1);
		}
}

int find_max(std::string line)
{
		line += " ";
		unsigned begin = 0, end = 0;
		const char *p = line.c_str();
		std::string columns[3];
		int i = 0;
		while (end < line.length()) {
				char c = p[end];
				//cout << "[" << c << "]";
				if (!isalnum(c)) {
						columns[i++] = line.substr(begin, end-begin);
						while (end < line.length() and !isalnum(p[end])) end++;
						begin = end;
				} else {
						end++;
				}
		}
		int src = atoi(columns[0].c_str());
		int dest = atoi(columns[1].c_str());

		if(src >= dest)
		{
				return src;
		}
		else
		{
				return dest;
		}
}

int find_array_size(std::string filename)
{
		int max = 0;
		std::string line;
		int size;
		std::ifstream topo(filename.c_str());
		if(topo.is_open())
		{
				while(getline(topo, line))
				{
						size = find_max(line);
						if(size > max)
						{
								max = size;
						}
				}
				topo.close();
		}
		else
		{
				std::cout << "Unable to open file" << std::endl;
		}
		return max;
}

void parse_msg(std::string line, std::vector<Message> & msg_list)
{
	int src, dest;
	char *content = (char *)alloca(sizeof (char) * line.length()); 
	sscanf(line.c_str(), "%d %d %[^\n]", &src, &dest, content); 
	Message message;
	message.src = src;
	message.dest = dest;
	message.content = content;
	msg_list.push_back(message);
}

void read_msg_file(std::string filename, std::vector<Message> & msg_list)
{
		std::string line;
		std::ifstream topo(filename.c_str());
		if (topo.is_open()) {
				while(getline(topo, line)) {
						parse_msg(line, msg_list);
				}
				topo.close();
		} else {
				std::cout << "Unable to open file" << std::endl;
				exit(1);
		}
}

bool read_cfile(std::ifstream &topo, util::array<int,2> & amat)
{
		std::string line;
		// std::ifstream topo(filename.c_str());
		if (topo.is_open() && getline(topo, line)) {
				char * pch;
				int i = 0;
				std::string columns[3];
				char * line_copy = new char[line.size()+1];
				std::copy(line.begin(), line.end(), line_copy);
				line_copy[line.size()] = '\0';
				pch = strtok(line_copy, " ");
				while(pch != NULL)
				{
						columns[i] = pch;
						i++;
						pch = strtok(NULL, " ");
				}
				int src = atoi(columns[0].c_str());
				int dest = atoi(columns[1].c_str());
				int cost = atoi(columns[2].c_str());
				amat(src,dest) = cost;
				amat(dest,src) = cost;
				delete[] line_copy;
				return true;
		} else {
				return false;
		}
}

vector<int> get_neighbors(util::array<int, 2> &amat, int src)
{
		// neighbors
		vector<int> ns;

		// Find all the nodes connected to src
		for(int i = 0; i < amat.len(0); i++) {
				if(amat(src,i) > 0) {
						ns.push_back(i);
				}
		} 

		return ns;
}

// get next hop from src to dest
int get_next_hop(int src, int dest, vector<int> &prev)
{
		if (src == dest) { 
				return src;
		}

		int cur = dest;
		while (prev[cur] != src) {
				cur = prev[cur];
		}
		return cur;
}

typedef vector<pair<int, int> > Table;

vector<Table> build_tables(util::array<int, 2> & amat, std::set<int> & nodes)
{
		// mapping node -> node
		vector<Table> tables;
		tables.resize(amat.len(0));
		// Get starting node from msg_list
		for(int src : nodes) { 
				/* 
 		         1  function Dijkstra(Graph, source):
  		         2
  		         3      create vertex set Q
   		         4
	 	         5      for each vertex v in Graph:             // Initialization
	  	         6          dist[v] ← INFINITY                  // Unknown distance from source to v
	             7          prev[v] ← UNDEFINED                 // Previous node in optimal path from source
	             8          add v to Q                          // All nodes initially in Q (unvisited nodes)
		         9
		         10      dist[source] ← 0                        // Distance from source to source
		         11      
		         12      while Q is not empty:
		         13          u ← vertex in Q with min dist[u]    // Source node will be selected first
		         14          remove u from Q 
		         15          
		         16          for each neighbor v of u:           // where v is still in Q.
		         17              alt ← dist[u] + length(u, v)
		         18              if alt < dist[v]:               // A shorter path to v has been found
		         19                  dist[v] ← alt 
		         20                  prev[v] ← u 
		         21
		         22      return dist[], prev[]
				 */
				// mapping node -> distance from src
				map<int, int> dist; 
				map<int, int> q; 
				// mapping node -> previous node
				vector<int> prev; 
				prev.resize(amat.len(0));
				// prev by default is undefined (i.e. 0)
				fill(prev.begin(), prev.end(), 0);
				vector<bool> visited;
				visited.resize(amat.len(0));
				fill(visited.begin(), visited.end(), false);


				// begin with src
				dist[src] = 0; 
				q[src] = 0; 
				prev[src] = src;

				while (not q.empty()) { 
						auto min = q.begin(); 
						int u = min->first;
						int u_dist = min->second;
						visited[u] = true;
						q.erase(min); 

						for (int v: get_neighbors(amat, u)) {
								int alt = u_dist + amat(u, v);
								bool isinf = dist.find(v) == dist.end();
								if (isinf || alt < dist[v]) {
										dist[v] = alt;
										q[v] = alt;
										prev[v] = u;
								}
						}
				}
				// end dijkstra 
				// update routing table accordingly
				Table table;
				table.resize(amat.len(0)); 
				cerr << "table for " << src << endl;
				for (int dest: nodes) { 
						int next_hop = get_next_hop(src, dest, prev);
						table[dest] = pair<int, int>(next_hop, dist[dest]);
				}
				tables[src] = table;
		}
		return tables;
}

int main(int argc, char *argv[])
{
		// util::array<int, 2> amat(20,20);

		if (argc != 4)
		{
				fprintf(stderr,"usage: distvec topofile messagefile changesfile\n");
				exit(1);
		}

		// Get the topofile
		std::string topofile  = argv[1];
		std::string msgfile = argv[2];
		std::string cfile = argv[3];

		int asize = find_array_size(topofile);

		// std::cout << asize << std::endl;
		util::array<int, 2> amat(asize+1, asize+1);
		std::set<int> node_list;
		std::vector<Message> msg_list;

		for(int j = 0; j < amat.len(1); j++) {
				for(int i = 0; i < amat.len(0); i++) {
						amat(i,j) = -1;
				}
		}

		// Read topofile, and parse topofile
		read_file(topofile, amat, node_list);

		read_msg_file(msgfile, msg_list);

		std::ifstream changes(cfile.c_str());


		// Update graph

		
		//Now run the algorithm
		do {
				vector<Table> tables = build_tables(amat, node_list); 
				print_table(tables, node_list);
				send_messages(tables, msg_list);
		}
		while (read_cfile(changes, amat));
}
