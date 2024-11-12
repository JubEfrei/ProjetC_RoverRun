//
// Created by jubos on 12/11/2024.
//

#include <malloc.h>
#include <assert.h>
#include "tree.h"

t_node *createNode(int value, int depth, int *availablemvt)
{
    t_node *node = (t_node *)malloc(sizeof(t_node));
    node->value = value;
    node->depth = depth;
    node->availablemvt = availablemvt;

}