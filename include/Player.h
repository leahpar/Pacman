#ifndef PLAYER_H
#define PLAYER_H

#include "Config.h"
#include "Types.h"
#include "Game.h"

class Game;
class Player
{
   public:

      Player(Game * game, int id);
      ~Player();

      int play(int action);

      int getTic();
      void levelUp();

      void setOpponent(Player * oponnent);
      Player * getOpponent();

      void movePac();

      /********************************
         accessors
      ********************************/

      int getCurDirection();
      void setCurDirection(int curDirection);

      int getNewDirection();
      void setNewDirection(int newDirection);

      Position getPosition();
      void setPosition(Position position);

      int getScore();
      void setScore(int score);

      int getLevel();
      void setLevel(int level);

      /********************************
         end accessors
      ********************************/

   protected:

   private:

      int level;
      int score;

      int tic;
      int action;

      int curDirection;
      int newDirection;
      Position position;

      int idPlayer;
      Player * opponent;

      Game * game;

};
#endif // PLAYER_H
