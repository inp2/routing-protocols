#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <string.h>

#include "array.h"

void parse_edge(std::string line, util::array<int,2> & amat, std::set<int> & node_list)
{
	line += " ";
	int begin = 0, end = 0;
	const char *p = line.c_str();
	std::string columns[3];
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
	int begin = 0, end = 0;
	const char *p = line.c_str();
	std::string columns[3];
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

void parse_msg(std::string line, std::vector<int> & msg_list)
{
	line += " ";
	int space_count = 0;
	int begin = 0, end = 0;
	const char *p = line.c_str();
	std::string columns[2];
	int i = 0;
	while (end < line.length()) {
		char c = p[end];
		if (!isdigit(c)) {
			columns[i++] = line.substr(begin, end-begin);
			while (end < line.length() and !isdigit(p[end])) end++;
			begin = end;
			space_count++;
			if (space_count == 2) break;
		}
		else
		{
			end++;
		}
	}
	int src = atoi(columns[0].c_str());
	int dest = atoi(columns[1].c_str());
	msg_list.push_back(src);
	msg_list.push_back(dest);
}

void read_msg_file(std::string filename, std::vector<int> & msg_list)
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

void read_cfile(std::string filename, util::array<int,2> & amat)
{
	std::string line;
	std::ifstream topo(filename.c_str());
	if (topo.is_open())
	{
		while(getline(topo, line))
		{
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
		}
		topo.close();
	}
	else
	{
		std::cout << "Unable to open file" << std::endl;
		exit(1);
	}
}

void dijsktra(util::array<int, 2> & amat, std::vector<int> & msg_list, std::set<int> & node_list)
{
	// Get starting node from msg_list
	for(int i = 0; i < msg_list.size(); i++)
	{
		int src = msg_list.at(i);
		int dest = msg_list.at(++i);

		// Find all the nodes connected to src
		for(int j = 0; j < amat.len(1); j++)
		{
			for(int i = 0; i < amat.len(0); i++)
			{
				if(i == src || j == src)
				{
					if(amat(i,j) > 0)
					{
						std::cout << i << " " << j << " " << amat(i,j) << std::endl;
					}
				}
			}
		}
	}
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
  std::vector<int> msg_list;

  for(int j = 0; j < amat.len(1); j++)
  {
	  for(int i = 0; i < amat.len(0); i++)
	  {
		  amat(i,j) = -1;
	  }
  }

// Read topofile, and parse topofile
  read_file(topofile, amat, node_list);

  read_msg_file(msgfile, msg_list);
// Print out graph, should be 8
	std::cout << "graph_test: " << amat(1,2) << std::endl;
	std::cout << "graph_test: " << amat(2,3) << std::endl;
	std::cout << "graph_test: " << amat(2,5) << std::endl;
	std::cout << "graph_test: " << amat(4,1) << std::endl;
	std::cout << "graph_test: " << amat(4,5) << std::endl;

	for(auto it: node_list)
	{
		// std::cout << "node_list" << std::endl;
		std::cout << "node_list: " << it  << std::endl;
	}

	// Print out msglist
	//std::cout << msg_list.size() << std::endl;
	for(int i = 0; i < msg_list.size(); i++)
	{
		std::cout << "msg_list: " << msg_list.at(i) << std::endl;
	}

	// Update graph
	read_cfile(cfile, amat);
	std::cout << "Updated graph" << std::endl;
    std::cout << "graph_test: " << amat(1,2) << std::endl;
	std::cout << "graph_test: " << amat(2,3) << std::endl;
	std::cout << "graph_test: " << amat(2,4) << std::endl;
	std::cout << "graph_test: " << amat(2,5) << std::endl;
	std::cout << "graph_test: " << amat(4,1) << std::endl;
	std::cout << "graph_test: " << amat(4,5) << std::endl;

	//Now run the algorithm
	dijsktra(amat, msg_list, node_list);
}
