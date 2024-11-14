#include <stdio.h>
#include <stdlib.h>
#include "map.h"
#include "tree.h"

int main() {

    t_map map;

    // The following preprocessor directive checks if the code is being compiled on a Windows system.
    // If either _WIN32 or _WIN64 is defined, it means we are on a Windows platform.
    // On Windows, file paths use backslashes (\), hence we use the appropriate file path for Windows.
#if defined(_WIN32) || defined(_WIN64)
    map = createMapFromFile("..\\maps\\example1.map");
#else
    map = createMapFromFile("../maps/example1.map");
#endif

    printf("Map created with dimensions %d x %d\n", map.y_max, map.x_max);
    for (int i = 0; i < map.y_max; i++) {
        for (int j = 0; j < map.x_max; j++) {
            printf("%d ", map.soils[i][j]);
        }
        printf("\n");
    }

    // printf the costs, aligned left 5 digits
    for (int i = 0; i < map.y_max; i++) {
        for (int j = 0; j < map.x_max; j++) {
            printf("%-5d ", map.costs[i][j]);
        }
        printf("\n");
    }


    displayMap(map);

    // Génération des mouvements aléatoires possibles
    t_move* test;
    test = random_possibilities();  // Crée un tableau de mouvements aléatoires possibles

    // Affichage des mouvements possibles
    for (int j = 0; j < NB_possibilities; j++) {
        printf("%s   ", getMoveAsString(test[j]));
    }
    printf("\n");

    // Initialisation de la localisation du robot
    t_localisation loc = loc_init(2, 6, NORTH);  // Exemple : robot positionné à (2,6) avec orientation NORTH

    // Création de l'arbre des nœuds en fonction des mouvements possibles
    t_node* node = create_all_Node(NB_possibilities, 0, test, loc, map);

    // Recherche de la valeur min dans l'arbre

    // Calcul du chemin min
    int path[node->depth];  // Tableau pour stocker le chemin
    int path_length = 0;  // Variable pour la longueur du chemin
    int min_val = path_min(node, path, &path_length);  // Calcul du chemin avec la valeur min en utilisant l'arbre

    // Affichage de la valeur minimale et du chemin
    printf("Feuille de valeur minimale = %d\n", min_val);  // Affichage de la valeur minimale trouvée dans l'arbre
    for (int i = 0; i < path_length; i++) {
        printf("%d ", path[i]);  // Affichage de chaque valeur du chemin
        if (i != path_length - 1) {  // Si ce n'est pas le dernier élément
            printf("-> ");
        }
    }
    printf("\n");

    // Libération de la mémoire allouée
    free(node);  // Libération de la mémoire allouée pour les nœuds de l'arbre
    free(test);  // Libération de la mémoire allouée pour les mouvements possibles

    return 0;
}
