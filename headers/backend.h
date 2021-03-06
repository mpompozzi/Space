/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   backend.h
 * Author: Alejandro
 *
 * Created on 8 de diciembre de 2020, 16:01
 */

#ifndef BACKEND_H
#define BACKEND_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef RASPI  
#define LARGO 16 //el largo de la matriz
#define ANCHO 16
#define SEPARACION 4 // Separacion entre enemigos por fila en raspi
#elif ALLEGRO 
#define LARGO 24//el largo de la matriz
#define ANCHO 32
#define SEPARACION 4
  
  // Separacion entre enemegios por fila para allegro
//#define CELL    0
#endif

#define ENEMY1 4 //despues agrego mas tipos de naves
#define ENEMY2 5 
#define ENEMY3 6
#define ENEMYSHOT1 1//tipos de naves entre 1 a 6 es el q dispara
#define ENEMYSHOT2 2
#define ENEMYSHOT3 3
#define NAVNOD 7//siempre se mueve de izq a der, ocupa 2 bloques
#define PSHOT 8   
#define ESHOT 9
#define PLAYER 10
#define MURO 11
#define DER 1
#define IZQ 2
#define UP 3
#define ABAJO 4
#include<stdio.h>
#include<time.h>
#include <stdlib.h>
#include <stdint.h>
#include<unistd.h>
    /////////////////////////////////
    enum objetos {
    WARNING =-1,
  NADA = 0,
  NAVE_ENEMIGA,
  NAVE_NODRIZA,
  ESCUDO,
  JUGADOR,
};

typedef struct {
  int i;
  int j;
  int objeto; // Valor tomado de enum objetos
} coord_t;
typedef struct{
    int vidas;
    int puntaje;
    int nivel;
    coord_t coordsp;
    int *tablero;
    int naves; //cantidad de naves
    int mov; //tipo de movimiento
    int omit;
}juego_t;
///////////////////////////////////////////
void getcoordp(juego_t*);
void inimat (void);//inicializgea la matriz en 0
void printmat (void);//imprime la matriz (testeo)
void ininiv(int);//se fija en que nivel estas y carga cierto patron de naves, pone los escudos y la nave del player
int ciclonaves (juego_t *);//mueve las naves enemigas
coord_t ciclodispp(juego_t*,int,int);//mueve disparos del player y el enemigo
coord_t ciclodispe(juego_t*,int,int);//mueve disparos del player y el enemigo
void verparams(juego_t*);//se fija si pasaste de nivel, cambia las naves para que puedas disparar
void navdisp(void );//decide si dispara las naves, y spawnea nave nodriza
void pmov(juego_t*); //recibe el input del jugador y mueve o dispara, o se mueve en el menu /pone pausa al juego
int nav_nod (void);//spawnea y mueve nave nod
int getmat(int,int);//te dice que hay en esas coords de la matriz
void inigame(juego_t*,int);

#ifdef __cplusplus
}
#endif

#endif /* BACKEND_H */

