#include "func.h"

HANDLE hConsole;

// Fonctions permettant de gérer les couleurs
int color_init()
{
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}
int set_color(int k)
{
    SetConsoleTextAttribute(hConsole, k);
}
void color(int t,int f)
{
    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H,f*16+t);
}

// Fonction qui vérifie la saisie
int safe_type(int max)
{
    do {
        int type;
        printf("Saisie: ");
        if (scanf("%d", &type) == 1) {
            if ((1 <= type) && (type <= max)) return type;
            else printf("Le nombre n'est pas dans la borne autoris%ce ! (Entre 1 et %d)\n", 130, max);
        }
        else printf("La saisie n'est pas un nombre.\n");
        getchar();
    } while (1);
}

// Fonction qui alloue une liste de noeuds
node_list* create_node_list() {
    node_list* list = (node_list*)malloc(sizeof(node_list));
    list->n = (node*)malloc(0*sizeof(node));
    list->n_size = 0;
    return list;
}

// Fonction qui alloue un noeud dans la mémoire
node* create_node() {
    node* n = (node*)malloc(sizeof(node));
    n->is_word = (int*)malloc(sizeof(int));
    n->is_word = 0;
    n->list = create_node_list();
    return n;
}

// Fonction qui alloue un noeud avec un caractère
node* create_node_with_char(char value) {
    node* n = create_node();
    n->value = value;
    return n;
}

// Fonction qui ajoute un noeud à la liste de noeuds
void add_node_in_list(node_list* list, node* n) {
    list->n_size = list->n_size + 1;
    list->n = (node*)realloc(list->n, list->n_size*sizeof(node));
    list->n[list->n_size-1] = *n;
}

// Fonction qui ajoute un noeud comme enfant d'un autre noeud (au cas où)
void add_node_in_node(node* n, node* c) {
    add_node_in_list(n->list, c);
}

// Fonction qui initialise les arbres (noms, verbes, adjectifs, adverbes)
trees* trees_initialize() {
    trees* t = (trees*)malloc(sizeof(trees));
    t->noms = create_node_list();
    t->verbes = create_node_list();
    t->adjectifs = create_node_list();
    t->adverbes = create_node_list();
    return t;
}

// Fonction qui nettoie la console
void clear_console() {
    for (int i = 0; i < 50; i++) {
        printf("\n");
    }
}

// Fonction permettant de convertir le contenu du dictionnaire en arbre
int dict_to_tree(trees *t) {
    printf("G%cn%cration de l'arbre en cours...\n", 130, 130);
    
    FILE *fp;
    char line[150];
    size_t len = 0;

    fp = fopen("resources/dictionnaire_non_accentue.txt", "r");
    if (fp == NULL) { printf("Dictionnaire non trouv%c.", 130); return 0; }

    char flechie[50];
    char base[50];
    char type[4];
    char accord[50];

    // Lire le fichier
    while (fgets(line, sizeof(line), fp)) {

        // Sépare les trois parties de la ligne (flechie, base, type)
        sscanf(line, "%s\t%s\t%3[^:]:%s[^\n]", flechie, base, type, accord);


        // En fonction du type, ajouter le mot à l'arbre correspondant
        node_list* corresponding_tree;
        if (type[2] == 'j') { corresponding_tree = t->adjectifs; }
        else if (type[2] == 'v') { corresponding_tree = t->adverbes; }
        else if (type[2] == 'm') { corresponding_tree = t->noms; }
        else if (type[2] == 'r') { corresponding_tree = t->verbes; } 
        else continue;

        // 1. Décomposer le mot de base en caractères
        int len = strlen(base);
        node_list* current_node_list = corresponding_tree;
        // Décomposer le mot en caractères
        for (int i = 0; i < len; i++) {
            char c = base[i];
            int found = 0;

            node* current_node;

            for (int j = 0; j < current_node_list->n_size; j++) {
                if (current_node_list->n[j].value == c) {

                    // Verifier si c'est le dernier caractère
                    if (i == len - 1) {
                        int* a = (int*)malloc(sizeof(int));
                        *a = 1;
                        current_node_list->n[j].is_word = a;
                    }
                    current_node = &current_node_list->n[j];
                    current_node_list = current_node_list->n[j].list;
                    found = 1;
                    break;
                }
            }
            if (!found) {
                node* n = create_node_with_char(c);
                // Verifier si c'est le dernier caractère
                if (i == len - 1) {
                    int* a = (int*)malloc(sizeof(int));
                    *a = 1;
                    n->is_word = a;
                }
                add_node_in_list(current_node_list, n);
                current_node_list = n->list;
            }
        }
    }
    return 1;
}

