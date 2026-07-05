#ifndef APP_H_
#define APP_H_

#include "message-box.h"

typedef struct _Cell {
	short grid_x;
	short grid_y;
	char  state[3];
	unsigned long age;
	
} Cell;

typedef struct _App {
	Cell *cells;
	short cols;
	short rows;
	short cells_alive;
	short cell_count;
	short width;
	short height;
	char  cell_state_selector;
	char  cell_color_mode;

	bool is_complete;
	bool is_info_visible;
	bool is_running;
	bool is_started;

	Color grid_color;
	Color cell_color;
	Color text_color;
	Color bg_color;

	MessageBox msg_complete;
	MessageBox msg_play;

	double key_delay;
	float  speed;
	short  fps;

} App;

#endif
