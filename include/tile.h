#ifndef TILE_H
#define TILE_H

#include "display.h"


typedef enum {
	TILE_FLOOR,
	TILE_BEDROCK,
	TILE_DIRT,
	TILE_STONE,
	TILE_COAL,
	TILE_IRON,
	TILE_DIAMOND,
	TILE_SIZE,
} Tile;


extern Glyph tile_glyphs[];


#endif // TILE_H
