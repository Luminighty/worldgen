#ifndef BIOME_H
#define BIOME_H

#include "tile.h"
#include "feature.h"

#define MAX_FEATURE_SIZE 32

typedef struct {
    char key[32];
    Tile solid;

    Feature features[MAX_FEATURE_SIZE];

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
	BIOME_ICE,
	BIOME_SIZE,
} Biome;


#endif // BIOME_H
