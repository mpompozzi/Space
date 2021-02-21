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
#include <SDL2/SDL.h>

#include "../../../headers/joydrv.h"
#include "../../../headers/termlib.h"
#include "../../../headers/disdrv.h"
#include "../../../headers/audio.h"

#include "../../../headers/backend.h"
#include "../../../headers/menu.h"


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
 *  un int (ciclos) con la cantidad de ciclos que se ha realizado 
 *  
 * Devuelve un int entre 9 y 1
 */

//////////// NAVES_POR_CICLO ///////////
int naves_por_ciclo (juego_t* componentes, int times);
//Indica cuantas veces por ciclo voy a llamar a la funcion que mueve a los enemigos

/*
 * Recibe un puntero a la estructura que tiene todos los componentes del juego,
 *  un int (times) con el valor que indica cada cuantos ciclos se mueven los enemigos 
 * 
 * Devuelve un int entre 1 y 4
 */
//////////// CONTROL_AUDIO_PLAYER ///////////

void control_audio_jugador (juego_t * componentes, int* nmadre, int audio);
//Llama a la funcion que reproduce los audios segun la colision encontrada
/*
 * Recibe un puntero a la estructura con los componentes del juego y un puntero int a nmadre
 *  que dice si hay o no una nave madre en juego y un int con el estado del audio(inicializado o no)
 */

//////////// CONTROL_AUDIO_ENEMY ///////////

void control_audio_enemy (juego_t * componentes, int audio);
//Llama a la funcion que reproduce los audios segun la colision encontrada

/*
 * Recibe un puntero a la estructura con los componentes del juego y un int con el estado del audio
 */

///////////////// SOUNDS ///////////////////

void sounds (int what);
//Reproduce el audio que se necesite
/*
 * Recibe un int que indica que audio se debe reproducir
 */

///////////////////////////////////////////////////////////////
//                      DEFINICIONES                         //
///////////////////////////////////////////////////////////////

//PARA EL JUEGO
#define THRESHOLD 40    //Límite a partir del cual se mueve el LED encendido
#define JUGAR 5         //Uso 5 y 12, porque son las posiciones en las que se encuentra el puntero
#define TERMINA 12
#define BLANK 0
#define MAX_ENEM 16     //Cantidad maxima de enemigos en juego
#define MENU 2          //Opcion menu y juego para saber que matriz voy a tener que llamar a imprimir
#define GAME 1
#define MAX_CICLOS 8

//PARA EL AUDIO
#define AUDIOREADY
#define PARLANTE

#define DISP_EN 1   //Los defines para la eleccion del audio
#define DISP_PL 2
#define ENEMIGO 3
#define NODRIZA 4
#define GAMEOVER 5
#define LVUP 6
#define MASVIDA 7
#define MENOSVIDA 8

#define SND_DISP_EN "res/audio/disparo.wav"
#define SND_DISP_PL "res/audio/invaderkilled.wav"
#define SND_ENEMIGO "res/audio/naveImpacto.wav"
#define SND_NODRIZA "res/audio/ufo_lowpitch.wav"
#define SND_GAMEOVER "res/audio/gameOver.wav"
#define SND_LVUP "res/audio/lvlup.wav"
#define SND_MASVIDA "res/audio/masvidas.wav"
#define SND_MENOSVIDA "res/audio/menosvida.wav"

#ifdef PARLANTE
#define VOL 60
#elif AURICULARES
#define VOL 40
#endif


/////////////////////////////////////////////////////////////
//                       CODIGO                            //
/////////////////////////////////////////////////////////////

