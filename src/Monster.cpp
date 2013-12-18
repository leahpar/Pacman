#include <cstdlib>

#include "Monster.h"
#include "Game.h"
#include "Position.h"

Monster::Monster(Game * game, int id) :
      Player(game, id)
{
   //ctor
   this->idPlayer = id;
   this->game = game;
   this->score = 0;
   this->dots  = 0;
   this->position = new Position(14, 13);
   this->curDirection = ACTION_STOP;
   this->weak = 0;
   this->speed = 25;
}

Monster::~Monster()
{
   //dtor
}

void Monster::live()
{
   this->alive = true;
   std::thread t(Monster::run, this);
   t.detach();
}

void Monster::die()
{
   this->alive = false;
}

void Monster::run(Monster * m)
{
   while (m->alive)
   {
      m->movePac();

      if (m->weak > 0) m->weak--;

      SDL_Delay(m->getSpeed());
   }
}

void Monster::movePac()
{
   int flag = 0;

   int step = 0;

   // move pacman
   step = this->position->moveDelta(this->curDirection);

   // if new case or no direction
   if ((step == 1)
    || (this->curDirection == ACTION_STOP))
   {
      this->newDirection = this->IA_newDirection();

      // test new direction
      switch (this->newDirection)
      {
         case ACTION_MOVE_UP:
            if (this->game->matrix[this->position->x()-1]
                                  [this->position->y()] <= PX_SOLID)
            {
               this->curDirection = this->newDirection;
               flag = 1;
            }
            break;
         case ACTION_MOVE_DOWN:
            if (this->game->matrix[this->position->x()+1]
                                  [this->position->y()] <= PX_SOLID)
            {
               this->curDirection = this->newDirection;
               flag = 1;
            }
            break;
         case ACTION_MOVE_LEFT:
            if (this->game->matrix[this->position->x()]
                                  [this->position->y()-1] <= PX_SOLID)
            {
               this->curDirection = this->newDirection;
               flag = 1;
            }
            break;
         case ACTION_MOVE_RIGHT:
            if (this->game->matrix[this->position->x()]
                                  [this->position->y()+1] <= PX_SOLID)
            {
               this->curDirection = this->newDirection;
               flag = 1;
            }
            break;
      }
   }
      // if no new direction, test current direction
   if (step == 1 && flag == 0)
   {
      switch (this->curDirection)
      {
         case ACTION_MOVE_UP:
            if (this->game->matrix[this->position->x()-1]
                                  [this->position->y()] > PX_SOLID)
            {
               this->curDirection = ACTION_STOP;
               flag = 1;
            }
            break;
         case ACTION_MOVE_DOWN:
            if (this->game->matrix[this->position->x()+1]
                                  [this->position->y()] > PX_SOLID)
            {
               this->curDirection = ACTION_STOP;
               flag = 1;
            }
            break;
         case ACTION_MOVE_LEFT:
            if (this->game->matrix[this->position->x()]
                                  [this->position->y()-1] > PX_SOLID)
            {
               this->curDirection = ACTION_STOP;
               flag = 1;
            }
            break;
         case ACTION_MOVE_RIGHT:
            if (this->game->matrix[this->position->x()]
                                  [this->position->y()+1] > PX_SOLID)
            {
               this->curDirection = ACTION_STOP;
               flag = 1;
            }
            break;
      }
   }
   //this->position.dir = this->curDirection;
}

int Monster::getTileProp(int tic)
{
   if (this->weak == 0)
   {
      return this->curDirection * NB_C_MONSTER
              + 5 * NB_C_MONSTER * ((this->idPlayer % NB_MONSTER) + 1)
              + (tic / 10) % NB_C_MONSTER;
   }
   else
   {
      return this->curDirection * NB_C_MONSTER
              + (tic / 10) % NB_C_MONSTER;
   }
}

int Monster::IA_newDirection()
{
   int dir = std::rand() % 4 + 1;
   switch (this->curDirection)
   {
   case ACTION_MOVE_RIGHT:
      if (dir == ACTION_NOT_RIGHT) dir = (dir + 1) % 4 + 1;
      break;
   case ACTION_MOVE_LEFT:
      if (dir == ACTION_NOT_LEFT) dir = (dir + 1) % 4 + 1;
      break;
   case ACTION_MOVE_UP:
      if (dir == ACTION_NOT_UP) dir = (dir + 1) % 4 + 1;
      break;
   case ACTION_MOVE_DOWN:
      if (dir == ACTION_NOT_DOWN) dir = (dir + 1) % 4 + 1;
      break;
   }
   return dir;
}

/********************************
	accessors
********************************/

bool Monster::getWeak() {
	return (this->weak > 0);
}

void Monster::setWeak() {
	this->weak = MONSTER_WEAK;
}

int Monster::getSpeed()
{
   if (this->weak > 0)
      return this->speed + 10;
   else
      return this->speed;
}

/********************************
	end accessors
********************************/
