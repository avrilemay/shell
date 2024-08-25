/* *******************************************************
# Nom ......... : cn-decouper.c
# Rôle ........ : découper ligne en token 
# Auteur ...... : tiré du cours
# Licence ..... : réalisé dans le cadre du cours de SE
# Compilation : compilé avec les autres programmes 
#********************************************************/

/* cn=decouper.c
Un wrapper autour de strtok
*/

# include <stdio.h> 
# include <string.h>

/* decouper == decouper une chaine en mots */

int decouper(char * ligne, char * separ, char * mot[], int maxmot) {
    int i;

    mot[0] = strtok(ligne, separ);
    for(i = 1; mot[i - 1] != 0; i++) {
        if (i == maxmot) {
            fprintf(stderr, "Erreur dans la fonction decouper: trop de mots\n");
            mot[i - 1] = 0;
            break;
        }
        mot[i] = strtok(NULL, separ);
    }
    return i-1; /* = nb total mots */
}

# ifdef TEST

int
main(int ac, char * av[]) {
    char *elem[10]; 
    int i;
    
    if (ac<3){
        fprintf(stderr, "usage:%sphrasesepar\n",av[0]); 
        return 1;
    }

    printf("Ondecoupe'%s'suivantles'%s' :\n", av[1], av[2]); 
    decouper(av[1],av[2], elem,10);
    
    for(i =0; elem[i] != 0; i++) 
        printf("%d:%s\n",i, elem[i]);
    
    return 0; 
    }

# endif