//
// Created by wengj on 21/11/2024.
//

#ifndef UNTITLED1_PATH_H
#define UNTITLED1_PATH_H
#include "node_queue.h"

p_node* tab_of_min(p_node node, int* len);//Trouve toutes les feuilles de valeur min dans un arbre.

p_node min_leaf(t_tree t); //Trouve la feuille ayant le coût total min dans un arbre.

p_node min_leaf_node(p_node node);//Trouve la feuille ayant le coût total min dans un sous-arbre donné.

t_move* best_path(t_tree t, int *len);//Calcule le meilleur chemin pour atteindre une feuille de coût min.

#endif //UNTITLED1_PATH_H
