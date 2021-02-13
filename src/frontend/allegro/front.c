#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>

#include "../../../headers/backend.h" 

#define STATE_START 0
#define STATE_MENU  1
#define STATE_PLAY  2
#define STATE_STATS 3
#define STATE_EXIT  4
#define STATE_PAUSE 5

#define BLACK   al_map_rgb(0,0,0)
#define WHITE   al_map_rgb(155,155,155)
#define RED     al_map_rgb(255,0,0)

#define MENU_BACKGROUND "res/img/invaddx.png"
#define GAME_BACKGROUND "res/img/invaders.png"
#define ENEMY_BMP "res/img/saucer1b.png"
#define ENEMY2_BMP "res/img/saucer2b.png"
#define ENEMY3_BMP "res/img/saucer3b.png"
#define NAVNOD_BMP "res/img/mysteryb.png"
#define PLAYER_BMP "res/img/baseshipa.png"

#define GAME_SOUND "../../../res/audio/"
#define OPTION_SOUND "../../../res/audio/"
#define SHOT_SOUND "res/audio/disparo.wav"
#define COLLISION_SOUND "res/audio/nave impacto.wav"
#define GAMEOVER_SOUND "res/audio/game over.wav"

int game_states;

ALLEGRO_MOUSE_STATE mouse;

