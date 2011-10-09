#ifndef PLANET_H
#define PLANET_H
#include <stdio.h>

typedef double Float;
#define FLOAT_SCANF_FORMAT "%lf"
#define FLOAT_PRINTF_FORMAT "%g"

typedef Float Vector[3];

typedef struct Planet
{
	Float mass;
	Vector position;
	Vector velocity;
} Planet;

typedef struct System
{
	Float time_step;
	int nplanets;
	int cur_step;
	int nsteps;
	int print_period; // print system state every N time steps
	Planet planets[];
} System;

extern System* load_system(FILE* file);
extern void print_system(FILE* file, const System* system);
#endif
