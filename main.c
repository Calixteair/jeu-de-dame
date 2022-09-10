#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

/*problemes du jeu pas encore corrig�s :
-les regles utilis�es ne sont pas les bonnes plus de d�taile dans le commentaire avant la fonction rule()
-situations  particuliere : si un pions veux se d�place sur une case et qu'il a deux possibilite pour y all�(un saut simple
et un saut multi) le jeu fera un saut simple
peut etre d'autres problemes pas encore trouves*/

const int NBPIONS = 20;
const int CASE_NOIRE = 1;
const int CASE_BLANCHE = 0;
const int DIRECTION_HAUT_DROITE = 14;
const int DIRECTION_BAS_DROITE = 16;
const int DIRECTION_BAS_GAUCHE = 19;
const int DIRECTION_HAUT_GAUCHE = 22;
const int PION_NOIR = 2;
const int PION_BLANC = 3;
const int DAME_NOIRE = 4;
const int DAME_BLANCHE = 5;
const char BLACK = 219;
const char WHITE = 32;
const char HLINE = 196;
const char VLINE = 179;
const char HLINEX = 197;
const char HLINEBX = 194;
const char HLINETX = 193;
const char ASG = 218;
const char ASD = 191;
const char AIG = 192;
const char AID = 217;
const char VLINEXD = 195;
const char VLINEXG = 180;
int move_saut_multi(int i_a , int i, int j_a, int j);
int transform_dame(int i_a , int i, int j_a, int j);
int check_arrive2(int i_a , int i, int j_a, int j);
int move_dame(int i_a , int i, int j_a, int j);
int colorp = 2;/*couleur joueur 3 = blanc 2 = noir*/
int sens = 1;/*1 si noir -1 si blanc*/
int acc_b = 0;/*nombre de pions blancs pris*/
int acc_n = 0;/*nombre de pions noirs pris*/
int i_a_temp = 0;
int j_a_temp = 0;
int direction = 0;
int mode = 0;

/*int grille[10][10]={{0}};*/
int grille_temp[10][10] = {{0}};

