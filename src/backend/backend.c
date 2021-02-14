/***************************************************************************//**
  @file     backend
  @brief   backend generico para raspi y para allegro
  @author   Alejandro Alatsis
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "../../headers/backend.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/



/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/



/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

// +ej: unsigned int anio_actual;+


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static void falta_envido (int);+

void crear_enemigo(int);
void crear_muro(void);

void movmat (int,int,int);//mueve elementos de la matriz a derecha izquierda arriba o abajo de donde estan
void playerdisp(int,int,int);//pone el disparo del player

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static const int temperaturas_medias[4] = {23, 26, 24, 29};+


/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static int space[LARGO][ANCHO];

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void inigame(juego_t *juego, int nivel_inicial) {
    srand(time(NULL));
    if (juego == NULL) {
        return;
    }
    if (nivel_inicial <= 0) {
        return;
    }
    inimat();
    ininiv(nivel_inicial);
    juego->nivel = nivel_inicial;
    juego->puntaje = 0;
    juego->vidas = 3;
    juego->tablero = &space[0][0];
    juego->omit =0;
}
/////////////////////////////////////

void inimat(void) { //inicializa la mat como vacia
    int i = 0, j = 0;
    for (i = 0; i < LARGO; ++i) {
        for (j = 0; j < ANCHO; ++j) {
            space[i][j] = 0;
        }
    }
}
/////////////////////////////////////////////////////////////////

void getcoordp(juego_t *juego) {
    if (juego == NULL) {
        return;
    }
    int j;
    for (j = 0; j < ANCHO; ++j) {
        if (space[LARGO - 1][j] == PLAYER) {
            juego->coordsp.i = LARGO-1;
            juego->coordsp.j = j;
            juego->coordsp.objeto = PLAYER;
        }
    }
}
/////////////////////////////////////////////////////////////////////

void ininiv(int nivel) {//inicializa las naves enemigas y la del jugador
    //harcodeo la nave del player
    if (nivel <= 0) {
        return;
    }
    space[LARGO - 1][ANCHO / 2] = PLAYER;
    crear_enemigo(nivel);
    crear_muro();
    /*case -1://PARA TESTEAR :)
         space[3][7]=ENEMYSHOT;
         space[11][15]=PSHOT;
        for(i=13;i<15;++i){
             for(j=1;j<LARGO;j+=3){//seteo defensas
                 space[i][j]=MURO;
                 ++j;
                 space[i][j]=MURO;
                 ++j;
                 space[i][j]=MURO;
             }       
         }*/
}
///////////////////////////////////////////////////////////////////////////////////

int ciclonaves(juego_t *juego) {//mueva las naves en la matriz
    int mov, i, j,exit;
    if(juego->omit==0){
        exit = 0;
        static int direccion = DER;
        static int bajar = 0; //defino si bajo o no 1 para si
        if (bajar == 0) {//si no tiene q bajar
            if (direccion == DER) {//muevo las naves a la derecha
                mov = DER;
                for (i = (LARGO - 2); i >= 0; --i) {
                    for (j = (ANCHO - 1); j >= 0; --j) {
                        if ((space[i][j] >= 1)&&(space[i][j] <= 4)) {//si hay nave
                            movmat(i, j, mov);
                            if ((space[i][ANCHO - 1] >= 1)&&(space[i][ANCHO - 1] <= 4)) {//si hay algo en la ultima col
                                direccion = IZQ; //invierto direccion
                                bajar = 1; //indico q tengo q bajar
                            }
                        }
                    }
                }
            } else {//muevo las naves a la izquierda
                mov = IZQ;
                for (i = (LARGO - 2); i >= 0; --i) {
                    for (j = 0; j < ANCHO; ++j) {
                        if ((space[i][j] >= 1)&&(space[i][j] <= 4)) {//si hay nave                       
                            movmat(i, j, mov);
                            if ((space[i][0] >= 1)&&(space[i][0] <= 4)) {//si hay algo en la primer col
                                direccion = DER; //invuerto direccion
                                bajar = 1; //indico q tengo q bajar
                            }
                        }
                    }
                }
            }
        } else {//tiene q bajar
            mov = ABAJO;
            bajar=0;
            for (i = (LARGO - 3); i >= 0; --i) {
                for (j = 0; j < ANCHO; ++j) {
                    if ((space[i][j] >= 1)&&(space[i][j] <= 4)) {//si hay nave                           
                        movmat(i, j, mov);
                    }
                }
            }
            for (i = (LARGO - 2), j = 0; j < ANCHO; ++j) {
                if ((space[i][j] >= 1)&&(space[i][j] <= 4)) {//si hay nave 
                    exit = 1; //marco salir
                    juego->omit=1;
                }
            }
            bajar=0;
        }
    }else{
        exit=1;
    }
    return exit;
}
////////////////////////////////////////////////////////////////////////////////////////

