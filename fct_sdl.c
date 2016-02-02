#include "fonctions.h"



/********************************************************************
Fonction: Main_SDL

Resume: main de la sdl tout les blitsurface etc vont ce passer ici

Interface:
	Input 
			-la structure de la SDL 
			-la structure du robots			
			-la structure du feu
			-la hauteur et la largeur du tableau			
		
**********************************************************************/

void Main_SDL(GRAPH *graph,ROBOT *robot,FEU *feu,int hauteur,int largeur){
int i;

//si j'avais mis en parametre la map j'aurais pu eviter d'aller dans le feu
//avec un GetCase(map,pos_old)!='D'
if(robot->pos_old.x!=0&&robot->pos_old.y!=0){
	//affichage de l'ancien sol
	Affiche_Case(robot->pos_old,graph->sol,graph->ecran);
}

//si on press echap on sort du programme
Evenement();

//affichage du feu qui bouge grace une icrementation modulo 2
if(graph->i==0){
	for(i=0;i<feu->pos;i++){
		Affiche_Case(feu->Tab[i],graph->feu1,graph->ecran);
	}
}
else{
	for(i=0;i<feu->pos;i++){
		Affiche_Case(feu->Tab[i],graph->feu2,graph->ecran);
	}
}
//si le robot eteind le feu alors il rentre dedans
if(robot->pompier==2){
	for(i=0;i<feu->pos;i++){
		Affiche_Case(feu->Tab[i],graph->feu3,graph->ecran);
	}
}
//affichage du robot quand il n'a pas l'extincteur
if(robot->pompier==0){
	SDL_Delay(DELAI_SDL_NORMAL);
	Affiche_Case(robot->pos,graph->robot,graph->ecran);
}
//affiche le robot qund il a le robot et le fait clignoter
else{
	SDL_Delay(DELAI_SDL_SPEED);
	if(graph->i==0){
		Affiche_Case(robot->pos,graph->pompier1,graph->ecran);
	}
	else{
		Affiche_Case(robot->pos,graph->pompier2,graph->ecran);
	}
}
//si victoire
if(robot->pompier==2){
	Win(graph,hauteur,largeur);
}
//affichage du texte
Affiche_Text(graph,robot,hauteur,largeur);
//pour le clignotement
graph->i++;
graph->i%=2;
//actualisation de la fenetre
SDL_Flip(graph->ecran);
return;
}


/********************************************************************
Fonction: Init_Graph

Resume: initialiser la sdl charge toute les surfaces et de la police

Interface:
	Input 
			-la structure de la SDL 
			-la hauteur et la largeur du tableau			
		
**********************************************************************/

void Init_Graph(GRAPH *graph,int hauteur,int largeur){

//initialisation de la SDL
if(SDL_Init(SDL_INIT_VIDEO) == -1) {
	fprintf(stderr, "Erreur d'initialisation de la SDL: %s\n", SDL_GetError());
	exit(1);
} 

//initialisation de ttf
if(TTF_Init() == -1){
	fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
	exit(1);
}

//icone fenetre
graph->icon = IMG_Load("icon.png");
SDL_WM_SetIcon(graph->icon, NULL);

//titre
SDL_WM_SetCaption("Mariobot",NULL);
//positione la fenetre au milieu de l'ecran
putenv("SDL_VIDEO_WINDOW_POS=center");


graph->ecran = SDL_SetVideoMode(TAILLE_PIXEL*largeur,TAILLE_PIXEL*hauteur+TAILLE_PIXEL*2, 32, SDL_HWSURFACE|SDL_DOUBLEBUF/*|SDL_FULLSCREEN*/|SDL_ANYFORMAT);

//ecran
if (graph->ecran == NULL) {
	fprintf(stderr, "Impossible de charger le mode Video : %s\n", SDL_GetError());
	exit(1);
}

//initialisation des images
graph->mur = SDL_LoadBMP("MUR.BMP"); 
graph->robot = SDL_LoadBMP("ROBOT.BMP"); 
graph->pompier1 = SDL_LoadBMP("POMPIER1.BMP"); 
graph->pompier2 = SDL_LoadBMP("POMPIER2.BMP"); 
graph->extincteur = SDL_LoadBMP("EXTINCTEUR.BMP");
graph->feu1 = SDL_LoadBMP("FEU1.BMP");
graph->feu2 = SDL_LoadBMP("FEU2.BMP");
graph->feu3 = SDL_LoadBMP("FEU3.BMP");
graph->sol = SDL_LoadBMP("SOL.BMP");
graph->depart = SDL_LoadBMP("DEPART.BMP");

//verifie tous les images et leur taille mais probleme normalement SDL_Surface a 2 parametres int h et int w pour la hauteur
//et largeur mais ils n'existent pas http://wiki.libsdl.org/SDL_Surface


//regarde si les images ont bien ete charger
if(!(graph->sol||graph->mur||graph->feu1||graph->feu2||graph->feu3||graph->sol||graph->robot||graph->extincteur||graph->pompier1||graph->pompier2)) { 
	fprintf(stderr, "Impossible de charger le(s) fichier(s) BMP: %s\n", SDL_GetError()); 
	exit(1);
}
return;
}


