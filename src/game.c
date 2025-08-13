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

int pass_buttons[10] = {
	KEY_ONE,
	KEY_TWO,
	KEY_THREE,
	KEY_FOUR,
	KEY_FIVE,
	KEY_SIX,
	KEY_SEVEN,
	KEY_EIGHT,
	KEY_NINE,
};


void game_update() {
	float dt = GetFrameTime();
	bool dirty = false;
	for (int pass_idx = 0; pass_idx < 10; pass_idx++) {
		if (IsKeyPressed(pass_buttons[pass_idx])) {
			printf("PASS: %s\n", noise_passes[pass_idx].key);
			fflush(stdout);
			game.selected_pass = pass_idx;
		}
	}

	if (IsKeyDown(KEY_RIGHT)) {
		noise_passes[game.selected_pass].period += 0.01;
		dirty = true;
	}
	if (IsKeyDown(KEY_LEFT)) {
		noise_passes[game.selected_pass].period -= 0.01;
		dirty = true;
	}
	if (IsKeyDown(KEY_UP)) {
		noise_passes[game.selected_pass].threshold += 0.01;
		dirty = true;
	}
	if (IsKeyDown(KEY_DOWN)) {
		noise_passes[game.selected_pass].threshold -= 0.01;
		dirty = true;
	}
	if (IsKeyDown(KEY_ENTER)) {
		game.seed = rand();
		dirty = true;
	}
	if (
		IsKeyReleased(KEY_RIGHT) || 
		IsKeyReleased(KEY_LEFT) || 
		IsKeyReleased(KEY_UP) || 
		IsKeyReleased(KEY_DOWN)
	) {
		printf(
			"[%s]: threshold: %f, period: %f\n", 
			noise_passes[game.selected_pass].key,
			noise_passes[game.selected_pass].threshold,
			noise_passes[game.selected_pass].period
		);
		fflush(stdout);
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
	EndTextureMode();
}
