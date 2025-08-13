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


extern NoisePass noise_passes[];

void worldgen_generate(Map* map, long seed);


#endif // WORLDGEN_H
