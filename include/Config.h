#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#define WINDOW_TITLE "Pacman"

#define DT_USLEEP  30000


#define MATRIX_FILE "ressources/matrix.txt"

#define SCORE_DISPLAY_Y 1
#define SCORE_DISPLAY_X 1
#define SCORE_DISPLAY_L 11

#define STEP 2

#define FONT_SIZE 22
#define FONT_FILE "ressources/font.dat"
#define FONT_COLOR (SDL_Color){128,128,128}

#define TILESET_FILE "ressources/world.dat"
#define TILE_S 26

#define AUDIO_FILE "ressources/music.dat"
#define AUDIO_RATE 22050
#define AUDIO_FORMAT AUDIO_S16SYS
/* mono */
#define AUDIO_CHANNELS 1
#define AUDIO_BUFFERS 4096

#define SCORE_FILENAME "scores.dat"
#define NB_HIGHSCORE 4
#define SCORE_PIECE 10
#define SCORE_DROP  1
#define SCORE_LINE  100
#define SCORE_BONUS 100

#define PLAYER_A  0
#define PLAYER_B  1


/**** PIXELS ******************************/

#define PX_VOID      (0)
#define PX_DOT       (1 + PX_VOID)
#define PX_POWER     (1 + PX_DOT)

#define PX_BONUS_1   (1 + PX_POWER)
#define PX_BONUS_2   (1 + PX_BONUS_1)
#define PX_BONUS_3   (1 + PX_BONUS_2)
#define PX_BONUS_4   (1 + PX_BONUS_3)
#define PX_BONUS_5   (1 + PX_BONUS_4)
#define PX_BONUS_6   (1 + PX_BONUS_5)
#define PX_BONUS_7   (1 + PX_BONUS_6)
#define PX_BONUS_8   (1 + PX_BONUS_7)

#define PX_SOLID     (PX_BONUS_8)  /* Pac cannot go > PX_SOLID */

#define PX_EDGE_C1   (1 + PX_BONUS_8)
#define PX_EDGE_C2   (1 + PX_EDGE_C1)
#define PX_EDGE_C3   (1 + PX_EDGE_C2)
#define PX_EDGE_C4   (1 + PX_EDGE_C3)
#define PX_EDGE_T1   (1 + PX_EDGE_C4)
#define PX_EDGE_T2   (1 + PX_EDGE_T1)
#define PX_EDGE_T3   (1 + PX_EDGE_T2)
#define PX_EDGE_T4   (1 + PX_EDGE_T3)
#define PX_EDGE_I1   (1 + PX_EDGE_T4)
#define PX_EDGE_I2   (1 + PX_EDGE_I1)
#define PX_EDGE_X    (1 + PX_EDGE_I2)

#define PX_PACMAN    (1 + PX_EDGE_X)

#define NB_PX        (1 + PX_PACMAN)

/**** CHARACTERS --************************/

#define PACMAN_FILE  "ressources/world-pac.dat"
#define NB_C_PACMAN  4

#define MONSTER_FILE  "ressources/world-mob.dat"
#define NB_C_MONSTER 2
#define NB_MONSTER   4
/* including weak monster */
#define NB_MONSTER2  (NB_MONSTER+1)

/**** USER ACTIONS ************************/

#define ACTION_NONE        -1
#define ACTION_QUIT        99
#define ACTION_PAUSE       98
#define ACTION_STOP         0
#define ACTION_MOVE_RIGHT   1
#define ACTION_MOVE_LEFT    2
#define ACTION_MOVE_DOWN    3
#define ACTION_MOVE_UP      4

#define ACTION_NOT_RIGHT   ACTION_MOVE_LEFT
#define ACTION_NOT_LEFT    ACTION_MOVE_RIGHT
#define ACTION_NOT_DOWN    ACTION_MOVE_UP
#define ACTION_NOT_UP      ACTION_MOVE_DOWN

#define MONSTER_WEAK       100

#endif // CONFIG_H_INCLUDED
