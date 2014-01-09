#ifndef GUI_EVENTS_H
#define GUI_EVENTS_H

#include <SDL/SDL.h>
#include <stdbool.h>
#include "constants.h"
#include "vector.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct GuiState
{
	Vector pos;
	GLfloat rot_x, rot_y; /* rotation along screen x & y == world x & y */
	GLfloat scale;

	int numFaces;
	Vector vertexes[1024];
	int faces[1024][3];
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
extern void init_viewport(void);
extern SDL_Surface* surface;
#endif
