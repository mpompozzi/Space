/*******************************************************************************
  @file     front.c
  @brief    
  @author   Alejandro Alatsis, Axel Cincunegui, Facundo Molina, Magali Pompozzi 
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "../../../headers/allegro.h"   

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/



/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/



/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

int game_states;
int frontboard[LARGO][ANCHO];


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

unsigned char key[ALLEGRO_KEY_MAX];
unsigned char counterKey[ALLEGRO_KEY_MAX];

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/



/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/*
 * must_init:   control de inicializacion. Imprime en pantalla si no se 
 *              inicializo correctamente la funcion.
 * Recibe:  variable inicializada y mensaje a imprimir de haber error.
*/
void must_init(bool test, const char *description)
{
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

/* ------------------     DISPLAY      ------------------ //
 * disp_init:   inicializacion variables de display
 * disp_deinit:   destruccion de tablero y buffer.
 * disp_predraw:   imprime elementos en buffer.
 * Recibe:  puntero a estructura con display y buffer.
*///----------------------------------------------------- //
void disp_init(display_t * display){
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

    display->disp = al_create_display(DISP_W, DISP_H);
    must_init(display->disp, "display");

    display->buffer = al_create_bitmap(BUFFER_W, BUFFER_H);
    must_init(display->buffer, "bitmap buffer");
}

void disp_deinit(display_t * display){
    al_destroy_bitmap(display->buffer);
    al_destroy_display(display->disp);
}

void disp_pre_draw(display_t * display){
    al_set_target_bitmap(display->buffer);
}

void disp_post_draw(display_t * display){
    al_set_target_backbuffer(display->disp);
    al_draw_scaled_bitmap(display->buffer, 0, 0, BUFFER_W, BUFFER_H, 0, 0, DISP_W, DISP_H, 0);
    al_flip_display();
}

/* ---------------------------     TECLADO      --------------------------- //
 * keyboard_init: inicializacion en cero de arreglos de gestion de teclado.
 * keyboard_update: actualizacion de tecla presionada o liberada en arreglo.
 * keyboard_counter: control de sensibilidad de teclado.
*///----------------------------------------------------------------------- //

void keyboard_init(void){
    memset(key, 0, sizeof(key));
    memset(counterKey, 0, sizeof(counterKey));
}

void keyboard_update(ALLEGRO_EVENT* event){
    switch(event->type)
    {
        case ALLEGRO_EVENT_KEY_DOWN:
            key[event->keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
            break;
        case ALLEGRO_EVENT_KEY_UP:
            key[event->keyboard.keycode] &= KEY_RELEASED;
            break;
    }
}

int keyboard_counter(unsigned char keyName){
    
    key[keyName] &= KEY_SEEN;   // Clear key
    
    if(game_states == STATE_PLAY){ 
        counterKey[keyName]++;  // Increase counter of this key
        if(counterKey[keyName] == 1)
            return 1;
        if(keyName == ALLEGRO_KEY_SPACE){
            if(counterKey[keyName] == 5) 
                counterKey[keyName] = 0;
        }
        else{
            if(counterKey[keyName] == 2) 
                counterKey[keyName] = 0;
        }
    }
    else{

    counterKey[keyName]++;  // Increase counter of this key
    if(counterKey[keyName] == 1)
        return 1;
    if(counterKey[keyName] == 10) 
        counterKey[keyName] = 0;
    }
    return 0;
}

/* -------------------     AUDIO      ------------------- //
 * audio_init:   inicializa elementos de audio.
 * audio_deinit:    destruye elementos de audio.
 * Recibe:  puntero a estructura con elementos de audios.
*///----------------------------------------------------- //
void audio_init(audio_t * audio){
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(128);

    /*audio->game_sound = al_load_sample("shot.flac");
    must_init(audio->game_sound, "game sound");

    audio->option_sound = al_load_sample("explode2.flac");
    must_init(audio->option_sound, "option selected sound");*/
    
    audio->shot_sound = al_load_sample(SHOT_SOUND);
    must_init(audio->shot_sound, "shot sound");
    
    audio->collision_sound = al_load_sample(COLLISION_SOUND);
    must_init(audio->collision_sound, "coliision sound sound");
    
    audio->gameover_sound = al_load_sample(GAMEOVER_SOUND);
    must_init(audio->gameover_sound, "game over sound");
}

void audio_deinit(audio_t * audio){
    /*al_destroy_sample(game_sound);
    al_destroy_sample(option_sound);*/
    al_destroy_sample(audio->shot_sound);
    al_destroy_sample(audio->collision_sound);
    al_destroy_sample(audio->gameover_sound);
}

/* ------------------     GRAFICOS      ------------------ //
 * graphics_init:   inicializa elementos de graficos bitmaps
 * graphics_deinit: destruye elementos de graficos bitmaps.
 * Recibe:  puntero a estructura con bitmaps.
*///----------------------------------------------------- //
void graphics_init(graphics_t * graphics){
    graphics->menu_background = al_load_bitmap(MENU_BACKGROUND);
    must_init(graphics->menu_background, "menu background");
  
    graphics->game_background = al_load_bitmap(GAME_BACKGROUND);
    must_init(graphics->game_background, "game background");
    
    graphics->enemy_bitmap = al_load_bitmap(ENEMY_BMP);
    must_init(graphics->enemy_bitmap, "enemy bitmap");
    
    graphics->enemy2_bitmap = al_load_bitmap(ENEMY2_BMP);
    must_init(graphics->enemy2_bitmap, "enemy 2 bitmap");
    
    graphics->enemy3_bitmap = al_load_bitmap(ENEMY3_BMP);
    must_init(graphics->enemy3_bitmap, "enemy 3 bitmap");
    
    graphics->navnod_bitmap = al_load_bitmap(NAVNOD_BMP);
    must_init(graphics->navnod_bitmap, "navnod bitmap");        
            
    graphics->player_bitmap = al_load_bitmap(PLAYER_BMP);
    must_init(graphics->player_bitmap, "player bitmap");
    
     graphics->muro_bitmap = al_load_bitmap(MURO_BMP);
    must_init(graphics->muro_bitmap, "muro bitmap");
    
    graphics->playerkilled_bitmap = al_load_bitmap(PLAYERKILLED_BMP);
    must_init(graphics->playerkilled_bitmap, "playerkilled bitmap");
    
    graphics->navnodkilled_bitmap = al_load_bitmap(NAVNODKILLED_BMP);
    must_init(graphics->navnodkilled_bitmap, "navnodkilled bitmap");
    
    graphics->enemykilled_bitmap = al_load_bitmap(ENEMYKILLED_BMP);
    must_init(graphics->enemykilled_bitmap, "enemykilled bitmap");
    
    graphics->vida_bitmap = al_load_bitmap(VIDA_BMP);
    must_init(graphics->vida_bitmap, "vida bitmap");
}

void graphics_deinit(graphics_t * graphics){
    al_destroy_bitmap(graphics->menu_background);
    al_destroy_bitmap(graphics->game_background);
    al_destroy_bitmap(graphics->enemy_bitmap);
    al_destroy_bitmap(graphics->enemy2_bitmap);
    al_destroy_bitmap(graphics->enemy3_bitmap);
    al_destroy_bitmap(graphics->navnod_bitmap);
    al_destroy_bitmap(graphics->player_bitmap);
    al_destroy_bitmap(graphics->playerkilled_bitmap);
    al_destroy_bitmap(graphics->enemykilled_bitmap);
    al_destroy_bitmap(graphics->navnodkilled_bitmap);
    al_destroy_bitmap(graphics->vida_bitmap);
}


/* ---------------- DIBUJO DE PUNTAJE Y VIDAS ---------------- //
 * hud_init:   inicializa fuente TrueTypeFormat.
 * hud_deinit:  destruye fuente cargada.
 * hud_draw: imprime en pantalla vidas y puntaje del jugador.
*///---------------------------------------------------------- //
ALLEGRO_FONT * hud_init(ALLEGRO_FONT* font){
    al_init_font_addon(); // initialize the font addon
    al_init_ttf_addon();
    font =  al_load_ttf_font("res/fonts/ARCADE_N.TTF", 20, 0);
    must_init(font, "font");
    
    return font;
    
}

void hud_deinit(ALLEGRO_FONT* font){
    al_destroy_font(font);
}

void hud_draw(ALLEGRO_FONT* font, juego_t * juego, graphics_t * graphics){
    char puntaje_str[10];
    sprintf(puntaje_str, "%i", juego->puntaje);
    al_draw_text(font, WHITE, BUFFER_W -150, 1, 0, puntaje_str);
    for(int i = 1; i <= juego->vidas; i++)
      al_draw_bitmap(graphics->vida_bitmap, SCALE * i, 1 , 0);
}

/****************************************************************************
*---------------------------- LOGICA DE JUEGO ----------------------------
****************************************************************************/

/* ------------------- ACTUALIZACION DE ACCION DE JUGADOR ------------------- //
 * game_update:  gestiona acciones de jugador y paso de nivel. Llama a funciones
 *               del back pmov y verparams par amovimiento de jugador y ininiv
 *               cuando ocurre transicion de nivel.
 * Recibe: puntero a estructura juego.
*///------------------------------------------------------------------------ //
void game_update(juego_t * juego){
    move_player(juego);
    getcoordp(juego);
    pmov(juego);
    verparams(juego);
    if(juego->naves == 0){
        //agregar secuencia de level up
        juego->vidas+=1;
        ininiv(juego->nivel );
    }
}

/* ----------------- GESTION MOVIMIENTO DE JUGADOR Y VIDAS ----------------- //
 * move_player: verifica teclas presionadas y activa flag de accion de jugador.
 * Recibe: puntero a estructura juego.
*///------------------------------------------------------------------------ //
void move_player(juego_t * juego){ 
    if(key[ALLEGRO_KEY_SPACE] && key[ALLEGRO_KEY_RIGHT]){
        if(keyboard_counter(ALLEGRO_KEY_SPACE) && keyboard_counter(ALLEGRO_KEY_RIGHT))
            juego->mov = SHOOT_RIGHT;     
    }  
    else if(key[ALLEGRO_KEY_SPACE] && key[ALLEGRO_KEY_LEFT]){
        if(keyboard_counter(ALLEGRO_KEY_SPACE) && keyboard_counter(ALLEGRO_KEY_LEFT))
            juego->mov = SHOOT_LEFT;
    } 
    else if(key[ALLEGRO_KEY_SPACE]){
        if(keyboard_counter(ALLEGRO_KEY_SPACE))
            juego->mov = SHOOT;
    }
    else if(key[ALLEGRO_KEY_RIGHT]){
        if(keyboard_counter(ALLEGRO_KEY_RIGHT))
            juego->mov = RIGHT;
    }        
    else if(key[ALLEGRO_KEY_LEFT]){
        if(keyboard_counter(ALLEGRO_KEY_LEFT))
            juego->mov = LEFT;  
    }
}

/* ------------------------- ACTUALIZACION DE TABLERO ------------------------ //
 * board_update: lee tablero desde backend y guarda elementos en front. Llama a 
 *               funcion getmat del back para leer elemento en una celda.
 * Recibe: puntero a estructura juego y estructura board.
*///-------------------------------------------------------------------------- //
void board_update(juego_t * juego, board_t * board){
    int i, j, a, b, c; 
    board->pshot.objeto = NADA;    //previamente limpio lo que habia
    board->navnod.objeto = NADA;
    for (i = 0, a = 0, b = 0, c = 0; i < LARGO ; i++) {
        for (j = 0; j < ANCHO; j++){  
            frontboard[i][j] = getmat(i, j);
            switch(frontboard[i][j]){
                case(NADA):
                    board->enemy_shot[a].objeto = NADA;    
                    board->enemy_cell[b].objeto = NADA;     
                    board->muro[c].objeto = NADA;  //limpio antes
                    break;
                case(PLAYER):
                    break;
                case(PSHOT):
                    board->pshot.i = i;
                    board->pshot.j = j; 
                    board->pshot.objeto = frontboard[i][j];
                    break;
                case(ESHOT):
                    board->enemy_shot[a].i = i;
                    board->enemy_shot[a].j = j; 
                    board->enemy_shot[a].objeto = frontboard[i][j];
                    a += 1;
                    break;
                case(ENEMY1):
                case(ENEMY2):
                case(ENEMY3):
                case(ENEMYSHOT1):
                case(ENEMYSHOT2):
                case(ENEMYSHOT3): 
                    board->enemy_cell[b].i = i;
                    board->enemy_cell[b].j = j; 
                    board->enemy_cell[b].objeto = frontboard[i][j]; 
                    b += 1;
                    break;
                case(NAVNOD):
                    board->navnod.i = i;
                    board->navnod.j = j;
                    board->navnod.objeto = frontboard[i][j];
                    j += 1;
                    break;
                case(MURO):
                    board->muro[c].i = i;
                    board->muro[c].j = j;
                    board->muro[c].objeto = frontboard[i][j];
                    c += 1;
                    break;
            }
        }
    }
    board->enemy_maxshots = a;
    board->enemy_maxcells = b;
}

/* ------------------ ACTUALIZACION DE DISPAROS Y COLISIONES ----------------- //
 * shots_update: mueve disparos de jugador y enemigos y notifica colisiones. Llama
 *               a funciones eventop y eventoe del backend. 
 * Recibe: puntero a estructura juego y estructura board para guardar colisiones.
*///-------------------------------------------------------------------------- //
void shots_update(juego_t * juego, board_t * board){
    int n, k = 0, ok;
    coord_t eventoe, eventop;
    if(board->pshot.objeto == PSHOT){
        eventop = ciclodispp(juego, board->pshot.i, board->pshot.j);
        for(k = 0, ok = 1; board->explosion[k].objeto == NADA && ok; k++){
            if(eventop.objeto == NAVE_NODRIZA){
                board->explosion[k].objeto = NAVNOD;
                board->explosion[k].i = board->pshot.i - 1;
                board->explosion[k].j = board->pshot.j;
                ok = 0;
            }
            else if(eventop.objeto == NAVE_ENEMIGA){
                board->explosion[k].objeto = ENEMY1;
                board->explosion[k].i = board->pshot.i - 1;
                board->explosion[k].j = board->pshot.j;
                ok = 0;
            }
        }
    }
    for(n = 0; n < board->enemy_maxshots; n++){
        if(board->enemy_shot[n].objeto == ESHOT){
            eventoe = ciclodispe(juego, board->enemy_shot[n].i, board->enemy_shot[n].j);
            if(eventoe.objeto == JUGADOR){
                board->explosion[k].objeto = PLAYER;
                board->explosion[k].i = board->enemy_shot[n].i + 1;
                board->explosion[k].j = board->enemy_shot[n].j;
            }
        }
    } 
}

/* ------------------     ACTUALIZACION DE ENEMIGOS      ------------------- //
 * enemies_update: disparo aleatorio de enemigos (llamando a navdisp() del back)
 *                 y movimiento de enemigos con ciclonaves del back.  
 * Recibe: puntero a estructura juego.
*///------------------------------------------------------------------------ //
void enemies_update(juego_t * juego){ //va a manejar timers de enemigos segun nivel
    navdisp();
    if(ciclonaves(juego)) juego->vidas = 0;
}

/****************************************************************************
*---------------------------- CONTROL DE ESTADOS ----------------------------
****************************************************************************/

/* ----------------     ACTUALIZACION DE MENU Y ESTADOS      ---------------- //
 * menu_update: administracion de estados START (ventana de inicio), MENU (menu principal), 
 *              PLAY (juego), PAUSE (pausa en juego), STATS (puntajes maximos) y
 *              GAMEOVER (transicion de juego finalizado). Mueve a traves de sus
 *              botones y determina el pase a otro estado.
 * Recibe: puntero a evento, estructura juego y arreglo de estructuras botones. 
*///------------------------------------------------------------------------ //
void menu_update(ALLEGRO_EVENT * ev, juego_t * juego, button_t * buttons[]){
    
    switch(game_states){
        case(STATE_START):
            if(ev->keyboard.keycode == ALLEGRO_KEY_ENTER){
                game_states = STATE_MENU;
                buttons[0][0].keyboard = 1; 
            }
            break;
        case(STATE_MENU):
            if(buttons[0][0].keyboard){
                if(ev->keyboard.keycode == ALLEGRO_KEY_ENTER){
                    game_states = STATE_PLAY; //habria que poner un break adentro para evitar presionar otro boton rapido??
                    buttons[0][0].keyboard = 0;
                    inigame(juego, 1);
                }
                else if(ev->keyboard.keycode == ALLEGRO_KEY_UP){
                    buttons[0][2].keyboard = 1;
                    buttons[0][0].keyboard = 0;
                }
                else if(ev->keyboard.keycode == ALLEGRO_KEY_DOWN){
                    buttons[0][1].keyboard = 1;
                    buttons[0][0].keyboard = 0;
                }
            } 
            else if(buttons[0][1].keyboard){
                if(ev->keyboard.keycode == ALLEGRO_KEY_ENTER){
                    game_states = STATE_STATS;
                    buttons[1][0].keyboard = 1;
                    buttons[0][1].keyboard = 0;
                }
                else if(ev->keyboard.keycode == ALLEGRO_KEY_UP){
                    buttons[0][0].keyboard = 1;
                    buttons[0][1].keyboard = 0;
                }
                else if(ev->keyboard.keycode == ALLEGRO_KEY_DOWN){
                    buttons[0][2].keyboard = 1;
                    buttons[0][1].keyboard = 0;
                }
            } 
            else if(buttons[0][2].keyboard){
                if(ev->keyboard.keycode == ALLEGRO_KEY_ENTER){
                    game_states = STATE_EXIT;
                    buttons[0][2].keyboard = 0;
                }
                else if(ev->keyboard.keycode == ALLEGRO_KEY_UP){
                    buttons[0][1].keyboard = 1;
                    buttons[0][2].keyboard = 0;
                }
                else if(ev->keyboard.keycode == ALLEGRO_KEY_DOWN){
                    buttons[0][0].keyboard = 1;
                    buttons[0][2].keyboard = 0;
                }
            }          
            break;   
            
        case(STATE_PLAY):
            if(ev->keyboard.keycode == ALLEGRO_KEY_ENTER){
                game_states = STATE_PAUSE;         
                buttons[2][0].keyboard = 1;
            }
            else if(juego->vidas == 0){
                game_states = STATE_GAMEOVER;
                buttons[4][0].keyboard = 1;
            }
            break;
        case(STATE_STATS):
            if(buttons[1][0].keyboard){
                if(ev->keyboard.keycode == ALLEGRO_KEY_ENTER){
                    game_states = STATE_MENU;
                    buttons[0][0].keyboard = 1;
                    buttons[1][0].keyboard = 0;
                }
            }
            break; 
        case(STATE_PAUSE):
            if(buttons[2][0].keyboard){
                if(ev->keyboard.keycode == ALLEGRO_KEY_ENTER){
                    game_states = STATE_PLAY;
                    buttons[2][0].keyboard = 0;
                }
                else if(ev->keyboard.keycode == ALLEGRO_KEY_UP){
                    buttons[2][1].keyboard = 1;
                    buttons[2][0].keyboard = 0;
                    break;
                }
                else if(ev->keyboard.keycode == ALLEGRO_KEY_DOWN){
                    buttons[2][1].keyboard = 1;
                    buttons[2][0].keyboard = 0;
                }
            }
            else if(buttons[2][1].keyboard){
                if(ev->keyboard.keycode == ALLEGRO_KEY_ENTER){
                    game_states = STATE_MENU;
                    buttons[0][0].keyboard = 1;
                    buttons[2][1].keyboard = 0;
                }
                else if(ev->keyboard.keycode == ALLEGRO_KEY_UP){
                    buttons[2][0].keyboard = 1;
                    buttons[2][1].keyboard = 0;
                }
                else if(ev->keyboard.keycode == ALLEGRO_KEY_DOWN){
                    buttons[2][0].keyboard = 1;
                    buttons[2][1].keyboard = 0;
                }
            }
            break;
        case(STATE_GAMEOVER):
            if(buttons[4][0].keyboard){
                if(ev->keyboard.keycode == ALLEGRO_KEY_ENTER){
                    game_states = STATE_STATS;
                    buttons[1][0].keyboard = 1;
                    buttons[4][0].keyboard = 0;
                }
                else if(ev->keyboard.keycode == ALLEGRO_KEY_UP){
                    buttons[4][1].keyboard = 1;
                    buttons[4][0].keyboard = 0;
                }
                else if(ev->keyboard.keycode == ALLEGRO_KEY_DOWN){
                    buttons[4][1].keyboard = 1;
                    buttons[4][0].keyboard = 0;
                }
            }
            else if(buttons[4][1].keyboard){
                if(ev->keyboard.keycode == ALLEGRO_KEY_ENTER){
                    game_states = STATE_MENU;
                    buttons[0][0].keyboard = 1;
                    buttons[4][1].keyboard = 0;
                }
                else if(ev->keyboard.keycode == ALLEGRO_KEY_UP){
                    buttons[4][0].keyboard = 1;
                    buttons[4][1].keyboard = 0;
                }
                else if(ev->keyboard.keycode == ALLEGRO_KEY_DOWN){
                    buttons[4][0].keyboard = 1;
                    buttons[4][1].keyboard = 0;
                }
            }
            break;
        case(STATE_TRANSITION):
            break;
        case(STATE_LEVELUP):  
            break;
    }
} 

/*------------------- DIBUJO DE BOTONES Y FONDOS DE ESTADOS ------------------ //
 * menu_draw: dibuja botones, texto y fondos principales de estados.
 * Recibe: puntero a evento, arreglo de estructuras botones, estructura de graficos y juego.
*///-------------------------------------------------------------------------- //
void menu_draw(button_t * buttons[], graphics_t * graphics, juego_t * juego){
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
    al_draw_bitmap(graphics->player_bitmap, SCALE*juego->coordsp.j - CELL/2, SCALE*juego->coordsp.i - CELL/2, 0);
}

/* -------------- DIBUJO DE ENEMIGOS -------------------- //
 * enemies_draw: ubica imagen de enemigo en diplay.
 * Recibe: puntero a estructura graficos y board.
*///----------------------------------------------------- //
void enemies_draw(graphics_t * graphics, board_t * board){
    int n, aux;
    for(n = 0; n < MAX_ENEMIES; n++){
        aux = board->enemy_cell[n].objeto;
        switch(aux){
             case(ENEMY1):
             case(ENEMYSHOT1):
                al_draw_bitmap(graphics->enemy_bitmap, SCALE*board->enemy_cell[n].j - CELL/2  ,SCALE*board->enemy_cell[n].i - CELL/2, 0);
                break;
             case(ENEMY2):
             case(ENEMYSHOT2):
                al_draw_bitmap(graphics->enemy2_bitmap, SCALE*board->enemy_cell[n].j - CELL/2 , SCALE*board->enemy_cell[n].i - CELL/2, 0);
                break;    
             case(ENEMY3):
             case(ENEMYSHOT3):
                al_draw_bitmap(graphics->enemy3_bitmap, SCALE*board->enemy_cell[n].j - CELL/2, SCALE*board->enemy_cell[n].i - CELL/2, 0);
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
        al_draw_bitmap(graphics->navnod_bitmap, SCALE *board->navnod.j - CELL/2, SCALE * board->navnod.i - CELL/2, 0);
    }
    
}

void call_nod(void){// funcion que .
  static int nav=0;
  static int random=0;
  if(nav == 0){ //si no hay nave nodriza en el juego, que busque crearla cuando coincida los rangos que emite rand.
      random = (rand () % 100) ; //numero entre 25 y 35
      if((random >=5)&&(random<=7)){
          nav=nav_nod();
          random=0;
        }
    }
  else{
      nav=nav_nod();//cuando ya hay nave, solamente la mueve.
    }
}

/* -------------     DIBUJO DE DISPAROS      ------------- //
 * shots_draw: ubica primitivas de disparos en display.
 * Recibe: puntero a estructura graficos y board.  
*///----------------------------------------------------- //
void shots_draw(graphics_t * graphics, board_t * board){
    int n;
    if(board->pshot.objeto == PSHOT){
        al_draw_line(SCALE * board->pshot.j, SCALE * board->pshot.i - 5, SCALE * board->pshot.j, SCALE * board->pshot.i + 5, RED, 4);
        //if(board->pshot.i > LARGO - 2) al_play_sample(shot_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
    for(n = 0; n < MAX_SHOTS; n++){
        if(board->enemy_shot[n].objeto == ESHOT){
            al_draw_line(SCALE * board->enemy_shot[n].j, SCALE * board->enemy_shot[n].i - 5, SCALE * board->enemy_shot[n].j, SCALE * board->enemy_shot[n].i + 5, GREEN, 4);
        }                        
    }
}

/* ---------------     DIBUJO DE MURO      --------------- //
 * muro_draw: dibuja bloques de muros en display.
 * Recibe: puntero a estructura graficos y board.  
*///------------------------------------------------------ //
void muro_draw(graphics_t * graphics, board_t * board){
    int n, aux;
    for(n = 0, aux = 1; n < MAX_MURO && aux == 1; n++){
        if(board->muro[n].objeto == MURO){
            al_draw_bitmap(graphics->muro_bitmap, SCALE*board->muro[n].j - CELL/2, SCALE*board->muro[n].i - CELL/2, 0);
        }       
        else aux = 0;
    }                        
}

/* ----------------- ACTUALIZACION DE VELOCIDADES ENEMIGOS ----------------- //
 * vel_nod: administracion de velocidad de timer de enemigos segun cantidad de 
 *          enemigos en juego.'
 * Recibe: puntero a estructura juego y board. 
*///------------------------------------------------------------------------ //
void vel_nod(juego_t * juego, board_t * board){
  
    if(juego->naves <= 40)
         al_set_timer_speed(board->timer_enemy , (1/ 0.5));

    if(juego->naves <= 35)
         al_set_timer_speed(board->timer_enemy , (1/ 0.8));

    if(juego->naves <= 30)
       al_set_timer_speed(board->timer_enemy , (1 / 1.4));

    if(juego->naves <= 25)
       al_set_timer_speed(board->timer_enemy , (1 / 1.8));

    if(juego->naves <= 20)
       al_set_timer_speed(board->timer_enemy ,(1/ 2.2));

    if(juego->naves <= 15)
       al_set_timer_speed(board->timer_enemy , (1 / 2.4));

    if(juego->naves <= 10)
       al_set_timer_speed(board->timer_enemy , (1 / 2.4));

    if(juego->naves <= 5)
       al_set_timer_speed(board->timer_enemy , (1 / 2.8));

    if(juego->naves <= 4)
       al_set_timer_speed(board->timer_enemy , (1/3.2));

    if(juego->naves <= 2)
      al_set_timer_speed(board->timer_enemy , (1/5.2));
      
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
                al_draw_bitmap(graphics->playerkilled_bitmap, SCALE*board->explosion[k].j - CELL, SCALE*board->explosion[k].i - CELL, 0);
                break;
            case(ENEMY1):
                al_draw_bitmap(graphics->enemykilled_bitmap, SCALE*board->explosion[k].j - CELL/2, SCALE*board->explosion[k].i - CELL/2, 0);
                break;
            case(NAVNOD):
                al_draw_bitmap(graphics->navnodkilled_bitmap, SCALE*board->explosion[k].j - CELL/2, SCALE*board->explosion[k].i - CELL/2, 0);
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

/*****************************************************************************
*----------------------------------- MAIN -----------------------------------*
* main: gestion de cola de eventos, timers, inicializacion y destruccion de  *
*       puntero ALLEGRO. Llamado a funciones logicas cuando ocurre un evento *
*       y de dibujo cuando se vacia la cola de eventos.                      *
*****************************************************************************/
int main(void){
   
    juego_t juego;
    display_t display;
    audio_t audio;
    graphics_t graphics;
    board_t board;
     
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");
    must_init(al_install_mouse(), "mouse");
    must_init(al_install_audio(), "audio");
    must_init(al_init_acodec_addon(), "audio codecs");
    must_init(al_reserve_samples(16), "reserve samples");
    must_init(al_init_primitives_addon(), "primitives");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 40.0);
    must_init(timer, "timer");
    
    ALLEGRO_TIMER* timer_shot = al_create_timer(1.0 / 40.0);
    must_init(timer_shot, "timer_shot");
    
    ALLEGRO_TIMER* timer_nod = al_create_timer(1.0/ 6.0);
    must_init(timer_nod, "timer_nod");

    board.timer_enemy = al_create_timer(1.0/1.0);
    must_init(board.timer_enemy, "timer_enemy");
    
    ALLEGRO_TIMER* timer_explosion = al_create_timer(0.8 / 1.0);
    must_init(timer_explosion, "timer_explosion");
    
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");
    
    /*ALLEGRO_FONT * font = al_create_builtin_font();
    must_init(font, "font");*/
    
    must_init(al_init_image_addon(), "image");
    
    disp_init(&display);
    audio_init(&audio);    
    keyboard_init();
    graphics_init(&graphics);
    
    ALLEGRO_FONT* font = NULL;
    
    font = hud_init(font);
    
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display.disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_timer_event_source(timer_nod));
    al_register_event_source(queue, al_get_timer_event_source(timer_shot));
    al_register_event_source(queue, al_get_timer_event_source(timer_explosion));
    al_register_event_source(queue, al_get_timer_event_source(board.timer_enemy));

    button_t buttons_menu [] = {{300, 30, "START", font, 0},
                        {350, 30, "STATS", font, 0},
                        {400, 30, "EXIT", font, 0}};
    
    button_t buttons_stats [] = {{450,25, "BACK", font, 0}};
    
    button_t buttons_pause [] = {{300,25, "BACK", font, 0}, {400,25, "EXIT", font, 0}};
    
    button_t buttons_gameover [] = {{300,25, "STATS", font, 0}, {400,25, "EXIT", font, 0}};
    
    button_t buttons_start [] = {{350,25, "PRESS ENTER", font, 1}};
    
    button_t * buttons[5];
    
    buttons[0] = buttons_menu;
    buttons[1] = buttons_stats;
    buttons[2] = buttons_pause;
    buttons[3] = buttons_start;
    buttons[4] = buttons_gameover;
    
    bool redraw = true;
    
    bool done = false;
    
    game_states = STATE_START;
    
    int k;
    
    ALLEGRO_EVENT event;
    
    al_start_timer(timer);
    al_start_timer(timer_shot);
    al_start_timer(board.timer_enemy);
    al_start_timer(timer_nod);
    al_start_timer(timer_explosion);
    while(!done){
        
        al_wait_for_event(queue, &event);
        
        if(redraw && al_is_event_queue_empty(queue)){
            disp_pre_draw(&display);
            menu_draw(buttons, &graphics, &juego);
            if(game_states == STATE_PLAY){
                //draw_all(&juego, &graphics, &board);              
                player_draw(&juego, &graphics);
                vel_nod(&juego, &board);
                hud_draw(font, &juego, &graphics);
                enemies_draw(&graphics, &board);
                shots_draw(&graphics, &board);
                muro_draw(&graphics, &board);
                navnod_draw(&graphics, &board);
                explosion_draw(&graphics, &board);
            }
            disp_post_draw(&display);
            redraw = false;
        }
        
        
        switch(event.type){
            case ALLEGRO_EVENT_TIMER:
                if(event.timer.source == timer){
                    redraw = true;
                    //menu_update(&event, &juego, buttons);
                    if(game_states == STATE_EXIT)
                        done = true;
                    if(game_states == STATE_PLAY){
                        game_update(&juego);                     
                    }
                }
                if(game_states == STATE_PLAY){
                    if(event.timer.source == timer_shot){

                            board_update(&juego, &board);
                            shots_update(&juego, &board);

                    }   
                    if(event.timer.source == board.timer_enemy){
                        enemies_update(&juego);
                    }
                    if(event.timer.source == timer_nod){
                            call_nod();  
                    } 
                    if(event.timer.source == timer_explosion){
                        for(k = 0; k < MAX_EXPLOSIONS; k++){
                            if(board.explosion[k].objeto != NADA){
                                board.explosion[k].objeto = NADA;
                            }
                        }   
                    } 
                }
                if(game_states == STATE_TRANSITION){
                    if(event.timer.source == board.timer_enemy){
                        enemies_update(&juego);
                    }
                }
                break;
            case ALLEGRO_EVENT_KEY_DOWN:
                menu_update(&event, &juego, buttons);
                break;
           
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }
        
        keyboard_update(&event);
    }
    
    disp_deinit(&display);
    hud_deinit(font);
    al_destroy_font(font);
    graphics_deinit(&graphics);
    audio_deinit(&audio);
    al_destroy_timer(timer);
    al_destroy_timer(timer_shot);
    al_destroy_timer(board.timer_enemy);
    al_destroy_timer(timer_explosion);
    al_destroy_event_queue(queue);
    
    return 0;
    
}

