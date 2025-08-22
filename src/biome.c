#include "biome.h"
#include "tile.h"
#include "feature.h"

#define FEAT_MOSS makeFeatureVein(.tile = TILE_MOSS, .frequency = 0.6, .size_min = 32, .size_max = 64)

#define FEAT_GRASS makeFeatureSurface(.tile = TILE_GRASS, .depth = 1)
#define FEAT_DIRT makeFeatureSurface(.tile = TILE_DIRT, .depth = 3)
#define FEAT_DIRT_PATCH makeFeatureBlob(.tile = TILE_DIRT, .frequency = 0.3, .size_min = 1, .size_max = 12)
#define FEAT_DIRT_PATCH makeFeatureBlob(.tile = TILE_DIRT, .frequency = 0.3, .size_min = 1, .size_max = 12)


#define FEAT_COAL makeFeatureVein(.tile = TILE_COAL, .frequency = 0.4, .size_min = 4, .size_max = 10)
#define FEAT_IRON makeFeatureVein(.tile = TILE_IRON, .frequency = 0.4, .size_min = 3, .size_max = 5)
#define FEAT_DIAMOND makeFeatureVein(.tile = TILE_DIAMOND, .frequency = 0.2, .size_min = 3, .size_max = 5)

#define FEAT_GEM(_tile) makeFeatureVein(.tile = (_tile), .frequency = 0.7, .size_min = 3, .size_max = 5)

BiomeDefinition biome_definitions[] = {
	[BIOME_PLAINS] = {
		.key = "PLAINS",
		.solid = TILE_STONE,
		.features = {
			FEAT_COAL, FEAT_IRON, FEAT_DIAMOND,
			FEAT_DIRT_PATCH,
			FEAT_MOSS,
			FEAT_DIRT, FEAT_GRASS,
		},
		.openness = -0.2,
	},
	[BIOME_FUNGAL] = {
		.key = "FUNGAL",
		.solid = TILE_FUNGAL,
		.features = {
			makeFeatureBlob(.tile = TILE_FUNGAL_MOSS, .frequency = 0.8, .size_min = 5, .size_max = 12),
			FEAT_COAL,
			makeFeatureSurface(.tile=TILE_FUNGAL_MOSS, .depth=2),
			makeFeatureTree(
				.cap=TILE_FUNGAL_CAP, .trunk=TILE_FUNGAL_TRUNK,
				.height_min = 3, .height_max = 7,
				.width_min = 2, .width_max = 4,
			)
		},
		.openness = -0.1,
	},
	[BIOME_CRYSTAL] = {
		.key = "CRYSTAL",
		.solid = TILE_CRYSTAL,
		.features = {
			FEAT_DIAMOND,
			FEAT_GEM(TILE_EMERALD),
			FEAT_GEM(TILE_AMETHYST),
			FEAT_GEM(TILE_RUBY),
		},
		.openness = -0.4,
	},
	[BIOME_MAGMA] = {
		.key = "MAGMA",
		.solid = TILE_MAGMA,
		.features = {
			FEAT_COAL, FEAT_IRON, FEAT_DIAMOND,
		},
		.openness = -0.4,
	},
	[BIOME_ASH_FOREST] = {
		.key = "ASH_FOREST",
		.solid = TILE_CHARCOAL,
		.features = {
			FEAT_COAL, FEAT_IRON, FEAT_DIAMOND,
		},
		.openness = 0.05,
	},
	[BIOME_GAS] = {
		.key = "GAS",
		.solid = TILE_STONE,
		.openness = -0.4,
	},
	[BIOME_ICE] = {
		.key = "ICE",
		.solid = TILE_ICE,
		.features = {
			FEAT_COAL, FEAT_IRON, FEAT_DIAMOND,
			makeFeatureSurface(.tile=TILE_SNOW),
			makeFeatureBlob(.tile = TILE_SNOW, .frequency = 0.9, .size_min = 4, .size_max = 12)
		},
		.openness = -0.25,
	},
	[BIOME_VOID] = {
		.key = "VOID",
		.solid = TILE_STONE,
		.openness = -0.2,
	},
	[BIOME_HELLSCAPE] = {
		.key = "HELLSCAPE",
		.solid = TILE_STONE,
		.openness = -0.6,
	},
};
