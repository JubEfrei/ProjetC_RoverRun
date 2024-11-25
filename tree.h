//
// Created by jubos on 12/11/2024.
//

#ifndef TREE_H
#define TREE_H

#include "loc.h"
#include "moves.h"

typedef struct s_node {
    struct s_node *parent;
    int value;
    int depth;
    int nbSons;
    struct s_node **sons;
    t_move *availablemvt;
    t_localisation *loc;
} t_node;

t_node *createNode(int value, int depth, t_move *availablemvt, t_node *parent, t_localisation *loc);

t_move *updateAvails(t_move *parentAvails, t_move currentChoice, int nbSons);

void buildTree(t_node *parent, int depth, t_move *avails, int nbSons);

void printTreeValues(t_node *root);

void printTree(t_node *root, int level);


#endif //TREE_H
