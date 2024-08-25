/* *******************************************************
# Nom ......... : redir_in_pipe.c
# Rôle ........ :Fonction de gestion des redirections dans les pipes:
#                 >, >>, 2>, 2>>, >&, <
# Auteur ...... : Avrile Floro 
# Version ..... : V0.1 du 28/10/2023
# Licence ..... : réalisé dans le cadre du cours de SE
# Compilation : Compilé grâce au Makefile (make) avec les autres fichiers
#********************************************************/

#include "sys.h"
#include "f_head.h"

int redirection_dans_pipe(char* mots_pipe[][MaxMot], int ligne){

/* GESTION DES REDIRECTIONS: CRÉATION */

    mode_t mode = S_IRUSR | S_IWUSR | S_IXUSR | /* tout le monde */
                    S_IRGRP | S_IWGRP | S_IXGRP | /* peut lire, écrire */
                    S_IROTH | S_IWOTH | S_IXOTH ; /* et exécuter */

    for (int i = 0; mots_pipe[ligne][i] != NULL  ; i++) { /* on parcourt les mots */
        /* redirection simple > */

        if (simple_sortie_pipe (mots_pipe, ligne, mode, i) == 2)
            continue;

        /* redirection double >> */
        if (double_sortie_pipe (mots_pipe, ligne, mode, i) == 2)
            continue;

        /* redirection stderr simple sortie 2> */
        if (stderr_simple_sortie_pipe(mots_pipe, ligne, mode, i) == 2)
            continue;

        /* redirection stderr double sortie 2>> */
        if (stderr_double_sortie_pipe(mots_pipe, ligne, mode, i) == 2)
            continue;

        /* redirection stderr et stdout en sortie simple  >& */
        if (stderr_stdout_sortie_pipe(mots_pipe, ligne, mode, i) == 2)
            continue;

        /* redirection simple < */
        if (simple_entree_pipe (mots_pipe, ligne, mode, i) == 2)
            continue;
    }
    return 0; 
}
     

/* CRÉATION DES REDIRECTIONS */

int simple_sortie_pipe (char* mots_pipe[][MaxMot], int ligne, mode_t mode, int i){

    /* redirection simple en sortie > */
    if (strcmp(mots_pipe[ligne][i], ">") == 0){    
        char* sortie = mots_pipe[ligne][i+1]; /* le fichier de sortie après le > */

        int fd_redir_sortie = open(sortie, O_WRONLY | O_CREAT | O_TRUNC, mode) ;
            /* write only, créé le fichier ou le remet à 0 si existe */
        if (fd_redir_sortie < 0)
            perror("fd_redir_sortie");

        int t = dup2(fd_redir_sortie, 1); /* sortie redirigée vers l'arg du > */
        if (t < 0)
            perror("dup2 fd_redir_sortie");
        
        mots_pipe[ligne][i] = 0;  /* suppression du > */
        mots_pipe[ligne][i + 1] = 0;  /* suppression du nom de fichier */
        
        close(fd_redir_sortie); /* fermeture */
        return 2; /* signe continue dans gestion_redirection */
    }
return 0 ;
}


int double_sortie_pipe (char* mots_pipe[][MaxMot], int ligne, mode_t mode, int i){

    /* redirection simple en sortie > */
    if (strcmp(mots_pipe[ligne][i], ">>") == 0){    

        char* sortie = mots_pipe[ligne][i+1]; /* le fichier de sortie après le > */

        int fd_redir_sortie = open(sortie, O_WRONLY | O_CREAT | O_APPEND, mode) ;
            /* write only, créé le fichier ou ajoute à la suite si existe */
        if (fd_redir_sortie < 0)
            perror("fd_redir_sortie");

        int t = dup2(fd_redir_sortie, 1); /* sortie redirigée vers l'arg du > */
        if (t < 0)
            perror("dup2 fd_redir_sortie");
        
        mots_pipe[ligne][i] = 0;  /* suppression du >> */
        mots_pipe[ligne][i + 1] = 0;  /* suppression du nom de fichier */
        
        close(fd_redir_sortie); /* fermeture */
        return 2; /* signe continue dans gestion_redirection */
    }
return 0 ;
}


