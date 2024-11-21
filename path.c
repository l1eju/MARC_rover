//
// Created by wengj on 21/11/2024.
//

#include "path.h"

p_node* tab_of_min(p_node node, int* len){

    int min_val = search_min_node(node);
    p_node* min_leaf = (p_node*) malloc(MAX * sizeof(p_node));

    t_queue_tab q;
    p_node cur;
    q = createEmptyQueue();
    enqueue_node(&q, node);

    while (q.first != q.last){
        cur = dequeue_node(&q);

        if(cur->value == min_val){
            int depth_of_min = cur->depth;

            while (cur->depth == depth_of_min && q.first != q.last){

                if(cur->value == min_val){
                    min_leaf[*len] = cur;
                    //printf("cur = %d, minleaf[] = %d, len = %d\n", cur->value, min_leaf[*len]->value, *len);
                    (*len)++;
                }
                cur = dequeue_node(&q);
            }
            return min_leaf;
        }
        else if (cur->nbSons != 0){
            for (int i = 0; i < cur->nbSons; i++) {
                if (cur->sons[i] != NULL){
                    enqueue_node(&q,cur->sons[i]);
                }
            }
        }
    }
    return NULL;
}

p_node min_leaf(t_tree t) {
    return min_leaf_node(t.root);
}

p_node min_leaf_node(p_node node){
    int len = 0, cost = 0, cheaper = 10000, idx;
    p_node* tab_min = tab_of_min(node, &len);

    for (int i = 0; i < len; i++){
        p_node cur = tab_min[i];
        while (cur != NULL) {
            cost += cur->value;
            cur = cur->parent;
        }
        if (cost < cheaper){
            cheaper = cost;
            idx = i;
        }
    }
    return tab_min[idx];
}

t_move* best_path(t_tree t, int *len){                                  //Fonction pour la recherche du meilleur chemin, c'est-à-dire, le cou^t le moins chère
    p_node leaf = min_leaf(t);
    int depth = leaf->depth;
    *len = depth - 1;
    t_move* path = (t_move*) malloc((depth) * sizeof(t_move));

    for (int i = depth - 1; i >= 0; i--){
        path[i] = leaf->mouvement;
        leaf = leaf->parent;
    }
    return path;
}
