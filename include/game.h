#ifndef GAME_H
#define GAME_H

#include <raylib.h>

#include "map.h"

typedef struct {
	long seed;
	int selected_pass;
	Map map;
} Game;

void game_init();
void game_draw();
void game_update();

extern Game game;

#endif // GAME_H
