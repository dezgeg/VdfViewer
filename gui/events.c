#include <GL/gl.h>
#include <SDL/SDL.h>
#include <math.h>
#include <stdbool.h>

#include "vector.h"
#include "constants.h"
#include "gui.h"

GuiState state;
// return small, normal or large depending on keyboard modifier keys pressed
static GLfloat get_modifier_value(GLfloat small, GLfloat normal, GLfloat large)
{
	SDLMod mod = SDL_GetModState();
	bool alt = mod & (KMOD_LALT | KMOD_RALT);
	bool shift = mod & (KMOD_LSHIFT | KMOD_RSHIFT);
	if((!alt && !shift) || (alt && shift))
		return normal;
	else if(alt)
		return large;
	else
		return small;
}
static void modify_setting(const char* name, GLfloat* setting, bool increment)
{
	*setting += (increment ? 1.0f : -1.0f) * get_modifier_value(0.1f, 1.0f, 10.0f);
	if(*setting < 0.0f)
		*setting = 0.0f;
	printf("%s: %g\n", name, *setting);
}
static void unlock_planet(void)
{
	if(state.locked_planet >= 0)
	{
		vector_add(state.pos, state.pos, state.sys->planets[state.locked_planet].position);
		state.locked_planet = -1;
	}
}
void handle_keypress(const SDL_keysym *keysym)
{
	switch (keysym->sym)
	{
		case SDLK_ESCAPE:
			exit(0);
			break;
		case SDLK_p:
			state.paused = !state.paused;
			printf("Simulation %s\n", state.paused ? "paused" : "unpaused");
			break;
		case SDLK_t:
			state.trails_enabled = !state.trails_enabled;
			printf("Trails %s\n", state.trails_enabled ? "enabled" : "disabled");
			break;
		case SDLK_r:
			state.first_trail = 0;
			state.num_trails = 0;
			printf("Trails cleared\n");
			break;
		case SDLK_F1:
		case SDLK_F2:
			modify_setting("Planet scale", &state.scale, keysym->sym == SDLK_F1);
			break;
		case SDLK_F3:
		case SDLK_F4:
			modify_setting("Simulation speed (earth hours/sec)", &state.hours_per_sec, keysym->sym == SDLK_F3);
			break;
		case SDLK_0:
			unlock_planet();
			printf("Movement set to free-moving\n");
			break;
		case SDLK_TAB:
			state.pos[0] = 0.0f;
			state.pos[1] = get_planet_radius(state.locked_planet >= 0 ? state.locked_planet : 0);
			state.pos[2] = 0.0f;
			break;
			/*
		case SDLK_F12:
			state.orthographic = !state.orthographic;
			init_viewport();
			break;
			*/
		default:
			break;
	}
	if(keysym->sym >= SDLK_1 && keysym->sym <= SDLK_9)
	{
		int planet = keysym->sym - SDLK_1;
		if(planet >= state.sys->nplanets)
			return;
		unlock_planet();
		state.locked_planet = planet;
		vector_sub(state.pos, state.pos, state.sys->planets[planet].position);
		printf("Movement locked on to planet %d\n", planet + 1);
	}
}
// Rotate the viewport by x, y (in screen coordinates) degrees
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
// Move the camera by amt units forward/backward (if forward == true) or left/right
// The movement works similarly to noclip mode in FPSs
// i.e. A/D keys (strafing) move on the XZ plane and W/S keys move in the camera direction
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
	SDLMod mod = SDL_GetModState();
	GLfloat pos_delta = POS_DELTA * (mod & (KMOD_LALT | KMOD_RALT) ? 10.0f :
			mod & (KMOD_LSHIFT | KMOD_RSHIFT) ? 0.1f :
			1.0f);

	if(keys[SDLK_LEFT])
		rotate(-ROT_DELTA, 0.0);
	if(keys[SDLK_RIGHT])
		rotate(ROT_DELTA, 0.0);
	if(keys[SDLK_UP])
		rotate(0.0, -ROT_DELTA);
	if(keys[SDLK_DOWN])
		rotate(0.0, ROT_DELTA);

	if(keys[SDLK_a])
		move(-pos_delta, false);
	if(keys[SDLK_e] || keys[SDLK_d])
		move(pos_delta, false);
	if(keys[SDLK_COMMA] || keys[SDLK_w])
		move(-pos_delta, true);
	if(keys[SDLK_o] || keys[SDLK_s])
		move(pos_delta, true);
}
