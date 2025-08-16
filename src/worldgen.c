#include "worldgen.h"
#include "tile.h"
#include <stdio.h>
#include <stdlib.h>
#include "OpenSimplex2S.h"
#include "raygui.h"
#include "raylib.h"

OpenSimplexEnv* ose = NULL;

static inline double grad_noise(double period, OpenSimplexGradients* gradient, int x, int y) {
	return noise2(
		ose, gradient, 
		x * (1.0 / period),
		y * (1.0 / period)
	);
}

static inline bool is_pass(NoisePass pass, OpenSimplexGradients* gradient, int x, int y) {
	return grad_noise(pass.period, gradient, x, y) < pass.threshold;
}

#define foreach(x, y)\
	for(int y = 0; y < MAP_HEIGHT; y++)\
	for(int x = 0; x < MAP_WIDTH; x++)


int p_large = 30;
int p_medium = 10;
int p_small = 5;
double threshold = -0.2;
static void pass_shape(Map* map, long seed) {
	OpenSimplexGradients* large = newOpenSimplexGradients(ose, seed + 1);
	OpenSimplexGradients* medium = newOpenSimplexGradients(ose, seed + 2);
	OpenSimplexGradients* small = newOpenSimplexGradients(ose, seed + 3);

	foreach(x, y) {
		double warp = grad_noise(p_large, large, x, y) * 5.0;
		double v = 
			grad_noise(p_large, large, x, y)   * 0.6 +
			grad_noise(p_medium, medium, x + warp, y + warp) * 0.3 +
			grad_noise(p_small, small, x, y)   * 0.1
		;
		
		if (v < threshold) {
			map->tiles[y][x] = TILE_AIR;
		} else {
			map->tiles[y][x] = TILE_STONE;
		}
	}
	free(large);
	free(medium);
	free(small);
}


NoisePass ore_passes[] = {
	{.key = "DIRT", .tile = TILE_DIRT, .threshold = -0.4, .period = 6.0},
	{.key = "COAL", .tile = TILE_COAL, .threshold = -0.4, .period = 6.0},
};
static const int passes_count = sizeof(ore_passes) / sizeof(ore_passes[0]);
static void pass_ores(Map* map, long seed) {
	OpenSimplexGradients* gradients[passes_count];
	for (int i = 0; i < passes_count; i++)
		gradients[i] = newOpenSimplexGradients(ose, seed + i * 10);

	foreach(x, y) {
		if (map->tiles[y][x] == TILE_AIR)
			continue;
		for (int i = 0; i < passes_count; i++) {
			if (is_pass(ore_passes[i], gradients[i], x, y)) {
				map->tiles[y][x] = ore_passes[i].tile;
				break;
			}
		}
	}

	for (int i = 0; i < passes_count; i++)
		free(gradients[i]);
}

void worldgen_generate(Map* map, long seed) {
	ose = initOpenSimplex();

	pass_shape(map, seed + 1);
	pass_ores(map, seed + 2);

	free(ose);
	ose = NULL;
}


bool worldgen_debug() {
	int y = 10;
	int changed = 0;
	changed += GuiSpinner((Rectangle){10, 10, 100, 30}, "L", &p_large, 1, 150, true);
	changed += GuiSpinner((Rectangle){10, y += 35, 100, 30}, "M", &p_medium, 1, 50, true);
	changed += GuiSpinner((Rectangle){10, y += 35, 100, 30}, "S", &p_small, 1, 50, true);
	int threshold_i = threshold * 100;
	changed += GuiSpinner((Rectangle){10, y += 35, 100, 30}, "T", &threshold_i, -100, 100, true);

	threshold = threshold_i / 100.0;
	return changed != 0;
}
