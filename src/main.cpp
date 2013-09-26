#include <iostream>
#include <time.h>

#include "Device.h"
#include "Game.h"
#include "Config.h"
#include "Exceptions.h"

using namespace std;
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
      _game->display();

      _game->actionPause(string("Init"));
      _game->play();

      // wait for end pause or quit
      while(action == 0)
      {
         SDL_WaitEvent(&event);
         switch(event.type)
         {
            case SDL_QUIT:
               action = 1;
               break;
            default:
               break;
         }
         SDL_Delay(20);
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
