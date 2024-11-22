//
// Created by wengj on 21/11/2024.
//

#include "node_queue.h"

//Fonction de queue pour p_node
/** @brief Crée une file vide
*/
t_queue_tab createEmptyQueue() //Initialise une file vide (FIFO) et return une file vide de type t_queue_tab
{
    t_queue_tab q;
    q.first = q.last = 0; //Initialise la file à vide, first & last pointent au même endroit
    return q;
}

/** @brief Ajoute un noeud au début de la file
 * @param pq pointe vers la file ou le noeud doit être ajouté
 * @param val est le noeud de type p_node à insérer
*/
void enqueue_node(t_queue_tab *pq, p_node val) //Ajoute val, le noeud de type p_node à la fin de la file
{
    int pos;
    pos = pq->last % MAX;//Calcule pour trouver la position circulaire ou insérer le noeud
    pq->values[pos] = val; //Insère le noeud à cette position
    pq->last = pq->last + 1; //Aller à la prochaine indice last pour la prochaine val à insérer
}

/** @brief Retire et return un noeud au début de la file
 * @param pq pointe vers la file ou le noeud doit être retiré
 * @return le noeud retiré au début de la file
*/
p_node dequeue_node(t_queue_tab *pq)
{
    p_node res;
    int pos = pq->first % MAX; //Calcule pour trouver la position circulaire ou retirer le noeud
    res = pq->values[pos]; //Retire le noeud à cette position
    pq->first = pq->first + 1; // Avance à l'indice first pour la prochaine val à retirer
    return res;
}