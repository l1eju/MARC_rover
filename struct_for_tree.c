//
// Created by wengj on 14/11/2024.
//

#include "struct_for_tree.h"

void add_node_to_list(t_ht_list *mylist, t_node node){

}

t_queue_tab createEmptyQueue()
{
    t_queue_tab q;
    q.first = q.last = 0;
    return q;
}

void enqueue(t_queue_tab *pq, p_node val)
{
    int pos;
    pos = pq->last % MAX;
    pq->values[pos] = val;
    pq->last = pq->last+1;
}

p_node dequeue(t_queue_tab *pq)
{
    p_node res;
    int pos = pq->first%MAX;
    res = pq->values[pos];
    pq->first = pq->first+1;
    return res;
}