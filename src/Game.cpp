#include <unistd.h>
#include <string>
#include <sstream>

#include <cmath>
#include <thread>

#include <stdio.h> /* need fscanf ! */

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "Game.h"
#include "Position.h"
#include "Config.h"
#include "Device.h"
#include "Exceptions.h"

using namespace std;

Game::Game()
{
   // may throw exceptions
   this->initMatrix();
   this->initSDL_Video();
   this->initSDL_ttf();
   //this->initSDL_Mixer();

   this->speed = 20;

   this->bonusType = PX_VOID;
   this->bonusTic  = 0;
}

Game::~Game()
{
   unsigned int i;

   for (i=0; i<this->players.size(); i++)
   {
      delete this->players[i];
   }
   for (i=0; i<this->monsters.size(); i++)
   {
      delete this->monsters[i];
   }

   TTF_CloseFont(this->font);
   TTF_Quit();

   //Mix_HaltMusic();
   //Mix_FreeMusic(this->sound);

   SDL_DestroyTexture(this->tileset);
   SDL_DestroyRenderer(this->renderer);
   SDL_DestroyWindow(this->screen);
   SDL_Quit();
}

void Game::initSDL_Video()
{
   // Init SDL
   SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
   this->screen = SDL_CreateWindow(WINDOW_TITLE,
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   (this->matrix_y + SCORE_DISPLAY_Y + SCORE_DISPLAY_L )* TILE_S,
                                   this->matrix_x * TILE_S,
                                   0);
   this->renderer = SDL_CreateRenderer(this->screen, -1, 0);

   // init black
   SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
   SDL_RenderClear(this->renderer);

   loadTextures(TILESET_FILE, &this->tileset, this->props, NB_PX);
   loadTextures(PACMAN_FILE, &this->tilesetPacman, this->propsPacman, 5 * NB_C_PACMAN);
   loadTextures(MONSTER_FILE, &this->tilesetMonster, this->propsMonster, 5 * NB_C_MONSTER * NB_MONSTER2);
}

void Game::initSDL_ttf()
{
   // Init SDL ttf
   TTF_Init();
   this->font  = TTF_OpenFont(FONT_FILE, FONT_SIZE);
}

void Game::initSDL_Mixer()
{
   // Init audio
   if (Mix_OpenAudio(AUDIO_RATE, AUDIO_FORMAT, AUDIO_CHANNELS, AUDIO_BUFFERS))
   {
      Alert(NULL, Mix_GetError(), NULL, 0);
      throw ERR_INIT_AUDIO;
   }
   this->sound = Mix_LoadMUS(AUDIO_FILE);
   if(this->sound == NULL) {
      Alert(NULL, Mix_GetError(), NULL, 0);
      throw ERR_INIT_AUDIO_FILE;
   }
   // auto pause
   //Mix_PlayMusic(this->sound, -1);
}

void Game::loadTextures(const char * file, SDL_Texture ** ppTileset, TileProp * props, int nbTiles)
{
   int i;
   SDL_Surface* img;

   if (access(file, F_OK) != 0)
   {
      Alert(NULL, file, NULL, 0);
      throw ERR_INIT_TEXTURE_FILE;
   }

   img = SDL_LoadBMP(file);
   if (img == NULL)
   {
      Alert(NULL, file, NULL, 0);
      throw ERR_INIT_TEXTURE;
   }

   *ppTileset = SDL_CreateTextureFromSurface(this->renderer, img);
   if (*ppTileset == NULL)
   {
      Alert(NULL, SDL_GetError(), NULL, 0);
      throw ERR_INIT_TEXTURE_FILE;
   }
   SDL_FreeSurface(img);


   for(i=0; i<nbTiles; i++)
   {
      props[i].R.h  = TILE_S;
      props[i].R.w  = TILE_S;
      props[i].R.y  = 0;
      props[i].R.x  = TILE_S * i;
      props[i].type = i;
   }
}

void Game::setPlayers(int n)
{
   this->players.resize(n);
   this->players[PLAYER_A] = new Player(this, PLAYER_A);
   this->players[PLAYER_A]->setOpponent(NULL);

   if (n == 2)
   {
      this->players[PLAYER_B] = new Player(this, PLAYER_B);
      this->players[PLAYER_A]->setOpponent(this->players[PLAYER_B]);
      this->players[PLAYER_B]->setOpponent(this->players[PLAYER_A]);
   }
}

