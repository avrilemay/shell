/* *******************************************************
# Nom ......... : pipe.c
# Rôle ........ : Gestion des pipes : | et |  | 
# Auteur ...... : Avrile Floro 
# Version ..... : V0.1 du 26/10/2023
# Licence ..... : réalisé dans le cadre du cours de SE
# Compilation : Compilé grâce au Makefile (make) avec les autres fichiers
#********************************************************/

#include "sys.h"
#include "f_head.h"



int cb_pipe(char* mot[], int nb_total_mots, bool* ilya_pipe)
{ /* compte le nb de signe "pipe" | dans la ldc */
    int c = 0;
    for (int i = 0; i < nb_total_mots -1 ; i++) {
        if (strcmp(mot[i], "|") == 0) /* à chaque pipe re ontré */
        {  
            c++; 
            *ilya_pipe= true;
        }
        
    }
    return c; /* renvoie le nb de pipes */
}

int gestion_pipes (int nb_pipe_total, int nb_total_mots, char* mot[], char* mots_pipe[][MaxMot], int mes_pipes[][2], char* dirs[], char pathname[], int pathname_size, bool attend_enfant, bool* ilya_pipe){

    int* nb_mots_lastpipe = malloc(sizeof(int));
    *nb_mots_lastpipe = 0; /* pour calculer longueur pipes */

    if (nb_pipe_total > 0) /* qu'il y ait | ou | | */
        div_tab(nb_total_mots, mot, mots_pipe, mes_pipes, nb_mots_lastpipe); 
        /* on remplit les sous tableau de chaque commande pour les pipes */
    
    if (nb_pipe_total ==1){ /* s'il y a un |*/
        int v = un_pipe(mes_pipes, pathname, pathname_size, dirs, mots_pipe, attend_enfant, ilya_pipe, nb_mots_lastpipe);
        if (v == 0 || v == -1)
            return 0;
    }

    if (nb_pipe_total==2){ /* s'il y a deux | | */
        int v = deux_pipes(mes_pipes, pathname, pathname_size, dirs, mots_pipe, attend_enfant, ilya_pipe, nb_mots_lastpipe);
        if (v == 0 || v == -1)
            return 0;
    }
    return 0;
} 


int div_tab(int nb_total_mots, char* mot[], char* mots_pipe[][MaxMot], int mes_pipes[][2], int* nb_mots_lastpipe){
    
    /* permet de remplir les sous-tableaux [lignes] correspondant aux commandes des pipes */

    int ligne = 0; /* on commence au 1er sous-tableau */
    int mot_actuel=0; /* on commence au premier mot */
    for (int i = 0 ; i < nb_total_mots ; i++){
    
        if (strcmp(mot[i], "|") == 0){ /* si le caractère est un | */
            mots_pipe[ligne][mot_actuel] = NULL ; /* on le remplace par NULL */
            mot_actuel = 0; /* on recommencera à compter à 0 dans le prochain sous-tableau */
            (*nb_mots_lastpipe)=0; /* init nb laspipe */
            ligne++; /* changemt de sous-tableau car rencontré un pipe */
            }

        else { /* si le caractère est pas un | */ 
            mots_pipe[ligne][mot_actuel] = mot[i]; /* j'écris le mot dans le tableau */
            mot_actuel++; /* je parcours les mots */
            (*nb_mots_lastpipe)++; /* on incrémente */
        }
    }
    mots_pipe[ligne][mot_actuel] = NULL; /* on termine le tableau avec NULL */
    
    return 0;
} 

