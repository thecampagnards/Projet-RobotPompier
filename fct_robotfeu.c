#include "fonctions.h"



/********************************************************************
Fonction: BackFire

Resume: si le robot est passé dans le feu pour trouver l'extincteur alors il va retourner sur ces pas 'i'
		jusqu'a l'ancienne coordonnee du feu (j'aurais pu l'integrer a ma fct deplacement mais en DEV)

Interface:
	Input 
			-la largeur et la hauteur du tableau
			-la structure robot
			-le tableau
			-la chaine du type d'affichage
			-la structure pour la sdl
			-la structure feu

**********************************************************************/

void BackFire(int hauteur,int largeur,ROBOT *robot,char **map,char *type_affichage,GRAPH *graph,FEU *feu){
int continuer=1;
int i;
while(continuer){
    //conserve les dernieres coordonnees du robots avec le type de la case
	robot->pos_old=robot->pos;

    if(map[robot->pos.y][robot->pos.x-1]=='i'){
		robot->pos.x--;//GAUCHE
	}
    else if(map[robot->pos.y-1][robot->pos.x]=='i'){
		robot->pos.y--;//NORD
	}
    else if(map[robot->pos.y][robot->pos.x+1]=='i'){
		robot->pos.x++;//DROITE
	}
    else if(map[robot->pos.y+1][robot->pos.x]=='i'){
		robot->pos.y++;//SUD
	}
	//regarde si il est dans le feu
	for(i=0;i<feu->pos;i++){
		if(robot->pos.y==feu->Tab[i].y&&robot->pos.x==feu->Tab[i].x&&(feu->Tab[i].type=='1'||feu->Tab[i].type=='2')){
			robot->vie--;
			continuer=0;
		}		
	}
	//augemente le nb de pas
	robot->dep++;
	//ecrit le pas precedent et celle du robot
	SetCase(map,robot->pos_old,'j');
	SetCase(map,robot->pos,'P');
	//affichage
	Affichage(map,hauteur,largeur,robot,type_affichage,graph,feu);
}
return;
}


/********************************************************************
Fonction: SetCheckPoint

Resume: place des checkpoints a des positions strategique
		mais le placement est pas top pour trouver le feu (en DEV)

Interface:
	Input 
			-le tableau
			-la largeur et la hauteur du tableau

**********************************************************************/

void SetCheckPoint (char **map,int largeur,int hauteur){
int i,j;

//place un checkpoint tout les 3 cases de hauteur et tout les 2 cases de largeur
for (i=2;i<hauteur;i+=3){
	for(j=1;j<largeur;j++){
		if(map[i][j]!='x'){
			map[i][j]='h';
		}
	}
}
return;
}


