#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <math.h>

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP     16
#define FRAME_INTERVAL 30

#define FOV 45.0f
#define NEAR_PLANE 1.0f
#define FAR_PLANE 1.0e3f
#define DENSITY_FACTOR (5.9e24 / pow(6400.0e3f, 3.0f)) // planet's m/r^3, value used for earth

#define FAR_AWAY 1.5e11f // axis endpoint coordinate

#define ROT_DELTA 2.0 // Viewport angle changed by up/down/left/right keys in degress
#define POS_DELTA 5.0 // Amount moved each frame by the WASD keys
#define MOUSE_X_DELTA 0.15
#define MOUSE_Y_DELTA 0.15

#endif
