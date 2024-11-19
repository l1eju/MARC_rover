//
// Created by wengj on 14/11/2024.
//

#ifndef UNTITLED1_STRUCT_FOR_TREE_H
#define UNTITLED1_STRUCT_FOR_TREE_H
#include "tree.h"

typedef struct s_cell {
    struct s_node* value;
    struct s_cell* next;
} t_cell;

typedef struct s_ht_list
{
    t_cell *head;   	// adresse de la première cellule
    t_cell *tail;   	// adresse de la dernière cellule
} t_ht_list;

typedef struct s_queue_tab
{
    p_node* values;
    int first, last;
} t_queue_tab;

t_queue_tab createEmptyQueue();
void enqueue(t_queue_tab *, p_node);
p_node dequeue(t_queue_tab *);

void add_node_to_list(t_ht_list *mylist, t_node node);
#endif //UNTITLED1_STRUCT_FOR_TREE_H
