#ifndef SIMULATE_H
#define SIMULATE_H

#include <stdio.h>
#include "planet.h"

extern void init_simulation(System* sys);
extern void simulate_one_step(System* sys, int turn, Float time_step);
extern void simulate(FILE* outfile, System* sys);
#endif
