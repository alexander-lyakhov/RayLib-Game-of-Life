#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "raylib.h"

#define MIN_FPS 5
#define MAX_FPS 100
#define TARGET_FPS 15

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 720

#define CELL_WIDTH 10
#define CELL_HEIGHT 10

#define KEY_DELAY 0.25

#define clamp(value, minvalue, maxvalue) fmax(fmin((value), (maxvalue)), (minvalue));

#define MESSAGE_BOX_IMPLEMENTATION
#include "h/message-box.h"

#define STATUS_BAR_IMPLEMENTATION
#include "h/status-bar.h"

#define INFO_IMPLEMENTATION
#include "h/info.h"

#include "h/app.h"

Color cell_colors[] = {
	(Color) { 255, 255, 255, 255},
	(Color) { 255, 255, 0, 255},
	(Color) { 255, 80, 80, 255},
	(Color) { 192, 0, 128, 255},
	(Color) { 0, 128, 128, 255},
};

// =============================================================================
// @@@ - App_init_cells
// =============================================================================
static void App_init_cells(App *app)
{
	// This part should be defined here because of possible app resolution changes
	//-------------------------------------------------
	int width  = GetRenderWidth();
	int height = GetRenderHeight() - STATUS_BAR_HEIGHT;

	app->width  = width;
	app->height = height;
	//-------------------------------------------------

	int cols = app->width / CELL_WIDTH;
	int rows = app->height / CELL_HEIGHT;

	size_t cell_count = cols * rows;

	app->cols       = cols;
	app->rows       = rows;
	app->cell_count = cell_count;
	
	if (!app->cells)
	{
		app->cells = malloc(cell_count * sizeof(Cell));
		// printf("cells malloc: %zu bytes\n", cell_count * sizeof(Cell));
	}

	for (int index = 0, y = 0; y < rows; y++)
	{
		for (int x = 0; x < cols; x++)
		{
			char state = GetRandomValue(0, 100) < 25 ? 1 : 0;

			app->cells[index++] = (Cell) {
				.grid_x = x,
				.grid_y = y,
				.state[0] = state,
				.state[1] = state,
				.age = 0,
			};
		}
	}
}

// =============================================================================
// @@@ + App_init
// =============================================================================
void App_init(App *app)
{
	SetTraceLogLevel(LOG_NONE);
	
	SetTargetFPS(TARGET_FPS);

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Conway's game of life");

	app->cell_state_selector  = 0;

	app->grid_color = (Color){ 48,  48,  48, 255};
	app->cell_color = (Color){224, 160,   0, 255};
	app->text_color = (Color){224, 224, 224, 255};
	app->bg_color   = BLACK;

	app->is_complete     = 0;
	app->is_info_visible = 0;
	app->is_running      = 0;

	app->key_delay = 0;
	app->speed     = ((float)TARGET_FPS / MAX_FPS) * 100.0f;
	app->fps       = TARGET_FPS;

	App_init_cells(app);

	// Init MessageBox
	app->msg_complete = (MessageBox){
		.font_size = 30,
		.text      = "Similation complete",
		.padding   = { 128, 64},
	};
	app->msg_play = (MessageBox){
		.font_size = 30,
		.text      = "Press Space to start",
		.padding   = { 128, 64},
	};

	msgbox_init(&app->msg_complete);
	msgbox_init(&app->msg_play);
}

// =============================================================================
// @@@ + App_reset
// =============================================================================
void App_reset(App *app)
{
	app->is_info_visible = 0;
	app->is_complete = 0;
	
	App_init_cells(app);
}

// =============================================================================
// @@@ + App_destroy
// =============================================================================
void App_destroy(App *app)
{
	free(app->cells);
	app->cells = NULL;

	// printf("cells free\n");
}

// =============================================================================
// @@@ + App_check_complete
// =============================================================================
static void App_check_complete(App *app)
{
	for (size_t i = 0; i < app->cell_count; i++)
	{
		if (app->cells[i].state[2] != app->cells[i].state[app->cell_state_selector])
			return;
	}
	app->is_complete = 1;
}

// =============================================================================
// @@@ + App_update
// =============================================================================
void App_update(App *app)
{
	app->cells_alive = 0;

	for (int index = 0; index < app->cell_count; index++)
	{
		int row = index / app->cols;
		int col = index % app->cols;
		int n = 0;

		Cell *cell = &app->cells[index];
		char *state = app->cells[index].state;

		//
		// check area around current cell
		//
		for (int y = row - 1; y <= row + 1; y++)
		{
			if (y < 0 || y > app->rows - 1)
				continue;

			for (int x = col - 1; x <= col + 1; x++)
			{
				if (x < 0 || x > app->cols - 1)
					continue;
				
				if (y == row && x == col)
					continue;
				
				int check_index = y * app->cols + x;

				if (app->cells[check_index].state[app->cell_state_selector]) n++;
			}
		}

		state[2] = state[app->cell_state_selector ^ 1];
		
		switch(n) {
			// no changes
			case 2:
				state[app->cell_state_selector ^ 1] = state[app->cell_state_selector];
				cell->age++;
				break;
				
			// New cell is borning
			case 3:
				state[app->cell_state_selector ^ 1] = 1;
				break;
				
			// Cell dies (n < 2 or n > 3)
			default:
				state[app->cell_state_selector ^ 1] = 0;
				cell->age = 0;
		}

		app->cells_alive += state[app->cell_state_selector ^ 1];
	}

	app->cell_state_selector ^= 1;
	App_check_complete(app);
}

