/* *******************************************************
# Nom ......... : redirection.c
# Rôle ........ :Fonction de gestion des indirections:
#                 >, >>, 2>, 2>>, >&, <
# Auteur ...... : Avrile Floro 
# Version ..... : V0.1 du 18/10/2023
# Licence ..... : réalisé dans le cadre du cours de SE
# Compilation : Compilé grâce au Makefile (make) avec les autres fichiers
#********************************************************/

#include "sys.h"
#include "f_head.h"

int redirection(char** mot, int* nb_total_mots, bool* redir_sortie, bool* stderr_sortie,bool* redir_s_entree, int* old_entree, int* old_sortie, int* old_erreur, bool* redir, bool* reset){

    /* CRÉATION DES REDIRECTIONS */

    if (*redir == true){

        mode_t mode = S_IRUSR | S_IWUSR | S_IXUSR | /* tout le monde */
                      S_IRGRP | S_IWGRP | S_IXGRP | /* peut lire, écrire */
                      S_IROTH | S_IWOTH | S_IXOTH ; /* et exécuter */

        for (int i = 0; i < *nb_total_mots -1  ; i++) { /* on parcourt les mots */
            /* redirection simple > */
            simple_sortie (mot, nb_total_mots, mode, redir_sortie, i);
            if (*redir_sortie)
                continue;

            /* redirection double >> */
            double_sortie (mot, nb_total_mots, redir_sortie, mode, i);
            if (*redir_sortie)
                continue;

            /* redirection stderr simple sortie 2> */
            stderr_simple_sortie(mot, nb_total_mots, stderr_sortie, mode, i);
            if (*stderr_sortie)
                continue;

            /* redirection stderr double sortie 2>> */
            stderr_double_sortie(mot, nb_total_mots, stderr_sortie, mode, i);
            if (*stderr_sortie)
                continue;

            /* redirection stderr et stdout en sortie simple  >& */
            stderr_stdout_sortie(mot, nb_total_mots, stderr_sortie, redir_sortie, mode, i);
            if ((*stderr_sortie) && (*redir_sortie))
                continue;

            /* redirection simple < */
            simple_entree (mot, nb_total_mots, redir_s_entree, mode, i);
            if (*redir_s_entree)
                continue;
        }
        *redir = false;
    }

    /* QUAND ON VEUT RÉINITIALISER LES ENTRÉES ET SORTIES */
    else if (*reset == true) {

        /* reset > et >> et >& stdout */
        reset_sortie(redir_sortie, old_sortie);

        /* reset 2> et 2>> et >&, stderr */
        reset_stderr(stderr_sortie, old_erreur);

        /* reset < , stdin */
        reset_stdin(redir_s_entree, old_entree); 
        
        *reset = false;}
    
    return 0; 

    }



/* CRÉATION DES REDIRECTIONS */

int simple_sortie (char** mot, int* nb_total_mots, mode_t mode, bool* redir_sortie, int i){

    /* redirection simple en sortie > */
    if (strcmp(mot[i], ">") == 0){    
        char* sortie = mot[i+1]; /* le fichier de sortie après le > */

        int fd_redir_sortie = open(sortie, O_WRONLY | O_CREAT | O_TRUNC, mode) ;
            /* write only, créé le fichier ou le remet à 0 si existe */
        if (fd_redir_sortie < 0)
            perror("fd_redir_sortie");

        int t = dup2(fd_redir_sortie, 1); /* sortie redirigée vers l'arg du > */
        if (t < 0)
            perror("dup2 fd_redir_sortie");
        
        mot[i] = 0;  /* suppression du > */
        (*nb_total_mots)-- ; /* mise à jour du nb de mots */
        mot[i + 1] = 0;  /* suppression du nom de fichier */
        (*nb_total_mots)--; /* mise à jour du nb de mots */
        
        close(fd_redir_sortie); /* fermeture */
        *redir_sortie = true ;
        /* passage à vrai du bool */
    }
return 0 ;
}


int double_sortie (char** mot, int* nb_total_mots, bool* redir_sortie, mode_t mode, int i){

    /* redirection simple en sortie > */
    if (strcmp(mot[i], ">>") == 0){    

        char* sortie = mot[i+1]; /* le fichier de sortie après le > */

        int fd_redir_sortie = open(sortie, O_WRONLY | O_CREAT | O_APPEND, mode) ;
            /* write only, créé le fichier ou ajoute à la suite si existe */
        if (fd_redir_sortie < 0)
            perror("fd_redir_sortie");

        int t = dup2(fd_redir_sortie, 1); /* sortie redirigée vers l'arg du > */
        if (t < 0)
            perror("dup2 fd_redir_sortie");
        
        mot[i] = 0;  /* suppression du >> */
        (*nb_total_mots)--; /* mise à jour du nb de mots */
        mot[i + 1] = 0;  /* suppression du nom de fichier */
        (*nb_total_mots)--; /* mise à jour du nb de mots */
        
        close(fd_redir_sortie); /* fermeture */
        *redir_sortie = true ; /* passage à vrai du bool */
    }
return 0 ;
}