void must_init(bool test, const char *description)
{
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

#define BUFFER_W 640
#define BUFFER_H 640
#define DISP_SCALE 1
#define DISP_W (BUFFER_W * DISP_SCALE)
#define DISP_H (BUFFER_H * DISP_SCALE)

ALLEGRO_DISPLAY* disp;
ALLEGRO_BITMAP* buffer;

void disp_init()
{
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

    disp = al_create_display(DISP_W, DISP_H);
    must_init(disp, "display");

    buffer = al_create_bitmap(BUFFER_W, BUFFER_H);
    must_init(buffer, "bitmap buffer");
}

void disp_deinit()
{
    al_destroy_bitmap(buffer);
    al_destroy_display(disp);
}

void disp_pre_draw()
{
    al_set_target_bitmap(buffer);
}

void disp_post_draw()
{
    al_set_target_backbuffer(disp);
    al_draw_scaled_bitmap(buffer, 0, 0, BUFFER_W, BUFFER_H, 0, 0, DISP_W, DISP_H, 0);

    al_flip_display();
}


// --- keyboard ---

#define KEY_SEEN     1
#define KEY_RELEASED 2
unsigned char key[ALLEGRO_KEY_MAX];

void keyboard_init()
{
    memset(key, 0, sizeof(key));
}

void keyboard_update(ALLEGRO_EVENT* event)
{
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

//------------------------
/*ALLEGRO_SAMPLE* game_sound;
ALLEGRO_SAMPLE* option_sound;*/
ALLEGRO_SAMPLE* shot_sound;
ALLEGRO_SAMPLE* collision_sound;
ALLEGRO_SAMPLE* gameover_sound;

void audio_init()
{
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

void audio_deinit()
{
    /*al_destroy_sample(game_sound);
    al_destroy_sample(option_sound);*/
    al_destroy_sample(shot_sound);
    al_destroy_sample(collision_sound);
    al_destroy_sample(gameover_sound);
}

//------------------------

ALLEGRO_FONT* font;
long score_display;
/*
void hud_init()
{
    font = al_create_builtin_font();
    must_init(font, "font");

    score_display = 0;
}

void hud_deinit()
{
    al_destroy_font(font);
}*/
/*
void hud_update()
{
    if(frames % 2)
        return;

    for(long i = 5; i > 0; i--)
    {
        long diff = 1 << i;
        if(score_display <= (score - diff))
            score_display += diff;
    }
}*/

void hud_draw(int vidas, int puntaje, ALLEGRO_FONT * font)
{
    
    char puntaje_str[10];
    sprintf(puntaje_str, "%i", puntaje);
    al_draw_text(font, WHITE, 1, 1, 0, puntaje_str);

    //int spacing = LIFE_W + 1;
    for(int i = 0; i < vidas; i++)
        //al_draw_bitmap(sprites.life, 1 + (i * spacing), 10, 0);
        al_draw_filled_circle(10 + i*10, 14, 5, WHITE);

    if(vidas < 0)
        al_draw_text(
            font,
            al_map_rgb_f(1,1,1),
            BUFFER_W / 2, BUFFER_H / 2,
            ALLEGRO_ALIGN_CENTER,
            "G A M E  O V E R"
        );
}

//-------------------------------------

//void cell_draw(int int x, int y)
/*
void game_sounds(int n){
    switch(n){
        case(PSHOT):
        case(ESHOT):
            break;
        case(COLLISION):
            break;
        case(GAME_OVER):
            break;
    }
}
*/
/*
void game_draw(int space [LARGO][ANCHO]){
    for(int i = 0; i < LARGO*ANCHO; i++){
        switch(i){
            case(ENEMY):
                ALLEGRO_BITMAP* enemy = al_load_bitmap("misterya.png");
                al_draw_scaled_bitmap(enemy, 0, 0, 48, 48, i*CELL_X+ANCHO, i*CELL_Y+LARGO, CELL_X, CELL_Y, 0); 
                break;
            case(ENEMYSHOT):
                al_draw_scaled_bitmap(enemy, 0, 0, 48, 48, i*CELL_X+ANCHO, i*CELL_Y+LARGO, CELL_X, CELL_Y, 0);  
                break;
            case(NAVNOD):
                ALLEGRO_BITMAP* enemy = al_load_bitmap("misterya.png");
                al_draw_scaled_bitmap(enemy, 0, 0, 48, 48, i*CELL_X+ANCHO, i*CELL_Y+LARGO, CELL_X, CELL_Y, 0); 
                break;
            case(PSHOT):
                al_draw_line(i*CELL_X+ANCHO+CELL_X/2, i*CELL_Y+LARGO, i*CELL_X+ANCHO+CELL_X/2, i*CELL_Y+LARGO+CELL_Y, RED, 2);
                break;
            case(ESHOT):
                al_draw_line(i*CELL_X+ANCHO+CELL_X/2, i*CELL_Y+LARGO, i*CELL_X+ANCHO+CELL_X/2, i*CELL_Y+LARGO+CELL_Y, RED, 2);
                break;
            case(PLAYER):
                ALLEGRO_BITMAP* enemy = al_load_bitmap("misterya.png");
                al_draw_scaled_bitmap(enemy, 0, 0, 48, 48, i*CELL_X+ANCHO, i*CELL_Y+LARGO, CELL_X, CELL_Y, 0);  
                break;
            case(MURO): 
                al_draw_scaled_bitmap(enemy, 0, 0, 48, 48, i*CELL_X+ANCHO, i*CELL_Y+LARGO, CELL_X, CELL_Y, 0);  
                break;
        }
    }
}
*/
//------graphics------------

typedef struct GRAPHICS {
    ALLEGRO_BITMAP* menu_background;
    ALLEGRO_BITMAP* game_background;
    ALLEGRO_BITMAP* enemy_bitmap;
    ALLEGRO_BITMAP* enemy2_bitmap;
    ALLEGRO_BITMAP* enemy3_bitmap;
    ALLEGRO_BITMAP* navnod_bitmap;
    ALLEGRO_BITMAP* player_bitmap;
} GRAPHICS;

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
    
    graphics.enemy2_bitmap = al_load_bitmap(ENEMY_BMP);
    must_init(graphics.enemy2_bitmap, "enemy 2 bitmap");
    
    graphics.enemy3_bitmap = al_load_bitmap(ENEMY_BMP);
    must_init(graphics.enemy3_bitmap, "enemy 3 bitmap");
    
    graphics.navnod_bitmap = al_load_bitmap(NAVNOD_BMP);
    must_init(graphics.navnod_bitmap, "nave nodriza bitmap");
    
    graphics.player_bitmap = al_load_bitmap(PLAYER_BMP);
    must_init(graphics.player_bitmap, "player bitmap");
}

void graphics_deinit()
{
    al_destroy_bitmap(graphics.menu_background);
    al_destroy_bitmap(graphics.game_background);
    al_destroy_bitmap(graphics.enemy_bitmap);
    al_destroy_bitmap(graphics.enemy2_bitmap);
    al_destroy_bitmap(graphics.enemy3_bitmap);
    al_destroy_bitmap(graphics.navnod_bitmap);
    al_destroy_bitmap(graphics.player_bitmap);
}

//------------------------

typedef struct BUTTON {
    int x;
    int y;
    int w; 
    int h;
    const char * text;
    ALLEGRO_FONT * font;
    int keyboard; //flag de posicion sobre boton de menu
} BUTTON; 


juego_t juego;
coord_t disparo;

#define MOUSE       0
#define KEYBOARD    1

#define CELL  48

#define STAY    0
#define RIGHT   1
#define LEFT    2
#define SHOOT   3
#define SHOOT_RIGHT 4
#define SHOOT_LEFT  5

void move_player(ALLEGRO_EVENT ev){ //no me reconoce dos teclas presionadas al mismo tiempo //UPDATE: se soluciono con los else if 
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

void game_update(ALLEGRO_EVENT ev){
    keyboard_update(&ev);
    move_player(ev);
    getcoordp(&juego);
    pmov(&juego);
    
    //printf("%d \n", juego.coordsp.j);
    //printf("%d \n", juego.coordsp.i);
    /*
    getcoordp(&juego, PSHOT);
    if(juego.coordsp.objeto == PSHOT){
        ciclodisp(&juego, juego.coordsp.i, juego.coordsp.j);
        //printf("%d \n", juego.coordsp.objeto);
    }*/
}

#define MAX_SHOTS   30
#define MAX_ENEMIES 50
//coord_t pshot;
coord_t shots [MAX_SHOTS]; //el primero es el del jugador 
coord_t enemies [MAX_ENEMIES];        

int frontboard[LARGO + 2][ANCHO + 2];

void board_update(juego_t * juego){
    int i, j, n, object, k;
    n = 0;
    k = 0;
    //frontboard[1][1] = juego->tablero;
    for (i = 0; i < LARGO ; i++) {
        for (j = 0; j < ANCHO; j++){
            object = getmat(i, j);
            //object = * (juego->tablero * i + j);
            switch(object){
                case(NADA):
                    shots[n].objeto = object; //antes de cargar el disparo, lo limpio
                    enemies[k].objeto = object;
                    break;
                case(PLAYER):
                    juego->coordsp.i = i;
                    juego->coordsp.j = j;
                    juego->coordsp.objeto = object;
                    break;
                case(PSHOT):
                    /*shots[0].i = i;
                    shots[0].j = j; 
                    shots[0].objeto = object;
                    break;*/
                case(ESHOT):
                    //eshots[n] = {i, j, object}; 
                    shots[n].i = i;
                    shots[n].j = j; 
                    shots[n].objeto = object;
                    n += 1;
                    break;
                case(ENEMY):
                //case(ENEMY_2):
                //case(ENEMY_3):
                    //eshots[n] = {i, j, object}; 
                    al_draw_bitmap(graphics.enemy_bitmap, SCALE* j - CELL/2, SCALE * i - CELL, 0);
                    enemies[k].i = i;
                    enemies[k].j = j; 
                    enemies[k].objeto = object;
                    k += 1;
                   
                    break;
                case(MURO):
                    break;
            }
        }
    }
}

void shots_update(ALLEGRO_EVENT ev){
    int n, aux;
    coord_t evento;
    for(n = 0; n < MAX_SHOTS && aux; n++){
        if(shots[n].objeto == PSHOT || shots[n].objeto == ESHOT){
            evento = ciclodisp(&juego, shots[n].i, shots[n].j);
        }
        else aux = 0;                         
    } 
    /*
    int i, j;
    for (i = 0; i < LARGO; i++) {
        for (j = 0; j < ANCHO; j++){
            evento = ciclodisp(&juego, i, j);
        }
    }
    evento = ciclodisp(&juego, i, j);
    if(pshot.i == LARGO - 2){
        al_play_sample(shot_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
     */
}

int button_update(ALLEGRO_EVENT ev, BUTTON * button){
    if((mouse.x >= button->x && mouse.y >= button->y && mouse.x < (button->x + button->w) && mouse.y < (button->y + button->h))){
        if(al_mouse_button_down(&mouse, 1))
            return 1;
        else 
            return 2;
    }
}

void menu_update(ALLEGRO_EVENT ev, BUTTON * buttons[]){
    /*if(ev.type != ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
        return 0;*/
   /* return((mouse.x >= button->x && mouse.y >= button->y && 
            mouse.x < (button->x + button->w) && mouse.y < (button->y + button->h)) && al_mouse_button_down(&mouse, 1));*/
    //-------KEYBOARD INTERACTION-----------//
   
    
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
                    break;
                }
                if(ev.keyboard.keycode == ALLEGRO_KEY_UP){
                    buttons[0][2].keyboard = 1;
                    buttons[0][0].keyboard = 0;
                    break;
                }
                if(ev.keyboard.keycode == ALLEGRO_KEY_DOWN){
                    buttons[0][1].keyboard = 1;
                    buttons[0][0].keyboard = 0;
                    break;
                }
            } 
            if(buttons[0][1].keyboard){
                if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER){
                    game_states = STATE_STATS;
                    buttons[1][0].keyboard = 1;
                    buttons[0][1].keyboard = 0;
                    break;
                }
                if(ev.keyboard.keycode == ALLEGRO_KEY_UP){
                    buttons[0][0].keyboard = 1;
                    buttons[0][1].keyboard = 0;
                    break;
                }
                if(ev.keyboard.keycode == ALLEGRO_KEY_DOWN){
                    buttons[0][2].keyboard = 1;
                    buttons[0][1].keyboard = 0;
                    break;
                }
            } 
            if(buttons[0][2].keyboard){
                if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER){
                    game_states = STATE_EXIT;
                    buttons[0][2].keyboard = 0;
                    break;
                }
                if(ev.keyboard.keycode == ALLEGRO_KEY_UP){
                    buttons[0][1].keyboard = 1;
                    buttons[0][2].keyboard = 0;
                    break;
                }
                if(ev.keyboard.keycode == ALLEGRO_KEY_DOWN){
                    buttons[0][0].keyboard = 1;
                    buttons[0][2].keyboard = 0;
                    break;
                }
            }          
            //keyboard_update(&ev);
            break;   
            
        case(STATE_PLAY):
            if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER){
                game_states = STATE_PAUSE;
                buttons[2][0].keyboard = 1;
            }
            //keyboard_update(&ev);
            break;
            
        case(STATE_STATS):
            if(buttons[1][0].keyboard){
                if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER){
                    game_states = STATE_MENU;
                    buttons[0][0].keyboard = 1;
                    buttons[1][0].keyboard = 0;
                }
                //keyboard_update(&ev);
            }
            break;
          
        case(STATE_PAUSE):
            if(buttons[2][0].keyboard){
                if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER){
                    game_states = STATE_PLAY;
                    buttons[2][0].keyboard = 0;
                    break;
                }
                if(ev.keyboard.keycode == ALLEGRO_KEY_UP){
                    buttons[2][1].keyboard = 1;
                    buttons[2][0].keyboard = 0;
                    break;
                }
                if(ev.keyboard.keycode == ALLEGRO_KEY_DOWN){
                    buttons[2][1].keyboard = 1;
                    buttons[2][0].keyboard = 0;
                    break;
                }
                //keyboard_update(&ev);
            }
            if(buttons[2][1].keyboard){
                if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER){
                    game_states = STATE_MENU;
                    buttons[0][0].keyboard = 1;
                    buttons[2][1].keyboard = 0;
                    break;
                }
                if(ev.keyboard.keycode == ALLEGRO_KEY_UP){
                    buttons[2][0].keyboard = 1;
                    buttons[2][1].keyboard = 0;
                    break;
                }
                if(ev.keyboard.keycode == ALLEGRO_KEY_DOWN){
                    buttons[2][0].keyboard = 1;
                    buttons[2][1].keyboard = 0;
                    break;
                }
                //keyboard_update(&ev);
            }
    }
} 
    
    
    //-------MOUSE INTERACTION-----------//
    /*
    switch(game_states){
        case(STATE_START):
            if(button_update(ev, &buttons[3][0]) == 1) {game_states = STATE_MENU;}
            break;
        case(STATE_MENU):
            if(button_update(ev, &buttons[0][0]) == 1) {game_states = STATE_PLAY;}
            if(button_update(ev, &buttons[0][1]) == 1) {game_states = STATE_STATS;}
            if(button_update(ev, &buttons[0][2]) == 1) {game_states = STATE_EXIT;}
            break;
            
        case(STATE_PLAY):
            if(key[ALLEGRO_KEY_ENTER]) {game_states = STATE_PAUSE;}
            keyboard_update(&ev);
            break;
            
        case(STATE_STATS):
          if(button_update(ev, &buttons[1][0]) == 1) {game_states = STATE_MENU;}
          break;
          
        case(STATE_PAUSE):
            if(button_update(ev, &buttons[2][0]) == 1) {game_states = STATE_PLAY;}
            if(button_update(ev, &buttons[2][1]) == 1) {game_states = STATE_MENU;}
            break;
    }*/

