#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>

#include "../../../headers/backend.h" 
#include "../../../headers/allegro.h" 

int game_states;

ALLEGRO_MOUSE_STATE mouse;

void must_init(bool test, const char *description)
{
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

ALLEGRO_DISPLAY* disp;
ALLEGRO_BITMAP* buffer;

void disp_init(){
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

    disp = al_create_display(DISP_W, DISP_H);
    must_init(disp, "display");

    buffer = al_create_bitmap(BUFFER_W, BUFFER_H);
    must_init(buffer, "bitmap buffer");
}

void disp_deinit(){
    al_destroy_bitmap(buffer);
    al_destroy_display(disp);
}

void disp_pre_draw(){
    al_set_target_bitmap(buffer);
}

void disp_post_draw(){
    al_set_target_backbuffer(disp);
    al_draw_scaled_bitmap(buffer, 0, 0, BUFFER_W, BUFFER_H, 0, 0, DISP_W, DISP_H, 0);

    al_flip_display();
}

// --- keyboard ---

unsigned char key[ALLEGRO_KEY_MAX];

void keyboard_init(){
    memset(key, 0, sizeof(key));
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
        case ALLEGRO_EVENT_TIMER:  
            if(key[ALLEGRO_KEY_RIGHT]) key[ALLEGRO_KEY_RIGHT] &= KEY_SEEN;
            if(key[ALLEGRO_KEY_LEFT]) key[ALLEGRO_KEY_LEFT] &= KEY_SEEN;
            if(key[ALLEGRO_KEY_SPACE]) key[ALLEGRO_KEY_SPACE] &= KEY_SEEN;
            break;
    }
}

//------sonido------------

/*ALLEGRO_SAMPLE* game_sound;
ALLEGRO_SAMPLE* option_sound;*/
ALLEGRO_SAMPLE* shot_sound;
ALLEGRO_SAMPLE* collision_sound;
ALLEGRO_SAMPLE* gameover_sound;

void audio_init(){
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(128);

    /*game_sound = al_load_sample("shot.flac");
    must_init(game_sound, "game sound");

    option_sound = al_load_sample("explode2.flac");
    must_init(option_sound, "option selected sound");*/
    
    shot_sound = al_load_sample(SHOT_SOUND);
    must_init(shot_sound, "shot sound");
    
    collision_sound = al_load_sample(COLLISION_SOUND);
    must_init(shot_sound, "coliision sound sound");
    
    gameover_sound = al_load_sample(GAMEOVER_SOUND);
    must_init(gameover_sound, "game over sound");
}

void audio_deinit(){
    /*al_destroy_sampvoid call_nod(void){// funcion que genera el llamado aleatorio para la nave nodriza.
  static int nav=0;
  static int random=0;
  if(nav == 0){ //si no hay nave nodriza en el juego, que busque crearla cuando coincida los rangos que emite rand.
      random = (rand () % 10) ; //numero entre 25 y 35
      if((random >=5)&& (random<=7)){
          nav=nav_nod();
          random=0;
        }
    }
  else{
      nav=nav_nod();//cuando ya hay nave, solamente la mueve.
    }
}le(game_sound);
    al_destroy_sample(option_sound);*/
    al_destroy_sample(shot_sound);
    al_destroy_sample(collision_sound);
    al_destroy_sample(gameover_sound);
}

//------------------------

ALLEGRO_FONT* font;
long score_display;

void hud_init(){
    font = al_create_builtin_font();
    must_init(font, "font");

    score_display = 0;
}

void hud_deinit(){
    al_destroy_font(font);
}

void hud_draw(juego_t * juego){
    char puntaje_str[10];
    sprintf(puntaje_str, "%i", juego->puntaje);
    al_draw_text(font, WHITE, 1, 1, 0, puntaje_str);

    for(int i = 0; i < juego->vidas; i++)
        al_draw_filled_circle(10 + i*10, 14, 5, WHITE);
}

//------graphics------------

GRAPHICS graphics;