// Fonction permettant la recherche d'un mot dans un arbre spécifique
node* recherche_in_tree(node_list* t, char* word) {
    node_list* current_node_list = t;

    // 1. Décomposer le mot de base en caractères
    int len = strlen(word);
    // Décomposer le mot en caractères
    for (int i = 0; i < len; i++) {
        char c = word[i];
        int found = 0;
        for (int j = 0; j < current_node_list->n_size; j++) {
            if (current_node_list->n[j].value == c) {
                found = 1;
                
                // Vérifier si c'est le dernier caractère
                if (i == len-1 && current_node_list->n[j].is_word != NULL) {
                    if (*current_node_list->n[j].is_word == 1) {
                        return &current_node_list->n[current_node_list->n_size-1];
                    }
                }
                current_node_list = current_node_list->n[j].list;
                break;
            }
        }
        if (!found) {
            break;
        }
    }
    return NULL;
}

// Fonction qui recherche un mot dans tous les arbres
node* recherche_in_trees(trees* t, char* word) {
    node_list* current_node_list;

    for (int tree_number = 0; tree_number < 4; tree_number++) {
        if (tree_number == 0) current_node_list = t->adjectifs;
        else if (tree_number == 1) current_node_list = t->adverbes;
        else if (tree_number == 2) current_node_list = t->noms;
        else if (tree_number == 3) current_node_list = t->verbes;

        node* n = recherche_in_tree(current_node_list, word);

        if (n != NULL) {
            return n;
        }
    }

    return NULL;
}

// Fonction prenant en charge la recherche d'un mot dans l'arbre
node* recherche_mot(trees* t){
    clear_console();
    printf("Entrez un mot %c rechercher: ", 133);
    char mot[50];
    scanf("%s", mot);

    node* found_node = recherche_in_trees(t, mot);

    if (found_node == NULL) {
        clear_console();
        printf("Le mot '%s' ", mot);
        set_color(RED); 
        printf("n'existe pas ");
        set_color(WHITE); 
        printf("dans le dictionnaire.\n\n");
        return NULL;
    }
    else {
        clear_console();
        printf("Le mot '%s' ", mot);
        set_color(GREEN); 
        printf("existe ");
        set_color(WHITE); 
        printf("dans le dictionnaire.\n\n");
        return found_node;
    }

    return found_node;
}

// Génère un mot aléatoire
node* generate_node(trees* t) {
    int word_found = 0;
    char* word = NULL;

    while (!word_found) {
        int tree_number = rand() % 4;
        node_list* current_node_list;
        if (tree_number == 0) current_node_list = t->adjectifs;
        else if (tree_number == 1) current_node_list = t->adverbes;
        else if (tree_number == 2) current_node_list = t->noms;
        else if (tree_number == 3) current_node_list = t->verbes;
        else {
            printf("Erreur: Type introuvable.\n");
            return NULL;
        }

        int len = rand() % 10 + 1;
        word = (char*)malloc(len*sizeof(char));
        for (int i = 0; i < len; i++) {
            int node_number = rand() % current_node_list->n_size;
            word[i] = current_node_list->n[node_number].value;
            current_node_list = current_node_list->n[node_number].list;
        }
        word[len] = '\0';

    }
}

// Fonction qui affiche un mot aléatoire
node* generateur_mot(trees* t){
    printf("Voici un mot g%cn%cr%ce al%catoirement: ", 130, 130, 130, 130);
    node* generated_node = generate_node(t);
    return generated_node;
}

// Générer un mot aleatoire depuis un arbre specifique en parametre
char* generate_random_word(node_list* t, char *t_type) {
    int word_found = 0;
    char* word = malloc(50*sizeof(char));;
    while (!word_found) {
        int len = rand() % 30;
        node_list* current_node_list = t;
        for (int i = 0; i < len; i++) {
            if (current_node_list->n_size == 0) {
                word[i] = '\0';
                break;
            }
            int node_number = rand() % current_node_list->n_size;
            word[i] = current_node_list->n[node_number].value;
            current_node_list = current_node_list->n[node_number].list;
        }
        word[len] = '\0';

        node* found_node = recherche_in_tree(t, word);
        if (found_node != NULL) word_found = 1;
    }

    // Dans le cas où le mot généré est un verbe, exception
    if ((strlen(word)) == 1 && (strcmp(t_type, "ver"))) return generate_random_word(t, t_type);
    return word;
}

