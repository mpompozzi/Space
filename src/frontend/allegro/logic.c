/*******************************************************************************
  @file     draw.c
  @brief    Funciones de graficacion de menu, player, disparos y enemigos. 
  @author   Alejandro Alatsis, Axel Cincunegui, Facundo Molina, Magali Pompozzi 
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "../../../headers/logic.h" 

#include "../../../headers/structures.h" 


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define PLAYERKILLED 22
#define LVLUP        23
#define GAMEOVER     24
#define MASVIDA      25
#define MENOSVIDA    26
#define OPTION       27

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


/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static const int temperaturas_medias[4] = {23, 26, 24, 29};+


/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

unsigned char key[ALLEGRO_KEY_MAX];
unsigned char counterKey[ALLEGRO_KEY_MAX];
int frontboard[LARGO][ANCHO];


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


/****************************************************************************
*---------------------------- LOGICA DE JUEGO ----------------------------
****************************************************************************/


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

int keyboard_counter(unsigned char keyName, int game_states){
    
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


/* ------------------- ACTUALIZACION DE ACCION DE JUGADOR ------------------- //
 * game_update:  gestiona acciones de jugador y paso de nivel. Llama a funciones
 *               del back pmov y verparams par amovimiento de jugador y ininiv
 *               cuando ocurre transicion de nivel.
 * Recibe: puntero a estructura juego.
*///------------------------------------------------------------------------ //



int game_update(juego_t * juego, int game_states){
    move_player(juego, game_states);
    getcoordp(juego);
    pmov(juego);
    verparams(juego);
    if(juego->naves == 0){
        game_states = TRANSITION_LEVELUP;
        juego->vidas+=1;
        inimat();
        ininiv(juego->nivel +1 );
    }
    if(juego->vidas == 0){
        game_states = TRANSITION_GAMEOVER;
    }
    return game_states;
}

/* ----------------- GESTION MOVIMIENTO DE JUGADOR Y VIDAS ----------------- //
 * move_player: verifica teclas presionadas y activa flag de accion de jugador.
 * Recibe: puntero a estructura juego.
*///------------------------------------------------------------------------ //
void move_player(juego_t * juego, int game_states){ 
    if(key[ALLEGRO_KEY_SPACE] && key[ALLEGRO_KEY_RIGHT]){
        if(keyboard_counter(ALLEGRO_KEY_SPACE, game_states) && keyboard_counter(ALLEGRO_KEY_RIGHT, game_states))
            juego->mov = SHOOT_RIGHT;     
    }  
    else if(key[ALLEGRO_KEY_SPACE] && key[ALLEGRO_KEY_LEFT]){
        if(keyboard_counter(ALLEGRO_KEY_SPACE, game_states) && keyboard_counter(ALLEGRO_KEY_LEFT, game_states))
            juego->mov = SHOOT_LEFT;
    } 
    else if(key[ALLEGRO_KEY_SPACE]){
        if(keyboard_counter(ALLEGRO_KEY_SPACE, game_states))
            juego->mov = SHOOT;
    }
    else if(key[ALLEGRO_KEY_RIGHT]){
        if(keyboard_counter(ALLEGRO_KEY_RIGHT, game_states))
            juego->mov = RIGHT;
    }        
    else if(key[ALLEGRO_KEY_LEFT]){
        if(keyboard_counter(ALLEGRO_KEY_LEFT, game_states))
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
    //call_audio(&audio,NAVNOD);
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
int menu_update(ALLEGRO_EVENT * ev, juego_t * juego, button_t * buttons[], int game_states){
    
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
                    game_states = TRANSITION_LEVELSTART; //habria que poner un break adentro para evitar presionar otro boton rapido??
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
    }
    return game_states;
} 

/* ------------------------- ACTUALIZACION DE NAVE NODRIZA ------------------------- //
 * call_nod: genera el llamado aleatorio para la nave nodriza y la mueve en el tablero.
*///-------------------------------------------------------------------------------- //
void call_nod(void){
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





/* ----------------- AUDIO DEL JUEGO ----------------- //
 * call_audio: esta funcion reprodice la pista de audio segun el evento que se 
 * requiere.
*///------------------------------------------------------------------------ //


 void call_audio(audio_t * audio,board_t * board,int game_states){
  
   static int flag[3];
   
   
   
   if(board->pshot.objeto == PSHOT){
      if(board->pshot.i >= LARGO-2) 
        al_play_sample(audio->playershot_sound,1.0,0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
    }    
  
   if(game_states == TRANSITION_LEVELUP && flag[0] == 1){
       flag[0]=0;
       al_play_sample(audio->levelup_sound,1.0,0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
     }
   else if(game_states != TRANSITION_LEVELUP)
     flag[0]=1;
    
   
   if(board->navnod.objeto == NAVNOD && flag[1] == 1){
      if(board->navnod.j <2 ) {
         flag[1]=0;
        al_play_sample(audio->navnod_sound,1.0,0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
        }
    } else if(board->navnod.objeto != NAVNOD)  
      flag[1]=1;
  
   if(game_states == TRANSITION_GAMEOVER && flag[2] == 1){
       flag[2]=0;
       al_play_sample(audio->gameover_sound,1.0,0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
     }
   else if(game_states != TRANSITION_GAMEOVER)
     flag[2]=1;
    
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/