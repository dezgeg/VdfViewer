#include <GL/gl.h>
#include <GL/glu.h>

#include "gui.h"

static void draw_grid(void)
{
	const GLfloat FAR_AWAY = 10000.0f;

	glLineWidth(3.0f);
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(FAR_AWAY, 0.0f, 0.0f);

	glColor3f(0.5f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-FAR_AWAY, 0.0f, 0.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, FAR_AWAY, 0.0f);

	glColor3f(0.0f, 0.5f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, -FAR_AWAY, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, FAR_AWAY);

	glColor3f(0.0f, 0.0f, 0.5f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, -FAR_AWAY);

	glEnd();
}
void draw_scene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glRotatef(state.rot_y, 1.0, 0.0, 0.0);
	glRotatef(state.rot_x, 0.0, 1.0, 0.0);
	glTranslatef(-state.pos[0], -state.pos[1], -state.pos[2]);

	glPushMatrix();
	glEnable(GL_LIGHTING);

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
	glPushMatrix();

	glTranslatef(0.0f, 2.0f, -6.0f);

	GLUquadricObj* qo = gluNewQuadric();
	glColor3f(1.0f, 1.0f, 1.0f);
	gluSphere(qo, 1.0, 32, 32);
	gluDeleteQuadric(qo);

	glPopMatrix();
	draw_grid();
}

void init_gl(void)
{
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	const GLfloat ambient_light[] = { 0.1, 0.1, 0.1, 1.0 };
	const GLfloat diffuse_light[] = { 0.8, 0.8, 0.8, 1.0 };
	const GLfloat ambient_pos[] = { 0.0, 5.0, 1.0, 1.0 };

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient_light);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse_light);
	glLightfv(GL_LIGHT1, GL_POSITION, ambient_pos);
	glEnable(GL_LIGHT1);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_MULTISAMPLE);

	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
}
