#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>

using namespace std;

//map<string, string, string> read_file(string filename)
void read_file(string filename)
{
	string line;
	std::string src;
	std::string dest;
	std::string weight;
	ifstream topo (filename.c_str());
	map<std::string, std::array<std::string, 2>> mymap;
	if(topo.is_open())
	{
		while(getline(topo, line))
		{
			 stringstream linestream(line);
			 src = line[0];
			 dest = line[2];
			 weight = line[4];
			 mymap.insert(weight,std::array<src, dest>);
		}
		topo.close();
		//return mymap;
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
