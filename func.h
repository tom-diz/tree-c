#ifndef FUNC_H_
#define FUNC_H_

#include "data.h"
#include <time.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Définition des couleurs
#define BLUE 1
#define CYAN 11
#define RED 12
#define GREEN 10
#define WHITE 15

// Fonctions permettant de gérer les couleurs
int color_init();
int set_color(int k);
void color(int t,int f);

// Fonction qui vérifie la saisie
int safe_type(int max);

// Fonction qui alloue une liste de noeuds
node_list* create_node_list();

// Fonction qui alloue un noeud dans la mémoire
node* create_node();

// Fonction qui alloue un noeud avec un caractère
node* create_node_with_char(char value);

// Fonction qui ajoute un noeud à la liste de noeuds
void add_node_in_list(node_list* list, node* n);

// Fonction qui ajoute un noeud comme enfant d'un autre noeud (au cas où)
void add_node_in_node(node* n, node* c);

// Fonction qui initialise les arbres (noms, verbes, adjectifs, adverbes)
trees* trees_initialize();

// Fonction qui nettoie la console
void clear_console();

// Fonction permettant de convertir le contenu du dictionnaire en arbre
int dict_to_tree(trees *t);

// Fonction permettant la recherche d'un mot dans un arbre spécifique
node* recherche_in_tree(node_list* t, char* word);

// Fonction qui recherche un mot dans tous les arbres
node* recherche_in_trees(trees* t, char* word);

// Fonction prenant en charge la recherche d'un mot dans l'arbre
node* recherche_mot(trees* t);

// Génère un mot aléatoire
node* generate_node(trees* t);

// Fonction qui affiche un mot aléatoire
node* generateur_mot(trees* t);

// Générer un mot aleatoire depuis un arbre specifique en parametre
char* generate_random_word(node_list* t, char *t_type);

// Génère une phrase aléatoire à partir de mots aléatoires (Nom + Nom2 + Verbe + Adjectif + Adverbe)
char* generate_random_sentence(trees* t);
char* generate_random_sentence2(trees* t);
char* generate_random_sentence3(trees* t);

// Sous-menu du générateur de mot
int sous_menu_generateur_mot(trees* t);

// Sous-menu du générateur de phrase
int sous_menu_generateur_phrase(trees* t);

// Fonction principale (menu principal)
int menu_principal(trees* t);

#endif