// =============================================================================
// @@@ + App_render
// =============================================================================
void App_render(App *app)
{
	ClearBackground(app->bg_color);
	BeginDrawing();

	statusbar_render(app);

	Color color;
	size_t cell_colors_count = sizeof(cell_colors) / sizeof(*cell_colors);

	for (int i = 0; i < app->cell_count; i++)
	{
		Cell *cell = &app->cells[i];

		// Choose cell color
		if (app->cell_color_mode)
		{
			color = cell->state[app->cell_state_selector]
				? cell->age > cell_colors_count - 1
					? cell_colors[cell_colors_count - 1]
					: cell_colors[cell->age]
				:app->grid_color;
		}
		else
		{
			color = cell->state[app->cell_state_selector]
				? app->cell_color
				: app->grid_color;
		}
			
		// Draw cell
		DrawRectangle(
			CELL_WIDTH  * cell->grid_x + 1,
			CELL_HEIGHT * cell->grid_y + 1 + STATUS_BAR_HEIGHT,

			CELL_WIDTH  - 2,
			CELL_HEIGHT - 2,

			color
		);
	}

	if (app->is_info_visible)
		info_render(app);

	if (app->is_complete)
	{
		app->is_info_visible = 0;
		app->is_running = 0;
		msgbox_draw(&app->msg_complete, app->width, app->height);
	}

	if (!app->is_started)
		msgbox_draw(&app->msg_play, app->width, app->height);

	EndDrawing();
}

// =============================================================================
// @@@ + App_listen
// =============================================================================
int App_listen(App *app)
{
	float delta_time = GetFrameTime();

	if (app->key_delay < KEY_DELAY)
	{
		app->key_delay += delta_time;
		return 1;
	}

	// 'app->key_delay' should not be less then delta_time
	if (app->key_delay < delta_time)
		app->key_delay = delta_time;

	// @@@ --> Quit
	if (IsKeyDown(KEY_Q))
	{
		app->key_delay = 0;
		return 0;
	}
		
	// @@@ --> Show debug Info
	if (IsKeyDown(KEY_I))
	{
		app->is_info_visible ^= 1;
		app->key_delay = 0;

		return 1;
	}
		
	// @@@ --> Reset
	if (IsKeyDown(KEY_R))
	{
		App_reset(app);

		app->key_delay = 0;
		app->is_running = 1;

		return 1;
	}

	// @@@ --> Play / Pause
	if (IsKeyDown(KEY_SPACE))
	{
		app->is_running ^= 1;
		app->is_started = 1;
		app->key_delay = 0;

		return 1;
	}

	// @@@ --> Switch between monochrome and color mode
	if (IsKeyDown(KEY_C) && app->is_started)
	{
		app->cell_color_mode ^= 1;
		app->key_delay = 0;

		return 1;
	}

	// @@@ --> One step forward
	if (IsKeyDown(KEY_N) && !app->is_running && app->is_started)
	{
		App_update(app);
		App_render(app);

		app->key_delay = 0;

		return 1;
	}

	// @@@ --> Toggle fullscreen
	if (IsKeyDown(KEY_F))
	{
		ToggleFullscreen();
		App_destroy(app);
		App_reset(app);

		return 1;
	}
	
	// @@@ --> Speed control
	float delta_mouse_wheel = GetMouseWheelMove();

	if (delta_mouse_wheel && app->is_running || app->is_info_visible)
	{
		app->fps += delta_mouse_wheel * 5;
		app->fps = clamp(app->fps, MIN_FPS, MAX_FPS);

		app->speed = (app->fps / (float)MAX_FPS) * 100.0f;
		SetTargetFPS(app->fps);
	}

	return 1;
}

// =============================================================================
// @@@ + main
// =============================================================================
int main()
{	
	App app = { 0 };
	App_init(&app);

	if (!IsWindowReady())
		return 0;
	/*
	Image imageCellEmpty = GenImageColor(CELL_WIDTH * 2 - 2, CELL_HEIGHT * 2 - 2, app.grid_color);
	Image imageCellAlive = GenImageColor(CELL_WIDTH * 2 - 2, CELL_HEIGHT * 2 - 2, app.cell_color);

	Texture2D textureCellEmpty = LoadTextureFromImage(imageCellEmpty);
	Texture2D textureCellAlive = LoadTextureFromImage(imageCellAlive);
	*/
	BeginDrawing();
		ClearBackground(app.bg_color);
	EndDrawing();

	// Game loop
	// --------------------------------------------
	while(!WindowShouldClose() && App_listen(&app))
	{
		if (app.is_running && app.is_started)
			App_update(&app);

		App_render(&app);
	}
	// --------------------------------------------

	App_destroy(&app);
	/*
	UnloadTexture(textureCellEmpty);
	UnloadTexture(textureCellAlive);

	UnloadImage(imageCellEmpty);
	UnloadImage(imageCellAlive);
	*/
	CloseWindow();

	return 0;
}
