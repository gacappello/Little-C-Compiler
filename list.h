#ifndef LIST_H
#define LIST_H

#define LIST_HEAD 0
#define LIST_END -1

typedef struct list_node {
    struct list_node *next;
    void *data;
    void (*dtor)(void *);
} list_node_t;

typedef struct list {
    list_node_t *head;
    unsigned int size;
} list_t;

list_t *list_create(void);
void list_destroy(list_t *list);

void list_insert_at(list_t *list, int at, void *data, void (*dtor)(void *));
void list_remove_at(list_t *list, int at);

void list_push(list_t *list, void *data, void (*dtor)(void *));
void list_pop(list_t *list);

#endif
