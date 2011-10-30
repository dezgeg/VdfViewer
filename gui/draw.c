#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

#include "constants.h"
#include "planet.h"
#include "gui.h"

static GLUquadricObj* sphere;
#define NUM_COLORS 7
static const Vector colors[] = { // almost the same colors as in octave
	{ 1.0f, 0.0f, 0.0f }, // red
	{ 0.0f, 1.0f, 0.0f }, // blue
	{ 0.0f, 0.0f, 1.0f }, // green
	{ 0.0f, 1.0f, 1.0f }, // cyan
	{ 1.0f, 0.0f, 1.0f }, // magenta
	{ 1.0f, 1.0f, 0.0f }, // yellow
	{ 1.0f, 1.0f, 1.0f }, // yellow
};

static const GLfloat* get_planet_color(int planet)
{
	return colors[planet % NUM_COLORS];
}
static void draw_grid(void)
{
	const Vector dirs[] = { { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.f, 0.0f, 1.0f } };
	glBegin(GL_LINES);

	for(int i = 0; i < 3; i++)
	{
		glColor3f(dirs[i][0], dirs[i][1], dirs[i][2]);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(FAR_AWAY * dirs[i][0], FAR_AWAY * dirs[i][1], FAR_AWAY * dirs[i][2]);

		glColor3f(0.5f * dirs[i][0], 0.5f * dirs[i][1], 0.5f * dirs[i][2]);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-FAR_AWAY * dirs[i][0], -FAR_AWAY * dirs[i][1], -FAR_AWAY * dirs[i][2]);
	}

	glEnd();
}
static void draw_trails(void)
{
	if(!state.trails_enabled)
		return;
	for(int i = 0; i < state.sys->nplanets; i++)
	{
		glColor3fv(get_planet_color(i));
		Vector* trails = state.views[i].trails;
		glBegin(GL_POINTS);
		for(int j = 0; j < state.num_trails; j++)
		{
			int trail_index = (state.first_trail + j) % MAX_TRAILS;
			glVertex3fv(trails[trail_index]);
		}
		glEnd();

		glBegin(GL_LINE_STRIP);
		for(int j = 0; j < state.num_trails; j++)
		{
			int trail_index = (state.first_trail + j) % MAX_TRAILS;
			glVertex3fv(trails[trail_index]);
		}
		glVertex3fv(state.sys->planets[i].position);
		glEnd();
	}
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
		glColor3fv(get_planet_color(i));
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
	glDisable(GL_LIGHTING);
	draw_grid();
	draw_trails();
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
	glLineWidth(1.0f);
}
