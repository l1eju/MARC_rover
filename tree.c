//
// Created by wengj on 22/10/2024.
//

#include "tree.h"


t_move* remove_move(t_move* moves, int len, int idx){
    t_move* new_moves = (t_move*) malloc((len-1)*sizeof(t_move));
    int j = 0;
    for (int i = 0; i < len-1; i++){
        if (i == idx) j++;
        new_moves[i] = moves[j];
        j++;
    }
    return new_moves;
}

p_node createNode(int nb_sons, int depth, t_move mouvement, t_localisation loc, t_map map, p_node node){
    p_node new_node;                                           //Initialise le nouvel arbre
    new_node = (t_node *)malloc(sizeof(t_node));

    new_node->value = map.costs[loc.pos.y][loc.pos.x];          //Définit le coût de la case
    new_node->depth = depth;                                    //Définit la profondeur du noeud
    new_node->mouvement = mouvement;
    new_node->parent = node;
    new_node->soil_type = map.soils[loc.pos.y][loc.pos.x];      //Définit le type de sol de la case
    new_node->nbSons = nb_sons;                                 //Définit son nombre d'enfants
    new_node->sons = (t_node **)malloc(nb_sons*sizeof(t_node *));
    for (int i = 0; i < nb_sons; i++)   new_node->sons[i] = NULL;

    return new_node;
}

p_node create_all_Node(int nb_poss, int depth, t_move mouvement, t_move* possibilities, t_localisation robot, t_map map, p_node parent_node){   //La fonction ne prends pas en compte si on avance de plus de 10 mètres ou si on a déjà marché sur une crevasse
    if (depth > NB_choices) return NULL;                                                                                //Si la profondeur est supérieur au nombre de choix, on retourne NULL

    else if (depth == NB_choices || map.costs[robot.pos.y][robot.pos.x] >= 10000) nb_poss = 0;                          //Si on est à la profondeur la plus bas, donc le dernier choix, ou que la case après le mouvement est une crevasse, le noeud n'aura pas d'enfant

    p_node node = createNode(nb_poss, depth, mouvement, robot, map, parent_node);                     //Initialise le nouveau noeud

    for (int i = 0; i < nb_poss; i++) {
        t_localisation new_loc = robot;
        updateLocalisation(&new_loc, possibilities[i]);                                                                 //On stocke la nouvelle position du robot selon le mouvement associé dans new_loc

        if (isValidLocalisation(new_loc.pos, map.x_max, map.y_max)) {                                                   //Si la position après le mouvement est valide, on crée les enfants
            t_move* new_possibilities = remove_move(possibilities, nb_poss, i);                  //On crée le nouveau tableau de possibilités en retirant la case du noeud qu'on va créer car on l'aura déjà utilisé et on a déjà stocker la position après le mouvement pour connaître le coût
            node->sons[i] = create_all_Node(nb_poss - 1, depth+1, possibilities[i], new_possibilities, new_loc, map, node); //On utilise la récursivité pour obtenir l'enfant avec les nouveaux paramètres
            free(new_possibilities);                                                                            //On libère la mémoire de new_possibilities
        }
        else{                                                                                                           //Si la position après le mouvement est valide, on ne crée pas d'enfant à la case concernée
            node->sons[i] = NULL;
        }
    }
    return node;
}

//Fonction pour la création d'un arbre
t_tree create_tree(int nb_poss, int depth, t_move mouvement, t_move* possibilities, t_localisation robot, t_map map){
    t_tree t;
    t.root = create_all_Node(nb_poss, depth, mouvement, possibilities, robot, map, NULL);
    return t;
}

int search_min(t_tree t){
    return search_min_node(t.root);
}

int search_min_node(p_node node){  //Fonction pour chercher une feuille de valeur minimum
    int min = node->value;

    if (node->nbSons != 0){
        for (int i = 0; i < node->nbSons; i++) {
            if (node->sons[i] != NULL){
                int min_son = search_min_node(node->sons[i]);
                if (min_son < min){
                    min = min_son;
                }
            }
        }
    }
    return min;
}

int nb_min(p_node node, int min){  //Fonction pour chercher le nombre de valeur minimum
    int nb = 0;

    if (node->nbSons != 0){
        for (int i = 0; i < node->nbSons; i++) {
            if (node->sons[i] != NULL){
                nb += nb_min(node->sons[i], min);
            }
        }
    }
    if (node->value == min) nb += 1;
    return nb;
}