// Génère une phrase aléatoire à partir de mots aléatoires (Nom + Nom2 + Verbe + Adjectif + Adverbe)
char* generate_random_sentence(trees* t) {
    char* sentence = (char*)malloc(150*sizeof(char));
    char* nom = (char*)malloc(50*sizeof(char));
    char* nom2 = (char*)malloc(50*sizeof(char));
    char* verbe = (char*)malloc(50*sizeof(char));
    char* adjectif = (char*)malloc(50*sizeof(char));
    char* adverbe = (char*)malloc(50*sizeof(char));

    // Sentence (phrase) = Nom + Nom2 + Verbe + Adjectif + Adverbe
    nom = generate_random_word(t->noms, "nom");
    adjectif = generate_random_word(t->adjectifs, "adj");
    verbe = generate_random_word(t->verbes, "ver");
    nom2 = generate_random_word(t->noms, "nom");
    sprintf(sentence, "%s %s %s %s", nom, adjectif, verbe, nom2);
    

    return sentence;

}


char* generate_random_sentence2(trees* t) {
    srand(time(NULL));
    char* sentence = (char*)malloc(150*sizeof(char));
    char* nom = (char*)malloc(50*sizeof(char));
    char* nom2 = (char*)malloc(50*sizeof(char));
    char* verbe = (char*)malloc(50*sizeof(char));
    char* verbe2 = (char*)malloc(50*sizeof(char));
    char* adjectif = (char*)malloc(50*sizeof(char));
    char* adverbe = (char*)malloc(50*sizeof(char));

    //sentence = nom+"qui"+verbe+verbe+nom+adjectif
    nom = generate_random_word(t->noms, "nom");
    adjectif = generate_random_word(t->adjectifs, "adj");
    verbe = generate_random_word(t->verbes, "ver");
    verbe2 = generate_random_word(t->verbes, "ver");
    nom2 = generate_random_word(t->noms, "nom");
    sprintf(sentence, "%s qui %s %s %s %s", nom, verbe, verbe2, nom2, adjectif);

    return sentence;

}

char* generate_random_sentence3(trees* t) {
    srand(time(NULL));
    char* sentence = (char*)malloc(150*sizeof(char));
    char* nom = (char*)malloc(50*sizeof(char));
    char* verbe = (char*)malloc(50*sizeof(char));
    char* verbe2 = (char*)malloc(50*sizeof(char));
    char* adverbe = (char*)malloc(50*sizeof(char));

    //sentence = nom+verbe+adverbe+verbe
    nom = generate_random_word(t->noms, "nom");
    verbe = generate_random_word(t->verbes, "ver");
    verbe2 = generate_random_word(t->verbes, "ver");
    adverbe = generate_random_word(t->adverbes, "adv");
    sprintf(sentence, "%s %s %s %s", nom, verbe, adverbe, verbe2);

    return sentence;

}

// Sous-menu du générateur de mot
int sous_menu_generateur_mot(trees* t) {
    clear_console();
    printf("G%cn%crateur de mot\n", 130, 130);
    set_color(BLUE);
    printf("1. ");
    set_color(WHITE);
    printf("G%cn%crer un mot al%catoire d'un type sp%ccifique\n", 130, 130, 130, 130);
    set_color(BLUE);
    printf("2. ");
    set_color(WHITE);
    printf("G%cn%crer un mot al%catoire d'un type al%catoire\n", 130, 130, 130, 130);
    set_color(BLUE);
    printf("3. ");
    set_color(WHITE);
    printf("Retour\n");
    int choix = safe_type(3);
    switch (choix) {
        case 1:
            clear_console();
            set_color(BLUE);
            printf("1. ");
            set_color(WHITE);
            printf("Adjectif\n");
            set_color(BLUE);
            printf("2. ");
            set_color(WHITE);
            printf("Adverbe\n");
            set_color(BLUE);
            printf("3. ");
            set_color(WHITE);
            printf("Nom\n");
            set_color(BLUE);
            printf("4. ");
            set_color(WHITE);
            printf("Verbe\n");
            
            int choix_type = safe_type(4);
            printf("\n\n");
            switch (choix_type) {
                case 1:
                    printf("Voici un adjectif g%cn%cr%c de mani%cre al%catoire: ", 130, 130, 130, 138, 130);
                    set_color(CYAN);
                    printf("%s", generate_random_word(t->adjectifs, "adj"));
                    break;
                case 2:
                    printf("Voici un adverbe g%cn%cr%c de mani%cre al%catoire: ", 130, 130, 130, 138, 130);
                    set_color(CYAN);
                    printf("%s", generate_random_word(t->adverbes, "adv"));
                    break;
                case 3:
                    printf("Voici un nom g%cn%cr%c de mani%cre al%catoire: ", 130, 130, 130, 138, 130);
                    set_color(CYAN);
                    printf("%s", generate_random_word(t->noms, "nom"));
                    break;
                case 4:
                    printf("Voici un verbe g%cn%cr%c de mani%cre al%catoire: ", 130, 130, 130, 138, 130);
                    set_color(CYAN);
                    printf("%s", generate_random_word(t->verbes, "ver"));
                    break;
                default:
                    printf("Choix invalide.\n\n");
                    break;
            }
            set_color(WHITE);
            printf("\n\n");
            break;
        case 2:
            printf("\n\n");
            printf("Voici un mot g%cn%cr%c de mani%cre al%catoire: ", 130, 130, 130, 138, 130);
            int random = rand() % 3 + 1;
            set_color(CYAN);
            switch (random) {
                case 1:
                    printf("%s", generate_random_word(t->adjectifs, "adj"));
                    break;
                case 2:
                    printf("%s", generate_random_word(t->adverbes, "adv"));
                    break;
                case 3:
                    printf("%s", generate_random_word(t->noms, "nom"));
                    break;
                case 4:
                    printf("%s", generate_random_word(t->verbes, "ver"));
                    break;
            }
            set_color(WHITE);
            printf("\n\n");
            break;
        case 3:
            clear_console();
            return 0;
        default:
            printf("Choix invalide.\n\n");
            break;
    }
    return 1;
}    

