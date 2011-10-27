#ifndef GUI_EVENTS_H
#define GUI_EVENTS_H

#include <SDL/SDL.h>

extern void handle_keypress(const SDL_keysym* keysym);
extern void handle_mouse(const SDL_MouseMotionEvent* mme);
extern void drawGLScene(void);
extern void update(void);

extern SDL_Surface* surface;
#endif
