#ifndef STATUS_BAR_H_
#define STATUS_BAR_H_

#include "app.h"

#define STATUS_BAR_HEIGHT 40
#define STATUS_BAR_FONT_SEZE 20

void statusbar_render(App *app);

// DEBUG
// -----------------------------------------------------------------------------
// #define STATUS_BAR_IMPLEMENTATION
// -----------------------------------------------------------------------------

#ifdef STATUS_BAR_IMPLEMENTATION
// =============================================================================
// @@@ + statusbar_render
// =============================================================================
void statusbar_render(App *app)
{
	DrawRectangle(
		0,
		0,
		app->width,
		STATUS_BAR_HEIGHT - 1,
		app->grid_color
	);
	
	DrawLine(
		0,
		STATUS_BAR_HEIGHT - 1,
		app->width,
		STATUS_BAR_HEIGHT - 1,
		app->cell_color
	);

	const char *status_speed_text = TextFormat("| Speed %.f %%", app->speed);
	const char *status_right_text = TextFormat("Cells: %zu | Resolution: %d x %d px", app->cell_count, app->width, app->height);
	
	int status_right_text_width = MeasureText(status_right_text, STATUS_BAR_FONT_SEZE);

	DrawText(
		TextFormat("%s %s",
			app->is_running ? "Running" : "Pause",
			app->is_running ? status_speed_text : "" 
		),
		10, 10, STATUS_BAR_FONT_SEZE, app->text_color
	);

	DrawText(
		TextFormat("%s", status_right_text),
		app->width - status_right_text_width - 10, 10, STATUS_BAR_FONT_SEZE, app->text_color
	);
}

#endif
#endif