int main (void)
{

  raspinit (); //Inicializo los componentes de la raspi
  int audio = 0;

#ifdef AUDIOREADY 
  // SDL_Init(SDL_INIT_AUDIO);

  audio = initAudio ();
  //initAudio ();

  if (audio == 0) //Inicio el audio
    {
      printf ("Audio no inicializado\n");
      endAudio ();
    }
#endif
  juego_t componentes;
  inigame (&componentes, 1); //Mando el nivel 1, ver si hay que elegir nivel
  getcoordp (&componentes); //Ubica la posicion del jugador  

  //Creo e inicializo las variables a utilizar
  int i, conta = 0, ciclos = 0;
  int naves, start;
  int opcion = JUGAR, preop = TERMINA;
  int estado = MENU; //empiezo en menu
  int quit_game = 0, final = 0;
  int nmadre = 0;
  int random;
  int get_disp;
  jswitch_t pausa = J_NOPRESS;
 
  componentes.naves = 12;
  jcoord_t coord = {0, 0}; //coordenadas medidas del joystick

  //Genero los dos numeros random, para nave madre y disparos enemigos    
  random = (rand () % 10) + 26; //numero entre 25 y 35
  get_disp = (rand () % 4) + 2; //numero entre 1 y 5

  //AUDIO
#ifdef AUDIOREADY

  Audio * soundDISP_EN = createAudio (SND_DISP_EN, 0, VOL);
  Audio * soundDISP_PL = createAudio (SND_DISP_PL, 0, VOL);
  Audio * soundENEMIGO = createAudio (SND_ENEMIGO, 0, VOL);
  Audio * soundNODRIZA = createAudio (SND_NODRIZA, 0, VOL);
  Audio * soundGO = createAudio (SND_GAMEOVER, 0, VOL);
  Audio * soundLVUP = createAudio (SND_LVUP, 0, VOL);
  Audio * soundMAS = createAudio (SND_MASVIDA, 0, VOL);
  Audio * soundMENOS = createAudio (SND_MENOSVIDA, 0, VOL);

  if (soundDISP_EN == NULL)
    {
      printf ("Falla disp en\n");
      final = 1;
      //return -1;
    }
  if (soundDISP_PL == NULL)
    {
      printf ("Falla disp pl\n");
      final = 1;
      //return -1;
    }
  if (soundENEMIGO == NULL)
    {
      printf ("Falla enemigo\n");
      final = 1;
      //return -1;
    }
  if (soundNODRIZA == NULL)
    {
      printf ("Falla nodriza\n");
      final = 1;
      //return -1;
    }
  if (soundGO == NULL)
    {
      printf ("Falla game over\n");
      final = 1;
      //return -1;
    }
  if (soundLVUP == NULL)
    {
      printf ("Falla lvl up\n");
      final = 1;
      // return -1;
    }
  if (soundMAS == NULL)
    {
      printf ("Falla mas vidas\n");
      final = 1;
      //  return -1;
    }
  if (soundMENOS == NULL)
    {
      printf ("Falla menos vida\n");
      final = 1;
      // return -1;
    }

#endif
  if (ciclos == 0)
    {
      for (start = 0; start < 5; start++)
        {
          spaceinvaders ();
          rasprint (MENU);
          sleep (1);
          clrmat ();

        }
      ciclos++;
    }

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
                preop = TERMINA; // para borrar el puntero de la opcion anterior
                menu (opcion); // lo mando al menu
                del_pointer (preop);
                rasprint (MENU);
                quit_game = 0; // y marco que se decidio seguir jugando 
              }
            else if (coord.y < -THRESHOLD)
              { //Caso contrario, hago lo mismo pero 
                quit_game = 1; // marco la opcion de salir
                opcion = TERMINA;
                preop = JUGAR;

                menu (opcion);
                del_pointer (preop);
                rasprint (MENU);
              }

            usleep (100);

            menu (BLANK);
            rasprint (MENU);
            usleep (100);

            pausa = joy_get_switch ();
            pausa = joy_get_switch ();
            joy_update ();
            coord = joy_get_coord (); //Guarda las coordenadas medidas

            if (pausa == J_PRESS)
              {
                estado = GAME;
                clrmat ();
                final = quit_game;
              }
            //pausa= J_NOPRESS;
            usleep (400);

          }
          break;
        case GAME:
          {
            rasprint (GAME);
            estado = GAME;
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
                get_disp += (rand () % 4) + 1; //sumo un numero entre 1 y 5
                if (audio == READY)
                  {
                    sounds (DISP_EN);
                  }
              }

            //MOVIMIENTO NAVES ENEMIGAS

            naves = llamo_naves (&componentes, ciclos); //Cada cuantos ciclos muevo a los enemigos
            conta = naves_por_ciclo (&componentes, naves); //Cuantas veces muevo a los enemigos por ciclo

            if ((ciclos % naves) == 0) //Utilizo naves 
              {
                for (i = 0; (i < conta) && (quit_game == 0); i++)
                  { //Mientras que no se haya perdido el juego, llama a la funcion 
                    quit_game = ciclonaves (&componentes); // la cantidad de veces que le indique conta
                    rasprint (GAME);
                    usleep (500);

                  }
              }

            //MOVIMIENTO NAVE MADRE
            if (nmadre == 1)
              {
                if (audio == READY)
                  {
                    sounds (NODRIZA);
                  }
                if (ciclos % 2 == 0)
                  { //La nave madre se mueve cada dos movimientos del jugador
                    nmadre = nav_nod ();
                  }
              }
            //MOVIMIENTO DE LOS DISPAROS
            //Los muevo tres veces en total para que tengan mas velocidad que el jugador

            control_audio_enemy (&componentes, audio);
            control_audio_jugador (&componentes, &nmadre, audio);
            verparams (&componentes);
            rasprint (GAME);
            usleep (200);

            control_audio_enemy (&componentes, audio);
            control_audio_jugador (&componentes, &nmadre, audio);
            verparams (&componentes);
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
                        if (audio == READY)
                          {
                            sounds (DISP_PL); //Reproduce el sonido del disparo
                          }
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
                        if (audio == READY)
                          {
                            sounds (DISP_PL);
                          }
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
            control_audio_enemy (&componentes, audio);
            control_audio_jugador (&componentes, &nmadre, audio);
            verparams (&componentes);
            rasprint (GAME);
            usleep (200);
            
            ciclos++; //Paso al ciclo siguiente

            rasprint (GAME); //Imprimo el juego

            if (componentes.naves == 0) //Si mate a todos los enemigos
              { //Si ya no hay naves
                if (audio == READY)
                  {
                    sounds (LVUP);
                    sounds (MASVIDA);
                  }
                componentes.nivel++; //Aumento de nivel   
                componentes.vidas++; //Agrega una vida
                ciclos = 1; //Reinicio los ciclos
                random = (rand () % 10) + 26; //numero entre 25 y 35
                get_disp = (rand () % 4) + 2; //numero entre 1 y 5
                componentes.naves = 12;
                inimat ();

                clrmat ();
                matniv (); //Mando a imprimir LV UP
                rasprint (MENU);

                sleep (1);
              }

            if (componentes.puntaje >= 1000)
              {//si tengo mas de 1000 puntos gano 1 vida y vuelvo el puntaje a 0
                componentes.vidas++;
                componentes.puntaje = 0;

                if (audio == READY)
                  {
                    sounds (MASVIDA);
                  }
              }

            if (componentes.vidas == 0 || quit_game == 1) //En caso de quedarme sin vidas o si las naves llegaron al jugador
              {
                if (audio == READY)
                  {
                    sounds (GAMEOVER);
                  }
                clrmat ();
                printscore (componentes.puntaje); //Carga el puntaje la matriz del back
                rasprint (MENU); //Imprime los puntajes

                sleep (2); //Espero dos segundos para que se vea el puntaje

                ciclos = 1; //Reinicio los ciclos
                inigame (&componentes, 1); //inicializa en nivel 1
                //Reinicio los numeros random
                random = (rand () % 10) + 26; //numero entre 25 y 35
                get_disp = (rand () % 4) + 2; //numero entre 1 y 5           
                componentes.naves = 12;
                quit_game = 0;

                clrmat ();
                estado = MENU; //Pongo el menu

                usleep (200); //Utilizo una pausa de 200 micro segundos para que los cambios no sean abruptos
              }
            usleep (300); //Utilizo una pausa de 300 micro segundos para mejorar la jugabilidad

          }
          break;


        }
    }
  clrmat (); //Vacio la matriz del menu 
  rasprint (MENU); //Y la imprimo para mostrar que se salio del juego