/*initialise le plateau pour jouer commence dans un situation particuliere (utilisé pour le debugage)*/
int grille[10][10] = {
        {0, 2, 0, 2, 0, 2, 0, 1, 0, 1},
		{1, 0, 1, 0, 3, 0, 1, 0, 1, 0},
		{0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
		{1, 0, 1, 0, 1, 0, 2, 0, 1, 0},
		{0, 1, 0, 1, 0, 3, 0, 3, 0, 1},
		{1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
		{0, 3, 0, 1, 0, 1, 0, 3, 0, 1},
		{3, 0, 3, 0, 2, 0, 1, 0, 1, 0},
		{0, 1, 0, 1, 0, 3, 0, 3, 0, 3},
		{1, 0, 1, 0, 1, 0, 1, 0, 3, 0}};


/*fonction debug*/

void debug(){
    acc_b = acc_n = NBPIONS;
    colorp = 3;
    for(int i = 0;i<10;i++){
        for(int j = 0;j<10;j++){
            switch( grille[i][j]){
            case 2:
                acc_n--;
                break;
            case 3:
                acc_b--;
                break;
            case 4:
                acc_n-= 2;
                break;
            case 5:
                acc_b-= 2;
                break;
            }
        }
    }
}
/* fonction pour afficher les regles
les regles de ce jeu ne sont malheureusement pas les vraies regles
-normalement les dames peuvent changer de direction n'importe
quand apres avoir pris un pions elles ne peuvent juste pas repasser sur un pions deja pris
-normalemnt le joueur doit faire sont d�lplacement et seulement apr�s enlever les pions pris du plateau
-le joueur est oblig� de jou� le coup ou il prend le plus de pions/dames cette regle peut etre apliquer par les joueur
mais elle n'est pas encore effectu� automatiquement par le programme*/

void choose_mode(int *mode){
    int tmp = 0;
    printf("Rentrer 1 si vous voulez jouer a deux joueurs, ou bien rentrer 2 si vous voulez jouer contre l'ordinateur\n");
    printf("l'ia ne joue pas de facon réflechit et elle ne peux pas bouger les dames : ");
    scanf("%d",&tmp);
    *mode = tmp;
}
void rule(){
    printf("              --LE JEU DE DAMES--\n\n");
    printf("L'affichage : \n");
    printf("-le jeu est sur un damier de 10 par 10\n");
    printf("-Les les pions blancs sont representes par les 'b' et les dames blanches par les 'B'\n");
    printf("-Les les pions noirs sont representes par les 'n' et les dames noires par les 'N'\n\n");
    printf("Les deplacements : \n");
    printf("    La base : \n    -pour chaque deplacement il faut au depart rentrer les coordonnees de depart horizontal puis vertical\n");
    printf("    il faut ensuite donner les coordonnees d'arrivees horizontal puis vertical\n");
    printf("    si les coordonnees sont fausses il faudra recommencer le mouvement du debut\n");
    printf("    Les pions : \n  les pions on trois mouvement possible : avancer en diagonale, faire un saut par dessus un pion ou dame adverse, ou faire un enchainement de sauts par dessus des pions ou des dames adverses\n -les movements simples sont effectues autaumatiquent si les coordonnees sont bonnes\n");
    printf("    -les sauts simples sont aussi sont effectues automatiquement si les coordonnees sont bonnes\n");
    printf("    -les sauts multiples : apres avoir donne des bonnes coordonnees vous devez indiquer le nombre de saut que vous allez faire\n");
    printf("    ensuite vous devrez donner les coordonnees de chaques sauts\n");
    printf("    Les dames : \n  -vous pouvez creer une dame en placant un pions sur la derniere ligne et en ayant au moins pions adverse pris en reserve\n");
    printf("    les dames peuvent faire les meme deplacement que les pions mais elles peuvent aussi se deplacer sur toute une diagonale\n");
    printf("    apres avoir pris un pions la dame peut changer de direction si elle prend un autre pions directement\n");
    printf("Condition de victoire : \n    -pour gagner voud devez prendre toutes les pieces adverses\n");
    printf("rentrer zero pour continuer\n");
    int temp = 0;
    scanf("%d",&temp);
}

/* Fonction qui permet d'initialiser le plateau de base de jeu avec les pions*/
int init_grille(){
    for(int i = 0;i<10;i++){
        for(int j = 0;j<10;j++){
            if(i<4){
                if (i % 2 == 0 ){
					grille[i][j]= 0;
					if(j % 2 == 1){
						grille[i][j]= 2;
					}
				}
				else {
					grille[i][j]= 2;
					if(j % 2 == 1){
						grille[i][j]= 0;
					}
				}
			}
			else if( i <6){
				if (i % 2 == 0 ){
					grille[i][j]= 0;
					if(j % 2 == 1){
						grille[i][j]= 1;
					}

				}
				else {
					grille[i][j]= 1;
					if(j % 2 == 1){
						grille[i][j]= 0;
					}
				}
			}
			else{
				 if (i % 2 == 0 ){
					grille[i][j]= 0;
					if(j % 2 == 1){
						grille[i][j]= 3;
					}

				}
				else {
					grille[i][j]= 3;
					if(j % 2 == 1){
						grille[i][j]= 0;
					}
				}
			}

		}
	}
	return 0;
    }

void win_condition(){
	if (acc_b == NBPIONS){
		printf("les noirs ont gagnes\n");
	}
	else if(acc_n == NBPIONS){
		printf("les blancs ont gagnes\n");
	}
}

/*fonction pour l'affichage de la grille*/
int print_grille(){
    int x,y,i;
    x = y = i = 0;

    printf("     0   1   2   3   4   5   6   7   8   9\n");
    printf("  ");
    printf(" %c",ASG);
    for( i = 0; i < 9; i++){
        printf("%c%c%c%c", HLINE, HLINE, HLINE, HLINEBX);
    }
    printf("%c%c%c%c", HLINE, HLINE, HLINE, ASD);

    for( x = 0; x < 10; x++){
        printf("\n %d %c", x, VLINE);
        for( y = 0; y < 10; y++){
            switch( grille[x][y])
            {
            case 1:
                printf("%c%c%c%c", WHITE, WHITE,WHITE,VLINE);
                break;
            case 0:
                printf("%c%c%c%c", BLACK, BLACK,BLACK,VLINE);
                break;
            case 3:
                printf("%c%c%c%c", WHITE,'b', WHITE, VLINE);/*pion blanc*/
                break;
            case 2:
                printf("%c%c%c%c", WHITE,'n', WHITE, VLINE);/*pion noire*/
                break;
            case 4:
                printf("%c%c%c%c", WHITE,'N', WHITE, VLINE);/*dame noire*/
                break;
            case 5:
                printf("%c%c%c%c", WHITE,'B', WHITE, VLINE);/*dame blanche*/
                break;
            }
        }
        if( x < 9){
            printf("\n   %c",VLINEXD);
            for( i = 0; i < 9; i++){
                printf("%c%c%c%c", HLINE, HLINE, HLINE, HLINEX);
            }
            printf("%c%c%c%c", HLINE, HLINE, HLINE, VLINEXG);
        }
    }
    printf("\n   %c",AIG);
    for( i = 0; i < 9; i++){
        printf("%c%c%c%c", HLINE, HLINE, HLINE, HLINETX);
    }
    printf("%c%c%c%c", HLINE, HLINE, HLINE, AID);
    printf("\n");
    printf("nombres de pions blancs pris %d\n",acc_b);
    printf("nombres de pions noirs pris %d\n",acc_n);
return 0;
}
/*fonction qui change le joueur qui doit jouer et le sens*/
int color_player(){
	if (colorp == 2){
		colorp = 3;
		sens = (-1);
		printf("c'est aux blancs de jouer\n");
	}
	else {
		colorp = 2;
		sens = 1;
		printf("c'est aux noirs de jouer\n");
	}
	return 0;
}
/*fonction qui recupere les coordonnees*/
int lecture_coo(int* x, int* y){
	printf("Verticale : ");
	scanf("%d",x);
	printf("Horizontale : ");
	scanf("%d",y);
	return 0;
}

/*fonction qui regarde si les coordonnees de depart sont bonnes avec print: si elles sont dans le damier et si le pions
et de la couleur du joueur*/
int check_dep(int i ,int j){
	if(grille[i][j]!= colorp && grille[i][j]!= (colorp + 2)){
		if(grille[i][j] == 1 || grille[i][j] == 0){
			printf("la case est vide\n");
			return 1;
		}
		else{
			printf("ce n'est pas votre couleur\n");
			return 1;
		}

	}
	else{
            if(grille[i][j]== colorp){
		printf("tu peux bouger ton pion\n");
            return 0;
            }
        else{
            printf("tu peux bouger ta dame\n");
            return 2;
        }
	}
	return 0;
}

/*fonction qui regarde si les coordonnees de depart sont bonnes sans print: si elles sont dans le damier et si le pions
et de la couleur du joueur*/
int check_dep2(int i ,int j){
	if(grille[i][j]!= colorp && grille[i][j]!= (colorp + 2)){
		if(grille[i][j] == 1 || grille[i][j] == 0){
			return 1;
		}
		else{
			return 1;
		}

	}
	else{
            if(grille[i][j]== colorp){
            return 0;
            }
        else{
            return 2;
        }
	}
	return 0;
}
/*fonction pour les sauts des pions
elle est appel� par la fonction move_simple et check_arrive2*/
int move_saut(int i_a , int i, int j_a, int j){
	int color_adv = 0;
	color_adv = colorp == 2 ? 3 : 2;
	if((abs(i - i_a) == 2) && (abs(j - j_a) == 2)){
		if(grille[(i+i_a)/2][(j+j_a)/2] == color_adv || grille[(i+i_a)/2][(j+j_a)/2] == (color_adv+2)){
			grille[i_a][j_a] = grille[i][j];
			grille[i][j] = 1;
			if(grille[(i+i_a)/2][(j+j_a)/2]==color_adv){
                grille[(i+i_a)/2][(j+j_a)/2] = 1;
                if(color_adv == 2){
                    acc_n = acc_n + 1;
                    return 0;
                }
                else {
                    acc_b = acc_b +1;
                    return 0;
                }
			}
			else{
                grille[(i+i_a)/2][(j+j_a)/2] = 1;
                if(color_adv == 2){
                    acc_n = acc_n + 2;
                    return 0;
                }
                else {
                    acc_b = acc_b +2;
                    return 0;
                }
			}
		}
		else{
            int var_temp = 0;
            if(mode != 2){printf("voulez vous faire un saut multiple : oui = 1 , non = 2  :  ");

            scanf("%d",&var_temp);
            }
            if(mode == 2){var_temp = 2;}
            if(var_temp == 1){
                if(/*i_a >(i + 2 * sens) && (*/(i_a+i)%2==0)/*)*/{
                    if(move_saut_multi(i_a,i,j_a,j)== 1){return 1;}
                    else{return 0;}
                }
                else{
                    if(mode != 2){printf("les coordonnees ne sont pas valides\n");}
                    return 1;
                    }
            }
        }
	}
	else{
        if(/*i_a >(i + 2 * sens) && (*/(i_a+i)%2==0)/*)*/{
			if(move_saut_multi(i_a,i,j_a,j)== 1){return 1;}
			else{return 0;}
		}
		else{
			printf("les coordonnees ne sont pas valides\n");
			return 1;
		}
	}
    return 1;
}
/*fonction pour faire plusieurs saut avec les pions
appelle par move_saut*/
int move_saut_multi(int i_a , int i, int j_a, int j){
	int nb_move = 0;
	printf("indiquez le nombre de sauts : ");
	scanf("%d",&nb_move);
    memcpy (grille_temp, grille,sizeof(grille_temp));
    int acc_b_temp = acc_b;
    int acc_n_temp = acc_n;
	while(nb_move>=1){
		printf("indiquez les coordonnees d'arrivee de votre prochain movement\n");
		lecture_coo(&i_a,&j_a);
		if(check_arrive2(i_a, i, j_a, j)== 1){
                memcpy (grille, grille_temp,sizeof(grille));
                acc_b = acc_b_temp;
                acc_n = acc_n_temp;
                print_grille();
                if(colorp == 2){printf("c'est aux noirs de jouer\n");}
                else{printf("c'est aux blancs de jouer\n");}
                return 1;
        }
        else{
            i = i_a;
            j = j_a;
            nb_move = nb_move - 1;
            print_grille();
            printf("il vous reste %d mouvement(s) a faire\n", nb_move);
		}


	}
	return 0;
}
/*fonction qui regarde si les coordonn�es d'arriv�es sont bonne(dans le damier et vide)
appel� par move_saut_multi*/
int check_arrive2(int i_a , int i, int j_a, int j){
	if(i_a >= 0 && i_a < 10 && j_a >= 0 && j_a < 10){
		if(grille[i_a][j_a] == 1){
			if(move_saut(i_a,i,j_a,j)== 1){return 1;}
			else{return 0;}
		}
		else{
			printf("les coordonnees ne sont pas valides\n");
			return 1;
		}
	}
	else{
		printf("les coordonnees ne sont pas valides\n");
		return 1;
	}
}



/*fonction pour deplacer les pions de une case en diagonale
appele par check_arrive*/
int move_simple(int i_a , int i, int j_a, int j){
	if(i_a == i+sens){
		if(j_a == (j+1) || j_a == (j-1)){
			grille[i_a][j_a] = grille[i][j];
			grille[i][j] = 1;
		}
	}
	else{if(grille[i][j]== colorp+2){
            move_dame(i_a,i,j_a,j);
        }
        else{
                if(move_saut(i_a,i,j_a,j)== 1){return 1;}
                else{
                    return 0;
                }
                }
	}
	return 0;
}


/*fonction qui regarde si les coordonn�es d'arriv�es sont bonne*/
int check_arrive(int i_a , int i, int j_a, int j){
	if(i_a >= 0 && i_a < 10 && j_a >= 0 && j_a < 10){/*dans le damier ?*/
		if(grille[i_a][j_a] == 1){                    /*vide ?*/
            if(move_simple(i_a,i,j_a,j)== 1){return 1;}
         }
		else{
			if(mode != 2){printf("la case d'arrivee n'est pas vide\n");}
			return 1;
		}
    }
	else{
		if(mode != 2){printf("les coordonnees d'arrivees ne sont pas sur le damier\n");}
		return 1;
	}
	return 0;
}
/* fonction pour les sauts avec une dame*/
int move_saut_d(int i_a , int i, int j_a, int j){
	int color_adv = 0;
	color_adv = colorp == 2 ? 3 : 2;
	i_a_temp = i;
	j_a_temp = j;
	if((abs(i - i_a) == 2) && (abs(j - j_a) == 2)){/*saut de une case ?*/
		if((grille[(i+i_a)/2][(j+j_a)/2] == color_adv) || (grille[(i+i_a)/2][(j+j_a)/2] == color_adv+2)){/* pion ou dame adverse dans le saut ?*/
			grille[i_a][j_a] = grille[i][j];
			grille[i][j] = 1;
			if(grille[(i+i_a)/2][(j+j_a)/2] == color_adv){
                grille[(i+i_a)/2][(j+j_a)/2] = 1;
                if(color_adv == 2){
                    acc_n = acc_n + 1;
                    if(i<i_a){
                        if(j<j_a){
                            direction = DIRECTION_BAS_DROITE;
                        }
                        else{ direction = DIRECTION_BAS_DROITE;

                        }
                    }
                    else{
                        if(j<j_a){
                            direction = DIRECTION_HAUT_DROITE;
                        }
                        else{direction = DIRECTION_HAUT_GAUCHE;

                        }
                    }
                    return 0;
                }
                else {
                    acc_b = acc_b +1;
                      if(i<i_a){
                        if(j<j_a){
                            direction = DIRECTION_BAS_DROITE;
                        }
                        else{ direction = DIRECTION_BAS_DROITE;

                        }
                    }
                    else{
                        if(j<j_a){
                            direction = DIRECTION_HAUT_DROITE;
                        }
                        else{direction = DIRECTION_HAUT_GAUCHE;

                        }
                    }
                    return 0;
                }
			}
			else{
                grille[(i+i_a)/2][(j+j_a)/2] = 1;
                if(color_adv == 2){
                    acc_n = acc_n + 2;
                      if(i<i_a){
                        if(j<j_a){
                            direction = DIRECTION_BAS_DROITE;
                        }
                        else{ direction = DIRECTION_BAS_DROITE;

                        }
                    }
                    else{
                        if(j<j_a){
                            direction = DIRECTION_HAUT_DROITE;
                        }
                        else{direction = DIRECTION_HAUT_GAUCHE;

                        }
                    }
                    return 0;
                }
                else {
                    acc_b = acc_b +2;
                      if(i<i_a){
                        if(j<j_a){
                            direction = DIRECTION_BAS_DROITE;
                        }
                        else{ direction = DIRECTION_BAS_DROITE;

                        }
                    }
                    else{
                        if(j<j_a){
                            direction = DIRECTION_HAUT_DROITE;
                        }
                        else{direction = DIRECTION_HAUT_GAUCHE;

                        }
                    }
                    return 0;
                }
			}
		}
		else{return 1;}
	}
	else{return 1;}
}
/* fonction pour les deplacement en diagonale des dame*/
int check_diago(int i_a , int i, int j_a, int j){
    int color_adv = 0;
	color_adv = colorp == 2 ? 3 : 2;
    if(i_a >= 0 && i_a < 10 && j_a >= 0 && j_a < 10){
		if(grille[i_a][j_a] == 1){
            if((MAX(i_a,i_a_temp)-MIN(i_a,i_a_temp)) != MAX(j_a,j_a_temp)-MIN(j_a,j_a_temp)){/*diagonale ?*/
                printf("ce n'est pas un diagonale ou une prise");
                return 1;
            }
            else{
                    int y = j_a;
                if(i_a > i_a_temp){/*s�paration des dif�rentes direction de movement*/
                    if(j_a > j_a_temp){
                        for (int u = i_a; u>= i_a_temp;u--){
                            if(u== i_a_temp){
                                        i_a_temp = i_a;
                                        j_a_temp = j_a;
                                        if(direction == DIRECTION_BAS_DROITE || direction == 0){
                                            direction = 0;
                                            return 0;
                                        }
                                        else{
                                            printf("restez dans la meme diagonale et direction que votre saut\n");
                                            return 1;
                                        }
                                 }
                            else{
                                if(grille[u][y] == 1){
                                y = y -1;
                                }
                                else{
                                    if(grille[u][y] == color_adv || grille[u][y] == color_adv){
                                        if(grille[u+1][y+1] != 1){
                                            printf("vous pouvez pas faire se saut\n");
                                            return 1;
                                        }
                                        else {
                                            grille[u][y]= 1;
                                            y = y -1;
                                        }
                                    }
                                    else{
                                            printf("la diagonale n'est pas vide\n");
                                        return 1;
                                    }
                                }
                            }
                        }
                    }
                    else{
                        for (int u = i_a; u>= i_a_temp;u--){
                            if(u== i_a_temp){
                                i_a_temp = i_a;
                                j_a_temp = j_a;
                                if(direction == DIRECTION_BAS_GAUCHE || direction == 0){
                                    direction = 0;
                                    return 0;
                                }
                                else{
                                    printf("restez dans la meme diagonale et direction que votre saut\n");
                                    return 1;
                                }
                            }
                            else{
                                if(grille[u][y] == 1){
                                y = y +1;
                                }
                                 else{
                                    if(grille[u][y] == color_adv || grille[u][y] == color_adv){
                                        if(grille[u+1][y-1] != 1){
                                            printf("vous pouvez pas faire se saut\n");
                                            return 1;
                                        }
                                        else {
                                            grille[u][y]= 1;
                                            y = y -1;
                                        }
                                    }
                                    else{
                                            printf("la diagonale n'est pas vide\n");
                                        return 1;
                                        }
                                    }
                                }
                        }
                    }
                }
                else{
                        if(j_a > j_a_temp){
                            for (int u = i_a; u<= i_a_temp;u++){
                                    if(u== i_a_temp){
                                            i_a_temp = i_a;
                                            j_a_temp = j_a;
                                        if(direction == DIRECTION_HAUT_DROITE || direction == 0){
                                        direction = 0;
                                        return 0;
                                        }
                                        else{
                                            printf("restez dans la meme diagonale et direction que votre saut\n");
                                            return 1;
                                        }
                                    }
                                    else{
                                        if(grille[u][y] == 1){
                                        y= y -1;
                                    }
                                         else{
                                            if(grille[u][y] == color_adv || grille[u][y] == color_adv){
                                                if(grille[u-1][y+1] != 1){
                                                    printf("vous pouvez pas faire se saut\n");
                                                    return 1;
                                                }
                                                else {
                                                    grille[u][y]= 1;
                                                    y = y -1;
                                                }
                                            }
                                            else{
                                                    printf("la diagonale n'est pas vide\n");
                                                return 1;
                                                }
                                            }
                                    }
                                }
                            }
                        else{
                            for(int u = i_a; u<= i_a_temp;u++){
                                if(u== i_a_temp){
                                        i_a_temp = i_a;
                                        j_a_temp = j_a;
                                    if(direction == DIRECTION_HAUT_GAUCHE || direction == 0){
                                        direction = 0;
                                        return 0;
                                    }
                                    else{
                                        printf("restez dans la meme diagonale et direction que votre saut\n");
                                        return 1;
                                    }
                                }
                                else{
                                    if(grille[u][y] == 1){
                                    y = y+1;

                                    }
                                    else{
                                        if(grille[u][y] == color_adv || grille[u][y] == color_adv){
                                            if(grille[u-1][y-1] != 1){
                                            printf("vous pouvez pas faire se saut\n");
                                            return 1;
                                            }
                                            else {
                                                grille[u][y]= 1;
                                                y = y -1;
                                            }
                                        }
                                        else{
                                            printf("la diagonale n'est pas vide\n");
                                            return 1;
                                        }
                                    }
                                }
                            }

                        }
                }
            }
		}
        else{
			return 1;
            }
    }
     else{
            printf("la case d'arrivee n'est pas dans le damier\n");
			return 1;
     }
        return 0;
}

/* fonction pour les deplacement des dames*/
int move_dame(int i_a , int i, int j_a, int j){
    printf("combien de movements voulez vous faire\n(les movements posssible sont : les sauts de un pion ou les diagonales) : ");
    int nb_moved = 0;
    i_a_temp = i;
    j_a_temp = j;
    memcpy (grille_temp, grille,sizeof(grille_temp));/* copy la grille pour avoir une sauvegarde si le joueur se trompe de coordonn�es*/
    int acc_b_temp = 0;
    acc_b_temp = acc_b;
    int acc_n_temp = 0;
    acc_n_temp = acc_n;
    int diago_count = 0;
    int check_mi = i;
    int check_mj = j;
    scanf("%d",&nb_moved);
    while(nb_moved >= 1){
        printf("indiquez les coordonnees d'arrivee de votre prochain movement\n");
		lecture_coo(&i_a,&j_a);
			if(i_a >= 0 && i_a < 10 && j_a >= 0 && j_a < 10){
                if(grille[i_a][j_a] == 1){
                        if(move_saut_d(i_a, i, j_a, j) == 1 && (check_diago(i_a, i, j_a, j)==1)){
                                memcpy (grille, grille_temp,sizeof(grille));/*le movement estfaux donc on r�tablille la grille de d�part*/
                                acc_b = acc_b_temp;
                                acc_n = acc_n_temp;
                                check_mi = i;
                                check_mj = j;
                                print_grille();
                                if(colorp == 2){printf("c'est aux noirs de jouer\n");}
                                else{printf("c'est aux blancs de jouer\n");}
                                    return 1;
                            }
                            else{
                                if(check_diago(i_a, i, j_a, j)==0){
                                    if((MAX(i_a,check_mi)-MIN(i_a,check_mi)) == MAX(j_a,check_mj)-MIN(j_a,check_mj)){
                                        grille[i_a][j_a] = grille[i][j];
                                        grille[i][j] = 1;
                                        i = i_a;
                                        j = j_a;
                                        nb_moved = nb_moved - 1;
                                        diago_count = diago_count + 1;
                                        print_grille();
                                        printf("il vous reste %d mouvement(s) a faire\n", nb_moved);
                                    }
                                    else{
                                            printf("vous devez restez dans la diagonale de votre saut\n");
                                            memcpy (grille, grille_temp,sizeof(grille));
                                            acc_b = acc_b_temp;
                                            acc_n = acc_n_temp;
                                            check_mi = i;
                                            check_mj = j;
                                            print_grille();
                                            if(colorp == 2){printf("c'est aux noirs de jouer\n");}
                                            else{printf("c'est aux blancs de jouer\n");}
                                            return 1;
                                    }
                                }
                                else{
                                    check_mi = i;
                                    check_mj = j;
                                    i = i_a;
                                    j = j_a;

                                    nb_moved = nb_moved - 1;
                                    diago_count = 0;
                                    print_grille();
                                    printf("il vous reste %d mouvement(s) a faire\n", nb_moved);
                                    continue;
                                    }

                                }
                            }
                }
                 if(diago_count > 1){
                            memcpy (grille, grille_temp,sizeof(grille));
                                acc_b = acc_b_temp;
                                acc_n = acc_n_temp;
                                print_grille();
                                printf("vous ne pouvez pas faire 2 diagonales a la suite");
                                if(colorp == 2){printf("c'est aux noirs de jouer\n");}
                                else{printf("c'est aux blancs de jouer\n");}
                                    return 1;
                        }

            }
            return 0;
    }


/*fonction pour transformer un pions en dame*/
int transform_dame(int i_a , int i, int j_a, int j){
   if(i_a == 0 && grille[i_a][j_a]== 3){ /*cas pions blanc sur la ligne la plus haute*/
        if(acc_b > 1){
        grille[i_a][j_a] = 5;
        acc_b = acc_b - 1;
        }
        else {
            printf("pas de pions pour la dame");
        }
    }
    else{
        if(grille[i_a][j_a]== 2 && i_a == 9){/*cas pions noir sur la ligne la plus basse*/
            if(acc_n>1){
            grille[i_a][j_a] = 4;
            acc_n = acc_n - 1;
            }
            else{
                printf("pas de pions pour la dame");
            }
        }
        return 1;
    }
    return 0;
}

int moves_ia(int i_a , int i, int j_a, int j){
    for(int i =0;i<10;i++){
        for(int j =0;j<10;j++){
            if(grille[i][j]==2){
                    i_a = i+1;
                    j_a = j+1;
                   if(check_arrive(i_a,i,j_a,j) == 0){return 0;}
                else{
                    i_a = i+1;
                    j_a = j-1;
                     if(check_arrive(i_a,i,j_a,j)== 0){return 0;}
                    else{
                    i_a = i+2;
                    j_a = j+2;
                        if(check_arrive(i_a,i,j_a,j)==0){return 0;}
                        else{
                            i_a = i+2;
                            j_a = j-2;
                            if(check_arrive(i_a,i,j_a,j)==0){return 0;}
                            else{
                                i_a = i-2;
                                j_a = j+2;
                                if(check_arrive(i_a,i,j_a,j)==0){return 0;}
                                else{
                                    i_a = i-2;
                                    j_a = j-2;
                                    if(check_arrive(i_a,i,j_a,j)==0){return 0;}
                                }
                            }
                        }
                    }
                }
            }

        }
    }
    return 1;
}


int main()
{
	int i,j;
	i = j = 0;
	int i_a, j_a;
	i_a = j_a = 0;/*i_a = coo verticale arriv� et j_a = coo horizontale arriv�*/

    /*init_grille();*/
    choose_mode(&mode);
    rule();
    debug();
    print_grille();
	color_player();
	while(acc_b != NBPIONS && acc_n != NBPIONS){/* boucle tant que que les deux joueur on au moins un pions ou dame*/
        if(mode == 2 && colorp==2){
            moves_ia(i_a,i,j_a,j);
            printf("\n");
            print_grille();
            color_player();
        }
        else{
		printf("coordonnees de depart ");
		lecture_coo(&i, &j);
		if(check_dep(i,j)== 1){continue;}
		else{
                if(check_dep2(i,j) == 0){
                    printf("coordonnees d'arrivee ");
                    lecture_coo(&i_a, &j_a);
                    if(check_arrive(i_a, i, j_a, j) == 1){continue;}
                    else{
                    transform_dame(i_a, i, j_a, j);
                    printf("\n");
                    print_grille();
                    color_player();
                    }
                }
                else{
                    if(move_dame(i_a, i, j_a, j)== 1){continue;}
                    else{
                        printf("\n");
                        color_player();

                    }
                }
            }
		}
	}
	win_condition();
}
