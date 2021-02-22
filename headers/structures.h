/*******************************************************************************
  @file     structures.h
  @brief    
  @author   Alejandro Alatsis, Axel Cincunegui, Facundo Molina, Magali Pompozzi 
 ******************************************************************************/

#ifndef STRUCTURES_H
#define STRUCTURES_H

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

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


// ---------------------     board constants      --------------------- //
#define MAX_SHOTS       50
#define MAX_ENEMIES     50
#define MAX_MURO        50  
#define MAX_EXPLOSIONS  10
#define CORRECCION      30
  
// --------------------------     display      -------------------------- //
#define BUFFER_W 800
#define BUFFER_H 620
#define DISP_SCALE 1
#define DISP_W (BUFFER_W * DISP_SCALE)
#define DISP_H (BUFFER_H * DISP_SCALE)


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

// ---------------------     estados de menu      --------------------- //
enum menu_states {STATE_START, STATE_MENU, STATE_PLAY, STATE_STATS, STATE_EXIT, STATE_PAUSE, STATE_GAMEOVER};
enum transitions_states {TRANSITION_LEVELUP = 7, TRANSITION_LEVELSTART, TRANSITION_GAMEOVER};

// ---------------------     estructura graficos      --------------------- //
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
    ALLEGRO_BITMAP* start_bitmap;
    ALLEGRO_BITMAP* gameover_bitmap;
} graphics_t;

// ---------------------     estructuras botones      --------------------- //
typedef struct {
    int y;  //posicion en y
    int h;  //altura
    const char * text;  //texto de boton
    ALLEGRO_FONT * font;    //fuente
    int keyboard; //flag de posicion sobre boton de menu
} button_t; 


// -------------------     estructura de display      ------------------ //
typedef struct {
    ALLEGRO_DISPLAY* disp;
    ALLEGRO_BITMAP* buffer;
} display_t;

// -------------------     estructura de audio      ------------------ //

typedef struct{
    ALLEGRO_SAMPLE* playershot_sound;
    ALLEGRO_SAMPLE* gameover_sound;
    ALLEGRO_SAMPLE* navnod_sound;
    ALLEGRO_SAMPLE* levelup_sound;
} audio_t;

// ---------     estructura de gestion de elementos en tablero      --------- //
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

#ifdef __cplusplus
}
#endif

#endif  /*STRUCTURES_H*/ 


  

  




  
  


