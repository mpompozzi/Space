/*******************************************************************************
  @file     allegro.h
  @brief    
  @author   Alejandro Alatsis, Axel Cincunegui, Facundo Molina, Magali Pompozzi 
 ******************************************************************************/

#ifndef ALLEGRO_H
#define ALLEGRO_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h> 

#include "backend.h" 

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

// --- colors ---
#define BLACK   al_map_rgb(0,0,0)
#define WHITE   al_map_rgb(155,155,155)
#define RED     al_map_rgb(255,0,0)
#define GREEN   al_map_rgb(0,155,0)
  
// --- bitmap ---
#define MENU_BACKGROUND "res/img/menuback.png"
#define GAME_BACKGROUND "res/img/invaders.png"
#define ENEMY_BMP "res/img/saucer1a.png"
#define ENEMY2_BMP "res/img/saucer2a.png"
#define ENEMY3_BMP "res/img/saucer3a.png"
#define NAVNOD_BMP "res/img/mysterya.png"
#define PLAYER_BMP "res/img/baseshipa.png"
#define MURO_BMP "res/img/block.png"
#define PLAYERKILLED_BMP "res/img/playerkilled.png"
#define ENEMYKILLED_BMP "res/img/enemykilled.png"
#define NAVNODKILLED_BMP "res/img/nodkilled.png"
#define LEVELUP_BMP "res/img/level_up2.png" 
#define VIDA_BMP "res/img/vida.png"  
#define LEVELUP_BMP "res/img/level_up2.png"  
  
// --- audio ---
#define GAME_SOUND "../../../res/audio/"
#define OPTION_SOUND "../../../res/audio/"
#define SHOT_SOUND "res/audio/disparo.wav"
#define COLLISION_SOUND "res/audio/naveImpacto.wav"
#define GAMEOVER_SOUND "res/audio/gameOver.wav"
  
// --- display ---
#define BUFFER_W 800
#define BUFFER_H 800
#define DISP_SCALE 1
#define DISP_W (BUFFER_W * DISP_SCALE)
#define DISP_H (BUFFER_H * DISP_SCALE)

// --- keyboard ---
// Key detection options
/// A key has been pressed
#    define KEY_SEEN            1
/// The pressed key has been released
#    define KEY_RELEASED        2
/// A key has been pressed and released. Actions can be taked
#    define KEY_READY           ( (KEY_SEEN | KEY_RELEASED) & KEY_RELEASED)
  
// --- keyboard ---
#define MOUSE       0
#define KEYBOARD    1
  

// --- board constants ---
#define MAX_SHOTS       20
#define MAX_ENEMIES     50
#define MAX_MURO        50  
#define MAX_EXPLOSIONS  10
  
// --- draw constants ---  
#define CELL    20
#define SCALE 25

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

enum menu_states {STATE_START, STATE_MENU, STATE_PLAY, STATE_STATS, STATE_EXIT, STATE_PAUSE, STATE_GAMEOVER};

enum player_move {STAY, RIGHT, LEFT, SHOOT, SHOOT_RIGHT, SHOOT_LEFT};

// --- graphic struct ---
typedef struct {
    ALLEGRO_BITMAP* menu_background;
    ALLEGRO_BITMAP* game_background;
    ALLEGRO_BITMAP* enemy_bitmap;
    ALLEGRO_BITMAP* enemy2_bitmap;
    ALLEGRO_BITMAP* enemy3_bitmap;
    ALLEGRO_BITMAP* navnod_bitmap;
    ALLEGRO_BITMAP* player_bitmap;
    ALLEGRO_BITMAP* muro_bitmap;
    ALLEGRO_BITMAP* playerkilled_bitmap;
    ALLEGRO_BITMAP* enemykilled_bitmap;
    ALLEGRO_BITMAP* navnodkilled_bitmap;
    ALLEGRO_BITMAP* vida_bitmap;
    ALLEGRO_BITMAP* levelup_bitmap;
    
} graphics_t;

// --- buttons struct ---
typedef struct {
    int y;
    int h;
    const char * text;
    ALLEGRO_FONT * font;
    int keyboard; //flag de posicion sobre boton de menu
} button_t; 

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

// +ej: extern unsigned int anio_actual;+


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief TODO: completar descripcion
 * @param param1 Descripcion parametro 1
 * @param param2 Descripcion parametro 2
 * @return Descripcion valor que devuelve
*/
// +ej: char lcd_goto (int fil, int col);+


/*******************************************************************************
 ******************************************************************************/

// ---------------     display struct      --------------- //
typedef struct {
    ALLEGRO_DISPLAY* disp;
    ALLEGRO_BITMAP* buffer;
} display_t;

// ---------------     keyboard struct      --------------- //
/*
typedef struct {
    unsigned char key[ALLEGRO_KEY_MAX];
    unsigned char counterKey[ALLEGRO_KEY_MAX];
} keyboard_t;*/

typedef struct{
    ALLEGRO_SAMPLE* game_sound;
    ALLEGRO_SAMPLE* option_sound;
    ALLEGRO_SAMPLE* shot_sound;
    ALLEGRO_SAMPLE* collision_sound;
    ALLEGRO_SAMPLE* gameover_sound;
} audio_t;

// ---------------     enemies struct      --------------- //
typedef struct{
    ALLEGRO_TIMER* timer_enemy;
    coord_t pshot;
    coord_t muro[MAX_MURO];
    coord_t enemy_cell [MAX_ENEMIES]; 
    coord_t enemy_shot [MAX_SHOTS];
    coord_t navnod;
    coord_t explosion [MAX_EXPLOSIONS];
    int   enemy_maxshots;
    int   enemy_maxcells;
} board_t;
  

// --- enemies struct ---
typedef struct {
  ALLEGRO_TIMER * timers  [10];
  coord_t cell [MAX_ENEMIES]; 
  coord_t shot [MAX_SHOTS];
  coord_t navnod;
  int   max_shots;
  int   max_enemies;
} enemylogic_t;

void must_init(bool test, const char *description);
void disp_init(display_t * display);
void disp_deinit(display_t * display);
void disp_pre_draw(display_t * display);
void disp_post_draw(display_t * display);
int keyboard_counter(unsigned char keyName);
void keyboard_init(void);
void keyboard_update(ALLEGRO_EVENT* event);
void audio_init(audio_t * audio);
void audio_deinit(audio_t * audio);
void graphics_deinit(graphics_t * graphics);
void hud_init(void);
void hud_deinit(void);
void hud_draw(juego_t * juego, graphics_t * graphics);
void move_player(juego_t * juego);
void game_update(juego_t * juego);
void board_update(juego_t * juego, board_t * board);
void shots_update(juego_t * juego, board_t * board);
void enemies_update(juego_t * juego);
void menu_update(ALLEGRO_EVENT ev, juego_t * juego, button_t * buttons[]);
void menu_draw(ALLEGRO_EVENT ev, button_t * buttons[], graphics_t * graphics, juego_t * juego);
void player_draw(juego_t * juego, graphics_t * graphics);
void enemies_draw(graphics_t * graphics, board_t * board);
void navnod_draw(graphics_t * graphics, board_t * board);
void call_nod(void);
void shots_draw(graphics_t * graphics, board_t * board);
void muro_draw(graphics_t * graphics, board_t * board);
void vel_nod(juego_t * juego, board_t * board);
void explosion_draw(graphics_t * graphics, board_t * board);
#ifdef __cplusplus
}
#endif

#endif /* ALLEGRO_H */
