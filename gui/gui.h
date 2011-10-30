#ifndef GUI_EVENTS_H
#define GUI_EVENTS_H

#include <SDL/SDL.h>
#include <stdbool.h>
#include "constants.h"
#include "simulate.h"
#include "vector.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct PlanetView
{
	GLfloat radius;
	Vector trails[MAX_TRAILS];
} PlanetView;
typedef struct GuiState
{
	System* sys;
	PlanetView* views;
	Vector pos;
	GLfloat rot_x, rot_y; /* rotation along screen x & y == world x & y */
	GLfloat scale;
	int locked_planet;
	GLfloat hours_per_sec;
	GLfloat time_step;
	bool paused;
	int first_trail;
	int num_trails;
	bool trails_enabled;
} GuiState;

// events.c
extern GuiState state;
extern void handle_keypress(const SDL_keysym* keysym);
extern void handle_mouse(const SDL_MouseMotionEvent* mme);
extern void update(void);
// draw.c
extern void init_gl(void);
extern void draw_scene(void);
extern GLfloat get_planet_radius(int planet);
// guimain.c
extern void init_viewport(void);
extern SDL_Surface* surface;
#endif
