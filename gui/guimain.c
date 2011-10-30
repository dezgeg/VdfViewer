#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

#include "gui.h"
#include "constants.h"

SDL_Surface *surface;

void init_viewport(void)
{
	glViewport(0, 0, (GLsizei)surface->w, (GLsizei)surface->h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/*
	if(state.orthographic)
		glOrtho(0.0f, surface->w, 0.0f, surface->h, NEAR_PLANE, FAR_PLANE);
	*/
	GLfloat ratio = (GLfloat)surface->w / (GLfloat)surface->h;
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
static void update_trails(void)
{
	int trail_index = (state.first_trail + state.num_trails) % MAX_TRAILS;
	for(int i = 0; i < state.sys->nplanets; i++)
		vector_copy(state.views[i].trails[trail_index], state.sys->planets[i].position);

	if(state.num_trails == MAX_TRAILS)
		state.first_trail++;
	else
		state.num_trails++;
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
		state.views[i].radius = pow(sys->planets[i].mass / DENSITY_FACTOR, 1.0f/3.0f);
	state.scale = 1.0f;
	Vector* fst_pos = &sys->planets[0].position;
	vector_copy(state.pos, *fst_pos);
	state.pos[1] += 1.1f*get_planet_radius(0);
	state.pos[0] -= get_planet_radius(0);
	state.rot_x = 90.0f;
	state.locked_planet = -1;
	state.hours_per_sec = DEFAULT_SIMULATION_SPEED;
	state.time_step = sys->time_step;
	state.paused = true;
	state.trails_enabled = true;

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
	init_viewport();
	SDL_ShowCursor(0);
	SDL_WM_GrabInput(SDL_GRAB_ON);

	SDL_Event event;
	while(SDL_PollEvent(&event))
		;  /* ignore spurious mouse events at startup */

	bool window_is_active = true;
	int step = 0;
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
					init_viewport();
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
		if(!state.paused)
		{
			for(int i = 0; i < (state.hours_per_sec * 3600.0f / FRAME_INTERVAL) / state.time_step; i++)
			{
				if((step % TRAILS_INTERVAL) == 0)
					update_trails();
				simulate_one_step(sys, step++, state.time_step);
			}
		}
		Sint32 delta = next_update - SDL_GetTicks();
		if(delta > 0)
			SDL_Delay(delta);
	}
out:

	return 0;
}
