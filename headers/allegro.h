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

// --------------------------     colores      -------------------------- //
#define BLACK   al_map_rgb(0,0,0)
#define WHITE   al_map_rgb(155,155,155)
#define RED     al_map_rgb(255,0,0)
#define GREEN   al_map_rgb(0,155,0)
  
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
  
// --------------------------     display      -------------------------- //
#define BUFFER_W 800
#define BUFFER_H 600
#define DISP_SCALE 1
#define DISP_W (BUFFER_W * DISP_SCALE)
#define DISP_H (BUFFER_H * DISP_SCALE)

// --------------------------     teclado      -------------------------- //
# define KEY_SEEN            1   //tecla presionada
# define KEY_RELEASED        2  //tecla liberada
  
// ---------------------     board constants      --------------------- //
#define MAX_SHOTS       20
#define MAX_ENEMIES     50
#define MAX_MURO        50  
#define MAX_EXPLOSIONS  10
  
// -------------------     constantes graficacion      ------------------- //  
#define CELL    20
#define SCALE 25

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
  
  
// ---------------------     estados de menu      --------------------- //
enum menu_states {STATE_START, STATE_MENU, STATE_PLAY, STATE_STATS, STATE_EXIT, STATE_PAUSE, STATE_GAMEOVER};

// -------------------     constantes de movimiento      ------------------- //
enum player_move {STAY, RIGHT, LEFT, SHOOT, SHOOT_RIGHT, SHOOT_LEFT};

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
    ALLEGRO_SAMPLE* game_sound;
    ALLEGRO_SAMPLE* option_sound;
    ALLEGRO_SAMPLE* shot_sound;
    ALLEGRO_SAMPLE* collision_sound;
    ALLEGRO_SAMPLE* gameover_sound;
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

/* ------------------     TECLADO      ------------------ //
 * keyboard_init: inicializacion en cero de arreglos de gestion de teclado.
 * keyboard_update: actualizacion de tecla presionada o liberada en arreglo.
 * keyboard_counter: control de sensibilidad de teclado.
*///----------------------------------------------------- //
void keyboard_init(void);
void keyboard_update(ALLEGRO_EVENT* event); //recibe puntero a evento de Allegro
int keyboard_counter(unsigned char keyName); //recibe tecla presionada

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
void hud_init(ALLEGRO_FONT* font);  //recibe fuente
void hud_deinit(ALLEGRO_FONT* font);
void hud_draw(ALLEGRO_FONT* font, juego_t * juego, graphics_t * graphics); //recibe fuente, puntero a juego y graficos

/****************************************************************************
*---------------------------- LOGICA DE JUEGO ----------------------------
****************************************************************************/

/* ------------ ACTUALIZACION DE ACCION DE JUGADOR ------------ //
 * game_update:  gestiona acciones de jugador y paso de nivel. Llama a funciones
 *               del back pmov y verparams par amovimiento de jugador y ininiv
 *               cuando ocurre transicion de nivel.
 * Recibe: puntero a estructura juego.
*///----------------------------------------------------------- //
void game_update(juego_t * juego);

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
void menu_update(ALLEGRO_EVENT * ev, juego_t * juego, button_t * buttons[]);


/* -------------- ACTUALIZACION DE NAVE NODRIZA -------------- //
 * call_nod: genera el llamado aleatorio para la nave nodriza y la mueve en el tablero.
*///---------------------------------------------------------- //
void call_nod(void);


/* --------- ACTUALIZACION DE VELOCIDADES ENEMIGOS --------- //
 * vel_nod: administracion de velocidad de timer de enemigos segun cantidad de 
 *          enemigos en juego.'
 * Recibe: puntero a estructura juego y board. 
*///------------------------------------------------------- //
void vel_nod(juego_t * juego, board_t * board);


/*-------------- DIBUJO DE BOTONES Y FONDOS DE ESTADOS -------------- //
 * menu_draw: dibuja botones, texto y fondos principales de estados.
 * Recibe: puntero a evento, arreglo de estructuras botones, estructura de graficos y juego.
*///----------------------------------------------------------------- //
void menu_draw(button_t * buttons[], graphics_t * graphics, juego_t * juego);


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


/* ------------ GESTION MOVIMIENTO DE JUGADOR Y VIDAS ------------ //
 * move_player: verifica teclas presionadas y activa flag de accion de jugador.
 * Recibe: puntero a estructura juego.
*///-------------------------------------------------------------- //
void move_player(juego_t * juego);

/*******************************************************************************
 ******************************************************************************/




#ifdef __cplusplus
}
#endif

#endif /* ALLEGRO_H */
