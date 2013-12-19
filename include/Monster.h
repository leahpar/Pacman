#ifndef MONSTER_H
#define MONSTER_H

#include "Config.h"
#include "Types.h"
#include "Player.h"

class Game;
class Player;
class Monster : public Player
{
   public:
      Monster(Game * game, int id);
      ~Monster();

      void movePac();
      int getTileProp(int tic);

      void live();
      void die();
      static void run(Monster * p);

      /********************************
         accessors
      ********************************/

      bool getWeak();
      void setWeak();

      int getSpeed();


      /********************************
         end accessors
      ********************************/


   protected:

   private:

      int weak;

      int IA_newDirection();

};

#endif // MONSTER_H
