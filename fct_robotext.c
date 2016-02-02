#include "fonctions.h"



/********************************************************************
Fonction: PushBack

Resume: envoit les coordonnees dans le tableau de la queue avec une incrementation du back

Interface:
	Input 
			-la structure queue
			-la structure coordonnees

**********************************************************************/

void PushBack(QUEUE *q,COORDONNEES current){

q->Tab[q->back++] = current;
return;
}


/********************************************************************
Fonction: PopFront

Resume: retourne la derniere coordonnee rentrée dans le tableau
		pour le pathfinding savoir ou il se trouve avec incrementation du front

Interface:
	Input 
			-la structure queue
	OutPut
			-la structure coordonnees
	
**********************************************************************/

COORDONNEES PopFront(QUEUE *q){

return q->Tab[q->front++];
}


/********************************************************************
Fonction: Scan

Resume: cette fonction va scanner la map et ne reviend pas sur ces pas grace des '.'

Interface:
	Input 
			-le tableau
			-la structure queue
			-la structure robot
			-un int pour savoir si il peut aller dans le feu ou non
			-un char pour le caractere d'arrivé
	
**********************************************************************/

void Scan(char** map,QUEUE *q,ROBOT *robot,int feu,char carac){

int i;
//le vecteur va permettre le deplacement a droite {1,0} a gauche {0,-1} en bas {-1,0} en haut {0,1}
//c'est l'ordre du pathfinding donc des fois pour les checkpoints le robot a un deplacement bizarre
int vec[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};

//remet la queue au debut

q->front = q->back = 0;

//envoit les coordonnees dans le Tab de queue
PushBack(q,robot->pos);
//tant que le back est plus petit ou egale que le front
// == si il n'a plus rien a checker (ref popfront et pushback)
while(!(q->back <= q->front)){
	//conserve l'indice du Tab pour apres le mettre dans le from si l'indice est bon
    int from = q->front;
	//recupere les dernieres coordonnees du Tab
    COORDONNEES current = PopFront(q);
    for(i=0;i<4;i++){
        COORDONNEES sonde;
        sonde.x = current.x + vec[i][0];
        sonde.y = current.y + vec[i][1];
		//les cases dispo de passage sont ' ', '1','2','3' et 'i'
        if ((GetCase(map,sonde) == ' '||GetCase(map,sonde) == 'i'||GetCase(map,sonde) == '1'||GetCase(map,sonde) == carac||GetCase(map,sonde) == 			'2'||GetCase(map,sonde) == '3')&&!feu){

			//garde les bons indices pour afficher le chemin
			q->from[q->back] = from;
			//rentre la coordonnee dans le tableau
       		PushBack(q,sonde);	
			if (GetCase(map,sonde) == carac){
				return;//carac choisi arrive
			}
			//marque la case pour ne pas revenir dessus
			SetCase(map,sonde,'.');
		}
		//si le robot connait la config du feu alors il prend les 2 et les 3 comme des murs
		//donc carac de passage ' ','i'et '1'
		else if ((GetCase(map,sonde) == ' '||GetCase(map,sonde) == 'i'||GetCase(map,sonde) == '1'||GetCase(map,sonde) == carac)&&feu){
			
			//garde les bons indices pour afficher le chemin
			q->from[q->back] = from;
			//rentre la coordonnee dans le tableau
        	PushBack(q,sonde);	
			if (GetCase(map,sonde) == carac){
				return;//carac choisi arrive
			}
		//marque la case pour ne pas revenir dessus
		SetCase(map,sonde,'.');
        }
    }
}
return;
}


/********************************************************************
Fonction: Tracer

Resume: cette fonction permet de tracer le chemin le plus court 
		en lisant le tableau de coordonnee
Interface:
	Input 
			-le tableau
			-la structure queue
	
**********************************************************************/

void Trace(char** map,QUEUE *q){

//commence par la fin tu Tab de coordonnee qui est l'arrive
int curseur = q->from[q->back-1];

while(curseur){
	//trace le chemin
	SetCase(map,q->Tab[curseur],'o');
	//recupere les bons indices du Tab pour avior les bonnes coordonnees
    curseur = q->from[curseur];
}
return;
}


