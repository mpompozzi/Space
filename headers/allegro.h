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
  
#include "draw.h" 
#include "logic.h" 

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
  
// --------------------------     bitmap      -------------------------- //
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
  
// --------------------------     audio      -------------------------- //
#define GAME_SOUND "../../../res/audio/"
#define OPTION_SOUND "../../../res/audio/"
#define SHOT_SOUND "res/audio/disparo.wav"
#define COLLISION_SOUND "res/audio/naveImpacto.wav"
#define GAMEOVER_SOUND "res/audio/gameOver.wav"
  

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*
 * must_init:   control de inicializacion. Imprime en pantalla si no se 
 *              inicializo correctamente la funcion.
 * Recibe:  variable inicializada y mensaje a imprimir de haber error.
*/
void must_init(bool test, const char *description);


/* ------------------     DISPLAY      ------------------ //
 * disp_init:   inicializacion variables de display
 * disp_deinit:   destruccion de tablero y buffer.
 * disp_predraw:   imprime elementos en buffer.
 * Recibe:  puntero a estructura con display y buffer.
*///----------------------------------------------------- //
void disp_init(display_t * display);
void disp_deinit(display_t * display);
void disp_pre_draw(display_t * display);
void disp_post_draw(display_t * display);


/* ------------------     AUDIO      ------------------ //
 * audio_init:   inicializa elementos de audio.
 * audio_deinit:    destruye elementos de audio.
 * Recibe:  puntero a estructura con elementos de audios.
*///----------------------------------------------------- //
void audio_init(audio_t * audio);
void audio_deinit(audio_t * audio);

/* ------------------     GRAFICOS      ------------------ //
 * graphics_init:   inicializa elementos de graficos bitmaps
 * graphics_deinit: destruye elementos de graficos bitmaps.
 * Recibe:  puntero a estructura con bitmaps.
*///----------------------------------------------------- //
void graphics_init(graphics_t * graphics);
void graphics_deinit(graphics_t * graphics);

/* ---------------- DIBUJO DE PUNTAJE Y VIDAS ---------------- //
 * hud_init:   inicializa fuente TrueTypeFormat.
 * hud_deinit:  destruye fuente cargada.
 * hud_draw: imprime en pantalla vidas y puntaje del jugador.
*///---------------------------------------------------------- //
ALLEGRO_FONT * hud_init(ALLEGRO_FONT* font);  //recibe fuente
void hud_deinit(ALLEGRO_FONT* font);
void hud_draw(ALLEGRO_FONT* font, juego_t * juego, graphics_t * graphics); //recibe fuente, puntero a juego y graficos


/* ---------------- INICIALIZACION BOARD STRUCT ---------------- //
 * board_init:   inicializa en cero objetos de elementos de la estructura.
 * Recibe: puntero a estructura board
*///---------------------------------------------------------- //
void board_init(board_t * board);




//void draw_all(juego_t * juego, graphics_t * graphics, board_t * board);

/*******************************************************************************
 ******************************************************************************/




#ifdef __cplusplus
}
#endif

#endif /* ALLEGRO_H */
