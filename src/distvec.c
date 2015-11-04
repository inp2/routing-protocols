#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>

void read_file(std::string filename)
{
	std::string line;
	std::ifstream topo (filename);
	if(topo.is_open())
	{
		while(getline(topo,line))
		{
			std::istringstream iss(line);

			do
			{
				std::string sub;
				iss >> sub;
				std::cout << sub << std::endl;
			}while(iss);
		}
		topo.close();
	}
	else
	{
		std::cout << "Unable to open file";
	}
}
int main(int argc, char *argv[])
{
	if (argc != 4) {
	    fprintf(stderr,"usage: distvec topofile messagefile changesfile\n");
	    exit(1);
	}

	// Get the topofile
	std::string topofile  = argv[1];

	// Read topofile, and parse topofile	
	read_file(topofile);	
}
