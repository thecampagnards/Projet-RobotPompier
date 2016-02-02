#include "main.h"
#include "fct_sdl.h"

//fonctions de "fct_sdl.c"
void main_GRAPH (GRAPH *graph,ROBOT *robot,FEU *feu,int hauteur,int largeur);
void Init_Graph(GRAPH *graph,int hauteur,int largeur);
void Affiche(char **map,GRAPH *graph,int hauteur,int largeur);
void Actu_Affiche(char **map,GRAPH *graph,ROBOT *robot);
void Affiche_Case(COORDONNEES current,SDL_Surface *sprite,SDL_Surface *ecran);
void Affiche_Text(GRAPH *graph,ROBOT *robot,int hauteur,int largeur);
void Evenement();
void Musique(GRAPH *graph,char *musique);
void Win(GRAPH *graph,int hauteur,int largeur);
//

//fonctions de "fct_createmap.c"
char **ConstrucMap(int* hauteur,int* largeur,ROBOT* robot,FEU *feu,char *name_map);
char **InitMap(int nbLin, int nbCol);
void Affichage(char** map,int hauteur,int largeur,ROBOT *robot,char *type_affichage,GRAPH *graph,FEU *feu);
void LibAll(char **tableau,QUEUE *q);
char GetCase(char** map,COORDONNEES current);
void SetCase(char** map,COORDONNEES current,char carac_remp);
void AddFire (char** map,FEU *feu);
void EnterHome(char **map,ROBOT *robot,int hauteur,int largeur);
void CleanMap(char **map,int largeur,int hauteur);
//

//fonctions de "fct_robotext.c"
void PushBack(QUEUE *q,COORDONNEES current);
int Empty(QUEUE *q);
COORDONNEES PopFront(QUEUE *q);
void Scan(char** map,QUEUE *q,ROBOT *robot,int feu,char carac);
void Trace(char** map,QUEUE *q);
void Deplacement(int hauteur,int largeur,ROBOT *robot,char **map,FEU *feu,QUEUE *q, char *type_affichage,GRAPH *graph,char arrive,char chemin,int stop);
int ScanFeu(char** map,QUEUE *q,ROBOT *robot);
//

//fonctions de "fct_robotfeu.c"
void BackFire(int hauteur,int largeur,ROBOT *robot,char **map,char *type_affichage,GRAPH *graph, FEU *feu);
void SetCheckPoint (char **map,int largeur,int hauteur);
//


