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

//////////// CONTROL_AUDIO_PLAYER ///////////

void control_audio_jugador (juego_t * componentes, int* nmadre);

/*
 * 
 */

//////////// CONTROL_AUDIO_ENEMY ///////////


void control_audio_enemy (juego_t * componentes, int* nmadre);
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
#define BLANK 0
#define MAX_ENEM 16     //Cantidad maxima de enemigos en juego
#define MENU 2          //Opcion menu y juego para saber que matriz voy a tener que llamar a imprimir
#define GAME 1
#define MAX_CICLOS 8


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
  int opcion = JUGAR, preop= TERMINA;
  int estado = GAME; //empiezo en menu
  int quit_game = 0, final=0;
  int nmadre = 0;
  int random;
  int naves;
  int get_disp;
  jswitch_t pausa = J_NOPRESS;

  
//  int prueba=0;
//  int veces=0;
 
  componentes.naves = 12;
  jcoord_t coord = {0, 0}; //coordenadas medidas del joystick

  //Genero los dos numeros random, para nave madre y disparos enemigos    
  random = (rand () % 10) + 26; //numero entre 25 y 35
  get_disp = (rand () % 4) + 2; //numero entre 1 y 5


  
  while (final != 1)
    { //Mientras no sse decida salir del juego

      switch (estado)
        {
        case MENU:
          {
            clrmat ();
            menu (opcion);
            rasprint (MENU);
            usleep (100);

            pausa = joy_get_switch ();
            joy_update ();
            coord = joy_get_coord (); //Guarda las coordenadas medidas

            if (coord.y > THRESHOLD)
              { //Si va hacia arriba 
                opcion = JUGAR; // selecciona la opcion JUGAR
                preop= TERMINA; // para borrar el puntero de la opcion anterior
                menu (opcion);  // lo mando al menu
                del_pointer(preop);
                rasprint (MENU);
                quit_game = 0; // y marco que se decidio seguir jugando 
              }
            else if (coord.y < -THRESHOLD)
              { //Caso contrario, hago lo mismo pero 
                quit_game = 1; // marco la opcion de salir
                opcion = TERMINA;
                preop= JUGAR;
                
                menu (opcion);
                del_pointer(preop); 
                rasprint (MENU);
              }

            usleep(100);

            menu(BLANK);
            rasprint(MENU);
            usleep (100);
            
            pausa = joy_get_switch ();
            pausa = joy_get_switch ();
            joy_update ();
            coord = joy_get_coord (); //Guarda las coordenadas medidas

            if (pausa == J_PRESS)
              {
                estado = GAME;
                clrmat();
                final=quit_game;
              }
            //pausa= J_NOPRESS;
            usleep(400);
            
          }
          break;
        case GAME:
          {
            //pausa = J_NOPRESS;
            rasprint (GAME);
            estado=GAME;
            //printmat();
            pausa = joy_get_switch ();
            joy_update ();
            coord = joy_get_coord (); //Guarda las coordenadas medidas
            
           //INICIO DE NIVELES
            
            if (ciclos == 1)//Si estoy en el primer ciclo
              {
                inigame (&componentes, componentes.nivel); //Inicializo el nivel
                //ACA QUIERO HACER QUE PARPADEE UN PAR DE VECES PARA QUE QUEDE LINDO NOMAS
                rasprint (GAME);
                usleep (500);
                clrmat ();
                rasprint (MENU);
                usleep (500);
                rasprint (GAME);
              }

            getcoordp (&componentes);

            //CONSIGO LOS RANDOMS DE DISPAROS Y NAVE MADRE
            
            if (random == ciclos)
              { //Una vez que la cantidad de ciclos 
                nmadre = 1; //Marco que tengo que crear una nave madre
                random += (rand () % 10) + 26; //Sumo un numero entre 25 y 35 al anterior
              }

            if (ciclos == get_disp)
              { //Funciona igual a nave madre pero llama a la funcion que los inicia 
                navdisp ();
                get_disp += (rand () %4 )+1; //sumo un numero entre 1 y 5
                //prueba++;
                //printf("prueba= %d\n",prueba);
              }

            //MOVIMIENTO NAVES ENEMIGAS
            
            naves = llamo_naves (&componentes, ciclos); //Cada cuantos ciclos muevo a los enemigos
            conta = naves_por_ciclo (&componentes, naves); //Cuantas veces muevo a los enemigos por ciclo

//            printf("Naves %d\n",naves);
//            printf("Conta %d\n",conta);
//            printf("Aca es %d\n",(ciclos%naves));
            
            if ((ciclos % naves) == 0) //Utilizo naves 
              { 
                for (i = 0; (i < conta) && (quit_game == 0); i++)
                  { //Mientras que no se haya perdido el juego, llama a la funcion 
//                    prueba++;
//                    printf("Prueba %d\n",prueba);
//                    veces++;
                    quit_game = ciclonaves (&componentes); // la cantidad de veces que le indique conta
                    rasprint (GAME);
                    usleep (500);
                    
                  }
              }
            
//            prueba=0;
            
            //MOVIMIENTO NAVE MADRE
            
            if (ciclos % 2 == 0 && nmadre == 1)
              { //La nave madre se mueve cada dos movimientos del jugador
                nmadre = nav_nod ();
              }

            //MOVIMIENTO DE LOS DISPAROS
            //Los muevo tres veces en total para que tengan mas velocidad que el jugador

            //printmat();
            control_audio_enemy (&componentes, &nmadre);
            control_audio_jugador (&componentes, &nmadre);
            verparams (&componentes);
         //   printf("puntaje %d\n",componentes.puntaje);
            rasprint (GAME);
            usleep (200);
            
            control_audio_enemy(&componentes, &nmadre);
            control_audio_jugador (&componentes, &nmadre);
            verparams (&componentes);
          //  printf("puntaje %d\n",componentes.puntaje);
            rasprint (GAME);
            usleep (200);

            rasprint (GAME);

            //LLAMO DOS VECES LA SECUENCIA PARA OBTENER DATOS DEL JOYSTICK
            pausa = joy_get_switch ();
            joy_update ();
            coord = joy_get_coord (); //Guarda las coordenadas medidas

            pausa = joy_get_switch ();
            joy_update ();
            coord = joy_get_coord (); //Guarda las coordenadas medidas
            
            if (pausa == J_PRESS)
              {
                estado = MENU;

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
            
            //TERCER MOV DE DISPAROS
            control_audio_enemy (&componentes, &nmadre);
            control_audio_jugador (&componentes, &nmadre);
            verparams (&componentes);
           // printf("puntaje %d\n",componentes.puntaje);
            rasprint (GAME);
            usleep (200);


            ciclos++;


            rasprint (GAME); //Imprimo el juego
            
            
            if (componentes.naves == 0 )
              { //Si ya no hay naves
                componentes.nivel++; //Aumento de nivel    
                ciclos = 1; //Reinicio los ciclos
                random = (rand () % 10) + 26; //numero entre 25 y 35
                get_disp = (rand () % 4) + 2; //numero entre 1 y 5
                componentes.naves = 12;
                inimat();


                clrmat ();
                matniv (); //Mando a imprimir LV UP
                rasprint (MENU);

                sleep (2);
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
                rasprint(MENU);

               //printf ("puntaje= %d\n", componentes.puntaje);
               // printf ("veces= %d\n", veces);
                sleep(2);

                ciclos = 1;
                inigame (&componentes, 1); //inicializa en nivel 1
                //ininiv(1);
                random = (rand () % 10) + 26; //numero entre 25 y 35
                get_disp = (rand () % 4) + 2; //numero entre 1 y 5           
                componentes.naves = 12;
                quit_game=0;

                clrmat ();
                estado = MENU;

                usleep (200);
              }
            
            usleep (300);
            opcion=JUGAR;
            
          }
          break;


        }
    }
  clrmat();
  rasprint(MENU);

  return 0;
}
/*
  End Simple-SDL2-Audio 
  endAudio();

  Important to free audio after ending Simple-SDL2-Audio because they might be referenced still 
  freeAudio(sound);
  freeAudio(music);

 */