void Game::setMonsters(int n)
{
   int i;
   for (i=0; i<n; i++)
   {
      this->monsters.push_back(new Monster(this, i));
   }
}

void Game::addMonster()
{
   Monster* m = new Monster(this, this->monsters.size());
   this->monsters.push_back(m);
   m->live();
}

void Game::initMatrix()
{
   int i, j, t;
   FILE* f;

	f = fopen(MATRIX_FILE, "r");
	if (f == NULL)
	{
		throw ERR_INIT_MATRIX_FILE;
	}

	fscanf(f, "%d %d", &this->matrix_x, &this->matrix_y);

	/* set lines */
	this->matrix.resize(this->matrix_x);

   this->dots = 0;
   for(i=0; i<this->matrix_x; i++)
	{
      /* set columns */
      this->matrix[i].resize(this->matrix_y);

		for(j=0; j<this->matrix_y; j++)
      {
         /* read PX */
         fscanf(f, "%d", &t);
			if (t > NB_PX)
			{
				throw ERR_INIT_MATRIX;
			}
			/* set PX */
			this->matrix[i][j] = t;
			/* count dots */
			if (t == PX_DOT)
         {
            this->dots++;
			}
		}
	}
	fclose(f);
	this->endGame = 0;
}

void Game::displayInit()
{
   int x, y;
   SDL_Rect rect;

   rect.h = TILE_S;
   rect.w = TILE_S;

   // display matrix
   for(x=0; x<this->matrix_x; x++)
   {
      for(y=0; y<this->matrix_y; y++)
      {
         rect.y = TILE_S * x;
         rect.x = TILE_S * y;

         SDL_RenderCopy(this->renderer,
                        this->tileset,
                        &(this->props[this->matrix[x][y]].R),
                        &rect);
      }
   }

   SDL_RenderPresent(this->renderer);
}

void Game::display(int tic)
{
   int x, y;
   unsigned int p;
   SDL_Rect rect;
   Player  * player;
   Monster * monster;
   Position pos;


   rect.h = TILE_S;
   rect.w = TILE_S;

   // display matrix
   /*
   // render only around pacman
   for (p=0; p<this->players.size(); p++)
   {
      pos = this->players[p]->getPosition();
      for(x=(pos.x)-1; x<=(pos.x)+1; x++)
      {
         for(y=(pos.y)-1; y<=(pos.y)+1; y++)
         {
            rect.y = TILE_S * x;
            rect.x = TILE_S * y;

            SDL_RenderCopy(this->renderer,
                           this->tileset,
                           &(this->props[this->matrix[x][y]].R),
                           &rect);
         }
      }
   }
   // render only around pacman
   for (p=0; p<this->monsters.size(); p++)
   {
      pos = this->monsters[p]->getPosition();
      for(x=(pos.x)-1; x<=(pos.x)+1; x++)
      {
         for(y=(pos.y)-1; y<=(pos.y)+1; y++)
         {
            rect.y = TILE_S * x;
            rect.x = TILE_S * y;

            SDL_RenderCopy(this->renderer,
                           this->tileset,
                           &(this->props[this->matrix[x][y]].R),
                           &rect);
         }
      }
   }
   */
   for(x=0; x<this->matrix_x; x++)
   {
      for(y=0; y<this->matrix_y; y++)
      {
         rect.y = TILE_S * x;
         rect.x = TILE_S * y;

         SDL_RenderCopy(this->renderer,
                        this->tileset,
                        &(this->props[this->matrix[x][y]].R),
                        &rect);
      }
   }

   // display bonus
   rect.y = TILE_S * 17;
   rect.x = TILE_S * 14;
   SDL_RenderCopy(this->renderer,
                  this->tileset,
                  &(this->props[this->bonusType].R),
                  &rect);

   // display pacmans
   for (p=0; p<this->players.size(); p++)
   {
      player = this->players[p];
      rect.y = TILE_S * player->getPosition()->fx();
      rect.x = TILE_S * player->getPosition()->fy();
      SDL_RenderCopy(this->renderer,
                     this->tilesetPacman,
                     &(this->propsPacman[player->getTileProp(tic)].R),
                     &rect);
   }

      // display monsters
   for (p=0; p<this->monsters.size(); p++)
   {
      monster = this->monsters[p];
      rect.y = TILE_S * monster->getPosition()->fx();
      rect.x = TILE_S * monster->getPosition()->fy();
      SDL_RenderCopy(this->renderer,
                     this->tilesetMonster,
                     &(this->propsMonster[monster->getTileProp(tic)].R),
                     &rect);
   }

   // display scores
   this->displayScore();

   SDL_RenderPresent(this->renderer);
}

