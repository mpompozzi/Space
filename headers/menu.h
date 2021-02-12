/* 
 * File:   menu.h
 * Author: magali
 *
 * Created on February 2, 2021, 9:44 PM
 */

#ifndef MENU_H
#define MENU_H

void menu (int opcion);//para raspi printea el menu
void printscore(int puntaje);//para raspi escribe el puntaje
void printnum(int num,int digit[]);//funcion local para printscore
void matniv(void);//para raspi, esccribe level up en la matriz
int get_menu (int i, int j);
void clrmat(void);  //limpia la matriz

#endif



       

