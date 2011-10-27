#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

#include "events.h"

struct State
{
	GLfloat x_pos, y_pos;
	GLfloat x_rot, y_rot;
} state = { 0.0, 0.0, 0.0, 0.0 };

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
	state.x_rot += x;
	if(state.x_rot > 360.0)
		state.x_rot -= 360.0;
	else if(state.x_rot < 0.0)
		state.x_rot += 360.0;

	state.y_rot += y;
	if(state.y_rot > 90.0)
		state.y_rot = 90.0;
	if(state.y_rot < -90.0)
		state.y_rot = -90.0;
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
			state.x_pos -= POS_DELTA;
	if(keys[SDLK_e] || keys[SDLK_d])
			state.x_pos += POS_DELTA;
	if(keys[SDLK_COMMA] || keys[SDLK_w])
			state.y_pos -= POS_DELTA;
	if(keys[SDLK_o] || keys[SDLK_s])
			state.y_pos += POS_DELTA;
}

void drawGLScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glRotatef(state.y_rot, 1.0, 0.0, 0.0);
	glRotatef(state.x_rot, 0.0, 1.0, 0.0);
	glTranslatef(-state.x_pos, 0, -state.y_pos);
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

	glTranslatef(1.5f, 0.0f, -6.0f);

	glColor3f(0.5f, 0.5f, 1.0f);

	glBegin(GL_QUADS);                 /* Draw A Quad                      */
	glColor3f(  0.0f,  1.0f,  0.0f); /* Set The Color To Green           */
	glVertex3f( 1.0f,  1.0f, -1.0f); /* Top Right Of The Quad (Top)      */
	glVertex3f(-1.0f,  1.0f, -1.0f); /* Top Left Of The Quad (Top)       */
	glVertex3f(-1.0f,  1.0f,  1.0f); /* Bottom Left Of The Quad (Top)    */
	glVertex3f( 1.0f,  1.0f,  1.0f); /* Bottom Right Of The Quad (Top)   */

	glColor3f(  1.0f,  0.5f,  0.0f); /* Set The Color To Orange          */
	glVertex3f( 1.0f, -1.0f,  1.0f); /* Top Right Of The Quad (Botm)     */
	glVertex3f(-1.0f, -1.0f,  1.0f); /* Top Left Of The Quad (Botm)      */
	glVertex3f(-1.0f, -1.0f, -1.0f); /* Bottom Left Of The Quad (Botm)   */
	glVertex3f( 1.0f, -1.0f, -1.0f); /* Bottom Right Of The Quad (Botm)  */

	glColor3f(  1.0f,  0.0f,  0.0f); /* Set The Color To Red             */
	glVertex3f( 1.0f,  1.0f,  1.0f); /* Top Right Of The Quad (Front)    */
	glVertex3f(-1.0f,  1.0f,  1.0f); /* Top Left Of The Quad (Front)     */
	glVertex3f(-1.0f, -1.0f,  1.0f); /* Bottom Left Of The Quad (Front)  */
	glVertex3f( 1.0f, -1.0f,  1.0f); /* Bottom Right Of The Quad (Front) */

	glColor3f(  1.0f,  1.0f,  0.0f); /* Set The Color To Yellow          */
	glVertex3f( 1.0f, -1.0f, -1.0f); /* Bottom Left Of The Quad (Back)   */
	glVertex3f(-1.0f, -1.0f, -1.0f); /* Bottom Right Of The Quad (Back)  */
	glVertex3f(-1.0f,  1.0f, -1.0f); /* Top Right Of The Quad (Back)     */
	glVertex3f( 1.0f,  1.0f, -1.0f); /* Top Left Of The Quad (Back)      */

	glColor3f(  0.0f,  0.0f,  1.0f); /* Set The Color To Blue            */
	glVertex3f(-1.0f,  1.0f,  1.0f); /* Top Right Of The Quad (Left)     */
	glVertex3f(-1.0f,  1.0f, -1.0f); /* Top Left Of The Quad (Left)      */
	glVertex3f(-1.0f, -1.0f, -1.0f); /* Bottom Left Of The Quad (Left)   */
	glVertex3f(-1.0f, -1.0f,  1.0f); /* Bottom Right Of The Quad (Left)  */

	glColor3f(  1.0f,  0.0f,  1.0f); /* Set The Color To Violet          */
	glVertex3f( 1.0f,  1.0f, -1.0f); /* Top Right Of The Quad (Right)    */
	glVertex3f( 1.0f,  1.0f,  1.0f); /* Top Left Of The Quad (Right)     */
	glVertex3f( 1.0f, -1.0f,  1.0f); /* Bottom Left Of The Quad (Right)  */
	glVertex3f( 1.0f, -1.0f, -1.0f); /* Bottom Right Of The Quad (Right) */
	glEnd();                            /* Done Drawing The Quad            */

}

