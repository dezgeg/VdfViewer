#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "planet.h"

static System* alloc_system(int nplanets)
{
	return malloc(sizeof(System) + nplanets * sizeof(Planet));
}

/* read a 3-dimensional vector from file */
static void read_vector(FILE* file, Vector out)
{
	fscanf(file, FLOAT_SCANF_FORMAT FLOAT_SCANF_FORMAT FLOAT_SCANF_FORMAT,
			&out[0], &out[1], &out[2]);
}
/* write a 3-dimensional vector to file */
static void print_vector(FILE* file, const Vector vec)
{
	fprintf(file, FLOAT_PRINTF_FORMAT " " FLOAT_PRINTF_FORMAT " " FLOAT_PRINTF_FORMAT,
			vec[0], vec[1], vec[2]);
}
/* load a solar system specification from file, return NULL on failure */
System* load_system(FILE* file)
{
	int nplanets;
	long long steps_to_write;
	Float duration, time_step;
	fscanf(file, "%d " FLOAT_SCANF_FORMAT " " FLOAT_SCANF_FORMAT " %lld",
			&nplanets, &duration, &time_step, &steps_to_write);
	System* sys = alloc_system(nplanets);
	if(!sys || duration < time_step)
	{
		free(sys);
		return NULL;
	}

	sys->time_step = time_step;
	sys->nplanets = nplanets;
	sys->cur_step = 0;
	sys->nsteps = ceil(duration / time_step);
	if(steps_to_write > sys->nsteps)
		steps_to_write = sys->nsteps;
	sys->print_period = (long long)((double)sys->nsteps / steps_to_write);

	for(int i = 0; i < nplanets; i++)
	{
		fscanf(file, FLOAT_SCANF_FORMAT, &sys->planets[i].mass);
		read_vector(file, sys->planets[i].position);
		read_vector(file, sys->planets[i].velocity);
	}
	return sys;
}

/* write a solar system state to file */
void print_system(FILE* file, const System* sys)
{
	int count = sys->nplanets;
	fprintf(file, "%d " FLOAT_PRINTF_FORMAT " ",
			count, sys->cur_step * sys->time_step);
	for(int i = 0; i < sys->nplanets; i++)
	{
		fprintf(file, " ");
		print_vector(file, sys->planets[i].position);
		fprintf(file, i == count - 1 ? "\n" : " ");
	}
}
