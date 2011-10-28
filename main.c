#include <stdlib.h>
#include <stdio.h>

#include "simulate.h"
#include "planet.h"

void usage(const char* progname)
{
	printf("usage: %s infile [outfile]\n", progname);
	exit(EXIT_FAILURE);
}

int main(int argc, char** argv)
{
	if(argc < 2 || argc > 3)
		usage(argv[0]);

	FILE* outfile;
	if(argc < 3)
		outfile = stdout;
	else
	{
		outfile = fopen(argv[2], "w");
		if(outfile == NULL)
		{
			perror("Error opening output file");
			return EXIT_FAILURE;
		}
	}

	FILE* infile = fopen(argv[1], "r");
	if(infile == NULL)
	{
		perror("Error opening input file");
		return EXIT_FAILURE;
	}

	System* system = load_system(infile);
	if(system == NULL)
	{
		printf("Loading input file failed.\n");
		return EXIT_FAILURE;
	}

	init_simulation(system);
	simulate(outfile, system);
}
