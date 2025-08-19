#include "game.h"
#include "config.h"
#include "display.h"
#include "map.h"
#include "worldgen.h"
#include <raylib.h>
#include <stdbool.h>
#include "raygui.h"

Game game = {0};
RenderTexture2D render_target;

static void regen_world() {
	worldgen_generate(game.map, game.seed);
	map_draw_border(game.map);
	GuiLoadStyle("assets/raygui_style_terminal.rgs");
}

void game_init() {
	game.map = map_create();
	game.x = (MAP_WIDTH - SCREEN_WIDTH) / 2;
	game.y = (MAP_HEIGHT - SCREEN_HEIGHT) / 2;
	render_target = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
	regen_world();
}

void game_destroy() {
	map_destroy(game.map);

}

void game_update() {
	bool dirty = false;
	if (IsKeyReleased(KEY_ENTER)) {
		game.seed++;
		dirty = true;
	}
	static const int STEP = 4;
	if (IsKeyDown(KEY_LEFT) && game.x > 0)
		game.x -= STEP;
	if (IsKeyDown(KEY_RIGHT) && game.x < MAP_WIDTH - SCREEN_WIDTH)
		game.x += STEP;
	if (IsKeyDown(KEY_UP) && game.y > 0)
		game.y -= STEP;
	if (IsKeyDown(KEY_DOWN) && game.y < MAP_HEIGHT - SCREEN_HEIGHT)
		game.y += STEP;

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
	map_render(game.map);

	BeginTextureMode(render_target);
	display_render();
	EndTextureMode();

	DrawTexturePro(render_target.texture, RENDER_SOURCE, RENDER_DEST, (Vector2){0}, 0, WHITE);
	if (worldgen_debug()) {
		regen_world();
	}
}