void graphics_init()
{
    //graphics.menu_background = al_load_bitmap("/home/famolina/Escritorio/Recursos/space-invaders-1978-cabinet-artwork-1/invadbez.png");
    graphics.menu_background = al_load_bitmap(MENU_BACKGROUND);
    must_init(graphics.menu_background, "menu background");
  
    graphics.game_background = al_load_bitmap(GAME_BACKGROUND);
    must_init(graphics.game_background, "game background");
    
    graphics.enemy_bitmap = al_load_bitmap(ENEMY_BMP);
    must_init(graphics.enemy_bitmap, "enemy bitmap");
    
    graphics.enemy2_bitmap = al_load_bitmap(ENEMY2_BMP);
    must_init(graphics.enemy2_bitmap, "enemy 2 bitmap");
    
    graphics.enemy3_bitmap = al_load_bitmap(ENEMY3_BMP);
    must_init(graphics.enemy3_bitmap, "enemy 3 bitmap");
    
    graphics.navnod1_bitmap = al_load_bitmap(NAVNOD1_BMP);
    must_init(graphics.navnod1_bitmap, "nave nodriza 1 bitmap");
    
    graphics.navnod2_bitmap = al_load_bitmap(NAVNOD2_BMP);
    must_init(graphics.navnod2_bitmap, "nave nodriza 2 bitmap");
    
    graphics.player_bitmap = al_load_bitmap(PLAYER_BMP);
    must_init(graphics.player_bitmap, "player bitmap");
    
     graphics.muro_bitmap = al_load_bitmap(MURO_BMP);
    must_init(graphics.muro_bitmap, "muro bitmap");
}

void graphics_deinit()
{
    al_destroy_bitmap(graphics.menu_background);
    al_destroy_bitmap(graphics.game_background);
    al_destroy_bitmap(graphics.enemy_bitmap);
    al_destroy_bitmap(graphics.enemy2_bitmap);
    al_destroy_bitmap(graphics.enemy3_bitmap);
    al_destroy_bitmap(graphics.navnod1_bitmap);
    al_destroy_bitmap(graphics.navnod2_bitmap);
    al_destroy_bitmap(graphics.player_bitmap);
}

/*****************************************************
*--------------- LOGICA DE JUEGO----------------------
******************************************************/
juego_t juego;

//-------- lee teclado e indica movimiento de jugador --------
void move_player(ALLEGRO_EVENT ev){ 
    if(key[ALLEGRO_KEY_SPACE] && key[ALLEGRO_KEY_RIGHT]){
        juego.mov = SHOOT_RIGHT;
    }  
    else if(key[ALLEGRO_KEY_SPACE] && key[ALLEGRO_KEY_LEFT]){
        juego.mov = SHOOT_LEFT;
    }  
    else if(key[ALLEGRO_KEY_SPACE]){
        juego.mov = SHOOT;
    }   
    else if(key[ALLEGRO_KEY_RIGHT]){
        juego.mov = RIGHT;
    }
    else if(key[ALLEGRO_KEY_LEFT]){
        juego.mov = LEFT;
    }
}

//-------- actualiza juego (hasta ahora enemigos y disparo por separado, luego ver timers) --------

void game_update(ALLEGRO_EVENT ev, juego_t * juego){
    keyboard_update(&ev);
    move_player(ev);
    getcoordp(juego);
    pmov(juego);
    verparams(juego);
    if(juego->naves == 0){
        ininiv(juego->nivel);
    }
}

coord_t pshot;
coord_t muro[MAX_MURO];

enemylogic_t enemy_logic;


int frontboard[LARGO][ANCHO];

//-------- lee tablero y guarda informacion --------

void board_update(juego_t * juego){
    int i, j, a, b, c; 
    //frontboard[1][1] = juego->tablero;
    pshot.objeto = NADA;    //previamente limpio lo que habia
    enemy_logic.navnod.objeto = NADA;
    for (i = 0, a = 0, b = 0, c = 0; i < LARGO ; i++) {
        for (j = 0; j < ANCHO; j++){  
            frontboard[i][j] = getmat(i, j);
            switch(frontboard[i][j]){
                case(NADA):
                    enemy_logic.shot[a].objeto = NADA;    
                    enemy_logic.cell[b].objeto = NADA;     
                    muro[c].objeto = NADA;  //limpio antes
                    break;
                case(PLAYER):
                    break;
                case(PSHOT):
                    pshot.i = i;
                    pshot.j = j; 
                    pshot.objeto = frontboard[i][j];
                    break;
                case(ESHOT):
                    enemy_logic.shot[a].i = i;
                    enemy_logic.shot[a].j = j; 
                    enemy_logic.shot[a].objeto = frontboard[i][j];
                    a += 1;
                    break;
                case(ENEMY1):
                case(ENEMY2):
                case(ENEMY3):
                case(ENEMYSHOT1):
                case(ENEMYSHOT2):
                case(ENEMYSHOT3): 
                    enemy_logic.cell[b].i = i;
                    enemy_logic.cell[b].j = j; 
                    enemy_logic.cell[b].objeto = frontboard[i][j]; 
                    b += 1;
                    break;
                case(NAVNOD):
                    enemy_logic.navnod.i = i;
                    enemy_logic.navnod.j = j;
                    enemy_logic.navnod.objeto = frontboard[i][j];
                    j += 1;
                    break;
                case(MURO):
                    muro[c].i = i;
                    muro[c].j = j;
                    muro[c].objeto = frontboard[i][j];
                    c += 1;
                    break;
            }
        }
    }
    enemy_logic.max_shots = a;
    enemy_logic.max_enemies = b;
}