int stderr_simple_sortie (char** mot, int* nb_total_mots, bool* stderr_sortie, mode_t mode, int i){

    /* redirection simple erreur en sortie 2> */
    if (strcmp(mot[i], "2>") == 0){
        char* sortie = mot[i+1]; /* le fichier de sortie après le 2> */

        int fd_stderr_s_redir = open(sortie, O_WRONLY | O_CREAT | O_TRUNC, mode) ;
            /* write only, créé le fichier ou le remet à 0 si existe */
        if (fd_stderr_s_redir < 0)
            perror("fd_stderr_s_redir");

        int t = dup2(fd_stderr_s_redir, 2); /* stderr redirigée vers l'arg du 2> */
        if (t < 0)
            perror("dup2 fd_stderr_sortie");

        mot[i] = 0;
        (*nb_total_mots)--;
        mot[i+1]=0;
        (*nb_total_mots)--;

        close(fd_stderr_s_redir);
        *stderr_sortie = true ;

    }  
    return 0;
}

int stderr_double_sortie (char** mot, int* nb_total_mots, bool* stderr_sortie, mode_t mode, int i){

    /* redirection double erreur en sortie 2>> */
    if (strcmp(mot[i], "2>>") == 0){
        char* sortie = mot[i+1]; /* le fichier de sortie après le 2>> */

        int fd_stderr_d_redir = open(sortie, O_WRONLY | O_CREAT | O_APPEND, mode) ;
            /* write only, créé le fichier ou ajoute à la fin si existe */
        if (fd_stderr_d_redir < 0)
            perror("fd_stderr_d_redir");

        int t = dup2(fd_stderr_d_redir, 2); /* stderr redirigée vers l'arg du 2>> */
        if (t < 0)
            perror("dup2 fd_stderr_sortie");

        mot[i] = 0;
        (*nb_total_mots)--;
        mot[i+1]=0;
        (*nb_total_mots)--;

        close(fd_stderr_d_redir);
        *stderr_sortie = true ;
    }  
    return 0;
}

int stderr_stdout_sortie (char** mot, int* nb_total_mots, bool* stderr_sortie, bool* redir_sortie, mode_t mode, int i){

    if (strcmp(mot[i], ">&") == 0){
        char* sortie = mot[i+1]; /* le fichier de sortie après le > */

        int fd_s_sortie_and = open(sortie, O_WRONLY | O_CREAT | O_TRUNC, mode) ;
            /* write only, créé le fichier ou le remet à 0 si existe */
        if (fd_s_sortie_and < 0)
            perror("fd_s_sortie_and");

        int t = dup2(fd_s_sortie_and, 1); /* redir stout vers le fichier */
        if (t < 0)
            perror("dup2 stout fd_s_sortie_and");

        int tt = dup2(fd_s_sortie_and, 2); /* redir stderr vers le fichier */
        if (tt < 0)
            perror("dup2 stderr fd_s_sortie_and");

        mot[i] = 0;  /* suppression du > */
        (*nb_total_mots)--; /* mise à jour du nb de mots */
        mot[i + 1] = 0;  /* suppression du nom de fichier */
        (*nb_total_mots)--; /* mise à jour du nb de mots */

        close(fd_s_sortie_and); /* fermeture */
        *stderr_sortie = true ; /* passage à vrai du bool */
        *redir_sortie = true ; /* passage à vrai du bool */
    }

    return 0;
}

int simple_entree (char** mot, int* nb_total_mots, bool* redir_s_entree, mode_t mode, int i){

    /* redirection simple en entrée <  */
    if (strcmp(mot[i], "<") == 0){ 
        char* entree = mot[i+1]; /* le fichier entrée après le < */

        int fd_redir_s_entree = open(entree, O_RDONLY, mode) ;
            /* read only  */
        if (fd_redir_s_entree < 0)
            perror("fd_redir_s_entree");

        int t = dup2(fd_redir_s_entree, 0); /* entrée redirigée vers l'arg du < */
        if (t < 0)
            perror("dup2 fd_redir_s_entree");
        else {
        mot[i] = 0;  /* suppression du < */
        (*nb_total_mots)--; 
        mot[i + 1] = 0;  /* suppression du nom de fichier */
        (*nb_total_mots)--; 
        close(fd_redir_s_entree); 
        *redir_s_entree = true ; /* passage à vrai du bool */
        }
    }
    return 0;
}

/* RÉINITIALISATION DES VALEURS INITIALES */

int reset_sortie(bool* redir_sortie, int* old_sortie){
    /* si > ou >> ou >& (la sortie)*/
    if (*redir_sortie == true) { 
        int t = dup2(*old_sortie, 1); /* réinitialisation de stdout */
        if (t < 0)
            perror("dup old_sortie");
        else if (*redir_sortie)
            *redir_sortie = false ;
    }
    return 0;
}

int reset_stderr(bool* stderr_sortie, int* old_erreur){
    /* si 2> ou 2>> ou >& (stderr)*/
    if (*stderr_sortie == true) { 
        int t = dup2(*old_erreur, 2); /* réinitialisation de stderr */
        if (t < 0)
            perror("dup old_sortie");
        else if (*stderr_sortie)
            *stderr_sortie = false ;
    }
    return 0;
}


int reset_stdin(bool* redir_s_entree, int* old_entree){
    /* si < */
    if (*redir_s_entree == true){
        int t = dup2(*old_entree,0); /* réinit de strdin */
        if (t < 0)
            perror("dup old_entree");
        *redir_s_entree = false ; 
    }
    return 0;
}
