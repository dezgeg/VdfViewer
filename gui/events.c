#include <GL/gl.h>
#include <SDL/SDL.h>
#include <math.h>
#include <stdbool.h>

#include "vector.h"
#include "gui.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

GuiState state;
static const GLfloat ROT_DELTA = 1.0;
static const GLfloat POS_DELTA = 1.0e9;
static const GLfloat MOUSE_X_DELTA = 0.2;
static const GLfloat MOUSE_Y_DELTA = 0.2;

void handle_keypress(const SDL_keysym *keysym)
{
	switch (keysym->sym)
	{
		case SDLK_ESCAPE:
			exit(0);
			break;
		case SDLK_F1:
			SDL_WM_ToggleFullScreen(surface);
			break;
		default:
			break;
	}
}
static void rotate(GLfloat x, GLfloat y)
{
	state.rot_x += x;
	if(state.rot_x > 360.0)
		state.rot_x -= 360.0;
	else if(state.rot_x < 0.0)
		state.rot_x += 360.0;

	state.rot_y += y;
	if(state.rot_y > 90.0)
		state.rot_y = 90.0;
	if(state.rot_y < -90.0)
		state.rot_y = -90.0;
}
static void move(GLfloat amt, bool forward)
{
	const GLfloat DEG2RAD = M_PI / 180.0;
	GLfloat cx = cos(state.rot_x * DEG2RAD);
	GLfloat sx = sin(state.rot_x * DEG2RAD);
	GLfloat cy = cos(state.rot_y * DEG2RAD);
	GLfloat sy = sin(state.rot_y * DEG2RAD);

	GLfloat dx = -cy * sx * amt;
	GLfloat dy = sy * amt;
	GLfloat dz = cx * cy * amt;
	if(forward)
	{
		state.pos[0] += dx;
		state.pos[1] += dy;
		state.pos[2] += dz;
	}
	else
	{
		state.pos[0] += cx * amt;
		state.pos[2] += sx * amt;
	}
}
void handle_mouse(const SDL_MouseMotionEvent* mme)
{
	rotate(mme->xrel * MOUSE_X_DELTA, mme->yrel * MOUSE_Y_DELTA);
}
void update(void)
{
	Uint8* keys = SDL_GetKeyState(0);

	if(keys[SDLK_LEFT])
		rotate(-ROT_DELTA, 0.0);
	if(keys[SDLK_RIGHT])
		rotate(ROT_DELTA, 0.0);
	if(keys[SDLK_UP])
		rotate(0.0, -ROT_DELTA);
	if(keys[SDLK_DOWN])
		rotate(0.0, ROT_DELTA);

	if(keys[SDLK_a])
		move(-POS_DELTA, false);
	if(keys[SDLK_e] || keys[SDLK_d])
		move(POS_DELTA, false);
	if(keys[SDLK_COMMA] || keys[SDLK_w])
		move(-POS_DELTA, true);
	if(keys[SDLK_o] || keys[SDLK_s])
		move(POS_DELTA, true);
}
