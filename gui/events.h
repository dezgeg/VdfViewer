#ifndef GUI_EVENTS_H
#define GUI_EVENTS_H

#include <SDL/SDL.h>

extern void handleKeyPress(const SDL_keysym* keysym);
extern void drawGLScene(void);
extern void update(void);

extern SDL_Surface* surface;
#endif
