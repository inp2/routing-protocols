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
				//fout << "[" << c << "]";
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
				//fout << "[" << c << "]";
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


typedef vector<pair<int, int> > Table;

vector<Table> build_tables(util::array<int, 2> & amat, std::set<int> & nodes)
{
		// mapping node -> node
		vector<Table> tables;
		tables.resize(amat.len(0));

		for (int src: nodes) { 
				// mapping node -> dist from source
				vector<int> dist;
				dist.resize(amat.len(0)); 
				// < 0 means inf
				fill(dist.begin(), dist.end(), -1);
				// mapping node -> predecessor
				vector<int> prev;
				prev.resize(amat.len(0)); 
				// 0 means null
				fill(prev.begin(), prev.end(), 0); 
				
				dist[src] = 0; 
				prev[src] = src;
				int changes;
				do {
						changes = 0;
						for (int u : nodes) {
								if (dist[u] < 0) continue;
								for (int v : get_neighbors(amat, u)) { 
										int alt = dist[u] + amat(u, v); 
										bool isinf = dist[v] < 0;
										if (isinf || alt < dist[v]) { 
												changes ++;
												dist[v] = alt;
												prev[v] = u;
										} else if (alt == dist[v] && u < prev[v]) { 
												dist[v] = u;
										}
								}
						}
				} while (changes > 0);
				// update routing table accordingly
				Table table;
				table.resize(amat.len(0)); 
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

		fout.open("output.txt");


		// Update graph

		
		//Now run the algorithm
		do {
				vector<Table> tables = build_tables(amat, node_list); 
				print_table(tables, node_list);
				send_messages(tables, msg_list);
		}
		while (read_cfile(changes, amat));
}