#define SCALE 2

void menu_draw(ALLEGRO_EVENT ev, BUTTON * buttons[]){
    
    int vidas = 2;
    int puntaje = 300;
    //puntaje = 300;
    switch(game_states){
        case(STATE_START):
            al_clear_to_color(BLACK);
            al_draw_bitmap(graphics.menu_background,0,0,0);
            al_draw_text(buttons[3][0].font, WHITE, buttons[3][0].x, buttons[3][0].y, 0, buttons[3][0].text);
            break;
        case(STATE_MENU):
            al_clear_to_color(al_map_rgb(0,0,0));
            al_draw_bitmap(graphics.menu_background,0,0,0);
            for(int i=0; i<3; i++){
                if(buttons[0][i].keyboard)
                    al_draw_text(buttons[0][i].font, RED, buttons[0][i].x, buttons[0][i].y, 0, buttons[0][i].text);
                else
                    al_draw_text(buttons[0][i].font, WHITE, buttons[0][i].x, buttons[0][i].y, 0, buttons[0][i].text);
            }
            break;
        case(STATE_PLAY):
            al_clear_to_color(al_map_rgb(0,0,0));
            getcoordp(&juego);
            al_draw_bitmap(graphics.game_background,0,0,0);
            al_draw_bitmap(graphics.player_bitmap, SCALE*juego.coordsp.j - CELL/2, SCALE*juego.coordsp.i - CELL, 0);
            
            /*if(ev.timer.source == timer_shot){
                if(juego.coordsp.objeto == PSHOT){
                    al_draw_line(juego.coordsp.j, juego.coordsp.i - 5, juego.coordsp.j, juego.coordsp.i + 5, RED, 2);
                    //al_play_sample(shot_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                }
            }*/
            hud_draw(vidas, puntaje, buttons[1][0].font);
            break;
            
        case(STATE_STATS):
            al_clear_to_color(al_map_rgb(0,0,0));
            al_draw_bitmap(graphics.menu_background,0,0,0);
            if(buttons[1][0].keyboard)
                al_draw_text(buttons[1][0].font, RED, buttons[1][0].x, buttons[1][0].y, 0, buttons[1][0].text);
            else
                al_draw_text(buttons[1][0].font, WHITE, buttons[1][0].x, buttons[1][0].y, 0, buttons[1][0].text);
            break;
        case(STATE_PAUSE):
            al_clear_to_color(al_map_rgb(0,0,0));
            al_draw_bitmap(graphics.game_background,0,0,0);
            for(int i=0; i<2; i++){
                if(buttons[2][i].keyboard)
                    al_draw_text(buttons[2][i].font, RED, buttons[2][i].x, buttons[2][i].y, 0, buttons[2][i].text);
                else
                    al_draw_text(buttons[2][i].font, WHITE, buttons[2][i].x, buttons[2][i].y, 0, buttons[2][i].text);
            }
            break;
    }
}    

