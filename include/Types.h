#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED

#include <SDL2/SDL.h>
typedef struct
{
    SDL_Rect R;
    int type;
} TileProp;

typedef struct
{
   int player;
   int action;
} Action;

typedef struct
{
   int x;
   int y;
   int subpos;
} Position;



#endif // TYPES_H_INCLUDED
