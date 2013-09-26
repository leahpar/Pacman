#include <cstddef>

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include <SDL2/SDL.h>

#include "Config.h"
#include "Player.h"
#include "Game.h"
#include "Exceptions.h"

Player::Player(Game * game, int id)
{
   this->idPlayer = id;
   this->game = game;
   this->score = 0;
   this->position.x = 23;
   this->position.y = 13;
   this->curDirection = ACTION_MOVE_LEFT;
   this->position.subpos = 0;
}

Player::~Player()
{
}


int Player::getTic()
{
   return (100 - 2*this->level);
}

int Player::play(int action)
{
   int res;

   // Quit game
   if (action == ACTION_QUIT) return 1;

   if (action != ACTION_NONE)
   {
      this->newDirection = action;
   }

   return 0;
}

void Player::movePac()
{
   int flag = 0;

   // move pacman
   this->position.subpos++;

   // if moved untill next case
   if (this->position.subpos >= TILE_S)
   {
      switch (this->curDirection)
      {
         case ACTION_MOVE_UP:
            this->position.x--;
            break;
         case ACTION_MOVE_DOWN:
            this->position.x++;
            break;
         case ACTION_MOVE_LEFT:
            this->position.y--;
            break;
         case ACTION_MOVE_RIGHT:
            this->position.y++;
            break;
      }
      this->position.subpos = 0;
   }

   // if new case or no direction
   if ((this->position.subpos == 0)
    || (this->curDirection == ACTION_STOP))
   {
      // test new direction
      switch (this->newDirection)
      {
         case ACTION_MOVE_UP:
            if (this->game->matrix[this->position.x-1][this->position.y] <= PX_SOLID)
            {
               this->curDirection = this->newDirection;
               flag = 1;
            }
            break;
         case ACTION_MOVE_DOWN:
            if (this->game->matrix[this->position.x+1][this->position.y] <= PX_SOLID)
            {
               this->curDirection = this->newDirection;
               flag = 1;
            }
            break;
         case ACTION_MOVE_LEFT:
            if (this->game->matrix[this->position.x][this->position.y-1] <= PX_SOLID)
            {
               this->curDirection = this->newDirection;
               flag = 1;
            }
            break;
         case ACTION_MOVE_RIGHT:
            if (this->game->matrix[this->position.x][this->position.y+1] <= PX_SOLID)
            {
               this->curDirection = this->newDirection;
               flag = 1;
            }
            break;
      }
   }
      // if no new direction, test current direction
   if (flag == 0)
   {
      switch (this->curDirection)
      {
         case ACTION_MOVE_UP:
            if (this->game->matrix[this->position.x-1][this->position.y] > PX_SOLID)
            {
               this->curDirection = ACTION_STOP;
               flag = 1;
            }
            break;
         case ACTION_MOVE_DOWN:
            if (this->game->matrix[this->position.x+1][this->position.y] > PX_SOLID)
            {
               this->curDirection = ACTION_STOP;
               flag = 1;
            }
            break;
         case ACTION_MOVE_LEFT:
            if (this->game->matrix[this->position.x][this->position.y-1] > PX_SOLID)
            {
               this->curDirection = ACTION_STOP;
               flag = 1;
            }
            break;
         case ACTION_MOVE_RIGHT:
            if (this->game->matrix[this->position.x][this->position.y+1] > PX_SOLID)
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

Position Player::getPosition() {
	return this->position;
}

void Player::setPosition(Position position) {
	this->position = position;
}

/********************************
	end accessors
********************************/

