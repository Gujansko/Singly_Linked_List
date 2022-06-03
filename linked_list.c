#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>
#include "tested_declarations.h"
#include "rdebug.h"

struct linked_list_t* ll_create()
{
    struct linked_list_t *linkedList = calloc(1, sizeof(struct linked_list_t));
    if(!linkedList) return NULL;

    linkedList->head = NULL;
    linkedList->tail = NULL;
    return linkedList;
}


int ll_push_back(struct linked_list_t* ll, int value)
{
    if(!ll) return 1;

    struct node_t *node = calloc(1, sizeof(struct node_t));
    if(!node) return 2;

    if(!ll->head && !ll->tail)
    {
        node->data = value;
        node->next = NULL;
        ll->head = ll->tail = node;
    }
    else
    {
        struct node_t *temp = ll->head;
        struct node_t *prev;

        while(temp)
        {
            prev = temp;
            temp = temp->next;
        }

        node->data = value;
        node->next = NULL;
        prev->next = node;
        ll->tail = node;
    }

    return 0;
}
int ll_push_front(struct linked_list_t* ll, int value)
{
    if(!ll) return 1;

    struct node_t *node = calloc(1, sizeof(struct node_t));
    if(!node) return 2;

    if(!ll->head && !ll->tail)
    {
        node->data = value;
        node->next = NULL;
        ll->head = ll->tail = node;
    }
    else
    {
        node->data = value;
        node->next = ll->head;
        ll->head = node;
    }

    return 0;
}
int ll_pop_front(struct linked_list_t* ll, int *err_code)
{
    if(!ll || !ll->head)
    {
        if(err_code) *err_code = 1;
        return 1;
    }

    int ret = ll->head->data;
    struct node_t *temp = ll->head;

    if(ll->head == ll->tail)
    {
        free(temp);
        ll->head = ll->tail = ll->head->next = ll->tail->next = NULL;
        if(err_code) *err_code = 0;
        return ret;
    }

    ll->head = ll->head->next;
    free(temp);

    if(err_code) *err_code = 0;
    return ret;
}
int ll_pop_back(struct linked_list_t* ll, int *err_code)
{
    if(!ll || !ll->tail)
    {
        if(err_code) *err_code = 1;
        return 1;
    }

    int ret = ll->tail->data;
    struct node_t *temp = ll->tail;

    struct node_t *pos = ll->head;
    struct node_t *before;

    while(pos != ll->tail)
    {
        before = pos;
        pos = pos->next;
    }
    if(pos == ll->head)
    {
        free(temp);
        ll->head = ll->tail = ll->head->next = ll->tail->next = NULL;
        if(err_code) *err_code = 0;
        return ret;
    }

    ll->tail = before;
    ll->tail->next = NULL;
    free(temp);

    if(err_code) *err_code = 0;
    return ret;
}

int ll_back(const struct linked_list_t* ll, int *err_code)
{
    if(!ll || !ll->tail)
    {
        if(err_code) *err_code = 1;
        return 1;
    }

    if(err_code) *err_code = 0;
    return ll->tail->data;
}
int ll_front(const struct linked_list_t* ll, int *err_code)
{
    if(!ll || !ll->head)
    {
        if(err_code) *err_code = 1;
        return 1;
    }

    if(err_code) *err_code = 0;
    return ll->head->data;
}

struct node_t* ll_begin(struct linked_list_t* ll)
{
    if(!ll || !ll->head) return NULL;
    return ll->head;
}
struct node_t* ll_end(struct linked_list_t* ll)
{
    if(!ll || !ll->tail) return NULL;
    return ll->tail;
}

int ll_size(const struct linked_list_t* ll)
{
    if(!ll) return -1;

    int len = 0;
    struct node_t *temp = ll->head;

    while(temp)
    {
        len++;
        temp = temp->next;
    }

    return len;
}
int ll_is_empty(const struct linked_list_t* ll)
{
    if(!ll) return -1;
    if(ll->head || ll->tail) return 0;
    return 1;
}
int ll_at(const struct linked_list_t* ll, unsigned int index, int *err_code)
{
    if(!ll || !ll->head || !ll->tail)
    {
        if(err_code) *err_code = 1;
        return 1;
    }

    struct node_t *temp = ll->head;

    unsigned int i = 0;
    while(i != index)
    {
        i++;
        temp = temp->next;
        if(!temp)
        {
            if(err_code) *err_code = 1;
            return 1;
        }
    }

    if(err_code) *err_code = 0;
    return temp->data;
}

int ll_insert(struct linked_list_t* ll, unsigned int index, int value)
{
    if(!ll || ((!ll->head || !ll->tail) && index != 0) || index > (unsigned int)ll_size(ll))
    {
        return 1;
    }

    struct node_t *new = calloc(1, sizeof(struct node_t));
    if(!new) return 2;
    if(!index)
    {
        new->data = value;

        if(!ll->head && !ll->tail)
        {
            ll->head = ll->tail = new;
            ll->head->next = ll->tail->next = NULL;
        }
        else
        {
            new->next = ll->head;
            ll->head = new;
        }
    }
    else
    {
        struct node_t *before = ll->head;
        struct node_t *after = before->next;

        unsigned int i = 0;
        while((i + 1) < index)
        {
            i++;
            before = after;
            after = before->next;
        }
        new->data = value;
        new->next = after;
        before->next = new;
        if(!after) ll->tail = new;

    }

    return 0;
}

int ll_remove(struct linked_list_t* ll, unsigned int index, int *err_code)
{
    if(!ll || !ll->head || !ll->tail || index >= (unsigned int)ll_size(ll))
    {
        if(err_code) *err_code = 1;
        return 1;
    }

    int ret;
    if(!index)
    {
        if(ll->head == ll->tail)
        {
            ret = ll->head->data;
            free(ll->head);
            ll->head = ll->tail = NULL;
        }
        else
        {
            struct node_t *temp = ll->head;
            ret = temp->data;
            ll->head = ll->head->next;
            free(temp);
        }
    }
    else
    {
        struct node_t *before = ll->head;
        struct node_t *del = before->next;

        unsigned int i = 0;
        while((i+1) < index)
        {
            before = del;
            del = before->next;
            i++;
        }
        struct node_t *after = del->next;
        ret = del->data;
        if(del == ll->tail)
        {
            ll->tail = before;
        }
        free(del);
        before->next = after;
    }

    if(err_code) *err_code = 0;
    return ret;
}

void ll_clear(struct linked_list_t* ll)
{
    if(!ll || !ll->head || !ll->tail) return;

    struct node_t *node = ll->head;
    while(node)
    {
        struct node_t *temp = node;
        node = node->next;
        free(temp);
    }
    ll->head = ll->tail = NULL;
}

void ll_display(const struct linked_list_t* ll)
{
    if(!ll || !ll->head || !ll->tail) return;

    struct node_t *node = ll->head;

    while(node)
    {
        printf("%d ",node->data);
        node = node->next;
    }
    printf("\n");
}
