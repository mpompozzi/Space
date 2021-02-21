/*******************************************************************************
  @file     draw.c
  @brief    Funciones de graficacion de menu, player, disparos y enemigos. 
  @author   Alejandro Alatsis, Axel Cincunegui, Facundo Molina, Magali Pompozzi 
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "../../../headers/draw.h" 


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

// -------------------     constantes graficacion      ------------------- //  
#define CELL    20
#define SCALE 25

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/



/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

// +ej: unsigned int anio_actual;+


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static void falta_envido (int);+


/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static const int temperaturas_medias[4] = {23, 26, 24, 29};+


/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static int temperaturas_actuales[4];+


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


/* ---------------- DIBUJO DE PUNTAJE Y VIDAS ---------------- //
 * hud_draw: imprime en pantalla vidas y puntaje del jugador.
 * Recibe: fuente, puntero a juego y graficos
*///---------------------------------------------------------- //
void hud_draw(ALLEGRO_FONT* font, juego_t * juego, graphics_t * graphics){
    char puntaje_str[10];
    sprintf(puntaje_str, "%i", juego->puntaje);
    al_draw_text(font, WHITE, BUFFER_W -150, 1, 0, puntaje_str);
    for(int i = 1; i <= juego->vidas; i++)
      al_draw_bitmap(graphics->vida_bitmap, SCALE * i, 1 , 0);
}

/*------------------- DIBUJO DE BOTONES Y FONDOS DE ESTADOS ------------------ //
 * menu_draw: dibuja botones, texto y fondos principales de estados.
 * Recibe: puntero a evento, arreglo de estructuras botones, estructura de graficos y juego.
*///-------------------------------------------------------------------------- //
void menu_draw(button_t * buttons[], graphics_t * graphics, juego_t * juego, int game_states){
    switch(game_states){
        case(STATE_START):
            al_clear_to_color(BLACK);
            al_draw_bitmap(graphics->menu_background,0,0,0);            
            al_draw_text(buttons[3][0].font, WHITE, (float) DISP_W / 2, (float) buttons[3][0].y + 80, ALLEGRO_ALIGN_CENTRE, buttons[3][0].text);
            break;
        case(STATE_MENU):
            al_clear_to_color(al_map_rgb(0,0,0));
            al_draw_bitmap(graphics->menu_background,0,0,0);
            for(int i=0; i<3; i++){
                if(buttons[0][i].keyboard)
                    al_draw_text(buttons[0][i].font, RED, DISP_W / 2, buttons[0][i].y + 80 , ALLEGRO_ALIGN_CENTRE, buttons[0][i].text);
                else
                    al_draw_text(buttons[0][i].font, WHITE, DISP_W / 2, buttons[0][i].y+ 80, ALLEGRO_ALIGN_CENTRE, buttons[0][i].text);
            }
            break;
        case(STATE_PLAY):
            al_clear_to_color(al_map_rgb(0,0,0));
            al_draw_bitmap(graphics->game_background,0,0,0);
            break;
            
        case(STATE_GAMEOVER):
          
            al_clear_to_color(al_map_rgb(0,0,0));
            al_draw_text(buttons[4][0].font, WHITE, DISP_W / 2, buttons[4][0].y -200, ALLEGRO_ALIGN_CENTRE, "G A M E  O V E R");
            al_draw_text(buttons[4][0].font, WHITE, DISP_W / 2, buttons[4][0].y -150, ALLEGRO_ALIGN_CENTRE, "FINAL SCORE:");
            char puntaje_str[10];
            sprintf(puntaje_str, "%i", juego->puntaje);
            al_draw_text(buttons[4][0].font, WHITE, DISP_W / 2, buttons[4][0].y -100, ALLEGRO_ALIGN_CENTRE, puntaje_str);
            for(int i=0; i<2; i++){
                if(buttons[4][i].keyboard)
                    al_draw_text(buttons[4][i].font, RED, DISP_W / 2, buttons[4][i].y  , ALLEGRO_ALIGN_CENTRE, buttons[4][i].text);
                else
                    al_draw_text(buttons[4][i].font, WHITE, DISP_W / 2, buttons[4][i].y , ALLEGRO_ALIGN_CENTRE, buttons[4][i].text);
            }
            break;
            
        case(STATE_STATS):
            al_clear_to_color(al_map_rgb(0,0,0));
            al_draw_bitmap(graphics->menu_background,0,0,0);
            if(buttons[1][0].keyboard)
                al_draw_text(buttons[1][0].font, RED, DISP_W / 2, buttons[1][0].y, ALLEGRO_ALIGN_CENTRE, buttons[1][0].text);
            else
                al_draw_text(buttons[1][0].font, WHITE, DISP_W / 2, buttons[1][0].y, ALLEGRO_ALIGN_CENTRE, buttons[1][0].text);
            break;
        case(STATE_PAUSE):
            al_clear_to_color(al_map_rgb(0,0,0));
            al_draw_bitmap(graphics->game_background,0,0,0);
            for(int i=0; i<2; i++){
                if(buttons[2][i].keyboard)
                    al_draw_text(buttons[1][0].font, RED, DISP_W / 2, buttons[2][i].y, ALLEGRO_ALIGN_CENTRE, buttons[2][i].text);
                else
                    al_draw_text(buttons[1][0].font, WHITE, DISP_W / 2, buttons[2][i].y, ALLEGRO_ALIGN_CENTRE, buttons[2][i].text);
            }
            break;
        /*case(STATE_TRANSITION):
            al_draw_bitmap(graphics->menu_background,0,0,0); 
            break;
        case(STATE_LEVELUP):  
            break;     */       
    }
}    
/*
void transition_draw(graphics_t * graphics, int from){
    int i, j;
    if(from == STATE_MENU){
        al_draw_bitmap(graphics->menu_background,0,0,0); 
        for (i = 0; i < LARGO ; i++){
            for(j = 0; j < ANCHO; j++){ 
                 al_draw_bitmap_region(graphics->game_background,0,0, j + CELL/2, i - CELL/2, 0, 0); 
            }
        }
    }
    else if(from == STATE_PLAY){
        
    }
    else if(from == STATE_LEVELUP){
        
    }  
}*/

