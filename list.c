#include <stdlib.h>
#include <assert.h>

#include "list.h"

static list_node_t *list_node_create(void *data, void (*dtor)(void *))
{
    list_node_t *n = calloc(1, sizeof(list_node_t));

    assert(n);

    n->data = data;
    n->dtor = dtor;
    return n;
}

static void list_node_destroy(list_node_t *node)
{
    if(!node) return;

    if(node->dtor && node->data) {
        node->dtor(node->data);
        node->data = NULL;
    }

    free(node);
}

list_t *list_create(void)
{
    list_t *l = calloc(1, sizeof(list_t));
    return l;
}

void list_destroy(list_t *list)
{
    if(!list) return;

    list_node_t *n = list->head;
    while(n && n->next) {
        list_node_t *r = n;
        n = n->next;
        list_node_destroy(r);
    }
    free(list);
}

void list_insert_at(list_t *list, int at, void *data, void (*dtor)(void *))
{
    assert(list);

    list_node_t *in = list_node_create(data, dtor);

    if(!list->head) {
        list->head = in;
        list->size++;
        return;
    }

    if(at == LIST_HEAD) {
        list_node_t *h = list->head;
        list->head = in;
        in->next = h;
        list->size++;
        return;
    }

    if(at == LIST_END) {
        list_node_t *n = list->head;
        while(n->next) {
            n = n->next;
        }
        n->next = in;
        list->size++;
        return;
    }

    list_node_t *n = list->head;
    while(n->next && at > 0) {
        n = n->next;
        at--;
    }
    
    list_node_t *p = n->next;
    n->next = in;
    in->next = p;
    list->size++;
}

void list_remove_at(list_t *list, int at)
{
    assert(list);

    list_node_t *rn = NULL;

    if(!list->head) return;

    if(at == LIST_HEAD) {
        rn = list->head;
        list->head = rn->next;
        list_node_destroy(rn);
        list->size--;
        return;
    }

    if(at == LIST_END) {
        rn = list->head;
        list_node_t *prev;
        while(rn->next) {
            prev = rn;
            rn = rn->next;
        }
        prev->next = NULL;
        list_node_destroy(rn);
        list->size--;
        return;
    }

    rn = list->head;
    list_node_t *prev;
    while(rn->next && at > 0) {
        prev = rn;
        rn = rn->next;
        at--;
    }

    prev->next = rn->next;
    list_node_destroy(rn);
    list->size--;
}

void list_push(list_t *list, void *data, void (*dtor)(void *))
{
    list_insert_at(list, LIST_END, data, dtor);
}

void list_pop(list_t *list)
{
    list_remove_at(list, LIST_END);
}
