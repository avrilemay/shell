/* *******************************************************
# Nom ......... : and_monexit.c
# Rôle ........ : Gestion des processus en arrière-plan 
#                 Gestion de mon_exit 
# Auteur ...... : Avrile Floro 
# Version ..... : V0.1 du 26/10/2023
# Licence ..... : réalisé dans le cadre du cours de SE
# Compilation : Compilé grâce au Makefile (make) avec les autres fichiers
#********************************************************/


#include "sys.h"
#include "f_head.h"


int arriere_plan(int* nb_total_mots, char* mot[], bool* attend_enfant){
    
    if ((*nb_total_mots > 1) && mot[*nb_total_mots-1] && (strcmp (mot[*nb_total_mots-1], "&") == 0)){
         // s'il y a plus d'un mot et que le dernier mot est "&"
        *attend_enfant = false; 
        /* pas besoin d'attendre l'enfant, passage à faux */

        mot[*nb_total_mots-1] = 0; /* remplacement & par 0 */
                    /* pour ne pas être pris en compte par execv */

        (*nb_total_mots)--; /* mise à jour du nb de mots */
        return 0;
    }

    return 0;
}


int mon_exit(char *mot[]){
    if (strcmp(mot[0], "monexit") == 0){ // si le mot[0] est monexit
        printf("Bye\n");
        exit(0); // on quitte le shell sans erreur
    }
    return 0;
}


int mon_exit_pipe(char *mot[]){
    if (strcmp(mot[0], "monexit") == 0){ // si le mot[0] est monexit
        printf("Bye\n");
    }
    return 0;
}