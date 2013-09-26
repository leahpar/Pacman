#include <unistd.h>
#include <string>
#include <sstream>

#include <stdio.h> /* need fscanf ! */

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "Game.h"
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

   this->nbPlayers = 0;

   this->speed = 10;

}

Game::~Game()
{
   int i;

   for (i=0; i<this->nbPlayers; i++)
   {
      delete this->players[i];
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
                                   this->matrix_y * TILE_S,
                                   this->matrix_x * TILE_S,
                                   0);
   this->renderer = SDL_CreateRenderer(this->screen, -1, 0);
   this->loadTextures();
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

void Game::loadTextures()
{
   int i;
   SDL_Surface* img;

   if (access(TILESET_FILE, F_OK) != 0)
   {
      Alert(NULL, "Could not open '" TILESET_FILE "'", NULL, 0);
      throw ERR_INIT_TEXTURE_FILE;
   }

   img = SDL_LoadBMP(TILESET_FILE);
   if (img == NULL)
   {
      Alert(NULL, "Could not load '" TILESET_FILE "'", NULL, 0);
      throw ERR_INIT_TEXTURE;
   }

   this->tileset = SDL_CreateTextureFromSurface(this->renderer, img);
   if (this->tileset == NULL)
   {
      Alert(NULL, SDL_GetError(), NULL, 0);
      throw ERR_INIT_TEXTURE_FILE;
   }
   SDL_FreeSurface(img);


   for(i=0; i<NB_PX; i++)
   {
      this->props[i].R.h  = TILE_S;
      this->props[i].R.w  = TILE_S;
      this->props[i].R.y  = 0;
      this->props[i].R.x  = TILE_S * i;
      this->props[i].type = i;
   }
}

void Game::setPlayers(int n)
{
   this->nbPlayers = n;

   this->players[PLAYER_A] = new Player(this, PLAYER_A);
   this->players[PLAYER_A]->setOpponent(NULL);

   if (n == 2)
   {
      this->players[PLAYER_B] = new Player(this, PLAYER_B);
      this->players[PLAYER_A]->setOpponent(this->players[PLAYER_B]);
      this->players[PLAYER_B]->setOpponent(this->players[PLAYER_A]);
   }
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

   for(i=0; i<this->matrix_x; i++)
	{
      /* set columns */
      this->matrix[i].resize(this->matrix_y);

		for(j=0; j<this->matrix_y; j++)
      {
         /* set PX */
			fscanf(f, "%d", &t);
			if (t > NB_PX)
			{
				throw ERR_INIT_MATRIX;
			}
			this->matrix[i][j] = t;
		}
	}
	fclose(f);
}

void Game::display()
{
   int x, y;
   int p;
   SDL_Rect rect;
   Player * player;
   Position pos;
   //SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
   //SDL_RenderClear(this->renderer);

   rect.h = TILE_S;
   rect.w = TILE_S;

   // display matrix
   // render only around pacman
   for (p=0; p<this->nbPlayers; p++)
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

   // display pacman(s)
   for (p=0; p<this->nbPlayers; p++)
   {
      player = this->players[p];
      rect.y = TILE_S * player->getPosition().x
              + player->getPosition().subpos *
                  ((player->getCurDirection() == ACTION_MOVE_DOWN)
                  - (player->getCurDirection() == ACTION_MOVE_UP));
      rect.x = TILE_S * player->getPosition().y
           + player->getPosition().subpos *
                  ((player->getCurDirection() == ACTION_MOVE_RIGHT)
                  - (player->getCurDirection() == ACTION_MOVE_LEFT));
      SDL_RenderCopy(this->renderer,
               this->tileset,
               &(this->props[PX_PACMAN].R),
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
   int p;

   ostringstream text("");

   text << "DEBUG" << endl;

   text << "speed : " << this->speed << endl;

   text << "score : " << endl;
   for (p=0; p<this->nbPlayers; p++)
      text << this->players[p]->getScore() << "    ";
   text << endl;

   text << "level : " << endl;
   for (p=0; p<this->nbPlayers; p++)
      text << this->players[p]->getLevel() << "    ";
   text << endl;

   surface = TTF_RenderText_Blended_Wrapped(this->font, text.str().c_str(), FONT_COLOR, SCORE_DISPLAY_L);
   texture = SDL_CreateTextureFromSurface(this->renderer, surface);

   // display texture
   rect.h = surface->h;
   rect.w = surface->w;
   rect.y = SCORE_DISPLAY_X;
   rect.x = SCORE_DISPLAY_Y;
   SDL_RenderCopy(this->renderer, texture, NULL, &rect);
   SDL_FreeSurface(surface);
   SDL_DestroyTexture(texture);
}

void Game::play()
{
   int act[this->nbPlayers];
   Action action;
   int i;
   int p;
   SDL_Event event;

   // purge queue events
   while(SDL_PollEvent(&event));

   i = 0;
   while (1)
   {
      action = this->getAction();
      if (action.player >= 0)
      {
         act[action.player] = action.action;
      }
      else if (action.action == ACTION_QUIT)
      {
         return;
      }
      i++;

      // play
      if (i >= TILE_S)
      {
         for (p=0; p<this->nbPlayers; p++)
         {
            if (this->players[p]->play(act[p]) == 1)
            {
               return;
            }
            //act[p] = ACTION_NONE;
         }

         i = 0;
      }
      for (p=0; p<this->nbPlayers; p++)
      {
         this->players[p]->movePac();
      }

      // display 1/2 frame
      if (i%2 == 0) this->display();
      SDL_Delay(this->speed);
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
                  this->speed--;
                  break;
               case SDLK_PAGEDOWN:
                  ret = ACTION_NONE;
                  this->speed++;
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

int Game::actionPause(string str)
{
   int action = ACTION_NONE;
   SDL_Event event;

   // purge events in queue
   while(SDL_PollEvent(&event));

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
      this->display();
      SDL_Delay(10);
   }
   return action;
}

