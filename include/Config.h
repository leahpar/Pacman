#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#define WINDOW_TITLE "Pacman beta"

#define DT_USLEEP  30000


#define MATRIX_FILE "matrix.txt"

#define SCORE_DISPLAY_Y (3*TILE_S)
#define SCORE_DISPLAY_X (3*TILE_S)
#define SCORE_DISPLAY_L (10*TILE_S)

#define STEP 2

#define FONT_SIZE 22
#define FONT_FILE "tetris.ttf"
#define FONT_COLOR (SDL_Color){128,128,128}

#define TILESET_FILE "texture2.bmp"
#define TILE_S 26

#define AUDIO_FILE "tetris.ogg"
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

/**** USER ACTIONS ************************/
#define ACTION_NONE        -1
#define ACTION_QUIT        99
#define ACTION_PAUSE       98
#define ACTION_STOP         0
#define ACTION_MOVE_UP      1
#define ACTION_MOVE_DOWN    2
#define ACTION_MOVE_LEFT    3
#define ACTION_MOVE_RIGHT   4

#endif // CONFIG_H_INCLUDED