int un_pipe(int mes_pipes[][2],char pathname[], int pathname_size, char* dirs[], char* mots_pipe[][MaxMot], bool attend_enfant, bool* ilya_pipe, int* nb_mots_lastpipe){

    int tmp, tmp2;

    if (pipe(mes_pipes[0]) < 0){ /* vérification erreur pipe */
        perror("pipe");
        return -1; }

    /* PROCESSUS PARENT: FORK1 */
    if ((tmp= fork())<0){ 
            perror("fork") ;
            return -1; }

        /* PROCESSUS ENFANT 1 */ 
    if (tmp ==0) {
        dup2(mes_pipes[0][1], 1); /* redirection sortie vers pipe */
        close(mes_pipes[0][0]); /* fermeture pipe */
        close(mes_pipes[0][1]);

        int nb_elt_tab1 = nb_mots(mots_pipe, 0); /* compte les mots du pipe */
        
        redirection_dans_pipe(mots_pipe, 0);

        if (strcmp(mots_pipe[0][0], "monman") == 0){  /*si monman*/
            monman(mots_pipe[0]);
            exit(0); /* sinon on reste bloqué on attend */
        }   

        if (strcmp(mots_pipe[0][0], "moncd") == 0){ /* si moncd*/
            appel_mon_cd(dirs, mots_pipe[0], &nb_elt_tab1);
            exit(0); /* sinon on reste bloqué on attend */
        } 

        if (strcmp(mots_pipe[0][0], "monexit") == 0){ /* si monexit*/
            mon_exit_pipe(mots_pipe[0]);
            exit(0); /* sinon on reste bloqué on attend */
        } 

        else{ 
            for(int i = 0; dirs[i] != 0; i++){
                snprintf(pathname, pathname_size, "%s/%s", dirs[i], mots_pipe[0][0]);
                execv(pathname, mots_pipe[0]);
            }

            fprintf(stderr, "%s:notfound enfant 1\n",mots_pipe[0][0]);
            exit(1) ; 
        }
    }

    /* PROCESSUS PARENT > FORK 2 */ 
    if ((tmp2=fork()) <0){ 
        perror("fork") ;
        return -1; 
}

    /* PROCESSUS ENFANT 2 */
    if (tmp2 ==0) {
        dup2(mes_pipes[0][0], 0); /* redirection entrée depuis pipe */
        close(mes_pipes[0][0]); /* fermeture pipe */
        close(mes_pipes[0][1]);
        
        redirection_dans_pipe(mots_pipe, 1);

        if (strcmp(mots_pipe[1][0], "monman") == 0){ /* si monman*/
            monman(mots_pipe[1]);
            exit(0); /* sinon on reste bloqué on attend */
        }   

        if (strcmp(mots_pipe[1][0], "moncd") == 0){ /* si moncd*/
            appel_mon_cd(dirs, mots_pipe[1], nb_mots_lastpipe);
            exit(0); /* sinon on reste bloqué on attend */
        }   

        if (strcmp(mots_pipe[1][0], "monexit") == 0){ /* si monexit*/
            mon_exit_pipe(mots_pipe[1]);
            exit(0); /* sinon on reste bloqué on attend */
        }   

        else{ 
            for(int i = 0; dirs[i] != 0; i++){
                snprintf(pathname, pathname_size, "%s/%s", dirs[i], mots_pipe[1][0]);
                execv(pathname, mots_pipe[1]);
            }

            fprintf(stderr, "%s:notfound enfant 2\n",mots_pipe[1][0]);
            exit(1) ; 
        }
    }

    /* PROCESSUS PARENT FIN */

    close(mes_pipes[0][0]); /* fermeture pipe */
    close(mes_pipes[0][1]);

    if (attend_enfant == true){ 
        /* on attend tous les enfants */
        while (wait(NULL) > 0)
            ;
    }
    
    *ilya_pipe = false; 
    return 0;
} 




