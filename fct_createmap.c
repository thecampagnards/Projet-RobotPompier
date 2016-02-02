#include "fonctions.h"



/********************************************************************
Fonction: ConstrucMap

Resume: cette fonction permet de tranferer la carte en txt dans un tableau a 2 dimensions
		elle garde les coordonnees du depart et des feux (le feu c'est pour l'affichage pas pour tricher)

Interface:
	Input 
			-la hauteur et la largeur du tableau	 
			-la structure du robots			
			-la structure du feu
			-le nom de la map
	Ouput
			-l'adresse du tableau a 2 dimension

**********************************************************************/

char **ConstrucMap(int* hauteur,int* largeur,ROBOT *robot,FEU *feu,char *name_map){
int extincteur=0;
int i,j;
char nbLin [LG_MAX];
char *nbCol=NULL;
feu->pos=0;

//ouverture du fichier contenant la map
FILE* fichier = NULL;
fichier = fopen(name_map, "r");
if(fichier==NULL){
	printf("Erreur lors dur chargement du fichier");
	exit(-1);
}

//obtention des dimensions de la map citué au debut du fichier texte
fgets(nbLin, LG_MAX, fichier);
nbCol=strchr(nbLin,':');
nbCol=(nbCol+1);
//transformation de la chaine de caractere en int
*largeur=atoi(nbLin);
*hauteur=atoi(nbCol);

//appel de la fct pour initialiser le tableau a 2 dim de taille variable
char **map=InitMap(*hauteur,*largeur);
//remplissage du tableau caractere par caractere
for(i=0;i<*hauteur;i++){
    for(j=0;j<*largeur;j++){
        map[i][j]=fgetc(fichier);
		//cherche si il y a un extincteur
        if(map[i][j]=='E'){
		  	extincteur=1;
        }
		//stockage du depart
        if(map[i][j]=='D'){
            robot->pos.x=j;
            robot->pos.y=i;
            robot->vie=10;
			robot->dep=0;
			robot->feu.x=0;
			robot->feu.y=0;
        }
		//stockage du feu pour le reafficher
		if(map[i][j]=='1'||map[i][j]=='2'||map[i][j]=='3'){
            feu->Tab[feu->pos].x=j;
            feu->Tab[feu->pos].y=i;
            feu->Tab[feu->pos].type=map[i][j];
			feu->pos++;
        }
    }
    //incremention du pointeur de 2 du fichier permet de sauter le retour chariot pour 1 linux et 2 pour windows
    fgetc(fichier);
	if(SYSTEME=='L'){
		fgetc(fichier);
	}
//verifie si il y a un extincteur et un depart
}
if(!extincteur){
	printf("\nIl n'y a pas d'extincteur !\n");
	exit(1);
}
if(robot->vie!=10){
	printf("\nIl n'y a pas de depart !\n");
	exit(1);
}
//fermeture du fichier
fclose(fichier);
return map;
}


/********************************************************************
Fonction: InitMap

Resume: cette fonction permet de creer le tableau a 2 dimensions
		la 1er dimension est crée, c'est la colonne
		ensuite on creer les lignes et les adresses des lignes sont stockées dans 
		chaque colonne

Interface:
	Input 
			-la hauteur et la largeur du tableau	 
	Ouput
			-l'adresse du tableau a 2 dimension

**********************************************************************/

char **InitMap(int nbLin, int nbCol){
int i;
//Allocation de la 1er dimension
char **tableau = (char **)malloc(sizeof(char*)*nbLin);
//Allocation 2e dimension
char *tableau2 = (char *)malloc(sizeof(char)*nbCol*nbLin);
for(i = 0 ; i < nbLin ; i++){
    tableau[i] = &tableau2[i*nbCol];
}
return tableau;
}


/********************************************************************
Fonction: Affichage

Resume: Cette fonction permet d'afficher la map la vie du robot et ces deplacements

Interface:
	Input 
			-le tableau
			-la hauteur et la largeur du tableau
	 		-la structure robot
			-le type d'affichage graphique ou texte
			-la structure pour la SDL
			-la structure feu

**********************************************************************/

