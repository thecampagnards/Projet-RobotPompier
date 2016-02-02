#ifndef MAIN_H
#define MAIN_H
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//Mac ou Linux
#if defined __APPLE__ || defined linux || defined __linux__ || defined (Q_OS_LINUX)
    #include <unistd.h> 
	#define CLEARSCREEN "clear"
	#define SYSTEME 'L'//L si la map est de format linux et W si la map est de format windaube
	#define SLEEP_TIME 500000

//Windows 32 bits ou 64 bits
#elif defined __WIN32__ || defined __WIN64__
    #include <windows.h>
	#define CLEARSCREEN "cls"
	#define SYSTEME 'W'
	#define SLEEP_TIME 200

#endif
	
//defines de "fct_createmap.c"
#define LG_MAX 100
//define de "main.h"
#define MAX_CASE_FEU 13


typedef struct COORDONNEES{
int x;
int y;
char type;
}COORDONNEES;

typedef struct QUEUE{
COORDONNEES *Tab;
int *from;
int front,back;
}QUEUE;

typedef struct ROBOT{
COORDONNEES pos;
COORDONNEES pos_old;
COORDONNEES feu;
int vie,dep,pompier;
}ROBOT;

typedef struct FEU{
COORDONNEES Tab[MAX_CASE_FEU];
int pos;//position dans le tableau
}FEU;

#endif


/*
Rapport:
	-intro
	-algorithm
	-difficultés
	-conclusion
*/