/********************************************************************
Fonction: Affiche

Resume: affiche pour la 1er fois la map

Interface:
	Input 
			-la map
			-la structure de la SDL 
			-la hauteur et la largeur du tableau			
		
**********************************************************************/

void Affiche(char **map,GRAPH *graph,int hauteur,int largeur){
int i,j;

//initialisation du rectangle
SDL_Rect current;
for(i=0;i<hauteur;i++){
	for(j=0;j<largeur;j++){
		current.x=j*TAILLE_PIXEL;
		current.y=i*TAILLE_PIXEL;
		if(map[i][j] == 'x') {
			SDL_BlitSurface(graph->mur, NULL, graph->ecran,&current);
		} 
		else if(map[i][j] == 'D') { 
			SDL_BlitSurface(graph->depart, NULL, graph->ecran,&current);
		}
		else if(map[i][j] == ' ') { 
			SDL_BlitSurface(graph->sol, NULL, graph->ecran,&current);
		} 
		else if(map[i][j] == '1' || map[i][j] == '2' || map[i][j] == '3') { 
			SDL_BlitSurface(graph->feu1, NULL, graph->ecran,&current);
 		}
		else if(map[i][j] == 'E') { 
			SDL_BlitSurface(graph->extincteur, NULL, graph->ecran,&current); 
		} 
		else if(map[i][j] == 'R') { 
			SDL_BlitSurface(graph->robot, NULL, graph->ecran,&current);
		} 
	}
}

//actualise la fenetre
SDL_Flip(graph->ecran);
return;
}


/********************************************************************
Fonction: Affiche_Case

Resume: Va remplacer a la position voulu l'image voulu

Interface:
	Input 
			-la structure avec les coordonnees a reafficher 
			-la surface qui va reecrire		
			-la surface du tableau			
		
**********************************************************************/

void Affiche_Case(COORDONNEES current,SDL_Surface *sprite,SDL_Surface *ecran){
			
SDL_Rect rect_temp;
				
rect_temp.y = current.y*TAILLE_PIXEL;
rect_temp.x = current.x*TAILLE_PIXEL;
		
SDL_BlitSurface(sprite,NULL,ecran,&rect_temp);
				
return;
}


/********************************************************************
Fonction: Musique

Resume: va permettre de jouer une musique dont le nom est envoyée en parametre

Interface:
	Input 
			-la structure de la SDL 
			-la chaine de craractere qui va contenir le nom de la musique		
		
**********************************************************************/

void Musique(GRAPH *graph,char *musique){

//chargement du mixer
if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT,MIX_DEFAULT_CHANNELS,1024) == -1){
	printf("Erreur chargement sdl mixer\n");
}

//chargement de la musique
graph->musique = Mix_LoadMUS(musique);
if(graph->musique==NULL){
	printf("Erreur lors du chargement de la musique : %s\n",musique);
}

//joue la musique	
Mix_PlayMusic(graph->musique, -1);
return;
}


