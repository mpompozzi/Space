/* 
 * File:   audioRaspi.c
 * Author: magali
 *
 * Created on February 7, 2021, 8:54 PM
 */

#include "../../../headers/audioRaspi.h"


//CAMBIAR LOS .WAV

#define DOOR1 "../sounds/door1.wav"
#define DOOR2 "../sounds/door2.wav"
#define HIGHLANDS "../music/highlands.wav"
#define ROAD "../music/road.wav"

void dummyFun(void);
void shortDummyFun(void);

#define DISP "../../res/audio/disparo.wav"
#define GAMEOVER "game over.wav"
#define IMPACT "nave impacto.wav"
#define NMADRE "ufo_highpitch.wav"

    
    //FUNCION PARA EL FRONT
    /* Init Simple-SDL2-Audio */
    

    /* Play music and a sound */
//    playMusic(HIGHLANDS, SDL_MIX_MAXVOLUME);    //ESTO ES LO QUE TENEMOS QUE CAMBIAR Y LA FUNCION QUE SE LLAMA DESDE EL FRONT PARA LA MUSICA DE FONDO
  

//  playSound(DISP, SDL_MIX_MAXVOLUME / 2);    //SONIDOS DE EXPLOSIONES Y DEMAS


//    /* While using delay for showcase, don't actually do this in your project */
//    printf("Delay showcase\n");
//    if ( playerStatus() == READY && musicStatus() == PLAYING)
//    {
//        printf("Music playing ok.\n");
//    }
//    
    //SDL_Delay(5000);
  //  shortDummyFun();
  //  printf("End delay\n");

    /* Override music, play another sound */
    //playMusic(ROAD, SDL_MIX_MAXVOLUME);
    //SDL_Delay(1000);

    /* Pause audio test */
//    printf("Pausing and waiting 1 second\n");
    
    //pauseAudio();
    
//    if ( musicStatus() == PAUSED )
//    {
//        printf("Music paused.\n");
//    }

//    SDL_Delay(1000);
//    printf("Continuing...\n");
    
   // unpauseAudio();
    
//    if ( musicStatus() == PLAYING )
//    {
//        printf("Music unpaused.\n");
//    }

//    playSound(DOOR2, SDL_MIX_MAXVOLUME / 2);
//    SDL_Delay(2000);

    /* Caching sound example, create, play from Memory, clear */

    Audio * sound = createAudio(DISP, 0, SDL_MIX_MAXVOLUME / 2);
    playSoundFromMemory(sound, SDL_MIX_MAXVOLUME);
    SDL_Delay(2000);

//    Audio * music = createAudio(ROAD, 1, SDL_MIX_MAXVOLUME);
//    playMusicFromMemory(music, SDL_MIX_MAXVOLUME);
//    dummyFun();

//    if ( musicStatus() == PLAYING )
//    {
//        printf("Music unpaused.\n");
//    }

    

    /* End Simple-SDL2-Audio */
    endAudio();

    /* Important to free audio after ending Simple-SDL2-Audio because they might be referenced still */
    freeAudio(sound);
    freeAudio(music);



void disp_sound(void)
{
    int i = 1;

    for (i = 1; i < S2WAIT; i++)
    {
        printf("%d ",i);
        fflush(stdout);
        if (i % 5 == 0)
        {
            printf("Door\n");
            playSound(DOOR2, SDL_MIX_MAXVOLUME / 2);
        }

        SDL_Delay(1000);
        
    }
    putchar('\n');
}

//void shortDummyFun(void)
//{
//    int i;
//    for (i = 1; i < S2WAIT/12; i++)
//    {
//        printf("%d ",i);
//        fflush(stdout);
//        if (i % 5 == 0)
//        {
//            printf("DISP\n");
//            playSound(DISP, SDL_MIX_MAXVOLUME / 2);
//        }
//
//        SDL_Delay(1000);
//
//    }
//    putchar('\n');
//}
