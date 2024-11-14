#include "tree.h"
#include <stdlib.h>
#include <time.h>
#include "map.h"
#include "moves.h"

// Fonction pour générer des mouvements possibles aléatoires
t_move* random_possibilities() {
    srand(time(NULL));  // Initialisation du générateur de nombres aléatoires
    t_move* possibilities = (t_move*) malloc(NB_possibilities * sizeof(t_move));  // Allocation mémoire pour les mouvements possibles

    // Génère aléatoirement un mouvement parmi les 7 possibles
    for (int i = 0; i < NB_possibilities; i++) {
        switch (1 + rand() % 7) {  // Génère un nombre entre 1 et 7 pour choisir un mouvement
            case 1: possibilities[i] = F_10; break;  // Mouvement de 10 unités en avant
            case 2: possibilities[i] = F_20; break;  // Mouvement de 20 unités en avant
            case 3: possibilities[i] = F_30; break;  // Mouvement de 30 unités en avant
            case 4: possibilities[i] = B_10; break;  // Mouvement de 10 unités en arrière
            case 5: possibilities[i] = T_LEFT; break;  // Rotation à gauche
            case 6: possibilities[i] = T_RIGHT; break;  // Rotation à droite
            case 7: possibilities[i] = U_TURN; break;  // Rotation à 180 degrés
        }
    }
    return possibilities;  // Retourne le tableau de mouvements possibles
}

// Fonction pour supprimer un mouvement spécifique de la liste des possibilités
t_move* remove_possibility(t_move* possibilities, int len, int idx) {
    t_move* new_possibilities = (t_move*) malloc((len - 1) * sizeof(t_move));  // Nouvelle liste avec une case en moins
    int j = 0;

    // Parcourt toutes les possibilités et copie celles qui ne sont pas supprimées
    for (int i = 0; i < len - 1; i++) {
        if (i == idx) j++;  // Sauter l'élément à l'index idx
        new_possibilities[i] = possibilities[j];  // Copie le mouvement
        j++;
    }
    return new_possibilities;  // Retourne la nouvelle liste sans le mouvement supprimé
}

// Fonction pour créer un nœud avec les informations de localisation et de carte
t_node* createNode(int nb_sons, int depth, t_localisation loc, t_map map) {
    t_node* new_node = (t_node*)malloc(sizeof(t_node));  // Allocation mémoire pour un nœud
    new_node->value = map.costs[loc.pos.y][loc.pos.x];   // Coût de la case actuelle
    new_node->depth = depth;  // Profondeur du nœud dans l'arbre
    new_node->soil_type = map.soils[loc.pos.y][loc.pos.x];  // Type de sol à cette position
    new_node->nbSons = nb_sons;  // Nombre d'enfants du nœud
    new_node->sons = (t_node**)malloc(nb_sons * sizeof(t_node*));  // Allocation mémoire pour les enfants
    for (int i = 0; i < nb_sons; i++) {
        new_node->sons[i] = NULL;
    }

    return new_node;  // Retourne le nouveau nœud
}

// Fonction récursive pour créer l'arbre des nœuds à partir des mouvements possibles
t_node* create_all_Node(int nb_possibilities, int depth, t_move* possibilities, t_localisation robot, t_map map) {
    if (depth > NB_choices) return NULL;  // Condition d'arrêt : si la profondeur dépasse le nombre maximum de choix

    if (depth == NB_choices || map.costs[robot.pos.y][robot.pos.x] >= 10000)
        nb_possibilities = 0;  // Si on a atteint la profondeur ou une case invalide, plus de mouvements possibles

    t_node* node = createNode(nb_possibilities, depth, robot, map);  // Création d'un nœud avec les paramètres donnés

    // Parcours de tous les mouvements possibles
    for (int i = 0; i < nb_possibilities; i++) {
        t_localisation new_loc = robot;  // Copie de la localisation actuelle
        updateLocalisation(&new_loc, possibilities[i]);  // Mise à jour de la localisation en fonction du mouvement

        // Si la nouvelle localisation est valide, on crée un enfant dans l'arbre
        if (isValidLocalisation(new_loc.pos, map.x_max, map.y_max)) {
            t_move* new_possibilities = remove_possibility(possibilities, nb_possibilities, i);  // Suppression du mouvement utilisé
            node->sons[i] = create_all_Node(nb_possibilities - 1, depth + 1, new_possibilities, new_loc, map);  // Appel récursif pour les enfants
            free(new_possibilities);  // Libère la mémoire allouée pour la nouvelle liste de possibilités
        }
        else {
            node->sons[i] = NULL;  // Si la localisation est invalide, pas d'enfant à cet index
        }
    }

    return node;  // Retourne le nœud créé
}

// Fonction pour rechercher la valeur minimale parmi les nœuds d'un arbre
int search_min(t_node* node) {
    int min = node->value;  // La valeur minimale commence par celle du nœud actuel

    // Si le nœud a des fils, on cherche la valeur minimale parmi eux
    if (node->nbSons != 0) {
        for (int i = 0; i < node->nbSons; i++) {
            if (node->sons[i] != NULL) {
                int min_son = search_min(node->sons[i]);  // Recherche récursive dans les sous-arbres
                if (min_son < min) {
                    min = min_son;  // Mise à jour de la valeur minimale
                }
            }
        }
    }
    return min;  // Retourne la valeur minimale trouvée
}

// Fonction pour trouver le chemin menant à la valeur minimale dans l'arbre
int path_min(t_node* node, int* path, int* path_length) {
    if (node->nbSons == 0) {  // Si le nœud est une feuille (pas d'enfants)
        path[0] = node->value;  // Ajoute la valeur du nœud au chemin
        (*path_length)++;  // Incrémente la longueur du chemin
        return node->value;  // Retourne la valeur de la feuille
    }

    int min = node->value;  // Initialisation de la valeur minimale avec celle du nœud actuel

    // Recherche dans tous les fils
    for (int i = 0; i < node->nbSons; i++) {
        if (node->sons[i] != NULL) {
            int temp_length = 0;  // Variable pour la longueur du chemin temporaire
            int* temp_path = (int*)malloc(node->depth * sizeof(int));  // Tableau pour stocker le chemin temporaire
            int min_son = path_min(node->sons[i], temp_path, &temp_length);  // Recherche récursive dans l'enfant

            // Si un fils a une valeur plus petite, met à jour le chemin
            if (min_son < min) {
                min = min_son;  // Mise à jour de la valeur minimale
                path[0] = node->value;  // Ajoute la valeur du nœud actuel au chemin
                for (int j = 0; j < temp_length; j++) {
                    path[j + 1] = temp_path[j];  // Ajoute le chemin du sous-arbre au chemin total
                }
                *path_length = temp_length + 1;  // Met à jour la longueur du chemin
            }

            free(temp_path);  // Libère la mémoire allouée pour le chemin temporaire
        }
    }
    return min;  // Retourne la valeur minimale trouvée
}
// DEMANDER A CLEMENT !!!