//-------- movimiento de disparos --------

void shots_update(ALLEGRO_EVENT ev){
    int n;
    coord_t eventoe, eventop;
    if(pshot.objeto == PSHOT){
        eventop = ciclodispp(&juego, pshot.i, pshot.j);
        /*if(eventop.objeto == NADA){ 
        ACA VAN COLISIONES, ETC
        }*/
    }
    for(n = 0; n < enemy_logic.max_shots; n++){
        if(enemy_logic.shot[n].objeto == ESHOT){
            eventoe = ciclodispe(&juego, enemy_logic.shot[n].i, enemy_logic.shot[n].j);
            /*
            if(eventoe.objeto == NADA){
            ACA VAN COLISIONES, ETC
            }*/
        }
    } 
}

//-------- movimiento y disparo de enemigos --------

void enemies_update(juego_t * juego){ //va a manejar timers de enemigos segun nivel
    navdisp();
    if(ciclonaves(juego)) juego->vidas = 0;
}

/*****************************************************
*------------------CONTROL DE MENU--------------------
******************************************************/

void menu_update(ALLEGRO_EVENT ev, BUTTON * buttons[]){
    
    switch(game_states){
        case(STATE_START):
            if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER){
                game_states = STATE_MENU;
                buttons[0][0].keyboard = 1; 
            }
            //keyboard_update(&ev);
            break;
        case(STATE_MENU):
            if(buttons[0][0].keyboard){
                if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER){
                    game_states = STATE_PLAY; //habria que poner un break adentro para evitar presionar otro boton rapido??
                    buttons[0][0].keyboard = 0;
                    inigame(&juego, 1);
                    //board_init(&juego);
                }
                else if(ev.keyboard.keycode == ALLEGRO_KEY_UP){
                    buttons[0][2].keyboard = 1;
                    buttons[0][0].keyboard = 0;
                }
                else if(ev.keyboard.keycode == ALLEGRO_KEY_DOWN){
                    buttons[0][1].keyboard = 1;
                    buttons[0][0].keyboard = 0;
                }
            } 
            else if(buttons[0][1].keyboard){
                if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER){
                    game_states = STATE_STATS;
                    buttons[1][0].keyboard = 1;
                    buttons[0][1].keyboard = 0;
                }
                else if(ev.keyboard.keycode == ALLEGRO_KEY_UP){
                    buttons[0][0].keyboard = 1;
                    buttons[0][1].keyboard = 0;
                }
                else if(ev.keyboard.keycode == ALLEGRO_KEY_DOWN){
                    buttons[0][2].keyboard = 1;
                    buttons[0][1].keyboard = 0;
                }
            } 
            else if(buttons[0][2].keyboard){
                if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER){
                    game_states = STATE_EXIT;
                    buttons[0][2].keyboard = 0;
                }
                else if(ev.keyboard.keycode == ALLEGRO_KEY_UP){
                    buttons[0][1].keyboard = 1;
                    buttons[0][2].keyboard = 0;
                }
                else if(ev.keyboard.keycode == ALLEGRO_KEY_DOWN){
                    buttons[0][0].keyboard = 1;
                    buttons[0][2].keyboard = 0;
                }
            }          
            break;   
            
        case(STATE_PLAY):
            if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER){
                game_states = STATE_PAUSE;         
                buttons[2][0].keyboard = 1;
            }
            else if(juego.vidas == 0){
                game_states = STATE_GAMEOVER;
                buttons[4][0].keyboard = 1;
            }
            break;
        case(STATE_STATS):
            if(buttons[1][0].keyboard){
                if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER){
                    game_states = STATE_MENU;
                    buttons[0][0].keyboard = 1;
                    buttons[1][0].keyboard = 0;
                }
            }
            break; 
        case(STATE_PAUSE):
            if(buttons[2][0].keyboard){
                if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER){
                    game_states = STATE_PLAY;
                    buttons[2][0].keyboard = 0;
                }
                else if(ev.keyboard.keycode == ALLEGRO_KEY_UP){
                    buttons[2][1].keyboard = 1;
                    buttons[2][0].keyboard = 0;
                    break;
                }
                else if(ev.keyboard.keycode == ALLEGRO_KEY_DOWN){
                    buttons[2][1].keyboard = 1;
                    buttons[2][0].keyboard = 0;
                }
            }
            else if(buttons[2][1].keyboard){
                if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER){
                    game_states = STATE_MENU;
                    buttons[0][0].keyboard = 1;
                    buttons[2][1].keyboard = 0;
                }
                else if(ev.keyboard.keycode == ALLEGRO_KEY_UP){
                    buttons[2][0].keyboard = 1;
                    buttons[2][1].keyboard = 0;
                }
                else if(ev.keyboard.keycode == ALLEGRO_KEY_DOWN){
                    buttons[2][0].keyboard = 1;
                    buttons[2][1].keyboard = 0;
                }
            }
            break;
        case(STATE_GAMEOVER):
            if(buttons[4][0].keyboard){
                if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER){
                    game_states = STATE_STATS;
                    buttons[1][0].keyboard = 1;
                    buttons[4][0].keyboard = 0;
                }
                else if(ev.keyboard.keycode == ALLEGRO_KEY_UP){
                    buttons[4][1].keyboard = 1;
                    buttons[4][0].keyboard = 0;
                }
                else if(ev.keyboard.keycode == ALLEGRO_KEY_DOWN){
                    buttons[4][1].keyboard = 1;
                    buttons[4][0].keyboard = 0;
                }
            }
            else if(buttons[4][1].keyboard){
                if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER){
                    game_states = STATE_MENU;
                    buttons[0][0].keyboard = 1;
                    buttons[4][1].keyboard = 0;
                }
                else if(ev.keyboard.keycode == ALLEGRO_KEY_UP){
                    buttons[4][0].keyboard = 1;
                    buttons[4][1].keyboard = 0;
                }
                else if(ev.keyboard.keycode == ALLEGRO_KEY_DOWN){
                    buttons[4][0].keyboard = 1;
                    buttons[4][1].keyboard = 0;
                }
            }
            break;
    }
} 

