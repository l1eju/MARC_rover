#include <stdio.h>
#include "map.h"
#include "tree.h"

int main() {

    t_map map = createMapFromFile("..\\maps\\example1.map");
    printf("Map created with dimensions %d x %d\n", map.y_max, map.x_max);
    for (int i = 0; i < map.y_max; i++)
    {
        for (int j = 0; j < map.x_max; j++)
        {
            printf("%d ", map.soils[i][j]);
        }
        printf("\n");
    }
    // printf the costs, aligned left 5 digits
    for (int i = 0; i < map.y_max; i++)
    {
        for (int j = 0; j < map.x_max; j++)
        {
            printf("%-5d ", map.costs[i][j]);
        }
        printf("\n");
    }
    displayMap(map);


    t_move* test;
    test = random_possibilities();

    for (int j = 0; j < NB_possibilities; j++)
    {
        printf("%s   ", getMoveAsString(test[j]));
    }
    printf("\n");

    test = remove_possibility(test, NB_possibilities-1, 2);

    /*for (int j = 0; j < NB_possibilities-1; j++)
    {
        printf("%s   ", getMoveAsString(ln[j]));
    }
    printf("\n");*/
    t_localisation loc = loc_init(6, 3, NORTH);

    t_node* node = NULL;
    node = create_all_Node(node, NB_possibilities, NB_choices, test, loc,  map);
    return 0;
}