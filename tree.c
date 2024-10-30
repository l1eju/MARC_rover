//
// Created by wengj on 22/10/2024.
//

#include "tree.h"

t_move* random_possibilities(){
    srand(time(NULL));  //Set la fonction rand
    t_move* possibilities = (t_move*) malloc(NB_possibilities*sizeof(t_move));   //Initialise le tableau de mouvement tiré au hasard

    for(int i=0; i<NB_possibilities; i++){
        switch (1 + rand() % (NB_choices)) {    //On prend le mouvement associé au chiffre tiré et on le met dans le tableau de mouvement
            case 1:
                possibilities[i] = F_10;
                break;
            case 2:
                possibilities[i] = F_20;
                break;
            case 3:
                possibilities[i] = F_30;
                break;
            case 4:
                possibilities[i] = B_10;
                break;
            case 5:
                possibilities[i] = T_LEFT;
                break;
            case 6:
                possibilities[i] = T_RIGHT;
                break;
            case 7:
                possibilities[i] = U_TURN;
                break;
        }
    }
    return possibilities;   //A modifier pour 9 possibilités car certains mouvements sont limité à 7
}

t_move* remove_possibility(t_move* possibilities, int len, int idx){
    for (int i=idx; i<len; i++){
        possibilities[i] = possibilities[i+1];
    }
    realloc(possibilities, len-1);
    return possibilities;
}

t_node *createNode(int nb_sons, t_localisation loc, t_map map){
    t_node *new_node;                                           //Initialise le nouvel arbre
    new_node = (t_node *)malloc(sizeof(t_node));
    new_node->value = map.costs[loc.pos.x][loc.pos.y];      //Définit le coût de la case
    new_node->soil_type = map.soils[loc.pos.x][loc.pos.y];  //Définit le type de sol de la case
    new_node->nbSons = nb_sons;                                 //Définit son nombre d'enfants
    new_node->sons = (t_node **)malloc(nb_sons*sizeof(t_node *));
    for (int i=0; i <nb_sons; i++)
    {
        new_node->sons[i]=NULL;
    }
    return new_node;
}

t_node *create_all_Node(t_node* node, int nb_possibilities, int height_node, t_move* possibilities, t_localisation robot, t_map map){
    if (node == NULL) {
        node = createNode(height_node, robot, map);  //Initialise le nouvel arbre
    }
    if (node->nbSons!=0){
        for (int j = 0; j < nb_possibilities; j++) {
            t_localisation new_loc = move(robot, possibilities[j]); //On stocke la nouvelle position du robot selon le mouvement associé
            t_node *new_node = NULL;                                //Initialise l'enfant
            new_node = create_all_Node(new_node, nb_possibilities-1, height_node-1, remove_possibility(possibilities, nb_possibilities, j), new_loc, map); //On utilise la récursivité pour obtenir l'enfant avec les nouveaux paramètres
            node->sons[j] = new_node;
        }
    }
    return node;
}