void menu_draw(ALLEGRO_EVENT ev, BUTTON * buttons[]){
    switch(game_states){
        case(STATE_START):
            al_clear_to_color(BLACK);
            al_draw_bitmap(graphics.menu_background,0,0,0);
            al_draw_text(buttons[3][0].font, WHITE, DISP_W / 2, buttons[3][0].y, ALLEGRO_ALIGN_CENTER, buttons[3][0].text);
            break;
        case(STATE_MENU):
            al_clear_to_color(al_map_rgb(0,0,0));
            al_draw_bitmap(graphics.menu_background,0,0,0);
            for(int i=0; i<3; i++){
                if(buttons[0][i].keyboard)
                    al_draw_text(buttons[0][i].font, RED, DISP_W / 2, buttons[0][i].y, ALLEGRO_ALIGN_CENTER, buttons[0][i].text);
                else
                    al_draw_text(buttons[0][i].font, WHITE, DISP_W / 2, buttons[0][i].y, ALLEGRO_ALIGN_CENTER, buttons[0][i].text);
            }
            break;
        case(STATE_PLAY):
            al_clear_to_color(al_map_rgb(0,0,0));
            al_draw_bitmap(graphics.game_background,0,0,0);
            getcoordp(&juego);
            al_draw_bitmap(graphics.player_bitmap, SCALE*juego.coordsp.j - CELL/2, SCALE*juego.coordsp.i - CELL/2, 0);
            hud_draw(&juego);
            break;
            
        case(STATE_GAMEOVER):
            al_clear_to_color(al_map_rgb(0,0,0));
            al_draw_text(buttons[4][0].font, WHITE, DISP_W / 2, buttons[4][0].y - 100, ALLEGRO_ALIGN_CENTER, "G A M E  O V E R");
            for(int i=0; i<2; i++){
                if(buttons[4][i].keyboard)
                    al_draw_text(buttons[4][i].font, RED, DISP_W / 2, buttons[4][i].y, ALLEGRO_ALIGN_CENTER, buttons[4][i].text);
                else
                    al_draw_text(buttons[4][i].font, WHITE, DISP_W / 2, buttons[4][i].y, ALLEGRO_ALIGN_CENTER, buttons[4][i].text);
            }
            break;
            
        case(STATE_STATS):
            al_clear_to_color(al_map_rgb(0,0,0));
            al_draw_bitmap(graphics.menu_background,0,0,0);
            if(buttons[1][0].keyboard)
                al_draw_text(buttons[1][0].font, RED, DISP_W / 2, buttons[1][0].y, ALLEGRO_ALIGN_CENTER, buttons[1][0].text);
            else
                al_draw_text(buttons[1][0].font, WHITE, DISP_W / 2, buttons[1][0].y, ALLEGRO_ALIGN_CENTER, buttons[1][0].text);
            break;
        case(STATE_PAUSE):
            al_clear_to_color(al_map_rgb(0,0,0));
            al_draw_bitmap(graphics.game_background,0,0,0);
            for(int i=0; i<2; i++){
                if(buttons[2][i].keyboard)
                    al_draw_text(buttons[2][i].font, RED, DISP_W / 2, buttons[2][i].y, ALLEGRO_ALIGN_CENTER, buttons[2][i].text);
                else
                    al_draw_text(buttons[2][i].font, WHITE, DISP_W / 2, buttons[2][i].y, ALLEGRO_ALIGN_CENTER, buttons[2][i].text);
            }
            break;
    }
}    

