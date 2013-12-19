#include <cstddef>

#include <iostream>
#include <fstream>
#include <string>

#include <thread>

using namespace std;

#include <SDL2/SDL.h>

#include "Config.h"
#include "Device.h"
#include "Position.h"
#include "Player.h"
#include "Game.h"
#include "Exceptions.h"

Player::Player(Game * game, int id)
{
   this->idPlayer = id;
   this->game = game;
   this->score = 0;
   this->dots  = 0;
   this->position = new Position(23, 13);
   this->curDirection = ACTION_STOP;

   this->alive = false;
   this->speed = 20;
}

Player::~Player()
{
}

void Player::live()
{
   this->alive = true;
   std::thread t(Player::run, this);
   t.detach();
}

void Player::die()
{
   this->alive = false;
}

void Player::run(Player * p)
{
   int tic = 0;
   while (p->alive)
   {
      p->movePac();

      if (tic%100 == 0) p->score -= 1;

      SDL_Delay(p->getSpeed());
      tic++;
   }
}

void Player::play(int action)
{
   if (action != ACTION_NONE)
   {
      this->newDirection = action;
   }
}

void Player::eatAction()
{
   switch (this->game->matrix[this->position->x()]
                             [this->position->y()])
   {
      case PX_DOT:
         this->eatDot();
         this->addScore(PX_DOT);
         break;
      case PX_POWER:
         this->eatPower();
         this->addScore(PX_POWER);
         break;
      case PX_BONUS_1:
      case PX_BONUS_2:
      case PX_BONUS_3:
      case PX_BONUS_4:
      case PX_BONUS_5:
      case PX_BONUS_6:
      case PX_BONUS_7:
      case PX_BONUS_8:
         this->eatBonus();
         this->addScore(this->game->matrix[this->position->x()]
                                          [this->position->y()]);
         break;
   }
}

void Player::eatDot()
{
   this->dots++;
   this->game->eatDot(this->position);
}

void Player::eatPower()
{
   this->game->eatPower(this->position);
}

void Player::eatBonus()
{
   /* TODO */
}

void Player::addScore(int item)
{
   switch (item)
   {
      case PX_DOT:
         this->score += 1;
         break;
      case PX_POWER:
         this->score += 5;
         break;
      case PX_BONUS_1:
      case PX_BONUS_2:
      case PX_BONUS_3:
      case PX_BONUS_4:
      case PX_BONUS_5:
      case PX_BONUS_6:
      case PX_BONUS_7:
      case PX_BONUS_8:
         this->score += 5;
         break;
   }
}

void Player::movePac()
{
   int flag = 0;
   int step = 0;

   // move pacman
   step = this->position->moveDelta(this->curDirection);

   if (step == 1) this->eatAction();

   // if new case or no direction
   if ((step == 1)
    || (this->curDirection == ACTION_STOP))
   {
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
}

void Player::levelUp()
{
   this->level++;
}

int Player::getTileProp(int tic)
{
   return this->curDirection * NB_C_PACMAN + (tic/5)%NB_C_PACMAN;
}


/********************************
	accessors
********************************/

int Player::getLevel() {
	return this->level;
}

void Player::setLevel(int level) {
	this->level = level;
}

int Player::getScore() {
	return this->score;
}

void Player::setScore(int score) {
	this->score = score;
}

Player * Player::getOpponent() {
	return this->opponent;
}

void Player::setOpponent(Player * opponent) {
	this->opponent = opponent;
}


int Player::getCurDirection() {
	return this->curDirection;
}

void Player::setCurDirection(int curDirection) {
	this->curDirection = curDirection;
}

int Player::getNewDirection() {
	return this->newDirection;
}

void Player::setNewDirection(int newDirection) {
	this->newDirection = newDirection;
}

Position * Player::getPosition() {
	return this->position;
}

int Player::getSpeed()
{
   return this->speed;
}
/********************************
	end accessors
********************************/