int stderr_simple_sortie_pipe (char* mots_pipe[][MaxMot], int ligne, mode_t mode, int i){

    /* redirection simple erreur en sortie 2> */
    if (strcmp(mots_pipe[ligne][i], "2>") == 0){
        char* sortie = mots_pipe[ligne][i+1]; /* le fichier de sortie après le 2> */

        int fd_stderr_s_redir = open(sortie, O_WRONLY | O_CREAT | O_TRUNC, mode) ;
            /* write only, créé le fichier ou le remet à 0 si existe */
        if (fd_stderr_s_redir < 0)
            perror("fd_stderr_s_redir");

        int t = dup2(fd_stderr_s_redir, 2); /* stderr redirigée vers l'arg du 2> */
        if (t < 0)
            perror("dup2 fd_stderr_sortie");

        mots_pipe[ligne][i] = 0;
        mots_pipe[ligne][i+1]=0;

        close(fd_stderr_s_redir);
        return 2; /* signe continue dans gestion_redirection */

    }  
    return 0;
}

int stderr_double_sortie_pipe (char* mots_pipe[][MaxMot], int ligne, mode_t mode, int i){

    /* redirection double erreur en sortie 2>> */
    if (strcmp(mots_pipe[ligne][i], "2>>") == 0){
        char* sortie = mots_pipe[ligne][i+1]; /* le fichier de sortie après le 2>> */

        int fd_stderr_d_redir = open(sortie, O_WRONLY | O_CREAT | O_APPEND, mode) ;
            /* write only, créé le fichier ou ajoute à la fin si existe */
        if (fd_stderr_d_redir < 0)
            perror("fd_stderr_d_redir");

        int t = dup2(fd_stderr_d_redir, 2); /* stderr redirigée vers l'arg du 2>> */
        if (t < 0)
            perror("dup2 fd_stderr_sortie");

        mots_pipe[ligne][i] = 0;
        mots_pipe[ligne][i+1]=0;

        close(fd_stderr_d_redir);
        return 2; /* signe continue dans gestion_redirection */
    }  
    return 0;
}

int stderr_stdout_sortie_pipe (char* mots_pipe[][MaxMot], int ligne, mode_t mode, int i){

    if (strcmp(mots_pipe[ligne][i], ">&") == 0){
        char* sortie = mots_pipe[ligne][i+1]; /* le fichier de sortie après le > */

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

        mots_pipe[ligne][i] = 0;  /* suppression du > */
        mots_pipe[ligne][i+1] = 0;  /* suppression du nom de fichier */

        close(fd_s_sortie_and); /* fermeture */
        return 2; /* signe continue dans gestion_redirection */
    }

    return 0;
}

int simple_entree_pipe (char* mots_pipe[][MaxMot], int ligne, mode_t mode, int i){

    /* redirection simple en entrée <  */
    if (strcmp(mots_pipe[ligne][i], "<") == 0){ 
        char* entree = mots_pipe[ligne][i+1]; /* le fichier entrée après le < */

        int fd_redir_s_entree = open(entree, O_RDONLY, mode) ;
            /* read only  */
        if (fd_redir_s_entree < 0)
            perror("fd_redir_s_entree");

        int t = dup2(fd_redir_s_entree, 0); /* entrée redirigée vers l'arg du < */
        if (t < 0)
            perror("dup2 fd_redir_s_entree");

        mots_pipe[ligne][i] = 0;  /* suppression du < */
        mots_pipe[ligne][i+1] = 0;  /* suppression du nom de fichier */

        close(fd_redir_s_entree); 
        return 2; /* signe continue dans gestion_redirection */

    }
    return 0;
}