void enemies_draw(void){
    int n, aux;
    for(n = 0, aux = 1; n < enemy_logic.max_enemies && aux == 1; n++){
        switch(enemy_logic.cell[n].objeto){
             case(ENEMY1):
             case(ENEMYSHOT1):
                al_draw_bitmap(graphics.enemy_bitmap, SCALE*enemy_logic.cell[n].j - CELL/2  ,SCALE*enemy_logic.cell[n].i - CELL/2, 0);
                break;
             case(ENEMY2):
             case(ENEMYSHOT2):
                al_draw_bitmap(graphics.enemy2_bitmap, SCALE*enemy_logic.cell[n].j - CELL/2 , SCALE*enemy_logic.cell[n].i - CELL/2, 0);
                break;    
             case(ENEMY3):
             case(ENEMYSHOT3):
                al_draw_bitmap(graphics.enemy3_bitmap, SCALE*enemy_logic.cell[n].j - CELL/2, SCALE*enemy_logic.cell[n].i - CELL/2, 0);
                break;
             default:
                aux = 0;
                break;
        }                        
    }
}

void navnod_draw(void){
    if(enemy_logic.navnod.objeto == NAVNOD){
        al_draw_bitmap(graphics.navnod1_bitmap, SCALE *enemy_logic.navnod.j - CELL/2, SCALE * enemy_logic.navnod.i - CELL/2, 0);
    }
    
}

void call_nod(void){// funcion que genera el llamado aleatorio para la nave nodriza.
  static int nav=0;
  static int random=0;
  if(nav == 0){ //si no hay nave nodriza en el juego, que busque crearla cuando coincida los rangos que emite rand.
      random = (rand () % 15) ; //numero entre 25 y 35
      if((random >=5)&& (random<=7)){
          nav=nav_nod();
          random=0;
        }
    }
  else{
      nav=nav_nod();//cuando ya hay nave, solamente la mueve.
    }
}

