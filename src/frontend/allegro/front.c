/*******************************************************************************
  @file     front.c
  @brief    
  @author   Alejandro Alatsis, Axel Cincunegui, Facundo Molina, Magali Pompozzi 
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "../../../headers/front.h"   


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


/* -------------------     AUDIO      ------------------- //
 * audio_init:   inicializa elementos de audio.
 * audio_deinit:    destruye elementos de audio.
 * Recibe:  puntero a estructura con elementos de audios.
*///----------------------------------------------------- //
void audio_init(audio_t * audio){
    al_install_audio();
    al_init_acodec_addon();

    
    audio->gameover_sound = al_load_sample(GAMEOVER_SOUND);
    must_init(audio->gameover_sound, "game over sound");
    
    audio->playershot_sound = al_load_sample(PLAYERSHOT_SOUND);
    must_init(audio->playershot_sound, "playershot sound");
    

    audio->navnod_sound = al_load_sample(NAVNOD_SOUND);
    must_init(audio->navnod_sound, "navnod sound");
    
    audio->levelup_sound = al_load_sample(LEVELUP_SOUND);
    must_init(audio->levelup_sound, "level up sound");
}

void audio_deinit(audio_t * audio){
 
    al_destroy_sample(audio->gameover_sound);
    al_destroy_sample(audio->playershot_sound);
    al_destroy_sample(audio->navnod_sound);
    al_destroy_sample(audio->levelup_sound);
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
    
    graphics->levelup_bitmap = al_load_bitmap(LEVELUP_BMP);
    must_init(graphics->levelup_bitmap, "levelup bitmap");
    
    graphics->start_bitmap = al_load_bitmap(START_BMP);
    must_init(graphics->start_bitmap, "start bitmap");
    
    graphics->gameover_bitmap = al_load_bitmap(GAMEOVER_BMP);
    must_init(graphics->gameover_bitmap, "gameover bitmap");
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
    al_destroy_bitmap(graphics->levelup_bitmap);
    al_destroy_bitmap(graphics->start_bitmap);
    al_destroy_bitmap(graphics->gameover_bitmap);
    
}


