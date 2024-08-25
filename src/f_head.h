/* *******************************************************
# Nom ......... : f_head.h
# Rôle ........ : Les déclarations de fonctions
# Auteur ...... : Avrile Floro 
# Version ..... : V0.1 du 02/02/2024
# Licence ..... : réalisé dans le cadre du cours de SE
#********************************************************/

enum {
MaxLigne = 1024, // longueur max d'une ligne de commandes
MaxMot = MaxLigne / 2, // nbre max de mot dans la ligne
MaxDirs = 100, // nbre max de repertoire dans PATH MaxPathLength = 512, 
MaxPathLength = 512,// longueur max d'un nom de fichier
MaxPipes = 3, // nb de pipes maximum 
};


int decouper(char *, char *, char **, int);

/* & */
int arriere_plan(int* nb_total_mots, char* mot[], bool* attend_enfant);

/* MONCD */
int appel_mon_cd(char *dir[], char *mot[], int* nb_total_mots);
int mon_cd(char *dir[]);

/* MONEXIT */
int mon_exit(char *mot[]);

int redirection(char** mot, int* nb_total_mots, bool* redir_sortie, bool* stderr_sortie, bool* redir_s_entree, int* old_entree, int* old_sortie, int* old_erreur, bool* redir, bool* reset);

int gestion_pipes (int nb_pipe_total, int nb_total_mots, char* mot[], char* mots_pipe[][MaxMot], int mes_pipes[][2], char* dirs[], char pathname[], int pathname_size, bool attend_enfant, bool* ilya_pipe);

int cb_pipe(char* mot[], int nb_total_mots, bool* ilya_pipe);

int monman(char* mot[]);

int div_tab(int nb_total_mots, char* mot[], char* mots_pipe[][MaxMot], int mes_pipes[][2], int* nb_mots_lastpipe);

int un_pipe(int mes_pipes[][2],char pathname[], int pathname_size, char* dirs[], char* mots_pipe[][MaxMot], bool attend_enfant, bool* ilya_pipe, int* nb_mots_lastpipe);

int deux_pipes(int mes_pipes[][2], char pathname[], int pathname_size, char* dirs[], char* mots_pipe[][MaxMot], bool attend_enfant, bool* ilya_pipe, int* nb_mots_lastpipe);

int redirection_dans_pipe(char* mots_pipe[][MaxMot], int ligne);

int mon_exit_pipe(char *mot[]);




/* CRÉATION REDIRECTIONS*/
    /* > */
int simple_sortie(char** mot, int* nb_total_mots, mode_t mode,  bool* redir_sortie, int i) ;

    /* >> */
int double_sortie (char** mot, int* nb_total_mots, bool* redir_sortie, mode_t mode, int i) ;

    /* 2> */
int stderr_simple_sortie (char** mot, int* nb_total_mots, bool* stderr_sortie, mode_t mode, int i);

    /* 2>> */
int stderr_double_sortie (char** mot, int* nb_total_mots, bool* stderr_sortie, mode_t mode, int i);

    /* >& */
int stderr_stdout_sortie (char** mot, int* nb_total_mots, bool* stderr_sortie, bool* redir_sortie, mode_t mode, int i);

    /* < */
int simple_entree(char** mot, int* nb_total_mots, bool* redir_s_entree, mode_t mode, int i) ;


/* RÉINITIALISATION*/

    /* réinitialisation après > et >> */
int reset_sortie(bool* redir_sortie, int* old_sortie);

    /* réinitialisation après 2> et 2>> */
int reset_stderr(bool* stderr_sortie, int* old_stderr);

    /* réinitialisation après <*/
int reset_stdin(bool* redir_s_entree, int* old_entree);


/* CRÉATION REDIRECTIONS POUR LES PIPES */
    /* > */
int simple_sortie_pipe(char* mots_pipe[][MaxMot], int ligne, mode_t mode,  int i) ;

    /* >> */
int double_sortie_pipe (char* mots_pipe[][MaxMot], int ligne, mode_t mode, int i) ;

    /* 2> */
int stderr_simple_sortie_pipe (char* mots_pipe[][MaxMot], int ligne, mode_t mode, int i);

    /* 2>> */
int stderr_double_sortie_pipe (char* mots_pipe[][MaxMot], int ligne, mode_t mode, int i);

    /* >& */
int stderr_stdout_sortie_pipe (char* mots_pipe[][MaxMot], int ligne, mode_t mode, int i);

    /* < */
int simple_entree_pipe(char* mots_pipe[][MaxMot], int ligne, mode_t mode, int i) ;


/* MANUEL */
int monman(char* mot[]);

/* calcul nb mots dans pipes */
int nb_mots(char* mots_pipe[][512], int ligne);
