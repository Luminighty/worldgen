#include "worldgen.h"
#include "tile.h"
#include <stdlib.h>
#include "OpenSimplex2S.h"

#define PASS_COUNT 10

OpenSimplexEnv* ose;
OpenSimplexGradients* osg[PASS_COUNT];

NoisePass noise_passes[PASS_COUNT] = {
	{.key = "FLOOR", .tile = TILE_FLOOR, .threshold = -0.18, .period = 9.5},
	{.key = "DIRT", .tile = TILE_DIRT, .threshold = -0.4, .period = 6.0},
	{.key = "COAL", .tile = TILE_COAL, .threshold = -0.4, .period = 6.0},
};


static Tile get_tile(int x, int y) {
	for (int i = 0; i < PASS_COUNT; i++) {
		double v = noise2(
			ose, osg[i], 
			x * (1.0 / noise_passes[i].period),
			y * (1.0 / noise_passes[i].period)
		);
		if (v < noise_passes[i].threshold)
			return noise_passes[i].tile;
	}
	return TILE_STONE;
}


#define foreach(x, y)\
	for(int y = 0; y < MAP_HEIGHT; y++)\
	for(int x = 0; x < MAP_WIDTH; x++)

void worldgen_generate(Map* map, long seed) {
	ose = initOpenSimplex();
	for (int i = 0; i < PASS_COUNT; i++)
		osg[i] = newOpenSimplexGradients(ose, seed + i);

	foreach(x, y)
		map->tiles[y][x] = get_tile(x, y);
}


