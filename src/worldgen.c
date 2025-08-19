#include "worldgen.h"
#include "feature.h"
#include "tile.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "OpenSimplex2S.h"
#include "raygui.h"
#include "raylib.h"
#include "vec.h"
#include "biome.h"

OpenSimplexEnv* ose = NULL;
DATypedef(Gradients, OpenSimplexGradients*);

size_t _g_head = 0;
Gradients _gradiants = {0};
OpenSimplexGradients* gradient_next(long seed) {
	OpenSimplexGradients* result = NULL;
	if (_g_head >= _gradiants.count) {
		result = newOpenSimplexGradients(ose, seed + _g_head);
		da_push(_gradiants, result);
	} else {
		result = _gradiants.items[_g_head];
	}
	_g_head++;
	return result;
}

void gradients_free() {
	for (size_t i = 0; i < _gradiants.count; i++)
		free(_gradiants.items[i]);
	_gradiants.count = 0;
	_g_head = 0;
}

int gradient_reserve(long seed) {
	int head = _g_head;
	gradient_next(seed);
	return head;
}

void gradient_pop(int new_head) {
	_g_head = new_head;
}

int gradient_push() {
	return _g_head;
}

OpenSimplexGradients* gradient_get(int index) {
	return _gradiants.items[index];
}


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

#define foreach_chunk(x, y, chunk)\
	for(int y = 0; y < MAP_HEIGHT; y+=(chunk))\
	for(int x = 0; x < MAP_WIDTH; x+=(chunk))

static int biome_grad_moisture = 0;
static int biome_grad_temperature = 0;
static int biome_grad_magic = 0;
static double biome_fungal_treshold = 0.6;
static double biome_crystal_treshold = 0.7;
static double biome_period = 100;

static Biome biome_get(int x, int y) {
	OpenSimplexGradients* moist_grad = gradient_get(biome_grad_moisture);
	OpenSimplexGradients* magic_grad = gradient_get(biome_grad_magic);
	OpenSimplexGradients* temp_grad = gradient_get(biome_grad_temperature);
	double moist = grad_noise(biome_period, moist_grad, x, y);
	double magic = grad_noise(biome_period, magic_grad, x, y);
	double temp = grad_noise(biome_period, temp_grad, x, y);
	if (magic < -0.9)
		return BIOME_VOID;
	if (magic > 0.9)
		return BIOME_HELLSCAPE;

	enum {
		TEMP_HOT,
		TEMP_NORM,
		TEMP_COLD
	} temp_kind = TEMP_NORM;
	enum {
		MOIST_DRY,
		MOIST_NORM,
		MOIST_MOIST,
	} moist_kind = MOIST_NORM;

	if (temp > 0.7) temp_kind = TEMP_HOT;
	if (temp < -0.7) temp_kind = TEMP_COLD;
	if (moist > 0.4) moist_kind = MOIST_MOIST;
	if (moist < -0.6) moist_kind = MOIST_DRY;

	switch (moist_kind) {
	case MOIST_DRY: switch (temp_kind) {
		case TEMP_HOT: return BIOME_MAGMA;
		case TEMP_NORM: return BIOME_CRYSTAL;
		case TEMP_COLD: return BIOME_PLAINS;
	}; break;
	case MOIST_NORM: switch (temp_kind) {
		case TEMP_HOT: return BIOME_ASH_FOREST;
		case TEMP_NORM: return BIOME_PLAINS;
		case TEMP_COLD: return BIOME_ICE;
	}; break;
	case MOIST_MOIST: switch (temp_kind) {
		case TEMP_HOT: return BIOME_GAS;
		case TEMP_NORM: return BIOME_FUNGAL;
		case TEMP_COLD: return BIOME_ICE;
	}; break;
	}
	return BIOME_PLAINS;
}

static void pass_biomes(Map* map, long seed) {
	biome_grad_moisture = gradient_reserve(seed);
	biome_grad_temperature = gradient_reserve(seed);
	biome_grad_magic = gradient_reserve(seed);
}


int p_large = 30;
int p_medium = 10;
int p_small = 5;
double threshold = -0.2;
static void pass_shape(Map* map, long seed) {
	OpenSimplexGradients* large = gradient_next(seed);
	OpenSimplexGradients* medium = gradient_next(seed);
	OpenSimplexGradients* small = gradient_next(seed);

	foreach(x, y) {
		Biome b = biome_get(x, y);
		double warp = grad_noise(p_large, large, x, y) * 5.0;
		double v = 
			grad_noise(p_large, large, x, y)   * 0.6 +
			grad_noise(p_medium, medium, x + warp, y + warp) * 0.3 +
			grad_noise(p_small, small, x, y)   * 0.1
		;

		double openness = biome_definitions[b].openness;
		if (v < openness) {
			map->tiles[y][x] = TILE_AIR;
			continue;
		}
		map->tiles[y][x] = biome_definitions[b].solid;
	}
}



