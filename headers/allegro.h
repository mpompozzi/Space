/*
 * The MIT License
 *
 * Copyright 2021 famolina.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/* 
 * File:   allegro.h
 * Author: famolina
 *
 * Created on 15 de febrero de 2021, 18:22
 */

#ifndef ALLEGRO_H
#define ALLEGRO_H

#ifdef __cplusplus
extern "C" {
#endif

// --- menu states ---
#define STATE_START 0
#define STATE_MENU  1
#define STATE_PLAY  2
#define STATE_STATS 3
#define STATE_EXIT  4
#define STATE_PAUSE 5
#define STATE_GAMEOVER    6
  
// --- colors ---
#define BLACK   al_map_rgb(0,0,0)
#define WHITE   al_map_rgb(155,155,155)
#define RED     al_map_rgb(255,0,0)
#define GREEN   al_map_rgb(0,155,0)
  
// --- bitmap ---
#define MENU_BACKGROUND "res/img/invaddx.png"
#define GAME_BACKGROUND "res/img/invaders.png"
#define ENEMY_BMP "res/img/saucer1a.png"
#define ENEMY2_BMP "res/img/saucer2a.png"
#define ENEMY3_BMP "res/img/saucer3a.png"
#define NAVNOD1_BMP "res/img/mysterya.png"
#define NAVNOD2_BMP "res/img/mysterya.png"
#define PLAYER_BMP "res/img/baseshipa.png"
#define MURO_BMP "res/img/muro.png"

// --- audio ---
#define GAME_SOUND "../../../res/audio/"
#define OPTION_SOUND "../../../res/audio/"
#define SHOT_SOUND "res/audio/disparo.wav"
#define COLLISION_SOUND "res/audio/naveImpacto.wav"
#define GAMEOVER_SOUND "res/audio/gameOver.wav"
  
// --- display ---
#define BUFFER_W 640
#define BUFFER_H 640
#define DISP_SCALE 1
#define DISP_W (BUFFER_W * DISP_SCALE)
#define DISP_H (BUFFER_H * DISP_SCALE)

// --- keyboard ---
#define KEY_SEEN     1
#define KEY_RELEASED 2
  
// --- keyboard ---
#define MOUSE       0
#define KEYBOARD    1
  
// --- player movement ---
#define STAY    0
#define RIGHT   1
#define LEFT    2
#define SHOOT   3
#define SHOOT_RIGHT 4
#define SHOOT_LEFT  5

// --- board constants ---
#define MAX_SHOTS       20
#define MAX_ENEMIES     30
#define MAX_MURO        50  
  
// --- draw constants ---  
#define CELL    20
#define SCALE 20
  
// --- graphic struct ---
typedef struct GRAPHICS {
    ALLEGRO_BITMAP* menu_background;
    ALLEGRO_BITMAP* game_background;
    ALLEGRO_BITMAP* enemy_bitmap;
    ALLEGRO_BITMAP* enemy2_bitmap;
    ALLEGRO_BITMAP* enemy3_bitmap;
    ALLEGRO_BITMAP* navnod1_bitmap;
    ALLEGRO_BITMAP* navnod2_bitmap;
    ALLEGRO_BITMAP* player_bitmap;
    ALLEGRO_BITMAP* muro_bitmap;
} GRAPHICS;

// --- buttons struct ---
typedef struct BUTTON {
    int y;
    int h;
    const char * text;
    ALLEGRO_FONT * font;
    int keyboard; //flag de posicion sobre boton de menu
} BUTTON; 

// --- enemies struct ---
typedef struct {
  ALLEGRO_TIMER * timers  [10];
  coord_t cell [MAX_ENEMIES]; 
  coord_t shot [MAX_SHOTS];
  coord_t navnod;
  int   max_shots;
  int   max_enemies;
} enemylogic_t;
  

#ifdef __cplusplus
}
#endif

#endif /* ALLEGRO_H */

