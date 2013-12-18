#ifndef PLAYER_H
#define PLAYER_H

#include <thread>

#include "Config.h"
#include "Types.h"
#include "Position.h"



class Game;
class Player
{
   public:

      Player(Game * game, int id);
      ~Player();

      void play(int action);

      void levelUp();

      void setOpponent(Player * oponnent);
      Player * getOpponent();

      void live();
      void die();
      static void run(Player * p);

      void movePac();

      void eatAction();
      void eatDot();
      void eatPower();
      void eatBonus();

      int getTileProp(int tic);

      void addScore(int item);

      /********************************
         accessors
      ********************************/

      int getCurDirection();
      void setCurDirection(int curDirection);

      int getNewDirection();
      void setNewDirection(int newDirection);

      Position * getPosition();

      int getScore();
      void setScore(int score);

      int getLevel();
      void setLevel(int level);

      int getSpeed();

      /********************************
         end accessors
      ********************************/

   protected:

      bool alive;
      int speed;

      int level;
      int score;
      int dots;

      int tic;
      int action;

      int curDirection;
      int newDirection;
      Position * position;

      int idPlayer;
      Player * opponent;

      Game * game;

   private:

};
#endif // PLAYER_H
