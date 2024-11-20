//
// Created by wengj on 22/10/2024.
//

#include "tree.h"

t_move* random_possibilities(){
    srand(time(NULL));  //Set la fonction rand
    t_move* possibilities = (t_move*) malloc(NB_possibilities*sizeof(t_move));   //Initialise le tableau de mouvement tiré au hasard

    for(int i = 0; i < NB_possibilities; i++){
        switch (1 + rand() % 7) {    //On prend le mouvement associé au chiffre tiré et on le met dans le tableau de mouvement
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
    return possibilities;   //A modifier pour 9 possibilités car certains mouvements sont limités à 7
}

t_move* remove_possibility(t_move* possibilities, int len, int idx){
    t_move* new_possibilities = (t_move*) malloc((len-1)*sizeof(t_move));
    int j = 0;
    for (int i = 0; i < len-1; i++){
        if (i == idx) j++;
        new_possibilities[i] = possibilities[j];
        j++;
    }
    return new_possibilities;
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
    for (int i = 0; i < nb_sons; i++)
    {
        new_node->sons[i] = NULL;
    }
    return new_node;
}

p_node create_all_Node(int nb_poss, int depth, t_move mouvement, t_move* possibilities, t_localisation robot, t_map map, p_node parent_node){   //La fonction ne prends pas en compte si on avance de plus de 10 mètres ou si on a déjà marché sur une crevasse
    if (depth > NB_choices) return NULL;                                                                            //Si la profondeur est supérieur au nombre de choix, on retourne NULL

    else if (depth == NB_choices || map.costs[robot.pos.y][robot.pos.x] >= 10000) nb_poss = 0;             //Si on est à la profondeur la plus bas, donc le dernier choix, ou que la case après le mouvement est une crevasse, le noeud n'aura pas d'enfant

    p_node node = createNode(nb_poss, depth, mouvement, robot, map, parent_node);                                     //Initialise le nouveau noeud

    for (int i = 0; i < nb_poss; i++) {

        t_localisation new_loc = robot;
        updateLocalisation(&new_loc, possibilities[i]);                                                             //On stocke la nouvelle position du robot selon le mouvement associé dans new_loc

        if (isValidLocalisation(new_loc.pos, map.x_max, map.y_max)) {                                               //Si la position après le mouvement est valide, on crée les enfants

            t_move* new_possibilities = remove_possibility(possibilities, nb_poss, i);             //On crée le nouveau tableau de possibilités en retirant la case du noeud qu'on va créer car on l'aura déjà utilisé et on a déjà stocker la position après le mouvement pour connaître le coût
            node->sons[i] = create_all_Node(nb_poss - 1, depth+1, possibilities[i], new_possibilities, new_loc, map, node); //On utilise la récursivité pour obtenir l'enfant avec les nouveaux paramètres
            free(new_possibilities);                                                                        //On libère la mémoire de new_possibilities
        }
        else{                                                                                                        //Si la position après le mouvement est valide, on ne crée pas d'enfant à la case concernée
            node->sons[i] = NULL;
        }
    }
    return node;
}


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

    if (node->nbSons!=0){
        for (int i = 0; i < node->nbSons; i++) {
            if (node->sons[i]!=NULL){
                int min_son = search_min_node(node->sons[i]);
                if (min_son < min){
                    min = min_son;
                }
            }
        }
    }
    return min;
}

int nb_min(p_node node, int min){  //Fonction pour chercher une feuille de valeur minimum
    int nb = 0;

    if (node->nbSons!=0){
        for (int i = 0; i < node->nbSons; i++) {
            if (node->sons[i]!=NULL){
                nb += nb_min(node->sons[i], min);
            }
        }
    }
    if (node->value == min) nb += 1;
    return nb;
}

p_node* tab_of_min(p_node node, int* len){

    int min_val = search_min_node(node);
    p_node* min_leaf = (p_node*) malloc(MAX*sizeof(p_node));

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
        else if (cur->nbSons!=0){
            for (int i = 0; i < cur->nbSons; i++) {
                if (cur->sons[i]!=NULL){
                    enqueue_node(&q, cur->sons[i]);
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

    for (int i=0; i<len; i++){
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

t_move* best_path(t_tree t, int *len){
    p_node leaf = min_leaf(t);
    int depth = leaf->depth;
    *len = depth-1;
    t_move* path = (t_move*) malloc((depth) * sizeof(t_move));

    for (int i = depth-1; i>=0; i--){
        path[i] = leaf->mouvement;
        leaf = leaf->parent;
    }
    return path;
}

//Fonction de queue pour p_node
t_queue_tab createEmptyQueue()
{
    t_queue_tab q;
    q.first = q.last = 0;
    return q;
}

void enqueue_node(t_queue_tab *pq, p_node val)
{
    int pos;
    pos = pq->last % MAX;
    pq->values[pos] = val;
    pq->last = pq->last+1;
}

p_node dequeue_node(t_queue_tab *pq)
{
    p_node res;
    int pos = pq->first%MAX;
    res = pq->values[pos];
    pq->first = pq->first+1;
    return res;
}