/* --------------     DIBUJO DE JUGADOR      -------------- //
 * player_draw: ubica imagen de jugador en diplay.
 * Recibe: puntero a estructura juego y graficos.
*///------------------------------------------------------- //
void player_draw(juego_t * juego, graphics_t * graphics){
    getcoordp(juego);
    al_draw_bitmap(graphics->player_bitmap, SCALE*juego->coordsp.j - CELL/2, SCALE*juego->coordsp.i - CELL/2 + CORRECCION , 0);
}

/* -------------- DIBUJO DE ENEMIGOS -------------------- //
 * enemies_draw: ubica imagen de enemigo en diplay.
 * Recibe: puntero a estructura graficos y board.
*///----------------------------------------------------- //
void enemies_draw(graphics_t * graphics, board_t * board){
    int n, check;
    for(n = 0, check = 1; n < MAX_ENEMIES && check; n++){
        switch(board->enemy_cell[n].objeto){
            case(ENEMY1):
            case(ENEMYSHOT1):
                al_draw_bitmap(graphics->enemy_bitmap, SCALE*board->enemy_cell[n].j - CELL/2  ,SCALE*board->enemy_cell[n].i - CELL/2 +CORRECCION , 0);
                break;
            case(ENEMY2):
            case(ENEMYSHOT2):
                al_draw_bitmap(graphics->enemy2_bitmap, SCALE*board->enemy_cell[n].j - CELL/2 , SCALE*board->enemy_cell[n].i - CELL/2 +CORRECCION , 0);
                break;    
            case(ENEMY3):
            case(ENEMYSHOT3):
                al_draw_bitmap(graphics->enemy3_bitmap, SCALE*board->enemy_cell[n].j - CELL/2, SCALE*board->enemy_cell[n].i - CELL/2 +CORRECCION , 0);
                break;
            default:
                check = 0;
                break;
        }                        
    }
}

