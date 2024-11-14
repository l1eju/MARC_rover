#include "tree.h"
#include <stdlib.h>
#include <time.h>
#include "map.h"
#include "moves.h"

// Fonction pour générer des mouvements possibles aléatoires
t_move* random_possibilities() {
    srand(time(NULL));
    t_move* possibilities = (t_move*) malloc(NB_possibilities * sizeof(t_move));  // Allocation mémoire pour les mouvements possibles

    // Génère aléatoirement un mouvement parmi les 7 possibles
    for (int i = 0; i < NB_possibilities; i++) {
        switch (1 + rand() % 7) {  // Génère un nombre entre 1 et 7 pour choisir un mouvement
            case 1: possibilities[i] = F_10; break;  // Mouvement de 10 unités en avant
            case 2: possibilities[i] = F_20; break;  // Mouvement de 20 unités en avant
            case 3: possibilities[i] = F_30; break;  // Mouvement de 30 unités en avant
            case 4: possibilities[i] = B_10; break;  // Mouvement de 10 unités en arrière
            case 5: possibilities[i] = T_LEFT; break;  // Rotation à gauche
            case 6: possibilities[i] = T_RIGHT; break;  // Rotation à droite
            case 7: possibilities[i] = U_TURN; break;  // Rotation à 180 degrés
        }
    }
    return possibilities;
}

// Fonction pour supprimer un mouvement spécifique de la liste des possibilités
t_move* remove_possibility(t_move* possibilities, int len, int idx) {
    // Vérification de la validité de l'index
    if (idx < 0 || idx >= len) {
        return NULL;  // Retourne NULL si l'index est invalide
    }

    t_move* new_possibilities = (t_move*) malloc((len - 1) * sizeof(t_move));  // Nouvelle liste avec une case en moins
    int j = 0;

    // Parcourt toutes les possibilités et copie celles qui ne sont pas supprimées
    for (int i = 0; i < len; i++) {
        if (i == idx) continue;  // Sauter l'élément à l'index idx
        new_possibilities[j++] = possibilities[i];  // Copie le mouvement
    }
    return new_possibilities;  // Retourne la nouvelle liste sans le mouvement supprimé
}

t_node *createNode(int nb_sons, int depth, t_move mouvement, t_localisation loc, t_map map){
    t_node *new_node;                                           //Initialise le nouvel arbre
    new_node = (t_node *)malloc(sizeof(t_node));
    new_node->value = map.costs[loc.pos.y][loc.pos.x];          //Définit le coût de la case
    new_node->depth = depth;                                    //Définit la profondeur du noeud
    new_node->mouvement = mouvement;
    new_node->soil_type = map.soils[loc.pos.y][loc.pos.x];      //Définit le type de sol de la case
    new_node->nbSons = nb_sons;                                 //Définit son nombre d'enfants
    new_node->sons = (t_node **)malloc(nb_sons*sizeof(t_node *));
    for (int i = 0; i < nb_sons; i++)
    {

        new_node->sons[i] = NULL;
    }

    return new_node;
}


t_node *create_all_Node(int nb_poss, int depth, t_move mouvement, t_move* possibilities, t_localisation robot, t_map map){   //La fonction ne prends pas en compte si on avance de plus de 10 mètres ou si on a déjà marché sur une crevasse
    if (depth > NB_choices) return NULL;                                                                            //Si la profondeur est supérieur au nombre de choix, on retourne NULL

    else if (depth == NB_choices || map.costs[robot.pos.y][robot.pos.x] >= 10000) nb_poss = 0;             //Si on est à la profondeur la plus bas, donc le dernier choix, ou que la case après le mouvement est une crevasse, le noeud n'aura pas d'enfant

    t_node* node = createNode(nb_poss, depth, mouvement, robot, map);                                     //Initialise le nouveau noeud

    for (int i = 0; i < nb_poss; i++) {

        t_localisation new_loc = robot;
        updateLocalisation(&new_loc, possibilities[i]);                                                             //On stocke la nouvelle position du robot selon le mouvement associé dans new_loc

        if (isValidLocalisation(new_loc.pos, map.x_max, map.y_max)) {                                               //Si la position après le mouvement est valide, on crée les enfants

            t_move* new_possibilities = remove_possibility(possibilities, nb_poss, i);             //On crée le nouveau tableau de possibilités en retirant la case du noeud qu'on va créer car on l'aura déjà utilisé et on a déjà stocker la position après le mouvement pour connaître le coût
            node->sons[i] = create_all_Node(nb_poss - 1, depth+1, possibilities[i], new_possibilities, new_loc, map); //On utilise la récursivité pour obtenir l'enfant avec les nouveaux paramètres
            free(new_possibilities);                                                                        //On libère la mémoire de new_possibilities

        }
        else {
            node->sons[i] = NULL;  // Si la localisation est invalide, pas d'enfant à cet index
        }
    }

    return node;
}

// Fonction pour rechercher la valeur minimale parmi les nœuds d'un arbre
int search_min(t_node* node) {
    int min = node->value;  // La valeur minimale commence par celle du nœud actuel

    // Si le nœud a des fils, on cherche la valeur minimale parmi eux
    if (node->nbSons != 0) {
        for (int i = 0; i < node->nbSons; i++) {
            if (node->sons[i] != NULL) {
                int min_son = search_min(node->sons[i]);  // Recherche récursive dans les sous-arbres
                if (min_son < min) {
                    min = min_son;  // Mise à jour de la valeur minimale
                }
            }
        }
    }
    return min;
}


int nb_min(t_node *node, int min){  //Fonction pour chercher une feuille de valeur minimum
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
                *path_length = temp_length + 1;  // Met à jour la longueur du chemin
            }

            free(temp_path);  // Libère la mémoire allouée pour le chemin temporaire
        }
    }
    return min;
}*/
/*
void path_min(t_node *node, t_stack stack, int min){
    if (node->nbSons!=0){
        for (int i = 0; i < node->nbSons; i++) {
            if ((node->sons[i]!=NULL) && (stack.size != stack.nbElts)){
                if(search_min(node->sons[i]) == min){
                    printf("adding, %d, %d\n", stack.size, stack.nbElts);
                    push(&stack, i);
                    path_min(node, stack, min);
                }
            }
        }
    }
}*/