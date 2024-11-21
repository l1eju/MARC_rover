//
// Created by wengj on 21/11/2024.
//

#ifndef UNTITLED1_PATH_H
#define UNTITLED1_PATH_H
#include "node_queue.h"

p_node* tab_of_min(p_node node, int* len);

p_node min_leaf(t_tree t);

p_node min_leaf_node(p_node node);

t_move* best_path(t_tree t, int *len);

#endif //UNTITLED1_PATH_H
