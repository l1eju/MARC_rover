//
// Created by wengj on 22/10/2024.
//

#ifndef UNTITLED1_TREE_H
#define UNTITLED1_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "stack.h"
#include "map.h"
#include "moves.h"

#define NB_possibilities 5
#define NB_choices 3

typedef struct s_node
{
    int value;
    t_soil soil_type;
    struct s_node **sons;
    int nbSons; // taille physique du tableau
} t_node;



t_move* random_possibilities();

t_move* remove_possibility(t_move* possibilities, int len, int idx);

t_node *createNode(int, t_localisation, t_map);

t_node *create_all_Node(t_node*, int, int, t_move*, t_localisation, t_map);

#endif //UNTITLED1_TREE_H
