#include <iostream>
#include <time.h>

#include "Device.h"
#include "Game.h"
#include "Config.h"
#include "Types.h"
#include "Exceptions.h"

using namespace std;
#ifdef WIN32
#undef main
#endif
int main(int argc, char **argv)
{
   Game * _game;
   SDL_Event event;
   int action = 0;

   // init rand
   srand(time(NULL));

   try
   {
      _game = new Game();
      _game->setPlayers(1);
      _game->setMonsters(1);
      _game->displayInit();

      action = _game->play();

      // wait for end pause or quit
      while(action != ACTION_QUIT)
      {
         SDL_WaitEvent(&event);
         switch(event.type)
         {
            case SDL_QUIT:
               action = 1;
               break;
            case SDL_KEYDOWN:
               switch(event.key.keysym.sym)
               {
                  case SDLK_ESCAPE:
                     action = 1;
                     break;
               }
         }
         SDL_Delay(50);
      }
   }
   catch (int e)
   {
      // MessageBox(NULL, "KO", NULL, 0);
      return e;
   }

   // MessageBox(NULL, "OK", NULL, 0);

   delete _game;
   return 0;
}
