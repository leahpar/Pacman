#include "Config.h"
#include "Position.h"

#include <cmath>

Position::Position()
{
   //ctor
}

Position::Position(int x, int y)
{
   this->_x = x;
   this->_y = y;

   this->_fx = 0;
   this->_fy = 0;
}

Position::~Position()
{
   this->_x = 0;
   this->_y = 0;

   this->_fx = 0;
   this->_fy = 0;
}

int Position::moveDelta(int dir)
{
   switch (dir)
   {
      case ACTION_MOVE_UP:
         this->_fx -= 1;
         break;
      case ACTION_MOVE_DOWN:
         this->_fx += 1;
         break;
      case ACTION_MOVE_LEFT:
         this->_fy -= 1;
         break;
      case ACTION_MOVE_RIGHT:
         this->_fy += 1;
         break;
   }

   if (this->_fx > 9)
   {
      this->_fx  = 0;
      this->_x  += 1;
   }
   else if (this->_fx < 0)
   {
      this->_fx  = 9;
      this->_x  -= 1;
   }
   else if (this->_fy > 9)
   {
      this->_fy  = 0;
      this->_y  += 1;
   }
   else if (this->_fy < 0)
   {
      this->_fy  = 9;
      this->_y  -= 1;
   }

   if (this->_fx == 0 && this->_fy == 0)
      return 1;
   else
      return 0;
}

bool Position::collision(Position * p2)
{
   return (std::abs(this->fx() - p2->fx()) < 0.5)
       && (std::abs(this->fy() - p2->fy()) < 0.5);
}

/********************************
	accessors
********************************/

float Position::fx() {
	return this->_x + this->_fx/10;
}

float Position::fy() {
	return this->_y + this->_fy/10;
}

int Position::x() {
	return this->_x;
}

int Position::y() {
	return this->_y;
}

/********************************
	end accessors
********************************/

