#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

#include "events.h"

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP     16
#define FRAME_INTERVAL 30

SDL_Surface *surface;

int resizeWindow(int width, int height)
{
	if(height == 0)
		height = 1;

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	GLfloat ratio = (GLfloat)width / (GLfloat)height;
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	return true;
}

void initGL(GLvoid)
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
	glEnable(GL_LIGHTING);
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
void die(const char* message)
{
	fprintf(stderr, "%s: %s\n", message, SDL_GetError());
	exit(1);
}

int main(int argc, char **argv)
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
		die("Video initialization failed");
	atexit(SDL_Quit);

	const SDL_VideoInfo* videoInfo = SDL_GetVideoInfo();
	if(!videoInfo)
		die("Could not get video information");

	int videoFlags = SDL_OPENGL | SDL_DOUBLEBUF | SDL_HWPALETTE | SDL_RESIZABLE;
	videoFlags |= SDL_HWSURFACE | SDL_HWACCEL;
	/*
	if(videoInfo->hw_available)
		videoFlags |= SDL_HWSURFACE;
	else
		videoFlags |= SDL_SWSURFACE;
	if(videoInfo->blit_hw)
		videoFlags |= SDL_HWACCEL;
	*/

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	surface = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, videoFlags);

	if(!surface)
		die("Changing video mode failed");

	initGL();
	resizeWindow(SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_ShowCursor(0);
	SDL_WM_GrabInput(SDL_GRAB_ON);

	SDL_Event event;
	while(SDL_PollEvent(&event))
		;  /* ignore spurious mouse events at startup */

	bool isActive = true;
	while (true)
	{
		Uint32 next_update = SDL_GetTicks() + FRAME_INTERVAL;
		while (SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_ACTIVEEVENT:
					isActive = event.active.gain;
					break;			    
				case SDL_VIDEORESIZE:
					surface = SDL_SetVideoMode(event.resize.w,
							event.resize.h,
							SCREEN_BPP, videoFlags);
					if(!surface)
						die("Lost video surface during resize");
					resizeWindow(event.resize.w, event.resize.h);
					break;
				case SDL_KEYDOWN:
					handle_keypress(&event.key.keysym);
					break;
				case SDL_MOUSEMOTION:
					handle_mouse(&event.motion);
					break;
				case SDL_QUIT:
					goto out;
				default:
					break;
			}
		}
		update();
		if(isActive)
		{
			drawGLScene();
			glFlush();
			SDL_GL_SwapBuffers();
		}
		Sint32 delta = next_update - SDL_GetTicks();
		if(delta > 0)
			SDL_Delay(delta);
	}
out:

	return 0;
}
