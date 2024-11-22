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

int *updateAvails(int *parentAvails, int currentChoice, int nbSons);

void buildTree(t_node *parent, int depth, int *avails, int nbSons);


#endif //TREE_H
