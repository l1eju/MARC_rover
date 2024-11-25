#include <stdio.h>
#include "map.h"
#include "path.h"

int main() {

    t_map map;
    int val = 0, r;
    printf("You are going to choose a map : \n");

    printf("Choose between 1 2 3 or press any other number to exit\n");
    scanf("%d",&val);

    if (val == 1) map = createMapFromFile("..\\maps\\example1.map");
    else if (val == 2) map = createMapFromFile("..\\maps\\example2.map");
    else if (val == 3) map = createMapFromFile("..\\maps\\example3.map");
    else return 0;

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

    t_move *moves;
    moves = getRandomMoves(NB_possibilities);
    printf("Please enter MARC inital position \n");
    int x, y;
    do {
        do {
            printf("MARC x position (between 0 and %d)\n", map.x_max - 1);
            r = scanf("%d", &x);

            if (r != 1) {
                while (getchar() != '\n'); // Pour éviter un cycle infini si l'entrée est invalide
            }
        } while (x < 0 || x > map.x_max - 1 || r != 1);
        do {
            printf("MARC y position (between 0 and %d)\n", map.y_max - 1);
            r = scanf("%d", &y);

            if (r != 1) {
                while (getchar() != '\n'); // Pour éviter un cycle infini si l'entrée est invalide
            }
        } while (x < 0 || x > map.y_max - 1 || r != 1);

        if (map.costs[y][x] >= 10000) printf("The position is invalid, MARC is on a CREVASSE\n");
    } while (map.costs[y][x] >= 10000); //Si on est sur une crevasse on recommence

    int choice;
    t_orientation ori;
    do{
        printf("Enter MARC orientation number\n");
        printf("[NORTH : 1] [EAST : 2] [SOUTH : 3] [WEST : 4] \n");
        r = scanf("%d", &choice);
        if (r != 1) {
            while (getchar() != '\n'); // Pour éviter un cycle infini si l'entrée est invalide
        }
    } while (choice < 1 || choice > 4 || r!=1);

    if (choice == 1) ori = NORTH;
    else if (choice == 2) ori = EAST;
    else if (choice == 3) ori = SOUTH;
    else ori = WEST;

    t_localisation loc = loc_init(x, y, ori);

    printf("Here are the possible movements : [ ");
    for (int j = 0; j < NB_possibilities; j++) {
        printf("%s ", getMoveAsString(moves[j]));
        if (j < NB_possibilities - 1) {
            printf("; ");
        }
    }
    printf("]\n");



    t_tree tree = create_tree(moves, loc, map);

    int min_val = search_min(tree);
    printf("The minimum value on which MARC can arrive is %d\n", min_val);



    int nb_move;
    t_move *path = best_path(tree, &nb_move);
    if (nb_move == 0) printf("MARC can't move at all ");
    else {
        if (!min_val) printf("MARC can return to the base !\n");
        else printf("Sadly, MARC can't return to the base, but he can get close to it\n");

        printf("The movement(s) required to arrive at the minimum value sheet is: [");
        for (int j = 0; j < nb_move; j++) {
            printf("%s ", getMoveAsString(path[j]));
            if (j < nb_move) {
                printf("; ");
            }
        }
        printf("]\n");

        if (!min_val) printf("Welcome Home MARC!");
        else printf("We hope wind will push you to the base MARC");
    }
    return 0;
}