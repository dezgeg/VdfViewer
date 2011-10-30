#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "planet.h"

static System* alloc_system(int nplanets)
{
	return malloc(sizeof(System) + nplanets * sizeof(Planet));
}

static void read_vector(FILE* file, Vector out)
{
	(void)fscanf(file, FLOAT_SCANF_FORMAT FLOAT_SCANF_FORMAT FLOAT_SCANF_FORMAT,
			&out[0], &out[1], &out[2]);
}

static void print_vector(FILE* file, const Vector vec)
{
	fprintf(file, FLOAT_PRINTF_FORMAT " " FLOAT_PRINTF_FORMAT " " FLOAT_PRINTF_FORMAT,
			vec[0], vec[1], vec[2]);
}

System* load_system(FILE* file)
{
	int nplanets;
	long long steps_to_write;
	Float duration, time_step;
	(void)fscanf(file, "%d " FLOAT_SCANF_FORMAT " " FLOAT_SCANF_FORMAT " %lld",
			&nplanets, &duration, &time_step, &steps_to_write);
	System* sys = alloc_system(nplanets);
	if(duration < time_step)
		return NULL;

	sys->time_step = time_step;
	sys->nplanets = nplanets;
	sys->cur_step = 0;
	sys->nsteps = ceil(duration / time_step);
	if(steps_to_write > sys->nsteps)
		steps_to_write = 1;
	sys->print_period = (long long)((double)sys->nsteps / steps_to_write);

	for(int i = 0; i < nplanets; i++)
	{
		(void)fscanf(file, FLOAT_SCANF_FORMAT, &sys->planets[i].mass);
		read_vector(file, sys->planets[i].position);
		read_vector(file, sys->planets[i].velocity);
	}
	return sys;
}

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
