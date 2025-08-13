#include "map.h"
#include "display.h"
#include "game.h"
#include "tile.h"
#include "palette.h"


#define foreach(x, y)\
	for(int y = 0; y < MAP_HEIGHT; y++)\
	for(int x = 0; x < MAP_WIDTH; x++)


Map map_create() {
	Map map = {0};
	return map;
}


void map_render(Map *map) {
	foreach(x, y) {
		Tile t = map->tiles[y][x];
		Glyph g = tile_glyphs[t];
		display_putchar(x, y, g, 1);
	}
}


#define draw_line(map, tile, from_x, from_y, condition, step)\
	for (int x = (from_x), y = (from_y); (condition); (step))\
		map->tiles[y][x] = tile;

#define fill(map, tile, from_x, from_y, to_x, to_y)\
	for (int y = (from_y); y < (to_y); y++)\
	for (int x = (from_x); x < (to_x); x++)\
		map->tiles[y][x] = tile;


void map_draw_border(Map* map) {
	draw_line(map, TILE_BEDROCK, 0, 0, (x < MAP_WIDTH), (x++));
	draw_line(map, TILE_BEDROCK, 0, 0, (y < MAP_HEIGHT), (y++));
	draw_line(map, TILE_BEDROCK, 0, MAP_HEIGHT - 1 , (x < MAP_WIDTH), (x++));
	draw_line(map, TILE_BEDROCK, MAP_WIDTH - 1, 0, (y < MAP_HEIGHT), (y++));
}

#undef fill
#undef draw_line

Tile map_get(Map *map, int x, int y) {
	return map->tiles[y][x];
}

