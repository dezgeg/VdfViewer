#include <GL/gl.h>
#include <SDL/SDL.h>
#include <math.h>
#include <stdbool.h>

#include "vector.h"
#include "constants.h"
#include "gui.h"

GuiState state;
// return shift_val, normal_val or alt_val depending on keyboard modifier keys pressed
static GLfloat get_modifier_value(GLfloat shift_val, GLfloat normal_val, GLfloat alt_val)
{
	SDLMod mod = SDL_GetModState();
	bool alt = mod & (KMOD_LALT | KMOD_RALT);
	bool shift = mod & (KMOD_LSHIFT | KMOD_RSHIFT);
	if((!alt && !shift) || (alt && shift))
		return normal_val;
	else if(alt)
		return alt_val;
	else
		return shift_val;
}

static void modify_setting(const char* name, GLfloat* setting, bool increment)
{
	*setting += (increment ? 1.0f : -1.0f) * get_modifier_value(0.1f, 1.0f, 10.0f);
	if(*setting < 0.0f)
		*setting = 0.0f;
	printf("%s: %g\n", name, *setting);
}

void handle_keypress(const SDL_keysym *keysym)
{
	switch (keysym->sym)
	{
		case SDLK_ESCAPE:
			exit(0);
			break;

		case SDLK_F1:
		case SDLK_F2:
			modify_setting("Scale", &state.scale, keysym->sym == SDLK_F1);
			break;

		case SDLK_F3:
		case SDLK_F4:
			modify_setting("Animation lerp", &state.animLerp, keysym->sym == SDLK_F3);
			break;

		case SDLK_F5:
			if (state.animIndex == 1)
				state.animIndex = state.numAnims;
			else
				state.animIndex--;
			printf("Anim index: %d\n", state.animIndex);
			break;
		case SDLK_F6:
			if (state.animIndex == state.numAnims)
				state.animIndex = 1;
			else
				state.animIndex++;
			printf("Anim index: %d\n", state.animIndex);
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
	GLfloat pos_delta = get_modifier_value(0.1f, 0.3f, 10.0f) * POS_DELTA;

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