#ifdef AUDIOREADY
  //End Simple-SDL2-Audio 
  endAudio ();

  //Libero todos los audios 
  freeAudio (soundDISP_EN);
  freeAudio (soundDISP_PL);
  freeAudio (soundENEMIGO);
  freeAudio (soundNODRIZA);
  freeAudio (soundGO);
  freeAudio (soundLVUP);
  freeAudio (soundMAS);
  freeAudio (soundMENOS);
#endif


  return 0;
}

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
  if (ciclos <= 20)
    {
      conta = (8 - p);
    }
  else if (ciclos <= 40)
    {
      conta = (7 - p);
    }
  else if (ciclos <= 55)
    {
      conta = (6 - p);
    }
  else if (ciclos <= 70)
    {
      conta = (5 - p);
    }
  else if (ciclos <= 80)
    {
      conta = (4 - p);
    }
  else if (ciclos <= 90)
    {
      conta = (3 - p);
    }
  else
    conta = 1;

  (conta >= 1) ? devuelvo = conta : (devuelvo = 1);

  return devuelvo;

}
//////////////////////////////////////////////////

int naves_por_ciclo (juego_t* componentes, int times)
{
  int conta = 1;
  int cantidad = MAX_ENEM - componentes->naves; //Cantidad de enemigos eliminados
  int p;
  (componentes->nivel >= 4) ? p = 4 : (p = componentes->nivel);

  if (times > 1)
    { //Si todavia times tiene un valor mayor a uno, 
      conta = 1; // le va a devolver el contador en 1
    }
  else if (cantidad > 10)
    {
      conta = 2; 
    }

  return conta;
}

