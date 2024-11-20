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
#define MAX 18730


typedef struct s_node
{
    int value;
    int depth;
    t_move mouvement;
    t_soil soil_type;
    struct s_node *parent;
    struct s_node **sons;
    int nbSons; // Taille physique du tableau
} t_node;

typedef struct s_node t_node, *p_node;

typedef struct s_tree
{
    p_node root;
} t_tree;

typedef struct s_queue_tab
{
    p_node values[MAX];
    int first, last;
} t_queue_tab;

t_move* random_possibilities();

t_move* remove_possibility(t_move*, int, int);

p_node createNode(int nb_sons, int depth, t_move mouvement, t_localisation loc, t_map map, p_node node);

p_node create_all_Node(int nb_poss, int depth, t_move mouvement, t_move* possibilities, t_localisation robot, t_map map, p_node parent_node);

t_tree create_tree(int nb_poss, int depth, t_move mouvement, t_move* possibilities, t_localisation robot, t_map map);

int search_min(t_tree);

int search_min_node(p_node node);

int nb_min(t_node *node, int min); //Fonction bonus

p_node* tab_of_min(p_node node, int* len);

p_node min_leaf(t_tree t);

p_node min_leaf_node(p_node node);

t_move* best_path(t_tree t, int *len);


//Fonction de queue pour p_node
t_queue_tab createEmptyQueue();

void enqueue_node(t_queue_tab *, p_node);

p_node dequeue_node(t_queue_tab *);


#endif //UNTITLED1_TREE_H
