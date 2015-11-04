#include <stdio.h>
#include <stdlib.h>
#include <iostream>

int main(int argc, char *argv[])
{
	if (argc != 4) {
	    fprintf(stderr,"usage: linkstate topofile messagefile changesfile\n");
	    exit(1);
	}

	// Get the topofile
	std::string topofile  = argv[1];

	// Read topofile, and parse topofile	
	
}
