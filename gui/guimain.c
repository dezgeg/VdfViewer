#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

#include "gui.h"
#include "constants.h"

SDL_Surface *surface;

static void resize_window(int width, int height)
{
	if(height == 0)
		height = 1;

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	GLfloat ratio = (GLfloat)width / (GLfloat)height;
	gluPerspective(FOV, ratio, NEAR_PLANE, FAR_PLANE);

	glMatrixMode(GL_MODELVIEW);
}
static void die(const char* message)
{
	fprintf(stderr, "%s: %s\n", message, SDL_GetError());
	exit(1);
}
static void usage(const char* progname)
{
	fprintf(stderr, "usage: %s infile\n", progname);
	exit(EXIT_FAILURE);
}

int main(int argc, char** argv)
{
	if(argc != 2)
		usage(argv[0]);

	FILE* infile = fopen(argv[1], "r");
	if(infile == NULL)
	{
		perror("Error opening input file");
		return EXIT_FAILURE;
	}
	System* sys = load_system(infile);
	if(sys == NULL)
	{
		printf("Loading input file failed\n");
		return EXIT_FAILURE;
	}
	init_simulation(sys);
	state.sys = sys;
	state.views = malloc(sys->nplanets * sizeof(PlanetView));
	for(int i = 0; i < sys->nplanets; i++)
		state.views[i].radius = pow(sys->planets[i].mass / DENSITY, 1.0f/3.0f);
	state.pos[1] = get_planet_radius(0);
	state.rot_x = 90.0f;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
		die("SDL initialization failed");
	atexit(SDL_Quit);
	const SDL_VideoInfo* videoInfo = SDL_GetVideoInfo();
	if(!videoInfo)
		die("Could not get video information");
//	int videoFlags = SDL_OPENGL | SDL_HWPALETTE | SDL_RESIZABLE;
	int videoFlags = SDL_OPENGL | SDL_HWPALETTE | SDL_RESIZABLE | SDL_HWSURFACE | SDL_HWACCEL;

	/*
	if(videoInfo->hw_available)
		videoFlags |= SDL_HWSURFACE;
	else
		videoFlags |= SDL_SWSURFACE;
	if(videoInfo->blit_hw)
		videoFlags |= SDL_HWACCEL;
		*/

	//SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
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