/* -------------------------- HUD ---------------------------- //
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


/* ---------------- INICIALIZACION BOARD STRUCT ---------------- //
 * board_init:   inicializa en cero objetos de elementos de la estructura.
 * Recibe: puntero a estructura board
*///---------------------------------------------------------- //
void board_init(board_t * board){
    int n;
    board->enemy_maxshots = NADA;
    board->pshot.objeto = NADA;
    board->navnod.objeto = NADA;
    for(n = 0; n < MAX_ENEMIES; n++){
        board->enemy_shot[n].objeto = NADA;
        board->enemy_cell[n].objeto = NADA;
        board->muro[n].objeto = NADA;
        if(n <  MAX_EXPLOSIONS) board->explosion[n].objeto = NADA;
    }
}
/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/*****************************************************************************
*----------------------------------- MAIN -----------------------------------*
* main: gestion de cola de eventos, timers, inicializacion y destruccion de  *
*       puntero ALLEGRO. Llamado a funciones logicas cuando ocurre un evento *
*       y de dibujo cuando se vacia la cola de eventos.                      *
*****************************************************************************/
int main(void){
  
    int game_states;
   
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
    must_init(al_reserve_samples(4), "reserve samples");
    must_init(al_init_primitives_addon(), "primitives");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 50.0);
    must_init(timer, "timer");
    
    ALLEGRO_TIMER* timer_shot = al_create_timer(1.0 / 40.0);
    must_init(timer_shot, "timer_shot");
    
    ALLEGRO_TIMER* timer_nod = al_create_timer(1.0/ 6.0);
    must_init(timer_nod, "timer_nod");
    
    ALLEGRO_TIMER* timer_transition = al_create_timer(3.0/ 1.0);
    must_init(timer_transition, "timer_transition");

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
    al_register_event_source(queue, al_get_timer_event_source(timer_transition));
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
    int transition = 0;

    ALLEGRO_EVENT event;
    
    al_start_timer(timer);
    al_start_timer(timer_shot);
    al_start_timer(board.timer_enemy);
    al_start_timer(timer_nod);
    

    while(!done){
        
        al_wait_for_event(queue, &event);
        if(redraw && al_is_event_queue_empty(queue)){
            disp_pre_draw(&display);
            menu_draw(buttons, &graphics, &juego, game_states);
            call_audio(&audio,&board,game_states);
            if(game_states == STATE_PLAY){
                
                //draw_all(&juego, &graphics, &board);              
                player_draw(&juego, &graphics);
                vel_nod(&juego, &board);
                hud_draw(font, &juego, &graphics);
                enemies_draw(&graphics, &board);
                shots_draw(&graphics, &board);
                muro_draw(&graphics, &board);
                navnod_draw(&graphics, &board);
                explosion_draw(&graphics, &board ); 
            }
            if((game_states == TRANSITION_LEVELUP) || (game_states == TRANSITION_LEVELSTART) || (game_states == TRANSITION_GAMEOVER)){
                menu_draw(buttons, &graphics, &juego, STATE_PLAY);
                player_draw(&juego, &graphics);
                muro_draw(&graphics, &board);
                enemies_draw(&graphics, &board);
                hud_draw(font, &juego, &graphics);
                transition_draw(&graphics, game_states);
                if(transition){
                    board_init(&board);
                    if(game_states == TRANSITION_LEVELUP){
                        game_states = STATE_PLAY;
                    }
                    else if(game_states == TRANSITION_LEVELSTART){
                        game_states = STATE_PLAY;
                    }
                    else if(game_states == TRANSITION_GAMEOVER){
                        game_states = STATE_GAMEOVER;
                        buttons[4][1].keyboard = 1;
                    } 
                    al_stop_timer(timer_transition);
                    transition = 0;
                }
            }
            disp_post_draw(&display);
            redraw = false;
        }
        
        
        switch(event.type){
            case ALLEGRO_EVENT_TIMER:
                if(event.timer.source == timer){
                    redraw = true;
                    //call_audio(&audio,&board);
                    if(game_states == STATE_EXIT)
                        done = true;
                    if(game_states == STATE_PLAY){
                        game_states = game_update(&juego, game_states);                     
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
                    
                    for(k = 0; k < MAX_EXPLOSIONS; k++){
                        if(board.explosion[k].objeto != NADA){
                            al_start_timer(timer_explosion);
                        }
                    }
                    if(event.timer.source == timer_explosion){
                        for(k = 0; k < MAX_EXPLOSIONS; k++){
                            if(board.explosion[k].objeto != NADA){
                                board.explosion[k].objeto = NADA;
                            }
                        }
                        al_stop_timer(timer_explosion);
                    }
                            
                }
                if((game_states == TRANSITION_LEVELUP) || (game_states == TRANSITION_LEVELSTART) || (game_states == TRANSITION_GAMEOVER)){
                    al_start_timer(timer_transition);
                    board_update(&juego, &board);
                    if(event.timer.source == timer_transition){
                        transition = !transition;
                    }
                }
                break;
            case ALLEGRO_EVENT_KEY_DOWN:
                game_states = menu_update(&event, &juego, buttons, game_states);
                break;
           
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }
        
        keyboard_update(&event);
    }
    
    disp_deinit(&display);
    hud_deinit(font);
    graphics_deinit(&graphics);
    audio_deinit(&audio);
    al_destroy_timer(timer);
    al_destroy_timer(timer_shot);
    al_destroy_timer(board.timer_enemy);
    al_destroy_timer(timer_explosion);
    al_destroy_timer(timer_transition);
    al_destroy_timer(timer_nod);
    al_destroy_event_queue(queue);
    
    return 0;
    
}

