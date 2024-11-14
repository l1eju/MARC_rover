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

#define NB_possibilities 9
#define NB_choices 5

typedef struct s_node
{
    int value;
    int depth;
    t_move mouvement;
    t_soil soil_type;
    struct s_node **sons;
    int nbSons; // Taille physique du tableau
} t_node;


t_move* random_possibilities();

t_move* remove_possibility(t_move*, int, int);

t_node *createNode(int, int, t_move, t_localisation, t_map);

t_node *create_all_Node(int, int, t_move, t_move*, t_localisation, t_map);

int search_min(t_node *);

void path_min(t_node *, t_stack, int);

int nb_min(t_node *node, int min);

#endif //UNTITLED1_TREE_H