////////////////////////////////////////////////

//////////////// CONTROL_AUDIO /////////////////

void control_audio_jugador (juego_t * componentes, int* nmadre, int audio)
{// Mueve los disparos y si encuentra una colision reproduce el audio correspondiente
  int i, j;
  coord_t evento;
  for (i = 0; i < LARGO; i++) //Barre toda la matriz del back
    {
      for (j = 0; j < ANCHO; j++)
        {
          evento = ciclodispp (componentes, i, j); //Busca si en esa posicion esta el disparo del jugador y comprueba si choco
          if (evento.objeto == NAVE_ENEMIGA) //Con una nave enemiga
            {
              if (audio == READY)
                {
                  sounds (ENEMIGO); //Reproduce el audio
                }
            }
          else if (evento.objeto == NAVE_NODRIZA) //Con la nave madre
            {
              if (audio == READY)
                {
                  sounds (ENEMIGO); //Reproduce el audio
                }
              *nmadre = 0; //Marca que ya no hay nave madre en juego
            }
        }
    }
}
/////////////////////////////////////////////////
/////////////// CONTROL_AUDIO_ENEMY /////////////

void control_audio_enemy (juego_t * componentes, int audio)
{//Funciona igual que el del jugador, pero lee la matriz en forma contraria para evitar que se eliminen los disparos,
  // por lo tanto, reproduce sonido solo si choca con el jugador
  int i, j;
  coord_t evento;
  for (i = LARGO - 1; i >= 0; i--)
    {
      for (j = ANCHO - 1; j >= 0; j--)
        {
          evento = ciclodispe (componentes, i, j);
          if (evento.objeto == JUGADOR)
            {
              if (audio == READY)
                {
                  sounds (MENOSVIDA);
                }
            }
        }
    }
}

////////////////////////////////////////////////

void sounds (int what)
{// Reproduce los audios segun lo que se indica
  switch (what)
    {
    case DISP_EN:
      playSound (SND_DISP_EN, VOL);
      break;
    case DISP_PL:
      playSound (SND_DISP_PL, VOL);
      break;
    case ENEMIGO:
      playSound (SND_ENEMIGO, VOL);
      break;
    case NODRIZA:
      playSound (SND_NODRIZA, VOL);
      break;
    case GAMEOVER:
      playSound (SND_GAMEOVER, VOL);
      break;
    case LVUP:
      playSound (SND_LVUP, VOL);
      break;
    case MASVIDA:
      playSound (SND_MASVIDA, VOL);
      break;
    case MENOSVIDA:
      playSound (SND_MENOSVIDA, VOL);
      break;
    }
}
