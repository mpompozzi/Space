/* 
 * File:   Raspi.c
 * Author: magali
 *
 * Created on January 11, 2021, 7:55 PM
 */
//////////////////////////////////////////////////////////////
//                      HEADERS                             //
//////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>

#include "../../../headers/joydrv.h"
#include "../../../headers/termlib.h"
#include "../../../headers/disdrv.h"
//#include "../../../headers/libaudio.h"

#include "../../../headers/backend.h"
#include "../../../headers/menu.h"
//#include "../../../headers/audioRaspi.h"

////////////////////////////////////////////////////////////////
//                       FUNCIONES                            //
////////////////////////////////////////////////////////////////

///////////////// RASPINIT ///////////////
void raspinit (void); //Inicializa el joystick y el display


///////////////// RASPRINT ////////////////
void rasprint (int matriz); //Imprime la matriz elegida

/*
 * Recibe un int que le indica que matriz debe imprimir, la del menu o la del juego
 */


//////////////// PAUSE_MENU ////////////////
int pause_menu (void); //Menu de pausa

/*
 * Devuelve un int que indica si se eligio terminar el juego
 */


/////////////// LLAMO_NAVES ////////////////
int llamo_naves (juego_t* componentes, int ciclos);
//Indica cada cuantos ciclos y cuantas veces por ciclo se llama a la funcion que mueve a los enemigos

/*
 * Recibe un puntero a la estructura que tiene todos los componentes del juego,
 *  un int (ciclos) con la cantidad de ciclos que se ha realizado y otro int (naves) 
 *  que dice cada cuantos ciclos se llama a la funcion que mueve las naves
 * Devuelve un int
 */

int naves_por_ciclo (juego_t* componentes, int times);

////////////// CONTROL_AUDIO ////////////////
void control_audio (juego_t * componentes, int* nmadre);
//Comprueba si hubo una colision y reproduce el audio correspondiente

/*
 * Recibe un puntero a la estructura con los componentes del juego y un puntero int a nmadre
 *  que dice si hay o no una nave madre en juego
 * Devuelve un int para indicar si hubo colision o no
 */


///////////////////////////////////////////////////////////////
//                      DEFINICIONES                         //
///////////////////////////////////////////////////////////////
#define THRESHOLD 40    //Límite a partir del cual se mueve el LED encendido
#define JUGAR 5         //Uso 5 y 12, porque son las posiciones en las que se encuentra el puntero
#define TERMINA 12
#define MAX_ENEM 16     //Cantidad maxima de enemigos en juego
#define MENU 2          //Opcion menu y juego para saber que matriz voy a tener que llamar a imprimir
#define GAME 1
#define MAX_CICLOS 6


/////////////////////////////////////////////////////////////
//                       CODIGO                            //
/////////////////////////////////////////////////////////////