int nav_nod(void) { //spawnea nave nod y mueve nave nod
    static int haynavenod=0;
    int i, j;
    int mov = DER;
    for (j = 0; (j < ANCHO)&&(haynavenod==0); ++j) {//me fijo si hay una nave nod en juego
        if (space[0][j] == NAVNOD) {
            haynavenod = 1; //marco que hay nave nod
        } else {
            haynavenod = 0; //no hay una nave
        }
    }
    if (haynavenod == 0) {//si no hay nave me fijo de spawnear una
        space[0][0] = NAVNOD;
        space[0][1] = NAVNOD;
        haynavenod=1; //marca que hay nave
    }
    for (i = (LARGO - 2); i >= 0; --i) {
        for (j = (ANCHO - 1); j >= 0; --j) {
            if (space[i][j] == NAVNOD) {//si es la nave nodriza, siempre se mueve de izq a der
                if (j == (ANCHO - 1)) {//si la nave nod esta al borde desaparece
                    space[i][j - 1] = 0;
                    space[i][j] = 0;
                    haynavenod=0;
                } else {
                    movmat(i, j, mov);
                    haynavenod=1;
                }
            }
        }
    }
    return haynavenod;
}
//////////////////////////////////////////////////////////////////////////////////////////

coord_t ciclodispp(juego_t *juego, int i, int j) {//mueve los disparos y cambia puntaje y vidas
    int mov;
    coord_t evento;
    if (juego == NULL) {
        evento.objeto = WARNING;
        return evento;
    }
    evento.i = i;
    evento.j = j;
        
    
    if (space[i][j] == PSHOT) {//si hay un disparo de jugador
        mov = UP;
        if (i == 0) {//estoy en la primer fil 
            space[i][j] = 0;
        } else {
            if (space[i - 1][j] == 0) {//si adelante del disparo no hay nada
                movmat(i, j, mov);
            } else {//hay algo
                if ((space[i - 1][j] == ENEMY) || (space[i - 1][j] == ENEMYSHOT) || (space[i - 1][j] == NAVNOD) || (space[i - 1][j] == ENEMY_2) || (space[i - 1][j] == ENEMY_3)) {//si es enemigo cambiar si se agregan mas tipos de nemigos
                    juego->puntaje += ((space[i - 1][j])*10);
                    if (space[i - 1][j] == NAVNOD) {//si es la nave nod
                        if (space[i - 1][j - 1] == NAVNOD) {//me fijo donde esta la otra parte de la nave nod
                            space[i][j] = 0;
                            space[i - 1][j] = 0; //mato al enemigo
                            space[i - 1][j - 1] = 0; //mato la otra parte de la nav nodNADA = 0,NAVE_ENEMIGA,NAVE_NODRIZA, ESCUDO, JUGADOR,
                            evento.objeto = NAVE_NODRIZA;
                            return evento;
                        } else {
                            space[i][j] = 0;
                            space[i - 1][j] = 0; //mato al enemigo
                            space[i - 1][j + 1] = 0; //mato la otra parte de la nav nod
                        }
                    } else {//si es otra nave
                        space[i][j] = 0;
                        space[i - 1][j] = 0; //mato al enemigo
                        evento.objeto = NAVE_ENEMIGA;
                        return evento;
                    }
                } else {//si es un escudo
                    space[i][j] = 0;
                    space[i - 1][j] = 0; //rompe el escudo
                    evento.objeto = ESCUDO;
                    return evento;
                }
            }
        }
    }
    evento.objeto = NADA;
    return evento;
}

