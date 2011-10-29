#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

#include "constants.h"
#include "planet.h"
#include "gui.h"

static GLUquadricObj* sphere;

static void draw_grid(void)
{
	glLineWidth(1.0f);
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
GLfloat get_planet_radius(int planet)
{
	return state.scale * state.views[planet].radius;
}
void draw_scene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glRotatef(state.rot_y, 1.0, 0.0, 0.0);
	glRotatef(state.rot_x, 0.0, 1.0, 0.0);

	Vector pos_tmp;
	vector_copy(pos_tmp, state.pos);
	if(state.locked_planet >= 0)
		vector_add(pos_tmp, pos_tmp, state.sys->planets[state.locked_planet].position);

	glTranslatef(-pos_tmp[0], -pos_tmp[1], -pos_tmp[2]);
	glPushMatrix();
	for(int i = 0; i < state.sys->nplanets; i++)
	{
		glColor3f(1.0f, 1.0f, 1.0f);
		Planet* planet = &state.sys->planets[i];
		glTranslatef(planet->position[0], planet->position[1], planet->position[2]);
		glScalef(state.scale, state.scale, state.scale);
		glDisable(GL_LIGHTING);
		glBegin(GL_POINTS);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glEnd();
		glEnable(GL_LIGHTING);
		gluSphere(sphere, get_planet_radius(i), 32, 32);
		glPopMatrix();
		glPushMatrix();
	}
	draw_grid();
	glPopMatrix();
}

void init_gl(void)
{
	sphere = gluNewQuadric();
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

	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glPointSize(3.0f);
}