void Affichage(char** map,int hauteur,int largeur,ROBOT *robot,char *type_affichage,GRAPH *graph,FEU *feu){

//affichage de la sdl
if(*type_affichage=='g'){
	Main_SDL(graph,robot,feu,hauteur,largeur);	
}
//affichage en mode terminal
else{
	int i,j,k;
	system(CLEARSCREEN);
	for(i=0;i<hauteur;i++){
		for(j=0;j<largeur;j++){
			//affichage du feu
			for(k=0;k<feu->pos;k++){
				if( i==feu->Tab[k].y && j==feu->Tab[k].x && (GetCase(map,feu->Tab[k])!='R'&&GetCase(map,feu->Tab[k])!='P')){
					printf("%c",feu->Tab[k].type);
					j++;
				}
			}
			//on cache les carac nuisant
			if(map[i][j]=='o'||map[i][j]=='.'||map[i][j]=='i'||map[i][j]=='j'||map[i][j]=='h'||map[i][j]=='D'){
				printf(" ");
			}
			//on affiche les carac non nuisant
			else {
				printf("%c",map[i][j]);
			}

		}
		printf("\n");
	}
	printf("\nNb de pas:%d Vie:%d\n",robot->dep,robot->vie);
	if(SYSTEME=='W'){
		Sleep(SLEEP_TIME);
	}
	else if(SYSTEME=='L'){
		usleep(SLEEP_TIME);
	}
}
return;
}


/********************************************************************
Fonction: LibAll

Resume: cette fonction va liberer la memoire de la map, le tableau a 2 dimensions
		les tableaux pour le pathfinding et la sdl

Interface:
	Input 
			-le tableau
			-la structure queue

**********************************************************************/

void LibAll(char **tableau,QUEUE *q){
free(q->from);
free(q->Tab);
free(tableau[0]);
free(tableau);
Mix_CloseAudio();
TTF_Quit();
SDL_Quit();
return;
}


/********************************************************************
Fonction: SetCase

Resume: cette fonction permet de remplir une coordonnees de la map par 
		le carac voulu tout en envoyant une variable de type coordonnees

Interface:
	Input 
			-le tableau
			-la structure coordonnees
			-un char

**********************************************************************/

void SetCase(char** map,COORDONNEES current,char carac_remp){

map[current.y][current.x] = carac_remp;
return;
}


/********************************************************************
Fonction: GetCase

Resume: cette fonction permet de connaitre le caractere d'une coordonnees de la map 

Interface:
	Input 
			-le tableau
			-la structure coordonnees
	Output
			-un char

**********************************************************************/

char GetCase(char** map,COORDONNEES current){

return map[current.y][current.x];
}


/********************************************************************
Fonction: AddFire

Resume: Cette fonction permet de rajouter le feu sur la map

Interface:
	Input 
			-le tableau
			-la structure feu

**********************************************************************/

void AddFire (char** map,FEU *feu){
int i;
for(i=0;i<(feu->pos);i++){
	SetCase(map,feu->Tab[i],feu->Tab[i].type);
}
return;
}


/********************************************************************
Fonction: EnterHome

Resume: Cette fonction permet de faire avancer le robot d'une case depuis la case depart

Interface:
	Input 
			-le tableau
			-la structure robot
			-la largeur et a hauteur de la map

**********************************************************************/

void EnterHome(char **map,ROBOT *robot,int hauteur,int largeur){
//regarde en 1er condition si on sort du tableau et apres si la case n'est pas un mur

if ((/*regarde si le case de dessous n'est pas un mur*/(robot->pos.y+1 < hauteur)&&map[robot->pos.y+1][robot->pos.x]!= 'x')||
/* ou regarde si la case de dessus n'est pas un ' ' ou 'x'*/((robot->pos.y-1 > 0)&&map[robot->pos.y-1][robot->pos.x]!= ' '&&map[robot->pos.y-1][robot->pos.x]!= 'x')){
	//bas
	robot->pos.y++;
}  
else if (/*regarde si le case de dessus n'est pas un mur*/((robot->pos.y-1 > 0)&&map[robot->pos.y-1][robot->pos.x]!= 'x')||
/* ou regarde si la case de dessous n'est pas un ' ' ou 'x'*/((robot->pos.y+1 < hauteur)&&map[robot->pos.y+1][robot->pos.x]!= ' '&&map[robot->pos.y+1][robot->pos.x]!= 'x')){
	//haut
    robot->pos.y--;
}
SetCase(map,robot->pos,'R');
robot->dep++;
return;
}


/********************************************************************
Fonction: CleanMap

Resume: netoit la map pour le pathfinding

Interface:
	Input 
			-le tableau
			-la largeur et a hauteur de la map

**********************************************************************/

void CleanMap(char **map,int largeur,int hauteur){
int i,j;
for(i=0;i<hauteur;i++){
	for(j=0;j<largeur;j++){
		if(map[i][j]=='o'||map[i][j]=='.'||map[i][j]=='i'||map[i][j]=='j'||map[i][j]=='*'){
			map[i][j]=' ';
		}
	}
}	
return;
}
