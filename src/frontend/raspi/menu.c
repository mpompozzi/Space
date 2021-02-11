/* 
 * File:   menu.c
 * Author: magali
 *
 * Created on February 2, 2021, 9:39 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../../../headers/menu.h"

#define LARGO 16 
#define ANCHO 16

static int space[LARGO][ANCHO];

/*
 * 
 */
void menu(int opcion){//para allegro hacer otra cosa, escribe el simbolo de play o end
    if(opcion != 5 && opcion != 12)
    {
        return;
    }
    
    space[3][6]=1;
    space[4][6]=space[4][7]=1;
    space[5][6]=space[5][7]=space[5][8]=1;
    space[6][6]=space[6][7]=1;
    space[7][6]=1;
    //escribe END
    space[10][4]=space[10][5]=space[10][6]=1;
    space[11][4]=space[11][8]=space[11][11]=space[11][13]=space[11][14]=1;
    space[12][4]=space[12][5]=space[12][6]=space[12][8]=space[12][9]=space[12][11]=space[12][13]=space[12][15]=1;
    space[13][4]=space[13][8]=space[13][10]=space[13][11]=space[13][13]=space[13][15]=1;
    space[14][4]=space[14][5]=space[14][6]=space[14][8]=space[14][11]=space[14][13]=space[14][14]=1;
    
    space[opcion][0]=space[5][1]=1;//escribo el puntero en 5 o 12 segun corresponda
}

void matniv(void) {// escribe LV UP
    int i = 0;
    for (i = 4; i < 8; ++i) {
        switch (i) {
            case 4:/*dcoord_t myPoint = {i, j};
                disp_write(myPoint, D_ON);*/
                space[i][1] = space[i][5] = space[i][7] = space[i][9] = space[i][11] = space[i][13] = space[i][14] = 1;
                break;
            case 5:
                space[i][1] = space[i][5] = space[i][7] = space[i][9] = space[i][11] = space[i][13] = space[i][15] = 1;
                break;
            case 6:
                space[i][1] = space[i][5] = space[i][7] = space[i][9] = space[i][11] = space[i][13] = space[i][14] = 1;
                break;
            case 7:
                space[i][1] = space[i][2] = space[i][3] = space[i][6] = space[i][9] = space[i][10] = space[i][11] = space[i][13] = 1;
                break;
            default:
                break;
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////

void printnum(int num, int digit[]) {//imprimi un digito en la pos correspondiente
    int a, b, c, d;
    a = digit[0];
    b = digit[1];
    c = digit[2];
    d = digit[3];
    switch (num) {
        case 0://escribo 0
            space[10][a] = space[11][a] = space[12][a] = space[13][a] = space[14][a] = 1;
            space[10][c] = space[11][c] = space[12][c] = space[13][c] = space[14][c] = 1;
            space[10][b] = space[14][b] = 1;
            break;
        case 1://escribo 1
            space[10][a] = space[11][a] = space[12][a] = space[13][a] = space[14][a] = 1;
            break;
        case 2://escribo 2
            space[11][a] = space[14][a] = 1;
            space[10][b] = space[12][b] = space[14][b] = 1;
            space[10][c] = space[13][c] = space[14][c] = 1;
            space[11][d] = space[14][d] = 1;
            break;
        case 3://escribo 3
            space[10][a] = space[11][a] = space[12][a] = space[13][a] = space[14][a] = 1;
            space[10][b] = space[12][b] = space[14][b] = 1;
            space[10][c] = space[12][c] = space[14][c] = 1;
            space[10][d] = space[12][d] = space[14][d] = 1;
            break;
        case 4://escribo 4
            space[10][a] = space[11][a] = space[12][a] = space[13][a] = space[14][a] = 1;
            space[11][b] = 1;
            space[10][c] = space[11][c] = 1;
            break;
        case 5://escribo 5
            space[10][a] = space[13][a] = 1;
            space[10][b] = space[12][b] = space[14][b] = 1;
            space[10][c] = space[12][c] = space[14][c] = 1;
            space[10][d] = space[11][d] = space[14][d] = 1;
            break;
        case 6://escribo 6
            space[10][a] = space[12][a] = space[13][a] = space[14][a] = 1;
            space[10][b] = space[12][b] = space[14][b] = 1;
            space[10][c] = space[11][c] = space[12][c] = space[13][c] = space[14][c] = 1;
            break;
        case 7://escribo 7
            space[12][a] = 1;
            space[10][b] = space[11][b] = space[12][b] = space[13][b] = space[14][b] = 1;
            space[10][c] = space[12][c] = 1;
            space[10][d] = 1;
            break;
        case 8://escribo 8
            space[10][a] = space[11][a] = space[13][a] = space[14][a] = 1;
            space[10][b] = space[12][b] = space[14][b] = 1;
            space[10][c] = space[11][c] = space[13][c] = space[14][c] = 1;
            break;
        case 9://escribo 9
            space[10][a] = space[11][a] = space[12][a] = space[13][a] = space[14][a] = 1;
            space[10][b] = space[12][b] = 1;
            space[10][c] = space[11][c] = space[12][c] = 1;
            break;
        default:
            break;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////

void printscore(int puntaje) {//imprime el puntaje
    int num = 0; //aisla digitos para pasar a matriz
    int auxmat [4]; //para determinar que cols usar degun el digito
    int i;
    for (i = 4; i <= 8; ++i) {
        switch (i) {//escribe PTS
            case 4:
                space[i][1] = space[i][2] = space[i][6] = space[i][5] = space[i][7] = space[i][10] = space[i][11] = space[i][12] = 1;
                break;
            case 5:
                space[i][1] = space[i][3] = space[i][6] = space[i][9] = 1;
                break;
            case 6:
                space[i][1] = space[i][2] = space[i][6] = space[i][10] = space[i][11] = 1;
                break;
            case 7:
                space[i][1] = space[i][6] = space[i][12] = 1;
                break;
            case 8:
                space[i][1] = space[i][6] = space[i][9] = space[i][10] = space[i][11] = 1;
                break;
            default:
                break;
        }
    }
    for (i = 0; i < 3; ++i) {//pasa digit de der a izq 3 digitos
        num = (puntaje % 10);
        puntaje = (puntaje / 10);
        switch (i) {
            case 0:
                auxmat[0] = 15;
                auxmat[1] = 14;
                auxmat[2] = 13;
                auxmat[3] = 12;
                printnum(num, auxmat);
                break;
            case 1:
                auxmat[0] = 9;
                auxmat[1] = 8;
                auxmat[2] = 7;
                auxmat[3] = 6;
                printnum(num, auxmat);
                break;
            case 2:
                auxmat[0] = 3;
                auxmat[1] = 2;
                auxmat[2] = 1;
                auxmat[3] = 0;
                printnum(num, auxmat);
                break;
            default:
                break;
        }
    }
}

int get_menu(int i, int j)
{
    int dato;
    return dato = space[i][j];
}