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

int search_min(p_node node){  //Fonction pour chercher une feuille de valeur minimum
    int min = node->value;

    if (node->nbSons!=0){
        for (int i = 0; i < node->nbSons; i++) {
            if (node->sons[i]!=NULL){
                int min_son = search_min(node->sons[i]);
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

void BFVisit(p_node node)
{
    t_queue_tab q;
    p_node cur;
    q = createEmptyQueue();
    enqueue_node(&q, node);

    printf("[");
    while (q.first != q.last){
        cur = dequeue_node(&q);
        printf(" %d :", cur->value);

        for (int i = 0; i<node->parent->nbSons; i++){
            if (cur->sons[i] != NULL) {
                enqueue_node(&q, cur->sons[i]);
            }
        }
    }
    printf("]\n");
}

/*int* path_min_v1(t_node *node, int *path, int *path_length) {               //Fonction qui permet de chercher la valeur minimum et le chemin depuis la racine vers cette feuille

    int min = search_min(node);

    for (int i = 0; i < node->nbSons; i++) {                                                       //Boucle pour parcourir chaque fils
        if (node->sons[i] != NULL) {                                                               //Si le noeud du fils existe
            int temp_length = 0;                                                                   //Initialisation d'une variable temporaire de la longueur du chemin du fils
            int *temp_path = (int *)malloc(NB_choices *sizeof(int));                           //Tableau dynamique qui stocke temporairement le chemin
            int min_son = path_min(node->sons[i],temp_path ,&temp_length);    //Appel récursif pour stocker le chemin et la longueur temporaire du fils


            if (min_son < min) {                                                                    //Condition pour savoir si le minimum du fils est inférieur au minimum actuel
                min = min_son;                                                                      //Mise à jour du minimum
                path[0] = node->value;                                                              //Ajoute la valeur du noeud actuel
                for (int j = 0; j < temp_length; j++){                                              //Boucle pour avoir le chemin finale partant de la racine jusqu'à la feuille minimale
                    path[j+1] = temp_path[j];
                }
                *path_length = temp_length + 1;                                                         //Mise à jour de la longueur totale du chemin
            }
            free(temp_path);                                                                //Libère la mémoire car on ne l'utilise plus
        }
    }
    return min;
}*/




/*void list_of_min_node(t_node *node, int min, t_ht_list* mylist){

    if (node->nbSons!=0){
        for (int i = 0; i < node->nbSons; i++) {
            if (node->sons[i]!=NULL){
                if (node->sons[i]->value == min) add_node_to_list(&mylist, node->sons[i]);
                else if(search_min(node->sons[i]) == min){
                    path_min(node, min, mylist);
                }
            }
        }
    }
}

void compare_and_choose_best_path(t_ht_list mylist, t_node root){
    int best_val = 10000, idx=0, curr_idx = 0;

    while (mylist.head != NULL){
        t_stack stack = createStack(NB_choices);
        t_node current = mylist.head->value;
        int val = 0;
        while (current != root){
            val += current.value;
            current = current.parent;
        }
        if (val<best_val){
            best_val = val;
            idx = curr_idx;
        }
    }
}

void path(t_ht_list mylist, t_node root, int idx) {
    for (int i=0; i<idx; i++){
        mylist.head = mylist.head->next;
    }
    //stack t_move cm6
}
*/



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