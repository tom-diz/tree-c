#ifndef DATA_H_
#define DATA_H_

// Définit la structure "node" qui représente un noeud de l'arbre (ou enfant)
typedef struct node {
    char value; // Stocke la valeur du noeud
    int* is_word; // Stocke si le noeud est un mot
    struct node_list* list; // Stocke l'arbre pointant vers les noeuds enfants
} node;

// Définit la structure "node_list" qui représente une liste de noeuds
typedef struct node_list {
    node* n; // Stocke le noeud
    int n_size; // Stocke la taille du tableau n
} node_list;

// Définit la liste des arbres, nous permettant d'y accéder facilement
typedef struct trees_s {
    node_list* noms;
    node_list* verbes;
    node_list* adjectifs;
    node_list* adverbes;
} trees; 


#endif