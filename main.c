#include <stdio.h>
#include <stdlib.h>
#include "map.h"
#include "path.h"

int main() {

    t_map map = createMapFromFile("..\\maps\\example1.map");
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

    t_move* test;
    test = getRandomMoves(NB_possibilities);

    printf("Mouvements possibles : [ ");
    for (int j = 0; j < NB_possibilities; j++){
        printf("%s ", getMoveAsString(test[j]));
        if (j < NB_possibilities - 1)
        {
            printf("; ");
        }
    }
    printf("]\n");

    t_localisation loc = loc_init(4, 6, NORTH);

    t_tree tree;
    tree = create_tree(NB_possibilities, 0, INITIAL_POS, test, loc, map);
    int min_val = search_min(tree);
    printf("La valeur minimum sur laquelle MARC peut arriver = %d\n", min_val);

    p_node feuille_min = min_leaf(tree);    //La feuille de valeur minimale

    int nb_move;
    t_move* path = best_path(tree, &nb_move);

    printf("Mouvement a effectuer pour arriver a la feuille de valeur minimale : [");
    for (int j = 0; j <= nb_move; j++)
    {
        printf("%s ", getMoveAsString(path[j]));
        if (j < nb_move)
        {
            printf("; ");
        }
    }
    printf("]\n");

    return 0;
}