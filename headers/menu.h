/* 
 * File:   menu.h
 * Author: magali
 *
 * Created on February 2, 2021, 9:44 PM
 */

#ifndef MENU_H
#define MENU_H

/////////////////////////////////////////////////
//                 FUNCTIONS                   //
/////////////////////////////////////////////////

///////////// MENU ////////////////
void menu (int opcion);//Escribe las opciones del menu, junto con un puntero que marca la opcion elegida

/*
 * Recibe donde debe escribir el puntero en un int
 */

/////////// DEL_POINTER ///////////
void del_pointer (int opcion); //Elimina el puntero de la opcion anterior

/*
 * Recibe donde debe borrar el puntero en un int
 */

//////////// PRINTSCORE ///////////
void printscore(int puntaje); //Escribe el puntaje en la matriz de menu
/*
 * Recibe un int con el puntaje obtenido por el jugador
 */

///////////// MATNIV //////////////
void matniv(void); //Escribe level up en la matriz de menu

///////////// GET_MENU ////////////
int get_menu (int i, int j); //Comprueba si debe prender el led de determinada posicion, dependiendo de la matriz de menu 

/*
 * Recibe dos ints (i y j) que dan la posicion en la matriz
 * 
 * Devuelve un int que indica si debe o no prender el led
 */


////////////// CLRMAT /////////////
void clrmat(void);  //Vacia la matriz

//////// SPACEINVADERS ///////
void spaceinvaders(void); //Imprime "SPACE INVADERS" de a 3 letras 


#endif



       