/********************************************************************
Fonction: Deplacement

Resume: cettte fonction fait que le robot va suivre le chemin le plus court le robot remplace ca case precedente par un 'i'
		comme le petit poucet cela va lui permettre soit:
			->de revenir dans le feu si il l'a trouvé
			->ou pour chercher le feu savoir ou il est passé (en DEV)
Interface:
	Input 
			-la taille du tableau
			-la structure robot
			-le tableau
			-la structure feu
			-la structure queue
			-le type d'affichage
			-la structure pour la sdl
			-le char d'arrive
			-le char du chemin a suivre(c'etait pour reunir backfire dans cette fct mais en DEV)
			-un int si il doit stopper dans le feu
	
**********************************************************************/

void Deplacement(int hauteur,int largeur,ROBOT *robot,char **map,FEU *feu,QUEUE *q, char *type_affichage,GRAPH *graph,char arrive,char chemin,int stop){

int i;
char type_feu='0';
int continuer=1;

while(continuer){

	//conserve les dernieres coordonnees du robots avec le type de la case
    robot->pos_old=robot->pos;
	//suit le chemin du craractere envoyé en parametre et arrive au caractere de parametre envoyé
	
	if(map[robot->pos.y-1][robot->pos.x]==chemin||map[robot->pos.y-1][robot->pos.x]==arrive){
		robot->pos.y--;//NORD
	}
	else if(map[robot->pos.y][robot->pos.x-1]==chemin||map[robot->pos.y][robot->pos.x-1]==arrive){
		robot->pos.x--;//GAUCHE
	}
	else if(map[robot->pos.y+1][robot->pos.x]==chemin||map[robot->pos.y+1][robot->pos.x]==arrive){
		robot->pos.y++;//SUD
	}
	else if(map[robot->pos.y][robot->pos.x+1]==chemin||map[robot->pos.y][robot->pos.x+1]==arrive){
		robot->pos.x++;//DROITE
	}
	//arrive sur le caracteres envoye en parametre
	if(GetCase(map,robot->pos)==arrive){
		continuer=0;
	}
	//regarde si on est dans un feu quelle valeur du feu
	for(i=0;i<feu->pos;i++){
		if(robot->pos.y==feu->Tab[i].y&&robot->pos.x==feu->Tab[i].x){
			type_feu=feu->Tab[i].type;
		}
	}
	//si il est passé dans un feu il garde en memoire la derniere coordonnee et calcul le chemin le plus court
	if(type_feu=='1'){

		robot->vie--;
		//conserve les coordonnees du feu
		robot->feu=robot->pos;
		//si il arrive a un 1 pour la recherche du feu il se stop
		if(!stop){
			continuer=0;		
		}
	}
	//si il arrive dans le 2 il lance l'algo de pathfinding v2 pour reprendre le chemin le plus court tout en esquivant les 2 et 3
	if(type_feu=='2'){
			
		robot->vie-=2;
		//conserve les coordonnees du feu
		robot->feu=robot->pos;
		//netoit la map
		CleanMap(map,largeur,hauteur);
		//rajoute le feu pour le pathfinding
		AddFire (map,feu);
		//scan la map avec comme parametre de ne pas pouvoir aller dans le feu
		Scan(map,q,robot,1,'E');
		//trace le chemin le plus court
		Trace(map,q);
	}
	//regarde si on a 0 pdv
	if(robot->vie==0){

		system(CLEARSCREEN);
		printf("GAME OVER");
		exit(1);
	}
	//trace le passage du robot
	SetCase(map,robot->pos_old,'i');
	//si le robot est en mode pompier alors on ecrit p sinon r
	if(robot->pompier==0){
		SetCase(map,robot->pos,'R');
	}
	else{
		SetCase(map,robot->pos,'P');
	}
	
	//augemente le nb de deplacement
	robot->dep++;
	//affiche
	Affichage(map,hauteur,largeur,robot,type_affichage,graph,feu);
	type_feu='0';
}
return;
}

