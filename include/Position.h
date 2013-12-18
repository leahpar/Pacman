#ifndef POSITION_H
#define POSITION_H


class Position
{
   public:
      Position();
      Position(int x, int y);
      virtual ~Position();

      int moveDelta(int dir);

      bool collision(Position * p2);


      /********************************
         accessors
      ********************************/

      float fx();
      int x();
      float fy();
      int y();

      /********************************
         end accessors
      ********************************/

   protected:
   private:

      int _x;
      int _y;

      float _fx;
      float _fy;
};

#endif // POSITION_H
