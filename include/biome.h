#ifndef BIOME_H
#define BIOME_H

#include "tile.h"

#define MAX_PALETTE_SIZE 8


typedef struct {
    char key[32];

    // NOTE: Might need to add separate palettes, for now this is for solids
    Tile palette[MAX_PALETTE_SIZE];
    double palette_weights[MAX_PALETTE_SIZE];
    int palette_size;

    double openness;
} BiomeDefinition;

extern BiomeDefinition biome_definitions[];


typedef enum {
	BIOME_VOID,
	BIOME_HELLSCAPE,
	BIOME_MAGMA,
	BIOME_ASH_FOREST,
	BIOME_GAS,
	BIOME_FUNGAL,
	BIOME_PLAINS,
	BIOME_CRYSTAL,
	BIOME_ICE
} Biome;


#endif // BIOME_H
