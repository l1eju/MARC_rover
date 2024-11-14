#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "map.h"
#include "moves.h"  // Inclure les définitions de mouvements et localisation

#define NB_possibilities 7  // Nombre de mouvements possibles à chaque étape (mis à jour à 7)
#define NB_choices 5        // Profondeur maximale de l'arbre (mise à jour à 5)

typedef struct s_node {
    int value;             // Coût du nœud (représente par exemple le coût d'une case dans la carte)
    int depth;             // Profondeur dans l'arbre (distance depuis le nœud racine)
    t_soil soil_type;      // Type de sol du nœud (par exemple, terrain, obstacles, etc.)
    struct s_node **sons;  // Tableau des enfants du nœud (les nœuds suivants qui représentent les mouvements possibles)
    int nbSons;            // Nombre d'enfants (peut être 0 si ce nœud est une feuille)
} t_node;  // Définition du type de données pour un nœud de l'arbre

// Déclarations des fonctions utilisées dans la gestion des arbres

// Fonction pour générer des mouvements possibles aléatoires
t_move* random_possibilities();

// Fonction pour supprimer un mouvement d'une liste de mouvements possibles
t_move* remove_possibility(t_move*, int, int);

// Fonction pour créer un nœud avec des informations comme le coût, la profondeur et le type de sol
t_node *createNode(int, int, t_localisation, t_map);

// Fonction récursive pour créer un arbre complet à partir de la racine (incluant les mouvements et les nœuds enfants)
t_node *create_all_Node(int, int, t_move*, t_localisation, t_map);

// Fonction pour rechercher la valeur minimale parmi les nœuds de l'arbre
int search_min(t_node *);

// Fonction pour trouver le chemin avec la valeur minimale dans l'arbre (recherche du meilleur chemin)
int path_min(t_node *, int *, int *);

#endif // TREE_H