int deux_pipes(int mes_pipes[][2], char pathname[], int pathname_size, char* dirs[], char* mots_pipe[][MaxMot], bool attend_enfant, bool* ilya_pipe, int* nb_mots_lastpipe){

    int tmp, tmp2, tmp3;
    
    /* vérification réussite pipe */
    if (pipe(mes_pipes[0]) < 0 || pipe(mes_pipes[1]) < 0){ 
        perror("pipe");
        return -1; }

    /* PROCESSUS PARENT > FORK 1 */
    if ((tmp = fork()) <0){
        perror("fork") ; /* vérification réussite fork1 */
        return -1; }

    /* PROCESSUS ENFANT 1 */
    if (tmp ==0) {
        dup2(mes_pipes[0][1], 1); /* redirection sortie */
        close(mes_pipes[0][0]); /* fermeture de tous les pipes */
        close(mes_pipes[0][1]);
        close(mes_pipes[1][0]);
        close(mes_pipes[1][1]);

        int nb_elt_tab1 = nb_mots(mots_pipe, 0);

        redirection_dans_pipe(mots_pipe, 0); 

        if (strcmp(mots_pipe[0][0], "monman") == 0){ /* si monman*/
            monman(mots_pipe[0]);
            exit(0); /* sinon on reste bloqué on attend */
        }   

        if (strcmp(mots_pipe[0][0], "moncd") == 0){ /* si moncd*/
            appel_mon_cd(dirs, mots_pipe[0], &nb_elt_tab1);
            exit(0); /* sinon on reste bloqué on attend */
        } 

        if (strcmp(mots_pipe[0][0], "monexit") == 0){ /* si monexit*/
            mon_exit_pipe(mots_pipe[0]);
            exit(0); /* sinon on reste bloqué on attend */
        } 


        else{
            for(int i = 0; dirs[i] != 0; i++){
                snprintf(pathname, pathname_size, "%s/%s", dirs[i], mots_pipe[0][0]);
                execv(pathname, mots_pipe[0]);
            }

            fprintf(stderr, "%s:notfound enfant 1\n",mots_pipe[0][0]);
            exit(1) ; 
        }
    }

    /* PROCESSUS PARENT > FORK 2 */
    if ((tmp2 = fork()) <0){
        perror("fork") ;
        return -1; }

    /* PROCESSUS ENFANT 2 */
    if (tmp2 ==0) {
        dup2(mes_pipes[0][0], 0); /* redirection entrée depuis pipe1 */
        dup2(mes_pipes[1][1], 1); /* redirection sortie vers pipe2 */
        close(mes_pipes[0][0]);
        close(mes_pipes[0][1]);
        close(mes_pipes[1][0]);
        close(mes_pipes[1][1]);

        int nb_elt_tab2 = nb_mots(mots_pipe, 1); /* nb mots pipe 2 */
        redirection_dans_pipe(mots_pipe, 1);

        if (strcmp(mots_pipe[1][0], "monman") == 0){ /* si monman*/
            monman(mots_pipe[1]);
            exit(0); /* sinon on reste bloqué on attend */
        }   

        if (strcmp(mots_pipe[1][0], "moncd") == 0){ /* si moncd*/
            appel_mon_cd(dirs, mots_pipe[1], &nb_elt_tab2);
            exit(0); /* sinon on reste bloqué on attend */
        }   

        if (strcmp(mots_pipe[1][0], "monexit") == 0){ /* si monexit*/
            mon_exit_pipe(mots_pipe[1]);
            exit(0); /* sinon on reste bloqué on attend */
        }   

        else{
            for(int i = 0; dirs[i] != 0; i++){
                snprintf(pathname, pathname_size, "%s/%s", dirs[i], mots_pipe[1][0]);
                execv(pathname, mots_pipe[1]);
            }

            fprintf(stderr, "%s:notfound enfant 2\n",mots_pipe[1][0]);
            exit(1) ; 
        }
    }

    /* PROCESSUS PARENT > FORK3*/
    close(mes_pipes[0][0]); /* fermeture compète pipe1 */
    close(mes_pipes[0][1]);

    if ((tmp3= fork()) <0){
        perror("fork3") ;
        return -1; 
    }

    /* PROCESSUS ENFANT 3 */
    if (tmp3 == 0) {
        dup2(mes_pipes[1][0], 0); /* redirection entrée depuis pipe2 */
        close(mes_pipes[1][0]); /* fermeture pipe2 */
        close(mes_pipes[1][1]);

        redirection_dans_pipe(mots_pipe, 2);

        if (strcmp(mots_pipe[2][0], "monman") == 0){ /* si monman*/
            monman(mots_pipe[2]);
            exit(0); /* sinon on reste bloqué on attend */
        }   

        if (strcmp(mots_pipe[2][0], "moncd") == 0){ /* si moncd */
            appel_mon_cd(dirs, mots_pipe[2], nb_mots_lastpipe);
            exit(0); /* sinon on reste bloqué on attend */
        }  

        if (strcmp(mots_pipe[2][0], "monexit") == 0){ /* si monexit */
            mon_exit_pipe(mots_pipe[2]);
            exit(0); /* sinon on reste bloqué on attend */
        }  

        else{
            for(int i = 0; dirs[i] != 0; i++){
                snprintf(pathname, pathname_size, "%s/%s", dirs[i], mots_pipe[2][0]);
                execv(pathname, mots_pipe[2]);
            }

            fprintf(stderr, "%s:notfound enfant 3\n",mots_pipe[2][0]);
            exit(1) ; 
        }
    }

    /* PROCESSUS PARENT FIN*/
    close(mes_pipes[1][0]); /* fermeture pipe2 */
    close(mes_pipes[1][1]);

    if (attend_enfant == true){ 
            while (wait(NULL) > 0) /* on attend tous les processus enfant */
                ; /* se terminent */
        }

    *ilya_pipe = false; /* on marque bool "pipe" à faux */
    return 0; 

}



int nb_mots(char* mots_pipe[][MaxMot], int ligne){
    int nb_elmt = 0;
    for (int i = 0; i < MaxMot; i++){ /* parcourt la ligne  */
        if (mots_pipe[ligne][i] != NULL) /* si non nul */
            nb_elmt++; /* incrémente nb_elemt */
    }
    return nb_elmt; /* renvoie nb élémt */
}