////////////////////////////////////////////////////////////////
//                       FUNCIONES                            //
////////////////////////////////////////////////////////////////

//////////////////// RASPINIT /////////////////

void raspinit (void)
{
  joy_init (); //Inicializa el joystick
  disp_init (); //Inicializa el display
  disp_clear (); //Limpia todo el display
}
////////////////////////////////////////////////


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


///////////////// LLAMO_NAVES //////////////////

int llamo_naves (juego_t* componentes, int ciclos)
{ //Cada cuantos ciclos llamo a la funcion que mueve a las naves enemigas
  static int p = 0; //Nivel del 1 al 4
  int conta;
  int devuelvo;
  
  if (ciclos == 1)
    {
      (componentes->nivel >= 4) ? p = 4 : (p = componentes->nivel);
    }
  //El contador depende del nivel y de la cantidad de ciclos
  if(ciclos <=20)
    {
      conta=(9-p);
    }
  else if(ciclos <= 40)
    {
      conta=(8-p);
    }
  else if (ciclos <= 55)
    {
      conta=(7-p);
    }
  else if (ciclos<= 70)
    {
      conta=(6-p);
    }
  else if(ciclos<= 80)
    {
      conta=(5-p);
    }
  else if(ciclos<= 90)
    {
      conta=(4-p);
    }
  else if(ciclos<= 95)
    {
      conta=(3-p);
    }
  else 
    conta=1;
  
  (conta>=1)?devuelvo=conta: (devuelvo= 1);
  
  return devuelvo;
  
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
      conta = ((cantidad / 5) + 1); //Caso contrario, devuelve un numero del 1 al 4
    }
//  printf("conta x ciclo %d\n",conta);
  return conta;
}

////////////////////////////////////////////////

//////////////// CONTROL_AUDIO /////////////////

void control_audio_jugador (juego_t * componentes, int* nmadre)
{
  int i, j;
  coord_t evento;
  for (i = 0; i < LARGO; i++)
    {
      for (j = 0; j < ANCHO; j++)
        {
          evento = ciclodispp (componentes, i, j);
          if (evento.objeto == NAVE_ENEMIGA)
            {
              //audio 1

            }
          else if (evento.objeto == NAVE_NODRIZA)
            {
              //audio 2
              *nmadre = 0;
            }
        }
    }
}

/////////////// CONTROL_AUDIO_ENEMY /////////////////

void control_audio_enemy (juego_t * componentes, int* nmadre)
{
  int i, j;
  coord_t evento;
  for (i = LARGO-1; i >= 0; i--)
    {
      for (j = ANCHO-1; j >= 0; j--)
        {
          evento = ciclodispe (componentes, i, j);
          if (evento.objeto == JUGADOR)
            {
              //audio -1 vida
            }
          else if (evento.objeto == ESCUDO)
            {

            }
        }
    }
}

////////////////////////////////////////////////