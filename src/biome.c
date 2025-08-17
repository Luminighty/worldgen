#include "biome.h"
#include "tile.h"


BiomeDefinition biome_definitions[] = {
	[BIOME_PLAINS] = {
		.key = "PLAINS",
		.palette =   {TILE_STONE, TILE_DIRT, TILE_MOSS },
		.palette_weights = { 0.7,       0.05,  0.25 },
		.palette_size = 3,
		.openness = -0.2,
	},
	[BIOME_FUNGAL] = {
		.key = "FUNGAL",
		.palette =   {TILE_FUNGAL, TILE_FUNGAL_MOSS, TILE_MOSS, TILE_DIRT, },
		.palette_weights = { 0.3,  0.3,   0.2,   0.2, },
		.palette_size = 4,
		.openness = -0.1,
	},
	[BIOME_CRYSTAL] = {
		.key = "CRYSTAL",
		.palette =   {TILE_STONE, TILE_CRYSTAL, TILE_EMERALD, TILE_AMETHYST, TILE_RUBY },
		.palette_weights = { 0.2,          0.4,  0.2,   0.1,   0.1  },
		.palette_size = 5,
		.openness = -0.4,
	},
	[BIOME_MAGMA] = {
		.key = "MAGMA",
		.palette =   {TILE_STONE, TILE_MAGMA, TILE_HELLSTONE, },
		.palette_weights = { 0.35,       0.5,           0.1  },
		.palette_size = 3,
		.openness = -0.4,
	},
	[BIOME_ASH_FOREST] = {
		.key = "ASH_FOREST",
		.palette =   {TILE_STONE, TILE_CHARCOAL, TILE_COAL, },
		.palette_weights = { 0.2,           0.5,       0.3,  },
		.palette_size = 3,
		.openness = 0.05,
	},
	[BIOME_GAS] = {
		.key = "GAS",
		.palette =   {TILE_STONE, TILE_STEAM, TILE_MAGMA },
		.palette_weights = { 0.6,       0.3,  0.1  },
		.palette_size = 3,
		.openness = -0.4,
	},
	[BIOME_ICE] = {
		.key = "ICE",
		.palette = {TILE_STONE, TILE_ICE, TILE_SNOW },
		.palette_weights = { 0.3,      0.5, 0.2},
		.palette_size = 3,
		.openness = -0.25,
	},
	[BIOME_VOID] = {
		.key = "VOID",
		.palette = {TILE_STONE, TILE_VOIDSTONE, TILE_VOID },
		.palette_weights = { 0.5, 0.3, 0.2},
		.palette_size = 3,
		.openness = -0.2,
	},
	[BIOME_HELLSCAPE] = {
		.key = "HELLSCAPE",
		.palette = {TILE_STONE, TILE_MAGMA, TILE_HELLSTONE },
		.palette_weights = { 0.2, 0.3, 0.5},
		.palette_size = 3,
		.openness = -0.6,
	},
};
