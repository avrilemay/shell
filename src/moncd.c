/* *******************************************************
# Nom ......... : moncd.c
# Rôle ........ : la fonction mon_cd permet de changer de répertoire
# Auteur ...... : Avrile Floro 
# Version ..... : V0.1 du 01/09/2023
# Licence ..... : réalisé dans le cadre du cours de SE
# Compilation : compiler avec les autres programmes (selon l'appelant)
#********************************************************/

#include "sys.h"
#include "f_head.h"

int appel_mon_cd(char *dir[], char *mot[], int* nb_total_mots){
    if (strcmp(mot[0], "moncd") == 0){ // si le mot[0] est moncd
        
        if ((*nb_total_mots > 2)&&(mot[2]!=NULL)){ // s'il y a plus de 2 mots
                fprintf(stderr, "Usage: moncd [dir]\n");
                return 1;
            } 
        if (*nb_total_mots == 2){
            mon_cd(&mot[1]); // on lance la fonction mon_cd
            return 0;
        }
        
        else { // si mot[1] n'existe pas
            mon_cd(NULL); // on lance la fonction mon_cd
            return 0;
        }
    }
    return 2; /* pour ne pas trigger le "continue" dans main */
} 


int mon_cd(char *dir[]){
    int t;

    if (dir != NULL && *dir != NULL){ // s'il y a un path en argument
        t = (chdir(*dir)); // on change de répertoire 
        if (t < 0){ // si erreur
            perror("chdir"); // affiche msg erreur 
            return 1;
        }
    }

    else { // s'il n'y a pas de path en argument
        char * h = getenv("HOME"); // on récupère le path du répertoire HOME

        if (h == NULL){ // si erreur
            perror("Pas de répertoire HOME"); // affiche msg erreur
            return 1;
        }
        
        else{
            t = chdir(h) ; // on change de répertoire vers HOME

            if (t < 0){ // si erreur
                perror("Pas de répertoire HOME"); // affiche msg erreur
                return 1;
            }
        }
    }
    return 0;
}

