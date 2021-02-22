/*******************************************************************************
  @file     logic.h
  @brief    
  @author   Alejandro Alatsis, Axel Cincunegui, Facundo Molina, Magali Pompozzi 
 ******************************************************************************/

#ifndef LOGIC_H
#define LOGIC_H

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
  

// --------------------------     teclado      -------------------------- //
# define KEY_SEEN            1   //tecla presionada
# define KEY_RELEASED        2  //tecla liberada
  
  
/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
  
// -------------------     constantes de movimiento      ------------------- //
enum player_move {STAY, RIGHT, LEFT, SHOOT, SHOOT_RIGHT, SHOOT_LEFT};
    


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

// +ej: extern unsigned int anio_actual;+


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/* ------------------     TECLADO      ------------------ //
 * keyboard_init: inicializacion en cero de arreglos de gestion de teclado.
 * keyboard_update: actualizacion de tecla presionada o liberada en arreglo.
 * keyboard_counter: control de sensibilidad de teclado.
*///----------------------------------------------------- //
void keyboard_init(void);
void keyboard_update(ALLEGRO_EVENT* event); //recibe puntero a evento de Allegro
int keyboard_counter(unsigned char keyName, int game_states); //recibe tecla presionada y estado

/****************************************************************************
*---------------------------- LOGICA DE JUEGO ----------------------------
****************************************************************************/

/* ------------ ACTUALIZACION DE ACCION DE JUGADOR ------------ //
 * game_update:  gestiona acciones de jugador y paso de nivel. Llama a funciones
 *               del back pmov y verparams par amovimiento de jugador y ininiv
 *               cuando ocurre transicion de nivel.
 * Recibe: puntero a estructura juego.
*///----------------------------------------------------------- //
int game_update(juego_t * juego, int);

/* ---------------- ACTUALIZACION DE TABLERO --------------- //
 * board_update: lee tablero desde backend y guarda elementos en front. Llama a 
 *               funcion getmat del back para leer elemento en una celda.
 * Recibe: puntero a estructura juego y estructura board.
*///-------------------------------------------------------- //
void board_update(juego_t * juego, board_t * board);


/* ------------ ACTUALIZACION DE DISPAROS Y COLISIONES ------------ //
 * shots_update: mueve disparos de jugador y enemigos y notifica colisiones. Llama
 *               a funciones eventop y eventoe del backend. 
 * Recibe: puntero a estructura juego y estructura board para guardar colisiones.
*///--------------------------------------------------------------- //
void shots_update(juego_t * juego, board_t * board);

/* --------------     ACTUALIZACION DE ENEMIGOS      -------------- //
 * enemies_update: disparo aleatorio de enemigos (llamando a navdisp() del back)
 *                 y movimiento de enemigos con ciclonaves del back.  
 * Recibe: puntero a estructura juego.
*///--------------------------------------------------------------- //
void enemies_update(juego_t * juego);


/* --------------     ACTUALIZACION DE MENU Y ESTADOS      -------------- //
 * menu_update: administracion de estados START (ventana de inicio), MENU (menu principal), 
 *              PLAY (juego), PAUSE (pausa en juego), STATS (puntajes maximos) y
 *              GAMEOVER (transicion de juego finalizado). Mueve a traves de sus
 *              botones y determina el pase a otro estado.
 * Recibe: puntero a evento, estructura juego y arreglo de estructuras botones. 
*///--------------------------------------------------------------------- //
int menu_update(ALLEGRO_EVENT * ev, juego_t * juego, button_t * buttons[], int);


/* --------------------- ACTUALIZACION DE NAVE NODRIZA --------------------- //
 * call_nod: genera el llamado aleatorio para la nave nodriza y la mueve en el tablero.
*///------------------------------------------------------------------------- //
void call_nod(void);


/* --------- ACTUALIZACION DE VELOCIDADES ENEMIGOS --------- //
 * vel_nod: administracion de velocidad de timer de enemigos segun cantidad de 
 *          enemigos en juego.'
 * Recibe: puntero a estructura juego y board. 
*///------------------------------------------------------- //
void vel_nod(juego_t * juego, board_t * board);


/* ------------ GESTION MOVIMIENTO DE JUGADOR Y VIDAS ------------ //
 * move_player: verifica teclas presionadas y activa flag de accion de jugador.
 * Recibe: puntero a estructura juego.
*///-------------------------------------------------------------- //
void move_player(juego_t * juego, int game_states);

/* ----------------- AUDIO DEL JUEGO ----------------- //
 * call_audio: esta funcion reprodice la pista de audio segun el evento que se 
 * requiere.
*///------------------------------------------------------------------------ //
void call_audio(audio_t * audio,board_t * board,int game_states);

/*******************************************************************************
 ******************************************************************************/


#ifdef __cplusplus
}
#endif

#endif /* LOGIC_H */
