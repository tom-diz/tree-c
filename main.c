#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "func.h"

int main() {
    // Initialisation des couleurs
    color_init();
    
    // Initialisation du seed (pour la fonction rand())
    srand(time(NULL));

    // Initialisation de la variable "trees", stockant les différents arbres
    trees* trees = trees_initialize();

    // Conversion du dictionnaire (format texte) en arbre
    dict_to_tree(trees);

    clear_console();

    menu_principal(trees);
}