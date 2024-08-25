/* # Nom ......... : man.c
# Rôle ........ : Manuel du shell 
# Auteur ...... : Avrile Floro 
# Version ..... : V0.1 du 30/01/2024
# Licence ..... : réalisé dans le cadre du cours de SE
# Usage : Compilé via make */


#include "sys.h"
#include "f_head.h"

int monman(char* mot[]){
  if (strcmp(mot[0], "monman") == 0){ /* si commence par mon man */
      if (mot[1] == 0){ /* si pas d'argument */
          fprintf(stderr, "Quelle page du manuel voulez-vous?\nPar exemple, essayer 'monman monman'.\n");
      }

      else if (mot[2] != NULL){ /* si deux arguments */
          fprintf(stderr, "Usage: monman %s\n", mot[1]);
      }

      else if (strcmp(mot[1], "monman") == 0){ // man man
          printf("\nNOM\nmonman - une interface pour le manuel de référence du système\n\nDESCRIPTION\nmonman est le pagineur de manuel du système. Chaque argument donné à monman est le nom d'une commande créée pour ce shell élémentaire. La page de manuel associée à son argument est alors trouvée et affichée.\n\nEXEMPLES D'UTILISATION\nmonman monman\nmonman moncd\nmonman monexit\n\n");
      }

      else if (strcmp(mot[1], "monexit") == 0){ 
          printf("\nNOM\nmonexit - provoque la sortie du shell\n\nDESCRIPTION\nLa commande monexit permet au shell de sortir de son environnement d'exécution actuel. La commande monexit ne prend pas d'option.\n\nEXEMPLE D'UTILISATION\nmonexit\n\n");
      }

      else if (strcmp(mot[1], "moncd") == 0){ 
          printf("\nNOM\nmoncd - changer le répertoire de travail\n\nDESCRIPTION\nL'utilitaire moncd change le répertoire de travail de l'environnement d'exécution du shell actuel en exécutant les étapes suivantes dans l'ordre.\n      1. Si un répertoire est donné en argument à la commande `moncd`, alors le répertoire de travail du shell sera modifié pour correspondre au chemin spécifié en argument.\n      2. Si aucun répertoire n'est spécifié en argument, `moncd` se comportera comme si le chemin spécifié dans la variable d'environnement HOME était l'argument. Cela signifie que le répertoire de travail sera changé pour celui défini dans HOME, à condition que cette variable ne soit ni vide ni indéfinie. \n\nEXEMPLES D'UTILISATION\nmoncd\nmoncd /home/avrile/Desktop\n\n");
      }

      else{ /* si l'argument passé n'existe pas */
        fprintf(stderr, "Cette commande n'existe pas.\n");
      }
  }

  return 0;
}