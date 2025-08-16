#ifndef WORLDGEN_H
#define WORLDGEN_H

#include "map.h"
#include "tile.h"


typedef struct {
	char key[8];
	double threshold;
	double period;
	Tile tile;
} NoisePass;


void worldgen_generate(Map* map, long seed);
bool worldgen_debug();


#endif // WORLDGEN_H
