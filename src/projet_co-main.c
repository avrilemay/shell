/* *******************************************************
# Nom ......... : projet_co-main.c
# Rôle ........ : Main du shell simple du cours 
#               + gestion du "&" pour lancer un processus en arrière plan
#               + gestion de la commande "moncd"
#               + gestion de la commande "monexit"
#               + gestion des indirections: >, >>, 2>, 2>>, >&, <
#               + gestion des pipes |, | |
#               + manuel "monman" [monman monman, monman moncd, monman monexit]
#               + auto-complétion des noms de fichiers (appui sur tab)
# Auteur ...... : Avrile Floro 
# Version ..... : V0.1 du 30/01/2024
# Licence ..... : réalisé dans le cadre du cours de SE
# Compilation : gcc -Wall -g projet_co-main.c 
#               cn-decouper.c moncd.c redirection.c man.c -lreadline -o projet
#               un makefile a été préaré > make
# Usage : Pour exécuter : ./projet
#                        puis "monexit" pour quitter
#********************************************************/

#include "sys.h"
#include "f_head.h"


#define PROMPT "? "

int
main(int argc, char * argv[]) {
    char* ligne; /* pointeur vers ligne pour Readline */
    char pathname[MaxPathLength]; char * mot[MaxMot];
    char * dirs [MaxDirs];
    int i , tmp;
    int mon_cdrom, out; // pour moncd et monexit

    bool redir = false ; /* si on veut créer une redirection */
    bool reset = false ; /* si on veut réinitialiser les entrées-sorties */

    bool redir_sortie = false ; /* > et >> et >& */
    bool stderr_sortie = false ; /* 2> et 2 >> et >& */
    bool redir_s_entree = false ; /* < */

    int old_entree = dup(0); /* copie de stdin initiale */
    int old_sortie = dup(1); /* copie de stdout intiale */
    int old_erreur = dup(2); /* copie la sortie stderr initiale */

    int mes_pipes[MaxPipes][2]; /* tableaux pour les FD des pipes */

    bool ilya_pipe = false ;

    char * mots_pipe[MaxPipes][MaxMot]; /* sous-tab de mots pour c/ côté de pipe*/

    /* Decouper UNE COPIE de PATH en repertoires */ 
    decouper(strdup(getenv("PATH")), ":", dirs, MaxDirs);

    /* BOUCLE DU PROMPT */
    while((ligne = readline(PROMPT)) != NULL){

        bool attend_enfant = true; 
        /* réiniti pour c/ boucle: par défaut, on attend le processus enfant */

        int nb_total_mots = decouper(ligne, " \t\n", mot, MaxMot);
        /* on récupère le nb total de mot */

        /* ARRIÈRE-PLAN & */
        arriere_plan(&nb_total_mots, mot, &attend_enfant);

        int nb_pipe_total = cb_pipe(mot, nb_total_mots, &ilya_pipe); 
        /* renvoie nb total de pipes | + permet passage ilya_pipe à vrai */

        /* LES REDIRECTIONS */
        if (!ilya_pipe){
            redir = true ;
            redirection(mot, &nb_total_mots, &redir_sortie, &stderr_sortie, &redir_s_entree, &old_entree, &old_sortie, &old_erreur,&redir, &reset);
        }

        /* LIGNE VIDE */
        if (mot[0] == 0) 
            continue;

        /* MONCD */
        if ((!ilya_pipe)&&((mon_cdrom = appel_mon_cd(dirs, mot, &nb_total_mots)) == 0 || mon_cdrom == 1))
            continue;

        /* MON_EXIT */
        if ((!ilya_pipe)&&((out = mon_exit(mot)) == -1))
            perror("mon_exit");

        /* MONMAN */
        if ((!ilya_pipe)&&(strcmp(mot[0], "monman") == 0)){
            monman(mot);
        }

        /* LES PIPES */
        else if (ilya_pipe){
            gestion_pipes(nb_pipe_total, nb_total_mots, mot, mots_pipe, mes_pipes, dirs, pathname, sizeof(pathname), attend_enfant, &ilya_pipe) ;
        }


        /* FORK INITIAL SI PAS DE PIPE */
        else {

            tmp = fork(); /* lancement du processus enfant */

            if (tmp<0){ /* erreur tmp */
                perror("fork") ;
                continue; 
            }

            /* PROCESSUS PARENT  */
            else if (tmp > 0){ 
                /* si on attend le processus enfant */
                if (attend_enfant == true){ 
                    while (wait(NULL) > 0)
                        ; 
                }
                /* fin du processus enfant */

                /* RÉINITIALISATION STDIN, STDOUT, STDERR */
                reset = true;
                redirection(mot, &nb_total_mots, &redir_sortie, &stderr_sortie, &redir_s_entree, &old_entree, &old_sortie, &old_erreur,&redir, &reset);

                /* si on n'attend pas le processus enfant,
                ou que le processus enfant est déjà terminé on continue */
                continue; 
            }

            /* PROCESSUS ENFANT */
            else { /* tmp = 0 */

                for(i = 0; dirs[i] != 0; i++){
                snprintf(pathname, sizeof pathname, "%s/%s", dirs[i], mot[0]);
                execv(pathname, mot); /* exécution du programme */
                }
            }

            /* aucun exec n'a fonctionné */
            fprintf(stderr, "%s:notfound\n",mot[0]);
            exit(1) ; 
        }
    }
    
    printf ("Bye\n"); 
    return 0;

}
    

