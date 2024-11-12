//
// Created by jubos on 12/11/2024.
//

#ifndef TREE_H
#define TREE_H
#include <stdlib.h>

typedef struct s_node {
    struct s_node *parent;
    int value;
    int depth;
    int nbSons;
    struct s_node **sons;
    int *availablemvt;
} t_node;

t_node *createNode(int value, int depth, int *availablemvt, t_node *parent);

#endif //TREE_H
