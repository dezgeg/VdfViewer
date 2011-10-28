#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <math.h>
#include <stdbool.h>

#include "vector.h"
#include "events.h"

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

struct State
{
	Vector pos;
	GLfloat rot_x, rot_y; /* rotation along screen x & y == world x & y */
} state = { { 0.0, 0.0, 0.0 }, 0.0, 0.0 };

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
static const GLfloat MOUSE_X_DELTA = 0.2;
static const GLfloat MOUSE_Y_DELTA = 0.2;
void handle_mouse(const SDL_MouseMotionEvent* mme)
{
	rotate(mme->xrel * MOUSE_X_DELTA, mme->yrel * MOUSE_Y_DELTA);
}
static const GLfloat ROT_DELTA = 1.0;
static const GLfloat POS_DELTA = 0.1;
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

void drawGLScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glRotatef(state.rot_y, 1.0, 0.0, 0.0);
	glRotatef(state.rot_x, 0.0, 1.0, 0.0);
	glTranslatef(-state.pos[0], -state.pos[1], -state.pos[2]);
	glPushMatrix();

	glTranslatef(-1.5f, 0.0f, -6.0f);

	glBegin(GL_TRIANGLES);             /* Drawing Using Triangles       */
	glColor3f(  1.0f,  0.0f,  0.0f); /* Red                           */
	glVertex3f( 0.0f,  1.0f,  0.0f); /* Top Of Triangle (Front)       */
	glColor3f(  0.0f,  1.0f,  0.0f); /* Green                         */
	glVertex3f(-1.0f, -1.0f,  1.0f); /* Left Of Triangle (Front)      */
	glColor3f(  0.0f,  0.0f,  1.0f); /* Blue                          */
	glVertex3f( 1.0f, -1.0f,  1.0f); /* Right Of Triangle (Front)     */

	glColor3f(  1.0f,  0.0f,  0.0f); /* Red                           */
	glVertex3f( 0.0f,  1.0f,  0.0f); /* Top Of Triangle (Right)       */
	glColor3f(  0.0f,  0.0f,  1.0f); /* Blue                          */
	glVertex3f( 1.0f, -1.0f,  1.0f); /* Left Of Triangle (Right)      */
	glColor3f(  0.0f,  1.0f,  0.0f); /* Green                         */
	glVertex3f( 1.0f, -1.0f, -1.0f); /* Right Of Triangle (Right)     */

	glColor3f(  1.0f,  0.0f,  0.0f); /* Red                           */
	glVertex3f( 0.0f,  1.0f,  0.0f); /* Top Of Triangle (Back)        */
	glColor3f(  0.0f,  1.0f,  0.0f); /* Green                         */
	glVertex3f( 1.0f, -1.0f, -1.0f); /* Left Of Triangle (Back)       */
	glColor3f(  0.0f,  0.0f,  1.0f); /* Blue                          */
	glVertex3f(-1.0f, -1.0f, -1.0f); /* Right Of Triangle (Back)      */

	glColor3f(  1.0f,  0.0f,  0.0f); /* Red                           */
	glVertex3f( 0.0f,  1.0f,  0.0f); /* Top Of Triangle (Left)        */
	glColor3f(  0.0f,  0.0f,  1.0f); /* Blue                          */
	glVertex3f(-1.0f, -1.0f, -1.0f); /* Left Of Triangle (Left)       */
	glColor3f(  0.0f,  1.0f,  0.0f); /* Green                         */
	glVertex3f(-1.0f, -1.0f,  1.0f); /* Right Of Triangle (Left)      */
	glEnd();                            /* Finished Drawing The Triangle */

	glPopMatrix();
	glPushMatrix();

	glTranslatef(1.5f, 0.0f, -6.0f);

	glColor3f(0.5f, 0.5f, 1.0f);

	glBegin(GL_QUADS);                 /* Draw A Quad                      */
	glNormal3f(0.0f, 1.0f, 0.0f);
	glColor3f(  0.0f,  1.0f,  0.0f); /* Set The Color To Green           */
	glVertex3f( 1.0f,  1.0f, -1.0f); /* Top Right Of The Quad (Top)      */
	glVertex3f(-1.0f,  1.0f, -1.0f); /* Top Left Of The Quad (Top)       */
	glVertex3f(-1.0f,  1.0f,  1.0f); /* Bottom Left Of The Quad (Top)    */
	glVertex3f( 1.0f,  1.0f,  1.0f); /* Bottom Right Of The Quad (Top)   */

	glNormal3f(0.0f, -1.0f, 0.0f);
	glColor3f(  1.0f,  0.5f,  0.0f); /* Set The Color To Orange          */
	glVertex3f( 1.0f, -1.0f,  1.0f); /* Top Right Of The Quad (Botm)     */
	glVertex3f(-1.0f, -1.0f,  1.0f); /* Top Left Of The Quad (Botm)      */
	glVertex3f(-1.0f, -1.0f, -1.0f); /* Bottom Left Of The Quad (Botm)   */
	glVertex3f( 1.0f, -1.0f, -1.0f); /* Bottom Right Of The Quad (Botm)  */

	glNormal3f(0.0f, 0.0f, 1.0f);
	glColor3f(  1.0f,  0.0f,  0.0f); /* Set The Color To Red             */
	glVertex3f( 1.0f,  1.0f,  1.0f); /* Top Right Of The Quad (Front)    */
	glVertex3f(-1.0f,  1.0f,  1.0f); /* Top Left Of The Quad (Front)     */
	glVertex3f(-1.0f, -1.0f,  1.0f); /* Bottom Left Of The Quad (Front)  */
	glVertex3f( 1.0f, -1.0f,  1.0f); /* Bottom Right Of The Quad (Front) */

	glNormal3f(0.0f, 0.0f, -1.0f);
	glColor3f(  1.0f,  1.0f,  0.0f); /* Set The Color To Yellow          */
	glVertex3f( 1.0f, -1.0f, -1.0f); /* Bottom Left Of The Quad (Back)   */
	glVertex3f(-1.0f, -1.0f, -1.0f); /* Bottom Right Of The Quad (Back)  */
	glVertex3f(-1.0f,  1.0f, -1.0f); /* Top Right Of The Quad (Back)     */
	glVertex3f( 1.0f,  1.0f, -1.0f); /* Top Left Of The Quad (Back)      */

	glNormal3f(-1.0f, 0.0f, 0.0f);
	glColor3f(  0.0f,  0.0f,  1.0f); /* Set The Color To Blue            */
	glVertex3f(-1.0f,  1.0f,  1.0f); /* Top Right Of The Quad (Left)     */
	glVertex3f(-1.0f,  1.0f, -1.0f); /* Top Left Of The Quad (Left)      */
	glVertex3f(-1.0f, -1.0f, -1.0f); /* Bottom Left Of The Quad (Left)   */
	glVertex3f(-1.0f, -1.0f,  1.0f); /* Bottom Right Of The Quad (Left)  */

	glNormal3f(1.0f, 0.0f, 0.0f);
	glColor3f(  1.0f,  0.0f,  1.0f); /* Set The Color To Violet          */
	glVertex3f( 1.0f,  1.0f, -1.0f); /* Top Right Of The Quad (Right)    */
	glVertex3f( 1.0f,  1.0f,  1.0f); /* Top Left Of The Quad (Right)     */
	glVertex3f( 1.0f, -1.0f,  1.0f); /* Bottom Left Of The Quad (Right)  */
	glVertex3f( 1.0f, -1.0f, -1.0f); /* Bottom Right Of The Quad (Right) */
	glEnd();                            /* Done Drawing The Quad            */

	glPopMatrix();
	glTranslatef(0.0f, 2.0f, -6.0f);

	GLUquadricObj* qo = gluNewQuadric();
	glColor3f(1.0f, 1.0f, 1.0f);
	gluSphere(qo, 1.0, 32, 32);
	gluDeleteQuadric(qo);
}