void shots_draw(ALLEGRO_EVENT ev){
    int n, aux;
    if(pshot.objeto == PSHOT){
        al_draw_line(SCALE * pshot.j, SCALE * pshot.i - 5, SCALE * pshot.j, SCALE * pshot.i + 5, RED, 4);
        if(pshot.i > LARGO - 2) al_play_sample(shot_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
    for(n = 0; n < enemy_logic.max_shots; n++){
        if(enemy_logic.shot[n].objeto == ESHOT){
            al_draw_line(SCALE * enemy_logic.shot[n].j, SCALE * enemy_logic.shot[n].i - 5, SCALE * enemy_logic.shot[n].j, SCALE * enemy_logic.shot[n].i + 5, GREEN, 4);
        }                        
    }
}

void muro_draw(void){
    int n, aux;
    for(n = 0, aux = 1; n < MAX_MURO && aux == 1; n++){
        if(muro[n].objeto == MURO){
            al_draw_bitmap(graphics.muro_bitmap, SCALE*muro[n].j - CELL/2, SCALE*muro[n].i - CELL/2, 0);
        }       
        else aux = 0;
    }                        
}



int main(void){
   
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");
    must_init(al_install_mouse(), "mouse");
    must_init(al_install_audio(), "audio");
    must_init(al_init_acodec_addon(), "audio codecs");
    must_init(al_reserve_samples(16), "reserve samples");
    must_init(al_init_primitives_addon(), "primitives");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 40.0);
    must_init(timer, "timer");
    
    ALLEGRO_TIMER* timer_shot = al_create_timer(1 / 30.0);
    must_init(timer_shot, "timer_shot");
    
    ALLEGRO_TIMER* timer_nod = al_create_timer(1 / 6.0);
    must_init(timer_nod, "timer_nod");

    enemy_logic.timers[0] = al_create_timer(1 / 1.0);
    must_init(enemy_logic.timers[0], "timer_enemy");
    
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");
    
    ALLEGRO_FONT * font = al_create_builtin_font();
    must_init(font, "font");
    
    must_init(al_init_image_addon(), "image");
    
    disp_init();
    audio_init();
    hud_init();
    keyboard_init();
    graphics_init();
    
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_timer_event_source(timer_nod));
    al_register_event_source(queue, al_get_timer_event_source(timer_shot));
    al_register_event_source(queue, al_get_timer_event_source(enemy_logic.timers[0]));
    al_register_event_source(queue, al_get_mouse_event_source());
    
    BUTTON buttons_menu [] = {{300,25, "START", font, 0},
                        {350,25, "STATS", font, 0},
                        {400,25, "EXIT", font, 0}};
    
    BUTTON buttons_stats [] = {{450,25, "BACK", font, 0}};
    
    BUTTON buttons_pause [] = {{300,25, "BACK", font, 0}, {400,25, "EXIT", font, 0}};
    
    BUTTON buttons_gameover [] = {{300,25, "STATS", font, 0}, {400,25, "EXIT", font, 0}};
    
    BUTTON buttons_start [] = {{350,25, "PRESS ENTER", font, 1}};
    
    BUTTON * buttons[5];
    
    buttons[0] = buttons_menu;
    buttons[1] = buttons_stats;
    buttons[2] = buttons_pause;
    buttons[3] = buttons_start;
    buttons[4] = buttons_gameover;
    
    bool redraw = true;
    
    bool done = false;
    
    game_states = STATE_START;
    
    ALLEGRO_EVENT event;
    
    al_start_timer(timer);
    al_start_timer(timer_shot);
    al_start_timer(enemy_logic.timers[0]);
    al_start_timer(timer_nod);
    while(!done){
        
        al_wait_for_event(queue, &event);
        
        if(redraw && al_is_event_queue_empty(queue)){
            disp_pre_draw();
            menu_draw(event, buttons);
            if(game_states == STATE_PLAY){
                
                enemies_draw();
                shots_draw(event);
                muro_draw();
                navnod_draw();
            }
            disp_post_draw();
            redraw = false;
        }
        
        
        switch(event.type){
            case ALLEGRO_EVENT_TIMER:
                if(event.timer.source == timer){
                    redraw = true;
                    if(game_states == STATE_EXIT)
                        done = true;
                    if(game_states == STATE_PLAY){
                        game_update(event, &juego);                     
                    }
                }
                if(event.timer.source == timer_shot){
                    if(game_states == STATE_PLAY){
                        board_update(&juego);
                        shots_update(event);
                    }   
                }   
                if(event.timer.source == enemy_logic.timers[0]){
                    if(game_states == STATE_PLAY){
                        enemies_update(&juego);
                        
                    }
                }
                if(event.timer.source == timer_nod){
                    if(game_states == STATE_PLAY){
                        call_nod();
                    }   
                }   
                break;
            case ALLEGRO_EVENT_KEY_DOWN:
                menu_update(event, buttons);
                break;
           
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }
        
        keyboard_update(&event);
    }
    
    disp_deinit();
    al_destroy_font(font);
    graphics_deinit();
    hud_deinit();
    audio_deinit();
    al_destroy_timer(timer);
    al_destroy_timer(timer_shot);
    al_destroy_timer(enemy_logic.timers[0]);
    al_destroy_event_queue(queue);
    
    return 0;
    
}