/********************************************************************
Fonction: Evenement

Resume: Gestion de la fermeture de fenetre par "X" et de pause 'p'

Interface:
	Input 			
		
**********************************************************************/

void Evenement(){
SDL_Event event;
while ( SDL_PollEvent ( &event ) ) {
		switch ( event.type ) {
	
			case SDL_QUIT :
				exit(1);
			case SDL_KEYDOWN:
                if(event.key.keysym.sym){
					case SDLK_ESCAPE: //Appui sur la touche Echap, on arrête le programme
						exit(1);
					case SDLK_p: // Appui sur la touche p, => pause
						/*.......*/
						break;
                }
           break;
		}
}
return;
}


/********************************************************************
Fonction: Affiche_Text

Resume: genere le texte et le blit sur l'ecran avec un grand rectangle noir qui efface l'ancienne ecriture

Interface:
	Input 	
			-la structure de la SDL		
			-la structure du robot
			-la largeur et la hauteur
		
**********************************************************************/

void Affiche_Text(GRAPH *graph,ROBOT *robot,int hauteur,int largeur){

TTF_Font *police = NULL;
SDL_Rect position;

//efface le texte
SDL_Rect efface;
efface.x = 0;
efface.y = hauteur*TAILLE_PIXEL;
efface.w = largeur*TAILLE_PIXEL;
efface.h = TAILLE_PIXEL*2;
SDL_FillRect(graph->ecran, &efface,(0,0,0));


SDL_Color couleurBlanche = {255, 255, 255};

char chaine[20];

//affichage des pas
sprintf(chaine,"Pas : %d",robot->dep);
police = TTF_OpenFont(POLICE, TAILLE_POLICE);		
graph->texte = TTF_RenderText_Blended(police,chaine, couleurBlanche);
	
position.x = TAILLE_PIXEL;
position.y = hauteur*TAILLE_PIXEL;

SDL_BlitSurface(graph->texte, NULL, graph->ecran, &position);

//affichage de la vie
sprintf(chaine,"Vie : %d",robot->vie);
graph->texte = TTF_RenderText_Blended(police,chaine, couleurBlanche);
	
position.x = TAILLE_PIXEL*largeur/2;
position.y = hauteur*TAILLE_PIXEL;

SDL_BlitSurface(graph->texte, NULL, graph->ecran, &position);

return;
}


/********************************************************************
Fonction: Win

Resume: genere le texte et le blit sur l'ecran avec un grand rectangle rouge affiché
		au milieu de l'ecran (mais en DEV)

Interface:
	Input 	
			-la structure de la SDL		
			-la largeur et la hauteur
		
**********************************************************************/

void Win(GRAPH *graph,int hauteur,int largeur){

TTF_Font *police = NULL;
SDL_Rect position;

//creer le rectangle rouge
/*SDL_Rect *efface;
efface->x = 0;
efface->y = hauteur*TAILLE_PIXEL/2;
efface->w = largeur*TAILLE_PIXEL;
efface->h = TAILLE_PIXEL*2;

efface = SDL_CreateRGBSurface(SDL_HWSURFACE, 10, 10, 32, 0, 0, 0, 0); //Creating the surface.
SDL_FillRect(graph->ecran, efface, SDL_MapRGB(efface->format, 255, 0, 0)); //Filling the surface with red colour.
*/

//SDL_FillRect(graph->ecran, &efface, SDL_MapRGB(efface.format, 255, 0, 0));
//SDL_FillRect(graph->ecran, &efface,(0,255,0,0));


SDL_Color couleurNoir = {0, 0, 0};

char chaine[20];

//Affiche le message de victoire
sprintf(chaine,"Next Level !");
police = TTF_OpenFont(POLICE, 100);		
graph->texte = TTF_RenderText_Blended(police,chaine, couleurNoir);
	
position.x = TAILLE_PIXEL*4;
position.y = hauteur*TAILLE_PIXEL/2;

SDL_BlitSurface(graph->texte, NULL, graph->ecran, &position);

return;
}
