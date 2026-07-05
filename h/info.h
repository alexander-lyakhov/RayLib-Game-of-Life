#ifndef INFO_H_
#define INFO_H_

#include "app.h"
#include "status-bar.h"

#define INFO_LINE_HEIGHT 40
#define INFO_FONT_SIZE 20

void info_render(App *app);

// DEBUG
// -----------------------------------------------------------------------------
// #define INFO_IMPLEMENTATION
// -----------------------------------------------------------------------------

#ifdef INFO_IMPLEMENTATION
// =============================================================================
// @@@ + info_render
// =============================================================================
void info_render(App *app)
{
	const char *info[] = {
		"-- Debug Info --",
		TextFormat("Resolution: %zu x %zu px", app->width, app->height),
		TextFormat("Key delay: %.2f", app->key_delay),
		TextFormat("Speed: %.f%%", app->speed),
		TextFormat("Cells %zu", app->cell_count),
	};

	int size = sizeof(info) / sizeof(*info);

	DrawRectangle(
		0,
		STATUS_BAR_HEIGHT,
		app->width,
		size * INFO_LINE_HEIGHT,
		(Color) {0, 0, 0, 128}
	);

	for (size_t i = 0; i < size; i++)
	{
		DrawText(
			info[i],
			10,
			10 + STATUS_BAR_HEIGHT + i * INFO_LINE_HEIGHT,
			INFO_FONT_SIZE,
			app->text_color
		);
		// if (i) DrawLine(10, i * INFO_LINE_HEIGHT, width - 10, i * INFO_LINE_HEIGHT, (Color){160, 160, 160, 255});
	}
}

#endif
#endif
