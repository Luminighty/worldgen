#include "feature.h"
#include "map.h"
#include "random.h"
#include "tile.h"
#include <math.h>
#include <stdio.h>


static inline bool is_air(Map* map, int x, int y) {
	if (x < 0 || y < 0 || x >= MAP_WIDTH || y >= MAP_HEIGHT)
		return false;
	return map->tiles[y][x] == TILE_AIR;
}

static inline void set_if_solid(Map* map, int x, int y, Tile tile) {
	if (x < 0 || y < 0 || x >= MAP_WIDTH || y >= MAP_HEIGHT)
		return;
	if (map->tiles[y][x] == TILE_AIR)
		return;
	map->tiles[y][x] = tile;
}


static inline bool is_inside_ellipse(int center_x, int center_y, int width, int height, float angle, int x, int y) {
	float cos_angle = cos(angle);
	float sin_angle = sin(angle);

	float dx = (x - center_x);
	float dy = (y - center_y);
	float x_line = dx * cos_angle + dy * sin_angle;
	x_line *= x_line;
	x_line /= (float)width;

	float y_line = dy * cos_angle - dx * sin_angle;
	y_line *= y_line;
	y_line /= (float)height;

	return x_line + y_line < 1.0f;
}

static inline void feature_blob(
	Map* map, long seed,
	int x, int y,
	int chunk_size,
	FeatureBlob feature
) {
	int rnd = random(seed, feature.tile);
	double roll = randomf3d(seed, x, y, rnd++);
	if (roll > feature.frequency)
		return;
	int origin_x = (random3d(seed, x, y, rnd++) % chunk_size) + x;
	int origin_y = (random3d(seed, x, y, rnd++) % chunk_size) + y;

	int length_var = (feature.size_max - feature.size_min);
	int size_x = (random3d(seed, x, y, rnd++) % length_var) + feature.size_min;
	int size_y = (random3d(seed, x, y, rnd++) % length_var) + feature.size_min;
	float angle = randomf3d(seed, x, y, rnd++) * 3.14159;

	for (int y = -size_y; y < size_y; y++)
	for (int x = -size_x; x < size_x; x++)
	if (is_inside_ellipse(origin_x, origin_y, size_x, size_y, angle, origin_x + x, origin_y + y))
		set_if_solid(map, origin_x + x, origin_y + y, feature.tile);
}



static inline void feature_vein(
	Map* map, long seed,
	int x, int y,
	int chunk_size,
	FeatureVein feature
) {
	int rnd = random(seed, feature.tile);
	double roll = randomf3d(seed, x, y, rnd++);
	if (roll > feature.frequency)
		return;
	x += (random3d(seed, x, y, rnd++) % chunk_size);
	y += (random3d(seed, x, y, rnd++) % chunk_size);
	int size = (random3d(seed, x, y, rnd++) % (feature.size_max - feature.size_min)) + feature.size_min;
	for (; size >= 0; size--) {
		set_if_solid(map, x, y, feature.tile);
		int d = (random3d(seed, x, y, size) % 4);
		switch (d) {
			case 0: y--; break;
			case 1: y++; break;
			case 2: x--; break;
			case 3: x++; break;
		}
	}
}


// NOTE: We could improve the performance of this function by going
//	downwards and swapping the state from surface -> wall -> air
static inline void feature_surface(
	Map* map, long seed,
	int start_x, int start_y,
	int chunk_size,
	FeatureSurface feature
) {
	for (int y = start_y; y < start_y + chunk_size; y++)
	for (int x = start_x; x < start_x + chunk_size; x++) {
		if (is_air(map, x, y))
			continue;
		for (int d = 1; d <= feature.depth; d++) {
			if (!is_air(map, x, y - d))
				continue;
			set_if_solid(map, x, y, feature.tile);
			break;
		}

	}
	fflush(stdout);
}

void feature_apply(Map* map, long seed, int x, int y, int chunk_size, Feature feature) {
	switch (feature.kind) {
	case FEATURE_NONE: break;
	case FEATURE_VEIN: feature_vein(map, seed, x, y, chunk_size, feature.as.vein); break;
	case FEATURE_SURFACE: feature_surface(map, seed, x, y, chunk_size, feature.as.surface); break;
	case FEATURE_BLOB: feature_blob(map, seed, x, y, chunk_size, feature.as.blob); break;
	default:
		printf("UNDEFINED FEATURE %d\n", feature.kind);
		break;
	}
}


