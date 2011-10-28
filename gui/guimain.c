#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

#include "gui.h"

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP     16
#define FRAME_INTERVAL 30

SDL_Surface *surface;

static void resize_window(int width, int height)
{
	if(height == 0)
		height = 1;

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	GLfloat ratio = (GLfloat)width / (GLfloat)height;
	gluPerspective(45.0f, ratio, 0.1f, 3000.0f);

	glMatrixMode(GL_MODELVIEW);
}

static void die(const char* message)
{
	fprintf(stderr, "%s: %s\n", message, SDL_GetError());
	exit(1);
}

int main(int argc, char **argv)
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
		die("SDL initialization failed");
	atexit(SDL_Quit);

	const SDL_VideoInfo* videoInfo = SDL_GetVideoInfo();
	if(!videoInfo)
		die("Could not get video information");

	int videoFlags = SDL_OPENGL | SDL_HWPALETTE | SDL_RESIZABLE | SDL_HWSURFACE | SDL_HWACCEL;

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	surface = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, videoFlags);
	if(!surface)
	{
		printf("Surface creation failed, trying to disable anti-aliasing...\n");
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);
		surface = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, videoFlags);
	}

	if(!surface)
		die("Changing video mode failed");

	init_gl();
	resize_window(SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_ShowCursor(0);
	SDL_WM_GrabInput(SDL_GRAB_ON);

	SDL_Event event;
	while(SDL_PollEvent(&event))
		;  /* ignore spurious mouse events at startup */

	bool window_is_active = true;
	while (true)
	{
		Uint32 next_update = SDL_GetTicks() + FRAME_INTERVAL;
		while (SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_ACTIVEEVENT:
					window_is_active = event.active.gain;
					break;			    
				case SDL_VIDEORESIZE:
					surface = SDL_SetVideoMode(event.resize.w,
							event.resize.h,
							SCREEN_BPP, videoFlags);
					if(!surface)
						die("Lost video surface during resize");
					resize_window(event.resize.w, event.resize.h);
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
		if(window_is_active)
		{
			draw_scene();
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
