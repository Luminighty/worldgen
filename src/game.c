#include "game.h"
#include "display.h"
#include "worldgen.h"
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

Game game = {};
RenderTexture2D render_target;

static void regen_world() {
	game.map = map_create();
	worldgen_generate(&game.map, game.seed);
	map_draw_border(&game.map);
}

void game_init() {
	render_target = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
	regen_world();
}

void game_update() {
	bool dirty = false;
	if (IsKeyDown(KEY_ENTER)) {
		game.seed = rand();
		dirty = true;
	}
	if (dirty)
		regen_world();
}

static const Rectangle RENDER_SOURCE = {
	.x = 0, .y = 0, .width = WINDOW_WIDTH, .height = -WINDOW_HEIGHT
};
static const Rectangle RENDER_DEST = {
	.x = 0, .y = 0, .width = TARGET_WINDOW_WIDTH, .height = TARGET_WINDOW_HEIGHT
};
void game_draw() {
	display_clear();
	map_render(&game.map);

	BeginTextureMode(render_target);
	display_render();
	EndTextureMode();

	DrawTexturePro(render_target.texture, RENDER_SOURCE, RENDER_DEST, (Vector2){0}, 0, WHITE);
	if (worldgen_debug()) {
		regen_world();
	}
}