Feature features[] = {
makeFeatureVein(.tile = TILE_COAL, .frequency = 0.4, .size_min = 4, .size_max = 10),
makeFeatureVein(.tile = TILE_IRON, .frequency = 0.4, .size_min = 3, .size_max = 5),
makeFeatureVein(.tile = TILE_DIAMOND, .frequency = 0.2, .size_min = 3, .size_max = 5),
makeFeatureVein(.tile = TILE_EMERALD, .frequency = 0.2, .size_min = 3, .size_max = 5),
makeFeatureSurface(.tile = TILE_GRASS, .depth = 1),
};

static inline Biome biome_from_chunk(int x, int y, int chunk_size) {
	short biome_value[BIOME_SIZE] = {0};
	const static int corner_value = 1;
	const static int edge_value = 3;
	const static int center_value = 10;
	biome_value[biome_get(x, y)] += corner_value;
	biome_value[biome_get(x + chunk_size, y)] += corner_value;
	biome_value[biome_get(x, y + chunk_size)] += corner_value;
	biome_value[biome_get(x + chunk_size, y + chunk_size)] += corner_value;

	int half = chunk_size / 2;
	biome_value[biome_get(x + half, y)] += edge_value;
	biome_value[biome_get(x, y + half)] += edge_value;
	biome_value[biome_get(x + half, y + chunk_size)] += edge_value;
	biome_value[biome_get(x + chunk_size, y + half)] += edge_value;

	biome_value[biome_get(x + half, y + half)] += center_value;

	Biome biome = 0;
	short max_value = 0;
	for (int i = 1; i < BIOME_SIZE; i++) {
		if (biome_value[i] > max_value) {
			max_value = biome_value[i];
			biome = i;
		}
	}
	return biome;
}

static void pass_features(Map* map, long seed) {
	const int feature_c = sizeof(features) / sizeof(Feature);
	foreach_chunk(x, y, 16) {
		Biome b = biome_from_chunk(x, y, 16);
		for (int i = 0; i < MAX_FEATURE_SIZE; i++) {
			if (biome_definitions[b].features[i].kind == FEATURE_NONE)
				break;
			feature_apply(map, seed, x, y, 16, biome_definitions[b].features[i]);
		}
	}
}

void worldgen_generate(Map* map, long seed) {
	if (ose == NULL)
		ose = initOpenSimplex();
	pass_biomes(map, seed);
	pass_shape(map, seed);
	// pass_ores(map, seed);
	pass_features(map, seed);

	gradients_free();
}


static int y = 0;
static int selected_id = 0;
static int current_id = 0;
static bool dirty;
void spinner(const char* label, int* value, int min, int max) {
	current_id++;
	int prev = *value;
	if (GuiSpinner((Rectangle){50, y, 100, 30}, label, value, min, max, selected_id == current_id)) {
		selected_id = current_id;
	}
	y += 35;
	dirty = dirty || prev != *value;
}
void spinnerf(const char* label, double* value, double scale, int min, int max) {
	int int_val = round(*value * scale);
	spinner(label, &int_val, min * scale, max * scale);
	*value = int_val / scale;
}

void gap() {
	y += 20;
}

#define THRESHOLD 100.0
bool worldgen_debug() {
	y = 10;
	dirty = false;
	current_id = 0;
	spinner(" Large", &p_large, 1, 150);
	spinner("Medium", &p_medium, 1, 50);
	spinner(" Small", &p_small, 1, 50);
	spinnerf(" Thres", &threshold, 10, -1, 1);
	gap();
	spinnerf("Biome P", &biome_period, 1, 1, 200);
	spinnerf("Biome F", &biome_fungal_treshold, 100, 0, 1);
	spinnerf("Biome C", &biome_crystal_treshold, 100, 0, 1);

	// gap();
	// for (int i = 0; i < passes_count; i++) {
	// 	spinnerf(" Thresh", &ore_passes[i].threshold, 100, -1, 1);
	// 	spinner(" Period", &ore_passes[i].period, 0, 150);
	// }

	return dirty;
}
