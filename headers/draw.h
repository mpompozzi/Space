/*******************************************************************************
  @file     draw.h
  @brief    
  @author   Alejandro Alatsis, Axel Cincunegui, Facundo Molina, Magali Pompozzi 
 ******************************************************************************/

#ifndef DRAW_H
#define DRAW_H

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
 
#include "backend.h"   
#include "structures.h"  

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
  
// --------------------------     colores      -------------------------- //
#define BLACK   al_map_rgb(0,0,0)
#define WHITE   al_map_rgb(155,155,155)
#define RED     al_map_rgb(255,0,0)
#define GREEN   al_map_rgb(0,155,0)

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

// +ej: extern unsigned int anio_actual;+


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/* ---------------- DIBUJO DE PUNTAJE Y VIDAS ---------------- //
 * hud_draw: imprime en pantalla vidas y puntaje del jugador.
 * Recibe: fuente, puntero a juego y graficos
*///---------------------------------------------------------- //
void hud_draw(ALLEGRO_FONT* font, juego_t * juego, graphics_t * graphics); 


/*-------------- DIBUJO DE BOTONES Y FONDOS DE ESTADOS -------------- //
 * menu_draw: dibuja botones, texto y fondos principales de estados.
 * Recibe: puntero a evento, arreglo de estructuras botones, estructura de graficos y juego.
*///----------------------------------------------------------------- //
void menu_draw(button_t * buttons[], graphics_t * graphics, juego_t * juego, int game_states);


/* --------------     DIBUJO DE JUGADOR      -------------- //
 * player_draw: ubica imagen de jugador en diplay.
 * Recibe: puntero a estructura juego y graficos.
*///------------------------------------------------------- //
void player_draw(juego_t * juego, graphics_t * graphics);


/* -------------- DIBUJO DE ENEMIGOS -------------------- //
 * enemies_draw: ubica imagen de enemigo en diplay.
 * Recibe: puntero a estructura graficos y board.
*///----------------------------------------------------- //
void enemies_draw(graphics_t * graphics, board_t * board);


/* ----------------- DIBUJO NAVE NODRIZA ----------------- //
 * navnod_draw: ubica imagen de nave nodriza en display.
 * Recibe: puntero a estructura graficos y board.  
*///----------------------------------------------------- //
void navnod_draw(graphics_t * graphics, board_t * board);


/* -------------     DIBUJO DE DISPAROS      ------------- //
 * shots_draw: ubica primitivas de disparos en display.
 * Recibe: puntero a estructura graficos y board.  
*///------------------------------------------------------ //
void shots_draw(graphics_t * graphics, board_t * board);


/* ---------------     DIBUJO DE MURO      --------------- //
 * muro_draw: dibuja bloques de muros en display.
 * Recibe: puntero a estructura graficos y board.  
*///------------------------------------------------------ //
void muro_draw(graphics_t * graphics, board_t * board);


/* -------------- ACTUALIZACION DE EXPLOSIONES  -------------- //
 * explosion_draw: dibuja explosiones en display.
 * Recibe: puntero a estructura graficos y board.   
*///---------------------------------------------------------- //
void explosion_draw(graphics_t * graphics, board_t * board);

/*******************************************************************************
 ******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* DRAW_H */