void Game::displayScore()
{
   SDL_Surface * surface;
   SDL_Texture * texture;
   SDL_Rect      rect;
   unsigned int p;

   ostringstream text("");

   text << "dots  : " << this->dots  << endl;
   text << "speed : " << this->speed << endl;

   /*
   for (p=0; p<this->nbPlayers; p++)
      text << "pac " << this->players[p]->getPosition().x << ";" << this->players[p]->getPosition().y << endl;
   for (p=0; p<this->nbMonsters; p++)
      text << "mob " << this->monsters[p]->getPosition().x << ";" << this->monsters[p]->getPosition().y << endl;
   */

   text << "Score : ";
   for (p=0; p<this->players.size(); p++)
      text << this->players[p]->getScore() << "    ";
   text << endl;

   surface = TTF_RenderText_Blended_Wrapped(this->font, text.str().c_str(), FONT_COLOR, TILE_S * SCORE_DISPLAY_L);
   texture = SDL_CreateTextureFromSurface(this->renderer, surface);

   // define display
   rect.h = surface->h;
   rect.w = surface->w;
   rect.y = TILE_S * (SCORE_DISPLAY_X);
   rect.x = TILE_S * (SCORE_DISPLAY_Y + this->matrix_y);

   // clean display
   SDL_RenderCopy(this->renderer,
                  this->tileset,
                  &(this->props[PX_VOID].R),
                  &rect);
   // print text
   SDL_RenderCopy(this->renderer, texture, NULL, &rect);
   SDL_FreeSurface(surface);
   SDL_DestroyTexture(texture);
}

void Game::play()
{
   int act[this->players.size()];
   Action action;
   int tic;
   unsigned int p, m;
   SDL_Event event;

   // purge queue events
   while(SDL_PollEvent(&event));
   tic = 0;

   for (p=0; p<this->players.size(); p++)
   {
      this->players[p]->live();
   }
   for (m=0; m<this->monsters.size(); m++)
   {
      this->monsters[m]->live();
   }

   while (this->endGame == 0)
   {
      // get actions
      action = this->getAction();
      if (action.player >= 0)
      {
         act[action.player] = action.action;
      }
      else if (action.action == ACTION_QUIT)
      {
         this->endGame = 1;
      }
      tic++;

      // players actions
      if (tic%2 == 0)
      {
         for (p=0; p<this->players.size(); p++)
         {
            this->players[p]->play(act[p]);

            for (m=0; m<this->monsters.size(); m++)
            {
               if (this->players[p]->getPosition()->collision(this->monsters[m]->getPosition()))
               {
                  if (this->monsters[m]->getWeak() == true)
                  {
                     this->monsters[m]->die();
                     delete this->monsters[m];
                     this->monsters.erase(this->monsters.begin()+m);
                  }
                  else
                  {
                     this->endGame = 1;
                  }
               }
            }
         }
      }

      if (tic%111 == 0) this->addMonster();

      // display 1/2 frame
      if ((tic+1)%2 == 0) this->display(tic);
      SDL_Delay(this->speed);
   }

   this->setPause();
}

void Game::addBonus(int tic)
{
   this->matrix[17][14] = PX_BONUS_1;
   this->bonusTic = tic;
   this->bonusType = PX_BONUS_1;
}

void Game::delBonus()
{
   this->matrix[17][14] = PX_VOID;
   this->bonusTic = 0;
   this->bonusType = PX_VOID;
}

void Game::eatDot(Position * pos)
{
   this->dots--;
   this->matrix[pos->x()][pos->y()] = PX_VOID;

   if (this->dots == 0)
      this->endGame = 1;
}

