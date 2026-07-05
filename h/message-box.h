#ifndef MESSAGE_BOX_H_
#define MESSAGE_BOX_H_

#include "raylib.h"

typedef struct {
	Font font;
	float font_size;
	const char *text;
	
	Vector2 text_dim;
	Vector2 text_pos;
	Vector2 padding;
	Vector2 bounds_dim;
	Vector2 bounds_pos;

	Rectangle bounds;
	
	Color text_color;
	Color back_color;
	Color border_color;

} MessageBox;

void msgbox_init(MessageBox *msgbox);
void msgbox_text_color(MessageBox *msgbox, Color color);
void msgbox_background_color(MessageBox *msgbox, Color color);
void msgbox_border_color(MessageBox *msgbox, Color color);
void msgbox_draw(MessageBox *msgbox, size_t width, size_t height);

// DEBUG
// -----------------------------------------------------------------------------
// #define MESSAGE_BOX_IMPLEMENTATION
// -----------------------------------------------------------------------------

#ifdef MESSAGE_BOX_IMPLEMENTATION
// =============================================================================
// @@@ + msgbox_init
// =============================================================================
void msgbox_init(MessageBox *msgbox)
{
	msgbox->font = GetFontDefault();
	
	msgbox->text_dim = MeasureTextEx(
		msgbox->font,
		msgbox->text,
		msgbox->font_size,
		msgbox->font_size * 0.1f
	);

	msgbox->text_color   = WHITE;
	msgbox->back_color   = (Color){0, 0, 0, 128};
	msgbox->border_color = WHITE;
}

// =============================================================================
// @@@ + msgbox_text_color
// =============================================================================
void msgbox_text_color(MessageBox *msgbox, Color color)
{
	msgbox->text_color = color;
}

// =============================================================================
// @@@ + msgbox_background_color
// =============================================================================
void msgbox_background_color(MessageBox *msgbox, Color color)
{
	msgbox->back_color = color;
}

// =============================================================================
// @@@ + msgbox_border_color
// =============================================================================
void msgbox_border_color(MessageBox *msgbox, Color color)
{
	msgbox->border_color = color;
}

// =============================================================================
// @@@ + msgbox_draw
// =============================================================================
void msgbox_draw(MessageBox *msgbox, size_t width, size_t height)
{
	msgbox->text_pos = (Vector2){
		(width - msgbox->text_dim.x) / 2,
		(height - msgbox->text_dim.y) / 2
	};

	msgbox->bounds_dim = (Vector2){
		msgbox->text_dim.x + msgbox->padding.x,
		msgbox->text_dim.y + msgbox->padding.y
	};

	msgbox->bounds_pos = (Vector2){
		(width - msgbox->bounds_dim.x) / 2,
		(height - msgbox->bounds_dim.y) / 2
	};

	msgbox->bounds = (Rectangle){
		msgbox->bounds_pos.x,
		msgbox->bounds_pos.y,
		msgbox->bounds_dim.x,
		msgbox->bounds_dim.y
	};

	// Draw background
	DrawRectangleV(
		msgbox->bounds_pos,
		msgbox->bounds_dim,
		msgbox->back_color
	);

	// Draw border
	DrawRectangleLinesEx(msgbox->bounds, 1, msgbox->border_color);
	
	// Draw message
	DrawText(
		msgbox->text,
		msgbox->text_pos.x,
		msgbox->text_pos.y,
		msgbox->font_size,
		msgbox->text_color
	);
}
#endif // end of BANNER_IMPLEMENTATION
#endif
