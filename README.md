Pacman
======

Description
-----------

Well, it's a pacman...

Dependencies
------------

* SDL >= 2.0.0
* SDL_ttf
* SDL_mixer

Compile
-------

no makefile for now, sorry...

```
cd Pacman
g++ src/*.cpp `sdl2-config --cflags --libs` -Iinclude -DLINUX -lSDL2_ttf -lSDL2_mixer
```