int main (void)
{

  raspinit (); //Inicializo los componentes de la raspi

  juego_t componentes;
  inigame (&componentes, 1); //Mando el nivel 1, ver si hay que elegir nivel
  getcoordp (&componentes); //Ubica la posicion del jugador  

  //Creo e inicializo las variables a utilizar
  int i, conta = 0, ciclos = 1;
  int quit_game = 0;
  int nmadre = 0;
  int random;
  int naves;
  int get_disp = 0;
  srand (time (NULL));


  componentes.naves = 16;
  jcoord_t coord = {0, 0}; //coordenadas medidas del joystick

  //Genero los dos numeros random, para nave madre y disparos enemigos    
  random = (rand () % 10) + 21; //numero entre 20 y 30
  get_disp = (rand () % 6); //numero entre 0 y 5

  while (quit_game != 1)
    { //Mientras no sse decida salir del juego

      if (ciclos == 1)//Si estoy en el primer ciclo
        {
          ininiv (componentes.nivel); //Inicializo el nivel
          //ACA QUIERO HACER QUE PARPADEE UN PAR DE VECES PARA QUE QUEDE LINDO NOMAS
          rasprint(GAME);
          usleep(200);
          clrmat();
          rasprint(MENU);
          usleep(200);
          rasprint(GAME);
        }

      getcoordp(&componentes);
      if (random == ciclos)
        { //Una vez que la cantidad de ciclos 
          nmadre = 1; //Marco que tengo que crear una nave madre
          random += (rand () % 10) + 21; //Sumo un numero entre 20 y 30 al anterior
        }


      if (ciclos == get_disp)
        { //Funciona igual a nave madre pero llama a la funcion que los inicia 
          navdisp ();
          get_disp += (rand () % 6);
        }

      naves = llamo_naves (&componentes, ciclos); //Cada cuantos ciclos muevo a los enemigos
      conta = naves_por_ciclo (&componentes, naves); //Cuantas veces muevo a los enemigos por ciclo



      if ((ciclos % naves) == 0)
        { //Utilizo naves 
          for (i = 0; (i <= conta) && (quit_game == 0); i++)
            { //Mientras que no se haya perdido el juego, llama a la funcion 
              quit_game = ciclonaves (); // la cantidad de veces que le indique conta
              rasprint(GAME);
              usleep(300);
            }
        }
      if (ciclos % 2 == 0 && nmadre == 1)
        { //La nave madre se mueve cada dos movimientos del jugador
          nmadre = nav_nod ();
        }


      control_audio (&componentes, &nmadre);
      verparams (&componentes);
      usleep(200);
      control_audio (&componentes, &nmadre);
      verparams (&componentes);
      usleep(200);
      

      rasprint (GAME);

      coord = joy_get_coord (); //Guarda las coordenadas medidas
      if (joy_get_switch () != J_NOPRESS)
        {
          while (joy_get_switch () == J_NOPRESS)
            {
              quit_game = pause_menu ();
              rasprint (MENU);

            }
        }
      else
        {
          //MOVIMIENTOS DEL JUGADOR
          if (coord.x > THRESHOLD)
            { //Dereceha 
              if (coord.y > THRESHOLD)
                { // y arriba
                  componentes.mov = 4; // me muevo a la derecha y disparo
                  pmov (&componentes); //Llamo a la funcion que realiza el movimiento
                }
              else
                {
                  componentes.mov = 1; //No se marco arriba, no dispara
                  pmov (&componentes);
                }
            }
          else if (coord.x < -THRESHOLD)
            { //Izquierda
              if (coord.y > THRESHOLD)
                { // y arriba, dispara y se mueve
                  componentes.mov = 5;
                  pmov (&componentes);
                }
              else
                {
                  componentes.mov = 2; //No dispara
                  pmov (&componentes);
                }
            }
          else if (coord.y > THRESHOLD)
            { //Solo arriba, solo dispara
              componentes.mov = 3;
              pmov (&componentes);
            }
        }
      ciclos++;
      
      rasprint (GAME); //Imprimo el juego
      sleep(1);

      if (componentes.naves == 0)
        { //Si ya no hay naves
          componentes.nivel++; //Aumento de nivel    
          ciclos = 0; //Reinicio los ciclos
          random = (rand () % 10) + 21; //numero entre 20 y 30
          get_disp = (rand () % 6); //numero entre 0 y 5
          componentes.naves = 16;
          
          clearmat(MENU);
          matniv (); //Mando a imprimir LV UP
          rasprint (MENU);
          
          usleep (100);
        }
      
      if (componentes.puntaje >= 1000)
        {//si tengo mas de 1000 puntos gano 1 vida y vuelvo el puntaje a 0
          componentes.vidas++;
          componentes.puntaje = 0;
        }
      
      if (componentes.vidas == 0 || quit_game == 1)
        {
          printscore (componentes.puntaje);
          rasprint (MENU);
          usleep(500);
          
          ciclos = 0;
          inigame (&componentes, 1); //inicializa en nivel 1
          random = (rand () % 10) + 21; //numero entre 20 y 30
          get_disp = (rand () % 6); //numero entre 0 y 5            
          componentes.naves = 16;
          quit_game= pause_menu();
          rasprint(MENU);
          sleep(2);
          clearmat(MENU);
          menu(JUGAR);
          while (joy_get_switch () == J_NOPRESS)
            {
              quit_game = pause_menu ();
              rasprint (MENU);
              usleep (200);

            }

        }
      
      
    }
  /*
  End Simple-SDL2-Audio 
  endAudio();

  Important to free audio after ending Simple-SDL2-Audio because they might be referenced still 
  freeAudio(sound);
  freeAudio(music);

   */
  
  return 0;
}
/////////////////////////
/*
 

 * 
 *    dcoord_t pos = {DISP_MAX_X >> 1, DISP_MAX_Y >> 1}; //pos es la posición actual, empieza en el centro de la matriz
 *    dcoord_t npos = pos; //npos es la próxima posición
 * 
 */

////////////////////////////////////////////////////////////////
//                       FUNCIONES                            //
////////////////////////////////////////////////////////////////

//////////////////// RASPINIT /////////////////

void
raspinit (void)
{
  joy_init (); //Inicializa el joystick
  disp_init (); //Inicializa el display
  disp_clear (); //Limpia todo el display
}
////////////////////////////////////////////////
void clearmat(int matriz) //ver si agrego la de back
{
  int obj;
  if (matriz == MENU)
    {
      clrmat();
    }
   
}

