#ifndef GAME_H
#define GAME_H

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "Types.h"
#include "Player.h"
#include "Monster.h"

#include <vector>
using std::vector;


using namespace std;

class Player;
class Monster;
class Game
{
   public:
      Game();
      ~Game();

      // game matrix
      vector<vector<int> > matrix;
      int matrix_x;
      int matrix_y;

      void setPlayers(int n);
      void setMonsters(int n);
      void addMonster();

      int  play();
      void eatDot(Position * pos);
      void eatPower(Position * pos);
      void setWeakMonsters();

      void displayInit();
      void display(int tic);
      void displayScore();

      int actionPause(string str);
      Action getAction();
      string pauseStr;

      void setPause();
      void unsetPause();

      /********************************
         accessors
      ********************************/

      void setSpeed(int speed);

      /********************************
         end accessors
      ********************************/


   protected:
   private:

      // screen display
      SDL_Window*    screen;
      SDL_Renderer*  renderer;
 

      // textures
      SDL_Texture*   tileset;
      TileProp       props[NB_PX];

      SDL_Texture*   tilesetPacman;
      TileProp       propsPacman[5 * NB_C_PACMAN];

      SDL_Texture*   tilesetMonster;
      TileProp       propsMonster[5 * NB_C_MONSTER * NB_MONSTER2];

      // font
      TTF_Font*      font;

      // audio
      Mix_Music*     sound;
      int            pause;
      int            speed;

      // player(s)
      vector<Player*> players;

      // Monsters
      vector<Monster*> monsters;

      // game
      int            dots;
      int            endGame;
      int            bonusTic;
      int            bonusType;

      // Init functions - called by constructor
      void initSDL_Video();
      void initSDL_ttf();
      void initSDL_Mixer();
      void loadTextures(const char * file, SDL_Texture ** ppTileset, TileProp * props, int nbTiles);
      void initMatrix();

      void addBonus(int tic);
      void delBonus();


};

#endif // GAME_H
