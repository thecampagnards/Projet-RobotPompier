#include "fonctions.h"



int main( int argc,char** argv){

//affiche le fonctionnement de l'executable si les paramtres d'entrees sont pas bon
if(argc!=3){
	system(CLEARSCREEN);
	printf("Pour utiliser ce programme :\n\t-rentrer la map .txt en 1er argument.\n\t-rentrer le type d'affichage en 2eme argument (graphic ou texte).\n\n");
	return(1);
}
//

int hauteur,largeur,i;
FEU feu;
ROBOT robot;
QUEUE q;
GRAPH graph;
//le robot est en recherche d'extincteur donc => 0
robot.pompier=0;

//chargement de la map avec allocation des tableaux pour le pathfinding
char **map=ConstrucMap(&hauteur,&largeur,&robot,&feu,argv[1]);

//allocation de la queue
q.Tab=malloc(hauteur*largeur*sizeof(COORDONNEES));
q.from=malloc(hauteur*largeur*sizeof(int));

//chargement de la sdl si le 1er carac de la chaine du argv[2]
if(*(argv[2])=='g'){

	//Initialisation de la librairie graphique et changement des images
	Init_Graph(&graph,hauteur,largeur);
	//lancement de la musique de depart	
	Musique(&graph,MUSIQUE1);
	//affichage des images
	Affiche(map,&graph,hauteur,largeur);
}

//---------------------EXTINCTEUR-----------------------/

//avance depuis le depart
EnterHome(map,&robot,hauteur,largeur);

//affiche la map
Affichage(map,hauteur,largeur,&robot,argv[2],&graph,&feu);

//scan la map 0=droit aller dans le feu 'E'=arrive
Scan(map,&q,&robot,0,'E');

//tracer le chemin le plus court
Trace(map,&q);

//suit le deplacement le plus court 'E'=arrive 'o'=chemina suivre 1=stop le deplacement si il arrive dans le feu
Deplacement(hauteur,largeur,&robot,map,&feu,&q,argv[2],&graph,'E','o',1);

//-----------------------FEU----------------------------/

//le robot va chercher le feu
robot.pompier=1;

if(*(argv[2])=='g'){
	//lance la 2sd musique dans il a eu l'extincteur
	Musique(&graph,MUSIQUE2);
}
//si il est deja passé dans le feu
if(robot.feu.x!=0&&robot.feu.y!=0){

	BackFire(hauteur,largeur,&robot,map,argv[2],&graph,&feu);
}//si il n'etait pas passe dans le feu
else{
	//place des checkpoints pour trouver le feu
	SetCheckPoint(map,largeur,hauteur);
	
	//tant que le robot n'a pas mit en memoire la pos du feu
	while(robot.feu.x==0&&robot.feu.y==0){

		//netoit la map pour le 2sd scan
		CleanMap(map,largeur,hauteur);
		//scan la map pour trouver le h le plus proche avec le droit d'aller dans le feu
		Scan(map,&q,&robot,0,'h');
		//trace le chemin
		Trace(map,&q);
		//deplace
		Deplacement(hauteur,largeur,&robot,map,&feu,&q,argv[2],&graph,'h','o',0);
	}
}

//qaund le robot atteind le feu
robot.pompier=2;

//pour le terminal il passe le feu en '0'
for(i=0;i<feu.pos;i++){
	feu.Tab[i].type='0';
}
Affichage(map,hauteur,largeur,&robot,argv[2],&graph,&feu);

//si mode graphique
if(*(argv[2])=='g'){
	//lancement de la musique de fin avec un sleep pour avoir le temps que
	//la musique finisse
	Musique(&graph,MUSIQUE3);
	if(SYSTEME=='W'){
		Sleep(TEMPS_MUSIQUE_FIN);
	}
	else if(SYSTEME=='L'){
		sleep(TEMPS_MUSIQUE_FIN);
	}
}

//--------------------MEMOIRE------------------------/

//liberation de la memoire
LibAll(map,&q);

return 0;
}