////////////////// RASPRINT ////////////////////

void rasprint (int matriz)
{ //es el printmat para raspi
  static int disparo = 0;
  int i, j, obj;
  for (i = 0; i < ANCHO; ++i)
    { //Copia el contenido de la matriz en el buffer de leds
      for (j = 0; j < ANCHO; ++j)
        {
          if (matriz == MENU)
            { //Opcion para imprimir el menu en la raspi
              obj = get_menu (i, j);
            }
          else
            {
              obj = getmat (i, j); //Opcion para imprimir el juego
            }

          //Para diferenciar los disparos, los hago parpadear
          if ((obj == PSHOT) || (obj == ESHOT))
            {//si no es 0 prende el led
              if (disparo == 0)
                {
                  dcoord_t myPoint = {j, i};
                  disp_write (myPoint, D_ON);
                  disparo = 1;
                }
              else
                {
                  dcoord_t myPoint = {j, i};
                  disp_write (myPoint, D_OFF);
                  disparo = 0;
                }

            }
          else if ((obj > 0) && (obj != PSHOT) && (obj != ESHOT))
            {
              dcoord_t myPoint = {j, i};
              disp_write (myPoint, D_ON);
            }
          else
            {//lo apaga
              dcoord_t myPoint = {j, i};
              disp_write (myPoint, D_OFF);
            }
        }
    }
  disp_update ();
}
////////////////////////////////////////////////

///////////////// PAUSE_MENU ///////////////////

int pause_menu (void)
{
  jcoord_t newcoord = {0, 0};   //Coordenadas medidas del joystick
  newcoord = joy_get_coord ();     //Guarda las coordenadas medidas
  int quit_game = 0;
  int opcion=JUGAR;         //Por default lo marco como jugar
  if (newcoord.y > THRESHOLD)
    {                       //Si va hacia arriba 
      opcion = JUGAR;       // selecciona la opcion JUGAR
      menu (opcion);        // lo mando al menu
      quit_game = 0;        // y marco que se decidio seguir jugando 
    }
  else if (newcoord.y < -THRESHOLD)
    {                       //Caso contrario, hago lo mismo pero 
      quit_game = 1;        // marco la opcion de salir
      opcion = TERMINA;
      menu (opcion);
    }
  return quit_game;
}
////////////////////////////////////////////////

///////////////// LLAMO_NAVES //////////////////

int llamo_naves (juego_t* componentes, int ciclos)
{   //Cada cuantos ciclos llamo a la funcion que mueve a las naves enemigas
  static int p = 0; //Nivel del 1 al 4
  int conta = 1;
  int cantidad = MAX_ENEM - componentes->naves; //Cantidad de enemigos eliminados

  if(conta==1)
    {
      (componentes->nivel >= 4) ? p = 4 : (p = componentes->naves);
    }

  conta = MAX_CICLOS - p; //Conta va a ser un numero del 2 al 5, segun la velocidad que corresponda por nivel
  if ((conta > 1) && (p <= 4))
    {
      if ((cantidad % 4) || (ciclos % 5 == 0))
        { //Si la cantidad de enemigos que se elimino es multiplo de 4 o se cumplieron
          conta--; // 5, o un multiplo de este, ciclos, se resta en 1 conta
          p++; // y se aumenta en 1 el nivel
        }
    }
  else
    { //Caso contrario, se considera que conta es 1
      conta = 1;
    }
  return conta;
}
//////////////////////////////////////////////////

int naves_por_ciclo (juego_t* componentes, int times)
{
  int conta = 0;
  int cantidad = MAX_ENEM - componentes->naves; //Cantidad de enemigos eliminados

  if (times > 1)
    { //Si todavia times tiene un valor mayor a uno, 
      conta = 1; // le va a devolver el contador en 1
    }
  else
    {
      conta = ((cantidad / 4) + 1); //Caso contrario, devuelve un numero del 1 al 4
    }
  return conta;
}

////////////////////////////////////////////////

//////////////// CONTROL_AUDIO /////////////////

void control_audio (juego_t * componentes, int* nmadre)
{
  int i, j;
  coord_t evento;
  for (i = 0; i < LARGO; i++)
    {
      for (j = 0; j < ANCHO; j++)
        {
          evento = ciclodisp (componentes, i, j);
          if (evento.objeto == NAVE_ENEMIGA)
            {
              //audio 1

            }
          else if (evento.objeto == NAVE_NODRIZA)
            {
              //audio 2
              *nmadre = 0;
            }
          else if (evento.objeto == JUGADOR)
            {
              //audio 3
            }
          else if (evento.objeto == ESCUDO)
            {

            }
        }
    }
}