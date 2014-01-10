#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

#include "constants.h"
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

void draw_scene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glRotatef(state.rot_y, 1.0, 0.0, 0.0);
	glRotatef(state.rot_x, 0.0, 1.0, 0.0);

	Vector pos_tmp;
	vector_copy(pos_tmp, state.pos);

	glTranslatef(-pos_tmp[0], -pos_tmp[1], -pos_tmp[2]);
	glPushMatrix();

        glEnable(GL_LIGHTING);
        // glTranslatef(planet->position[0], planet->position[1], planet->position[2]);
        //glScalef(state.scale, state.scale, state.scale);
        //gluSphere(sphere, 100000000.0, 32, 32);
	for(int i = 1; i <= state.numFaces; i++)
	{
                Vector v1, v2;
                vector_sub(v1, state.vertexes[state.faces[i][1]], state.vertexes[state.faces[i][0]]);
                vector_sub(v2, state.vertexes[state.faces[i][2]], state.vertexes[state.faces[i][0]]);
                Vector cross;
                vector_cross(cross, v1, v2);
                Vector norm;
                vector_normalize(norm, cross);

                glBegin(GL_TRIANGLES);
                glColor3fv(state.colors[i]);
                glNormal3fv(norm);

                Vector vertexes[3];

		for (unsigned j = 0; j < 3; j++) {
			unsigned vertexIndex = state.faces[i][j];

			Vector tmp;
			vector_mul(tmp, state.animLerp / 100.0, state.anims[state.animIndex][vertexIndex]);
			vector_add(vertexes[j], state.vertexes[vertexIndex], tmp);
			glVertex3fv(vertexes[j]);
		}
		glEnd();
	}

        glDisable(GL_LIGHTING);
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
	glLineWidth(1.0f);
}
