#ifndef GUI_EVENTS_H
#define GUI_EVENTS_H

#include <SDL/SDL.h>
#include "simulate.h"
#include "vector.h"

typedef struct GuiState
{
	System* sys;
	Vector pos;
	GLfloat rot_x, rot_y; /* rotation along screen x & y == world x & y */
} GuiState;

// events.c
extern GuiState state;
extern void handle_keypress(const SDL_keysym* keysym);
extern void handle_mouse(const SDL_MouseMotionEvent* mme);
extern void update(void);
// draw.c
extern void init_gl(void);
extern void draw_scene(void);
// guimain.c
extern SDL_Surface* surface;
#endif
