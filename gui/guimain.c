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

static void read_model(void)
{
        int vertexIndex = 1;
        int faceIndex = 1;
        while (true) {
                char linebuf[1024];
                linebuf[0] = '\0';
                fgets(linebuf, sizeof(linebuf), stdin);
                if (linebuf[0] == '\0')
                        break;

                printf("%s", linebuf);
                char word[64];
                sscanf(linebuf, "%s", word);
                if (!strcmp(word, "v")) {
                        Vector v;
                        sscanf(linebuf, "%s %f %f %f", word, &v[0], &v[1], &v[2]);
                        memcpy(&state.vertexes[vertexIndex], &v, sizeof(v));
                        vertexIndex++;
                } else if (!strcmp(word, "f")) {
                        int f[3];
                        sscanf(linebuf, "%s %d %d %d", word, &f[0], &f[1], &f[2]);
                        memcpy(&state.faces[faceIndex][0], &f, 3 * sizeof(int));
                        faceIndex++;
                } else if (!strcmp(word, "usemtl")) {

                } else if (!strcmp(word, "mtllib")) {

                }
        }
        state.numFaces = faceIndex - 1;
        printf("done\n");
}

int main(int argc, char** argv)
{
	read_model();
	state.pos[0] = 4.3049760000000;
	state.pos[1] = 4.2958296000000;
	state.pos[2] = 1.07656760000000;
	state.rot_x = 333.299713f;
	state.rot_y = 24.149984f;
	state.scale = 1.0f;

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
		        // printf("%f %f %f, %f %f\n", state.pos[0], state.pos[1], state.pos[2], state.rot_x, state.rot_y);
		}
		Sint32 delta = next_update - SDL_GetTicks();
		if(delta > 0)
			SDL_Delay(delta);
	}
out:

	return 0;
}
