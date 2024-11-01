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

    t_localisation loc = loc_init(2, 6, NORTH);

    t_node* node = NULL;
    node = create_all_Node(NB_possibilities, 0, test, loc, map);

    //printf("Min = %d\n", search_min(node));

    int path[MAX_depth];
    int path_length = 0;
    int min_val = path_min(node,path,&path_length);
    printf("Min = %d\n", min_val);
    for (int i = 0; i < path_length; i++){
        printf("%d ",path[i]);
        if (i != path_length - 1) {
            printf("-> ");
        }
    }
    free(node);
    free(test);
    return 0;
}