//////////////////////////////////////////////////////////////////////////////////////
coord_t ciclodispe(juego_t*juego,int i,int j){//mueve disparos del player y el enemigo
        int mov;
    coord_t evento;
    if (juego == NULL) {
        evento.objeto = WARNING;
        return evento;
    }
    evento.i = i;
    evento.j = j;
     
if (space[i][j] == ESHOT) {//si hay un disparo enemigo
        mov = ABAJO;
        if (i == (LARGO - 1)) {//si estoy en la ultima fila lo desaparezco
            space[i][j] = 0;
        } else {//si estoy en otra fila
            if (space[i + 1][j] == 0) {//si adelante del disparo no hay nada
                movmat(i, j, mov);
            } else {//hay algo
                if (space[i + 1][j] == PLAYER) {//si es el jugador
                    space[i][j] = 0;
                    juego->vidas -= 1; //pierde una vida
                    evento.objeto = JUGADOR;
                    return evento;
                } else {//si es un escudo
                    space[i][j] = 0;
                    space[i + 1][j] = 0; //rompe el escudo
                    evento.objeto = ESCUDO;
                    return evento;
                }
            }
        }
    }
    evento.objeto = NADA;
    return evento;
}
////////////////////////////////////////////////////////////////////////////////////

void verparams(juego_t *juego) {//se fija si pasaste al siguiente nivel y de actualizar las naves q disparan
    int i, j;
    int enemy=0;
    juego->naves = 0; //pongo que hay 0 naves
//    int vacio = 0; //1 si hay naves 0 si no
    int prom = 0; //1 si tengo q promover una nave para q dispare
    for (i = 0; i < LARGO; ++i) {
        for (j = 0; j < ANCHO; ++j) {
            if ((space[i][j] >= 1)&&(space[i][j] <= 4)) {//si hay naves
//                vacio = 1; //marco q no incremento el nivel
                juego->naves += 1; //cuento las naves
            }
        }
    }
    //ojo aca q me manejo con cols primero y despues fils
    for (i = 0; i < ANCHO; ++i) {
        for (j = (LARGO - 2); (j >= 0) && (enemy != 2) && (enemy != 1); --j) {
            if (space[j][i] == ENEMYSHOT) {//si hay un enemigo q dispara en esa col
                prom = 0; //no necesito promover a otro para q dispare
                enemy=2;
            }
            else 
              {
                prom=1;
              if ((space[j][i] == ENEMY) || (space[j][i] == ENEMY_2) || (space[j][i] == ENEMY_3)) {//cambiar si agrego mas tipos de enemigos
                enemy=1;
                }
//                if (prom == 1) {//si no encontre ninguna nave q dispara antes q esta
//                    juego->puntaje -= (ENEMYSHOT - space[j][i])*10;
//                    space[j][i] = ENEMYSHOT; //promuevo esa nave para q dispare                    
//                    prom = 0; //ya no necesito promover otra
//                }
            }
        }
        if(prom == 1) //parto diciendo q en esa col hay naves q necesiten ser promovidas
          {
            space[j][i]=ENEMYSHOT;
          }
    }
}
//////////////////////////////////////////////////////////////////////////////