void Game::eatPower(Position * pos)
{
   this->setWeakMonsters();
   this->matrix[pos->x()][pos->y()] = PX_VOID;

}

void Game::setWeakMonsters()
{
   unsigned int m;
   for (m=0; m<this->monsters.size(); m++)
   {
      this->monsters[m]->setWeak();
   }
}

Action Game::getAction()
{
   Action action;

   int ret = ACTION_NONE;
   int player = -1;

   SDL_Event event;

   if (SDL_PollEvent(&event))
   {
      switch(event.type)
      {
         case SDL_WINDOWEVENT:
            // Set pause on focus lost
            if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST)
            {
               ret = this->actionPause(string("Pause..."));
            }
            break;
         case SDL_QUIT:
            ret = ACTION_QUIT;
            break;
         case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
               case SDLK_ESCAPE:
                  ret = this->actionPause(string("Pause..."));
                  break;
               case SDLK_m:
                  /* pause music */
                  if (this->pause == 1)
                  {
                     Mix_PlayMusic(this->sound, -1);
                     this->pause = 0;
                  }
                  /* resume music */
                  else
                  {
                     Mix_HaltMusic();
                     this->pause = 1;
                  }
                  ret = ACTION_NONE;
                  break;
               case SDLK_p:
               case SDLK_LALT:
                  ret = this->actionPause(string("Pause..."));
                  break;
               case SDLK_UP:
                  ret = ACTION_MOVE_UP;
                  player = PLAYER_A;
                  break;
               case SDLK_DOWN:
                  ret = ACTION_MOVE_DOWN;
                  player = PLAYER_A;
                  break;
               case SDLK_RIGHT:
                  ret = ACTION_MOVE_RIGHT;
                  player = PLAYER_A;
                  break;
               case SDLK_LEFT:
                  ret = ACTION_MOVE_LEFT;
                  player = PLAYER_A;
                  break;
               case SDLK_PAGEUP:
                  ret = ACTION_NONE;
                  this->setSpeed(this->speed - 1);
                  break;
               case SDLK_PAGEDOWN:
                  ret = ACTION_NONE;
                  this->setSpeed(this->speed + 1);
                  break;
               default:
                  ret = ACTION_NONE;
                  break;

            }
            break;
      }
   }
   // purge events in queue
   //while(SDL_PollEvent(&event));
   action.action = ret;
   action.player = player;
   return action;
}

void Game::setPause()
{
   unsigned int p, m;
   for (p=0; p<this->players.size(); p++)
   {
      this->players[p]->die();
   }
   for (m=0; m<this->monsters.size(); m++)
   {
      this->monsters[m]->die();
   }
}

void Game::unsetPause()
{
   unsigned int p, m;
   for (p=0; p<this->players.size(); p++)
   {
      this->players[p]->live();
   }
   for (m=0; m<this->monsters.size(); m++)
   {
      this->monsters[m]->live();
   }
}

int Game::actionPause(string str)
{
   int action = ACTION_NONE;
   SDL_Event event;

   // purge events in queue
   while(SDL_PollEvent(&event));

   this->setPause();

   // wait for end pause or quit
   while(action == ACTION_NONE)
   {
      SDL_WaitEvent(&event);
      switch(event.type)
      {
         case SDL_QUIT:
            action = ACTION_QUIT;
            break;
         case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
               case SDLK_ESCAPE:
                  action = ACTION_QUIT;
                  break;
               case SDLK_p:
               case SDLK_RETURN:
                  action = ACTION_PAUSE;
                  break;
               case SDLK_m:
                  /* pause music */
                  if (this->pause == 1)
                  {
                     Mix_PlayMusic(this->sound, -1);
                     this->pause = 0;
                  }
                  /* resume music */
                  else
                  {
                     Mix_HaltMusic();
                     this->pause = 1;
                  }
                  action = ACTION_NONE;
                  break;
               default:
                  action = ACTION_NONE;
                  break;
            }
            break;
      }
      this->display(0);
      SDL_Delay(5);
   }

   if (action != ACTION_QUIT)
   {
      this->unsetPause();
   }

   return action;
}

void Game::setSpeed(int speed)
{
   this->speed = (speed > 0) ? speed : 0;
}
