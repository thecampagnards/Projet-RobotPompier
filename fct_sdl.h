#ifndef FCT_SDL_H
#define FCT_SDL_H

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>

#define TAILLE_PIXEL 15

//define de "fct_sdl.c"
#define DELAI_SDL_NORMAL 200
#define DELAI_SDL_SPEED 100
#define MUSIQUE1 "musique1.mp3"
#define MUSIQUE2 "musique2.mp3"
#define MUSIQUE3 "musique3.mp3"
#define TEMPS_MUSIQUE_FIN 6 //seconde de la musiqe +1
#define POLICE "POLICE.TTF"
#define TAILLE_POLICE 29

typedef struct GRAPH{
SDL_Surface *icon;
SDL_Surface *ecran;
SDL_Surface *mur;
SDL_Surface *depart;
SDL_Surface *feu1;
SDL_Surface *feu2;
SDL_Surface *feu3;
SDL_Surface *sol;
SDL_Surface *robot;
SDL_Surface *extincteur;
SDL_Surface *pompier1;
SDL_Surface *pompier2;
SDL_Surface *texte;
Mix_Music *musique;
int i;
}GRAPH;

#endif