void shots_draw(ALLEGRO_EVENT ev){
    int n, aux;
    for(n = 0; n < MAX_SHOTS && aux; n++){
        if(shots[n].objeto == PSHOT){
            al_draw_line(SCALE * shots[n].j, SCALE * shots[n].i - 5, SCALE * shots[n].j, SCALE * shots[n].i + 5, RED, 4);
            if(shots[n].i > LARGO - 5) al_play_sample(shot_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        }
        else if(shots[n].objeto == ESHOT){
            al_draw_line(SCALE * shots[n].j, SCALE * shots[n].i - 5, SCALE * shots[n].j, SCALE * shots[n].i + 5, RED, 4);
        }
        else aux = 0;                         
    }
}
   
    /* MOUSE INTERACTION
    switch(game_states){
            case(STATE_START):
                al_clear_to_color(BLACK);
                al_draw_bitmap(graphics.background,0,0,0);
                al_draw_text(buttons[3][0].font, WHITE, buttons[3][0].x, buttons[3][0].y, 0, buttons[3][0].text);
                break;
            case(STATE_MENU):
                al_clear_to_color(al_map_rgb(0,0,0));
                for(int i=0; i<3; i++){
                    if(button_update(ev, &buttons[0][i]) == 2)
                        al_draw_text(buttons[0][i].font, RED, buttons[0][i].x, buttons[0][i].y, 0, buttons[0][i].text);
                    else
                        al_draw_text(buttons[0][i].font, WHITE, buttons[0][i].x, buttons[0][i].y, 0, buttons[0][i].text);
                }
                break;
            case(STATE_PLAY):
                al_clear_to_color(al_map_rgb(0,0,0));
                hud_draw(vidas, puntaje, buttons[1][0].font);
                al_draw_text(buttons[0][0].font, WHITE, buttons[0][0].x, buttons[0][0].y + 150, 0, "Holis 1");
                break;

            case(STATE_STATS):
                al_clear_to_color(al_map_rgb(0,0,0));
                if(button_update(ev, &buttons[1][0]) == 2)
                    al_draw_text(buttons[1][0].font, RED, buttons[1][0].x, buttons[1][0].y, 0, buttons[1][0].text);
                else
                    al_draw_text(buttons[1][0].font, WHITE, buttons[1][0].x, buttons[1][0].y, 0, buttons[1][0].text);
                break;
            case(STATE_PAUSE):
                al_clear_to_color(al_map_rgb(0,0,0));
                for(int i=0; i<2; i++){
                    if(button_update(ev, &buttons[2][i]) == 2)
                        al_draw_text(buttons[2][i].font, RED, buttons[2][i].x, buttons[2][i].y, 0, buttons[2][i].text);
                    else
                        al_draw_text(buttons[2][i].font, WHITE, buttons[2][i].x, buttons[2][i].y, 0, buttons[2][i].text);
                }
                break;
    }*/

    /*
    if(!(menu[0].pressed && menu[1].pressed && menu[2].pressed)){
        
    }
    if(menu[0].pressed){
        al_clear_to_color(al_map_rgb(0,0,0));
        al_draw_text(menu[0].font, color, menu[0].x, menu[0].y + 150, 0, "Holis 1");
        menu[0].pressed = 0; 
    }
    if(menu[1].pressed){
        al_clear_to_color(al_map_rgb(0,0,0));
        al_draw_text(menu[1].font, color, menu[1].x, menu[1].y + 150, 0, "Holis 2");
        menu[1].pressed = 0;
    }
/*
void update(ALLEGRO_EVENT ev){
    switch(MenuStates){
        case PLAY:
        case OPCIONES:
        case SALIR:
            break
    }
}
*/


int main(void){
    
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");
    must_init(al_install_mouse(), "mouse");
    must_init(al_install_audio(), "audio");
    must_init(al_init_acodec_addon(), "audio codecs");
    must_init(al_reserve_samples(16), "reserve samples");
    must_init(al_init_primitives_addon(), "primitives");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    must_init(timer, "timer");
    
    ALLEGRO_TIMER* timer_shot = al_create_timer(1 / 120.0);
    must_init(timer_shot, "timer_shot");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");
    
    ALLEGRO_FONT * font = al_create_builtin_font();
    must_init(font, "font");
    
    
    must_init(al_init_image_addon(), "image");
    
    disp_init();
    audio_init();
    //hud_init();
    keyboard_init();
    
    //GRAPHICS graphics;
    
    graphics_init();
    
    //Inicializar display, fuentes, etc.
    
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_timer_event_source(timer_shot));
    al_register_event_source(queue, al_get_mouse_event_source());
    
    BUTTON buttons_menu [] = {{310,300,100,25, "START", font, 0},
                        {310,350,100,25, "STATS", font, 0},
                        {310,400,100,25, "EXIT", font, 0}};
    
    BUTTON buttons_stats [] = {{310,450,100,25, "BACK", font, 0}};
    
    BUTTON buttons_pause [] = {{310,300,100,25, "BACK", font, 0}, {310,400,100,25, "EXIT", font, 0}};
    
    BUTTON buttons_start [] = {{290,350,100,25, "PRESS ENTER", font, 1}};
    
    /*BUTTON * pbuttons_menu = buttons_menu;
    BUTTON * pbuttons_stats = buttons_stats;*/
    
    BUTTON * buttons[4];
    
    buttons[0] = buttons_menu;
    buttons[1] = buttons_stats;
    buttons[2] = buttons_pause;
    buttons[3] = buttons_start;
   
    
    inigame(&juego, 1);
    
    
    //typedef struct BUTTON ARR_BUTTON [];
    
    //BUTTON (*buttons[2])[3] = {&buttons_menu, &buttons_stats};
    
   /* buttons[0] = &buttons_menu[0];
    buttons[1] = &buttons_stats[0];*/
    
    bool redraw = true;
    
    bool done = false;
    
    game_states = STATE_START;
    
    ALLEGRO_EVENT event;
    
    al_start_timer(timer);
    al_start_timer(timer_shot);
    
    while(!done){
        
        al_wait_for_event(queue, &event);
        
        if(redraw && al_is_event_queue_empty(queue)){
            disp_pre_draw();
            menu_draw(event, buttons);
            if(game_states == STATE_PLAY){
                shots_draw(event);
            }
            disp_post_draw();
            redraw = false;
        }
        
        
        switch(event.type){
            case ALLEGRO_EVENT_TIMER:
                if(event.timer.source == timer){
                    //al_get_mouse_state(&mouse);
                    //update();
                    //if(key[ALLEGRO_KEY_ESCAPE])
                    //    done = true;
                    redraw = true;
                    if(game_states == STATE_EXIT)
                        done = true;
                    if(game_states == STATE_PLAY)
                        game_update(event);
                }
                
                
                if(event.timer.source == timer_shot){
                    if(game_states == STATE_PLAY){
                        board_update(&juego);
                        shots_update(event);
                    }   
                }   
                break;
            
            case ALLEGRO_EVENT_KEY_DOWN:
                menu_update(event, buttons);
                break;
                
                
                
            
            /*case ALLEGRO_EVENT_KEY_CHAR:
                if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                    salida = true;    */
                
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }
        
        keyboard_update(&event);
    }
    
    disp_deinit();
    al_destroy_font(font);
    graphics_deinit();
    //hud_deinit();
    audio_deinit();
    al_destroy_timer(timer);
    al_destroy_timer(timer_shot);
    al_destroy_event_queue(queue);
    
    return 0;
    
}

