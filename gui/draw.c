#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

#include "planet.h"
#include "gui.h"

static GLUquadricObj* sphere;

static void draw_grid(void)
{
	const GLfloat FAR_AWAY = 1.5e11f;

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
	return state.views[planet].radius;
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
	for(int i = 0; i < state.sys->nplanets; i++)
	{
		if(i % 2)
			glColor3f(0.0f, 1.0f, 1.0f);
		else
			glColor3f(1.0f, 0.0f, 1.0f);
		Planet* planet = &state.sys->planets[i];
		glTranslatef(planet->position[0], planet->position[1], planet->position[2]);
		gluSphere(sphere, get_planet_radius(i), 32, 32);
		printf("%g\n", get_planet_radius(i));
		glPopMatrix();
		glPushMatrix();
	}
	printf("%g %g %g\n", state.pos[0], state.pos[1], state.pos[2]);
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

	/*
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	*/
	glEnable(GL_MULTISAMPLE);

	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
}
