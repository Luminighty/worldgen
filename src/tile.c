#include "tile.h"
#include "display.h"
#include "palette.h"
#include <stdbool.h>


Glyph tile_glyphs[] = {
	[TILE_AIR] = glyph(0x00, PAL_BLACK, PAL_BLACK),
	[TILE_DIRT] = glyph(0xB1, PAL_BROWN, PAL_DARK_BROWN),
	[TILE_STONE] = glyph(0xDB, PAL_GRAY(0x69), PAL_GRAY(0x69)),
	[TILE_COAL] = glyph(0x0F, PAL_BLACK, PAL_GRAY(0x69)),
	[TILE_IRON] = glyph(0x9C, PAL_RED, PAL_GRAY(0x69)),
	[TILE_DIAMOND] = glyph(0x0F, PAL_BLUE, PAL_GRAY(0x69)),
	[TILE_BEDROCK] = glyph(0xB0, PAL_GRAY(0x20), PAL_BLACK),
};
