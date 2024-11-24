#include <stdio.h>
#include <stdlib.h>
#include "map.h"
#include "path.h"

int main() {

    t_map map1 = createMapFromFile("..\\maps\\example1.map");
    t_map map2 = createMapFromFile("..\\maps\\example2.map");
    t_map map3 = createMapFromFile("..\\maps\\example3.map");
    int val = 0;
    printf("You are going to choose a map : \nChoose between 1 2 3\n");
    scanf("%d",&val);
    if (val == 1) {
        printf("Map created with dimensions %d x %d\n", map1.y_max, map1.x_max);
        for (int i = 0; i < map1.y_max; i++) {
            for (int j = 0; j < map1.x_max; j++) {
                printf("%d ", map1.soils[i][j]);
            }
            printf("\n");
        }

        // printf the costs, aligned left 5 digits
        for (int i = 0; i < map1.y_max; i++) {
            for (int j = 0; j < map1.x_max; j++) {
                printf("%-5d ", map1.costs[i][j]);
            }
            printf("\n");
        }
        displayMap(map1);
        t_move* test;
        test = getRandomMoves(NB_possibilities);

        printf("Voici les mouvements possibles : [ ");
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
        tree = create_tree(NB_possibilities, 0, INITIAL_POS, test, loc, map1);
        int min_val = search_min(tree);
        printf("La valeur minimum sur laquelle MARC peut arriver = %d\n", min_val);

        p_node feuille_min = min_leaf(tree);    //La feuille de valeur minimale

        int nb_move;
        t_move* path = best_path(tree, &nb_move);

        printf("Le(s) mouvement(s) a effectuer pour arriver a la feuille de valeur minimale : [");
        for (int j = 0; j <= nb_move; j++)
        {
            printf("%s ", getMoveAsString(path[j]));
            if (j < nb_move)
            {
                printf("; ");
            }
        }
        printf("]\n");

    }
    else if (val == 2)
    {
        printf("Map created with dimensions %d x %d\n", map2.y_max, map2.x_max);
        for (int i = 0; i < map2.y_max; i++) {
            for (int j = 0; j < map2.x_max; j++) {
                printf("%d ", map2.soils[i][j]);
            }
            printf("\n");
        }

        // printf the costs, aligned left 5 digits
        for (int i = 0; i < map2.y_max; i++) {
            for (int j = 0; j < map2.x_max; j++) {
                printf("%-5d ", map2.costs[i][j]);
            }
            printf("\n");
        }

        displayMap(map2);
        t_move* test;
        test = getRandomMoves(NB_possibilities);

        printf("Voici les mouvements possibles : [ ");
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
        tree = create_tree(NB_possibilities, 0, INITIAL_POS, test, loc, map2);
        int min_val = search_min(tree);
        printf("La valeur minimum sur laquelle MARC peut arriver = %d\n", min_val);

        p_node feuille_min = min_leaf(tree);    //La feuille de valeur minimale

        int nb_move;
        t_move* path = best_path(tree, &nb_move);

        printf("Le(s) mouvement(s) a effectuer pour arriver a la feuille de valeur minimale : [");
        for (int j = 0; j <= nb_move; j++)
        {
            printf("%s ", getMoveAsString(path[j]));
            if (j < nb_move)
            {
                printf("; ");
            }
        }
        printf("]\n");

    }
    else if (val == 3)
    {
        printf("Map created with dimensions %d x %d\n", map3.y_max, map3.x_max);
        for (int i = 0; i < map3.y_max; i++) {
            for (int j = 0; j < map3.x_max; j++) {
                printf("%d ", map3.soils[i][j]);
            }
            printf("\n");
        }

        // printf the costs, aligned left 5 digits
        for (int i = 0; i < map3.y_max; i++) {
            for (int j = 0; j < map3.x_max; j++) {
                printf("%-5d ", map3.costs[i][j]);
            }
            printf("\n");
        }

        displayMap(map3);
        t_move* test;
        test = getRandomMoves(NB_possibilities);

        printf("Voici les mouvements possibles : [ ");
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
        tree = create_tree(NB_possibilities, 0, INITIAL_POS, test, loc, map3);
        int min_val = search_min(tree);
        printf("La valeur minimum sur laquelle MARC peut arriver = %d\n", min_val);

        p_node feuille_min = min_leaf(tree);    //La feuille de valeur minimale

        int nb_move;
        t_move* path = best_path(tree, &nb_move);

        printf("Le(s) mouvement(s) a effectuer pour arriver a la feuille de valeur minimale : [");
        for (int j = 0; j <= nb_move; j++)
        {
            printf("%s ", getMoveAsString(path[j]));
            if (j < nb_move)
            {
                printf("; ");
            }
        }
        printf("]\n");

    }
    return 0;
}