/* ----------------- DIBUJO NAVE NODRIZA ----------------- //
 * navnod_draw: ubica imagen de nave nodriza en display.
 * Recibe: puntero a estructura graficos y board.  
*///----------------------------------------------------- //
void navnod_draw(graphics_t * graphics, board_t * board){
    if(board->navnod.objeto == NAVNOD){
        al_draw_bitmap(graphics->navnod_bitmap, SCALE *board->navnod.j - CELL/2, SCALE * board->navnod.i - CELL/2 + CORRECCION , 0);
    }
    
}

/* -------------     DIBUJO DE DISPAROS      ------------- //
 * shots_draw: ubica primitivas de disparos en display.
 * Recibe: puntero a estructura graficos y board.  
*///----------------------------------------------------- //
void shots_draw(graphics_t * graphics, board_t * board){
    int n, check;
    if(board->pshot.objeto == PSHOT){
        al_draw_line(SCALE * board->pshot.j, SCALE * board->pshot.i - 5, SCALE * board->pshot.j, SCALE * board->pshot.i + 5, RED, 4);
        //if(board->pshot.i > LARGO - 2) al_play_sample(shot_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
    for(n = 0, check = 1; n < MAX_SHOTS && check; n++){
        if(board->enemy_shot[n].objeto == ESHOT){
            al_draw_line(SCALE * board->enemy_shot[n].j, SCALE * board->enemy_shot[n].i - 5, SCALE * board->enemy_shot[n].j, SCALE * board->enemy_shot[n].i + 5, GREEN, 4);
        }      
        else check = 0;
    }
}

/* ---------------     DIBUJO DE MURO      --------------- //
 * muro_draw: dibuja bloques de muros en display.
 * Recibe: puntero a estructura graficos y board.  
*///------------------------------------------------------ //
void muro_draw(graphics_t * graphics, board_t * board){
    int n, check;
    for(n = 0, check = 1; n < MAX_MURO && check == 1; n++){
        if(board->muro[n].objeto == MURO){
            al_draw_bitmap(graphics->muro_bitmap, SCALE*board->muro[n].j - CELL/2, SCALE*board->muro[n].i - CELL/2 +CORRECCION , 0);
        }       
        else check = 0;
    }                        
}

/* -------------- ACTUALIZACION DE EXPLOSIONES  -------------- //
 * explosion_draw: dibuja explosiones en display.
 * Recibe: puntero a estructura graficos y board.   
*///---------------------------------------------------------- //
void explosion_draw(graphics_t * graphics, board_t * board){
    int k;
    for(k = 0; k < MAX_EXPLOSIONS; k++){
        switch(board->explosion[k].objeto){
            case(PLAYER):
                al_draw_bitmap(graphics->playerkilled_bitmap, SCALE*board->explosion[k].j - CELL, SCALE*board->explosion[k].i - CELL +CORRECCION , 0);
                break;
            case(ENEMY1):
                al_draw_bitmap(graphics->enemykilled_bitmap, SCALE*board->explosion[k].j - CELL/2, SCALE*board->explosion[k].i - CELL/2 + CORRECCION , 0);
                break;
            case(NAVNOD):
                al_draw_bitmap(graphics->navnodkilled_bitmap, SCALE*board->explosion[k].j - CELL/2, SCALE*board->explosion[k].i - CELL/2 +CORRECCION , 0);
                break;
        }
    }
}
/*
void draw_all(juego_t * juego, graphics_t * graphics, board_t * board){
    player_draw(juego, graphics);
    vel_nod(juego, board);
    hud_draw(juego, graphics);
    enemies_draw(graphics, board);
    shots_draw(graphics, board);
    muro_draw(graphics, board);
    navnod_draw(graphics, board);
    explosion_draw(graphics, board);
}*/

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
