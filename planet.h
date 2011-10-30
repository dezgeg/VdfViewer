#ifndef PLANET_H
#define PLANET_H

#include <stdio.h>

#include "vector.h"

typedef struct Planet
{
	Float mass;
	Vector position;
	Vector velocity;
	Vector acceleration[2]; // acceleration vector used varies with every other time step
} Planet;

typedef struct System
{
	Float time_step;
	int nplanets;
	long long cur_step;
	long long nsteps;
	long long print_period; // print system state every N time steps
	Planet planets[];
} System;

extern System* load_system(FILE* file);
extern void print_system(FILE* file, const System* system);
#endif
