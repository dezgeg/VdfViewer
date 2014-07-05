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

typedef union __attribute__((packed)) PsxFixed
{
	struct {
		unsigned fractional : 12;
		unsigned integral : 3;
		unsigned sign : 1;
	};
	short bits;
} PsxFixed;

static inline GLfloat to_float(uint16_t b)
{
	return (int16_t)b / 4096.0;
}

static const char* dump(uint16_t b)
{
	PsxFixed f;
	static char buf[64][64];
	static int bufptr;

	f.bits = b;
	bufptr = (bufptr + 1) % 64;
	snprintf(&buf[bufptr][0], 64, "(%c%01x.%03x)", f.sign ? '-' : '+', f.integral, f.fractional);
	return &buf[bufptr][0];
}

typedef struct __attribute__((packed)) PsxVector
{
	uint16_t x, y, z, pad;
} PsxVector;

static void read_model(FILE* fp)
{
        int vertexIndex = 1;
        int faceIndex = 1;
        while (true) {
                char linebuf[1024];
                linebuf[0] = '\0';
                fgets(linebuf, sizeof(linebuf), fp);
                if (linebuf[0] == '\0')
                        break;

                //printf("%s", linebuf);
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
                        int r, g, b, a;
                        sscanf(linebuf, "%s r%dg%db%da%d", word, &r, &g, &b, &a);
                        Vector v = { r/255.0, g/255.0, b/255.0 };
                        memcpy(&state.colors[faceIndex], &v, sizeof(v));
                } else if (!strcmp(word, "mtllib")) {

                }
        }
        state.numVertexes = vertexIndex - 1;
        state.numFaces = faceIndex - 1;
        printf("done\n");
}

static void read_anims(FILE* fp)
{
	uint32_t buf[20000];
	fread(&buf, sizeof(buf), 1, fp);

	uint32_t* ptr = buf;
	uint32_t objtotal = *ptr++;
	printf("objtotal: %u, sizeof(Vector) = %zu, sizeof(Fixed) = %zu\n",
		objtotal, sizeof(PsxVector), sizeof(PsxFixed));

	ptr++;
	for(unsigned i = 0; i < objtotal; i++) {
		uint32_t offset = *ptr++; // assume this is always 0
		uint32_t total = *ptr++;
		(void) offset, (void) total;

		printf("Convert %u array off=0x%lx\n", i, 4 * (long)(ptr - buf));
		PsxVector* array = (PsxVector*) ptr;
		for(unsigned j = 0; j < total; j++) {
			state.anims[i + 1][j + 1][0] = to_float(array[j].x);
			state.anims[i + 1][j + 1][1] = to_float(array[j].y);
			state.anims[i + 1][j + 1][2] = to_float(array[j].z);
			printf("    %2u/%3u: %s:%+f %s:%+f %s:%+f (0x%04hx 0x%04hx 0x%04hx 0x%04hx)\n",
					i, j + 1,
					dump(array[j].x), to_float(array[j].x),
					dump(array[j].y), to_float(array[j].y),
					dump(array[j].z), to_float(array[j].z),
					array[j].x, array[j].y, array[j].z, array[j].pad);
		}

		ptr += total * 2;
		ptr++;
	}
	state.numAnims = objtotal;
        printf("EOF at 0x%lx\n", 4 * (long)(ptr - buf));
}

int main(int argc, char** argv)
{
	if (argc < 3)
		die("args");
	FILE* model = fopen(argv[1], "r");
	if (!model)
		die("Can't open model");
	read_model(model);
	fclose(model);

	FILE* anims = fopen(argv[2], "r");
	if (!anims)
		die("Can't open anims");
	read_anims(anims);
	fclose(anims);

	state.pos[0] = 1.169238;
	state.pos[1] = 26.488846;
	state.pos[2] = -21.112965;
	state.rot_x = 182.249313;
	state.rot_y = 34.499950;
	state.scale = 1.0f;
	state.animIndex = 1;
	state.animLerp = 0.0f;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
		die("SDL initialization failed");
	atexit(SDL_Quit);
	const SDL_VideoInfo* videoInfo = SDL_GetVideoInfo();
	if(!videoInfo)
		die("Could not get video information");
	int videoFlags = SDL_OPENGL | SDL_HWPALETTE | SDL_RESIZABLE | SDL_HWSURFACE | SDL_HWACCEL | SDL_RESIZABLE;

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