void navdisp(void) {//determina cuando las naves disparan y cuando aparece la nave nodriza
    int i, j;
    int num = 0; //numero random
    for (i = 0; i < LARGO; ++i) {
        for (j = 0; j < ANCHO; ++j) {
            if (space[i][j] == ENEMYSHOT) {
                num = rand() % 100 + 1; //genero un num random entre 1 y 100
                if (num <= 20) {//20% de q dispare
                    space[i + 1][j] = ESHOT; //spawneo un disparo                   
                }
            }
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////

void pmov(juego_t* juego) {//movimiento y disparo del jugador
    int disparo = 0; //1 si hay diapro ya en juego
    int i, j;

    for (i = 0; i < LARGO; ++i) {//sacar estos for para multidisparo añadir breack point para testeo
        for (j = 0; j < ANCHO; ++j) {
            if (space[i][j] == PSHOT) {
                disparo = 1; //marco q ya hay un disparo en juego
            }
        }
    }
    switch (juego->mov) {//me fijo que tipo de mov recibo
        case 0://no me muevo
            break;
        case 1://me muevo a derecha
            if (juego->coordsp.j == (ANCHO - 1)) {//si estoy a la derecha de todo no me muevo
            } else {
                movmat(juego->coordsp.i, juego->coordsp.j, DER);
                juego->coordsp.j++;
            }
            break;
        case 2://me muevo a la izq
            if (juego->coordsp.j == 0) {//si estoy a la izquierda de todo no me muevo
            } else {
                movmat(juego->coordsp.i, juego->coordsp.j, IZQ);
                juego->coordsp.j--;
            }
            break;
        case 3://disparo sin moverme (arriba)              
            playerdisp(juego->coordsp.i, juego->coordsp.j, disparo);
            break;
        case 4://disparo y me muevo a la derecha
            playerdisp(juego->coordsp.i, juego->coordsp.j, disparo);
            if (juego->coordsp.j == (ANCHO - 1)) {//si estoy a la derecha de todo no me muevo
            } else {//si hay un disparo solo me muevo
                movmat(juego->coordsp.i, juego->coordsp.j, DER);
                juego->coordsp.j++;
            }
            break;
        case 5://disparo y me muevo a la izq
            playerdisp(juego->coordsp.i, juego->coordsp.j, disparo);
            if (juego->coordsp.j == 0) {//si estoy a la izq de todo no me muevo                   
            } else {
                movmat(juego->coordsp.i, juego->coordsp.j, IZQ);
                juego->coordsp.j--;
            }
            break;
        default:
            break;

    }
    juego->mov=0;
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 * 
 ******************************************************************************/
void playerdisp(int i, int j, int disparo) {
    if (disparo == 0) {//si no hay un disparo en juego
        if (space[LARGO - 2][j] == MURO) {//si hay muro
            space[LARGO - 2][j] = 0; //rompe el muro
        } else {
            space[LARGO - 2][j] = PSHOT; //spawneo el disparo del jugador
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////
#define TESTE
#ifdef TESTEO

void printmat(void) {//para testeo
    int m = 0;
    int n = 0;
    for (m = 0; m < LARGO; ++m) { /*mientras el contador de filas sea distinto a la cant. de filas, me fijo en las columnas*/
        for (n = 0; n < ANCHO; ++n) { /*mientras el contador de cols. sea distinto a la cant. de cols.imprime lo que haya en ese lugar de la matriz*/
            printf("%d", space[m][n]);
        }
        printf("\n");
    }
    printf("\n");
}
#endif
/////////////////////////////////////////////////////////////////////////////

void crear_enemigo(int nivel) {
    int i, j;
    if (nivel > 5) { // limita la cantidad de niveles a 5 para evitar desbordamientos del stack
        nivel = 5;
    }

    for (i = nivel; i < (nivel + 3); i += 2) {
        for (j = 1; j < ANCHO; j += SEPARACION) {//seteo enemigos
            if (i == nivel) {
                space[i][j] = ENEMY_3;
            } else {
                if (i == (nivel + 2)) {
                    if (nivel > 2) {
                        space[i][j] = ENEMY_2;
                    } else {
                        space[i][j] = ENEMY;
                    }
                }
            }
        }
    }
    for (i = (nivel + 4); i < (nivel + 5); ++i) {
        for (j = 1; j < ANCHO; j += SEPARACION) {
            space[i][j] = ENEMYSHOT;
        }
    }
}
/////////////////////////////////////////////////////////////////////////////

void crear_muro(void) {
    int i, j;
#ifdef RASPI
    for (i = 13; i < 15; ++i) {
        for (j = 1; j < ANCHO; j += 3) {//seteo defensas
            space[i][j] = MURO;
            ++j;
            space[i][j] = MURO;
            ++j;
            space[i][j] = MURO;
        }
    }
#elif ALLEGRO
    
#endif
}
//////////////////////////////////////////////////////////////////////////////

void movmat(int largo, int ancho, int mov) {//mueve elementos de la matriz a izquierda derecha arriba o abajo
    switch (mov) {
        case DER:
            space[largo][ancho + 1] = space[largo][ancho]; //muevo lo que haya ahi
            space[largo][ancho] = 0; //borro su anterior posicion
            break;
        case IZQ:
            space[largo][ancho - 1] = space[largo][ancho];
            space[largo][ancho] = 0;
            break;
        case UP:
            space[largo - 1][ancho] = space[largo][ancho];
            space[largo][ancho] = 0;
            break;
        case ABAJO:
            space[largo + 1][ancho] = space[largo][ancho];
            space[largo][ancho] = 0;
            break;
        default:
            break;


    }
}

int getmat(int i, int j) {
    int dato;
    return dato = space[i][j];
}
//////////////////////////////////////////////////////////////////
