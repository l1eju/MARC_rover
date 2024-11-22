//
// Created by wengj on 21/11/2024.
//

#include "path.h"
/**
 * @brief Trouve toutes les feuilles de val min dans l'arbre.
 * @param node Nœud racine de l'arbre.
 * @param len Pointeur pour stocker le nb de feuilles trouvées.
 * @return Tab de pointeurs des feuilles avec val min.
 */
p_node* tab_of_min(p_node node, int* len){

    int min_val = search_min_node(node); //Cherche la val min dans l'arbre
    p_node* min_leaf = (p_node*) malloc(MAX * sizeof(p_node));//Tab ou stocker les val min

    t_queue_tab q;
    p_node cur;
    q = createEmptyQueue(); //File pour parcourir l'arbre en profondeur
    enqueue_node(&q, node);// Ajoute le noeud racine dans la file

    while (q.first != q.last){ //Tant que file non vide
        cur = dequeue_node(&q); //Retirer un noeud de la file

        if(cur->value == min_val){ //Si le noeud est la val min
            int depth_of_min = cur->depth;//Profondeur du min est la profondeur actuel

            while (cur->depth == depth_of_min && q.first != q.last){

                if(cur->value == min_val){ //Si la val est val_min
                    min_leaf[*len] = cur; //Ajoute dans le tab la val min
                    //printf("cur = %d, minleaf[] = %d, len = %d\n", cur->value, min_leaf[*len]->value, *len);
                    (*len)++;
                }
                cur = dequeue_node(&q);//Passe au noeud suivant dans la file
            }
            return min_leaf; //Retourne le tab de feuilles min trouvées
        }
        else if (cur->nbSons != 0){//Si le noeud a des fils, on ajoute chaque fils dans la file
            for (int i = 0; i < cur->nbSons; i++) {
                if (cur->sons[i] != NULL){
                    enqueue_node(&q,cur->sons[i]);
                }
            }
        }
    }
    return NULL;//Si aucune feuille trouvée, retourne NULL
}

p_node min_leaf(t_tree t) {
    return min_leaf_node(t.root);//Appelle de la fonction min_leaf_mode en partant de la racine
}

/**
 * @brief Trouve la feuille qui a le coût total min parmi les feuilles de val min.
 * @param node Nœud racine de l'arbre.
 */
p_node min_leaf_node(p_node node){
    int len = 0, cost = 0, cheaper = 10000, idx;//Stock nb feuille min, coût total de la feuille, coût élevée
    p_node* tab_min = tab_of_min(node, &len); //Prend toutes les feuilles de val min

    for (int i = 0; i < len; i++){//Parcours toutes les feuilles pour calculer leur coût total
        p_node cur = tab_min[i];
        while (cur != NULL) {
            cost += cur->value;//Ajoute la val du noeud au coût
            cur = cur->parent;//Remonte au noeud parent
        }
        if (cost < cheaper){//Si le coût est inférieur au précédent
            cheaper = cost;//MAJ du coût min
            idx = i;//Garde l'indice de la feuille
        }
    }
    return tab_min[idx];
}

t_move* best_path(t_tree t, int *len){ //Fonction pour la recherche du meilleur chemin, càd, le coût le moins cher
    p_node leaf = min_leaf(t);// On commence par trouver la feuille avec le coût total minimal dans l'arbre
    int depth = leaf->depth;// Profondeur de la feuille est la longueur du chemin à partir de la racine.
    *len = depth - 1;//Longueur du chemin - la racine
    t_move* path = (t_move*) malloc((depth) * sizeof(t_move));//Stock les mouvements dont on a besoin en allouant un tab

    for (int i = depth - 1; i >= 0; i--){//Parcourt le chemin de la feuille jusqu'à la racine
        path[i] = leaf->mouvement; //Stock le mouvement associé au noeud
        leaf = leaf->parent;//On continue de remonter en passant par le parent du noeud
    }
    return path;//Retourne tab des mouvements avec le meilleur chemin
}