// Sous-menu du générateur de phrase
int sous_menu_generateur_phrase(trees* t) {
    clear_console();
    printf("G%cn%crateur de phrase\n", 130, 130);
    set_color(BLUE);
    printf("1. ");
    set_color(WHITE);
    printf("Forme : Nom - Adjectif - Verbe - Nom\n");
    set_color(BLUE);
    printf("2. ");
    set_color(WHITE);
    printf("Forme : Nom - qui - Verbe - Verbe - Nom - Adjectif\n");
    set_color(BLUE);
    printf("3. ");
    set_color(WHITE);
    printf("Forme : Nom - Verbe - Adverbe - Verbe\n");
    set_color(BLUE);
    printf("4. ");
    set_color(WHITE);
    printf("Retour\n");

    int choix = safe_type(4);
    switch (choix) {
        case 1:
            clear_console();
            printf("Voici une phrase g%cn%cr%ce al%catoirement: ", 130, 130, 130, 130);
            set_color(CYAN);
            printf("%s\n\n", generate_random_sentence(t));
            set_color(WHITE);
            break;
        case 2:
            clear_console();
            printf("Voici une phrase g%cn%cr%ce al%catoirement: ", 130, 130, 130, 130);
            set_color(CYAN);
            printf("%s\n\n", generate_random_sentence2(t));
            set_color(WHITE);
            break;
        case 3:
            clear_console();
            printf("Voici une phrase g%cn%cr%ce al%catoirement: ", 130, 130, 130, 130);
            set_color(CYAN);
            printf("%s\n\n", generate_random_sentence3(t));
            set_color(WHITE);
            break;
        case 4:
            clear_console();
            return 0;
        default:
            clear_console();
            break;
    }
    return 1;
}

// Fonction principale (menu principal)
int menu_principal(trees* t) {
    // Texte affiché au début du programme
    printf("Programme r%calis%c par : Tom Dizdarevic, Louis Vanlaeres, Thambir Nazrul Islam\n\n", 130, 130);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c PROGRAMME DE DICTIONNAIRE %c%c%c%c%c%c%c%c%c%c%c%c\n", 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196);
    set_color(BLUE);printf("1.");set_color(WHITE);printf(" Rechercher un mot\n");
    set_color(BLUE);printf("2.");set_color(WHITE);printf(" G%cn%crer un mot al%catoire\n", 130, 130, 130);
    set_color(BLUE);printf("3.");set_color(WHITE);printf(" G%cn%crer une phrase al%catoire\n", 130, 130, 130);
    set_color(BLUE);printf("4.");set_color(WHITE);printf(" Quitter\n");
    int choix = safe_type(4);
    switch (choix) {
        case 1:
            recherche_mot(t);
            menu_principal(t);
            break;
        case 2:
            sous_menu_generateur_mot(t);
            menu_principal(t);
            return 0;
        case 3:
            sous_menu_generateur_phrase(t);
            menu_principal(t);
            return 0;
        case 4:
            clear_console();
            printf("Merci d'avoir utilis%c le programme de dictionnaire. :)\n", 130);
            return 0;
        default:
            clear_console();
            return 0;
    }
    return 0;
}
