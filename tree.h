

//
// Created by jubos on 12/11/2024.
//

#ifndef TREE_H
#define TREE_H
#include <stdlib.h>
#include "loc.h"
#include "map.h"

typedef struct s_node {
    struct s_node *parent;      // Pointeur vers le parent
    int value;                  // Mouvement associé au nœud
    int depth;                  // Profondeur du nœud dans l'arbre
    int nbSons;                 // Nombre de fils du nœud
    struct s_node **sons;       // Tableau dynamique des fils
    int *availablemvt;          // Tableau des mouvements restants
    t_localisation loc;         // Localisation actuelle (position et orientation)
    int status;
    int next_phase;
    int case_cost;
} t_node;


t_node *createNode(int value, int depth, int *availablemvt, t_node *parent, t_localisation loc, int status, int next_phase, int case_cost);

int *updateAvails(int *parentAvails, int currentChoice, int nbSons);

void buildTree(t_node *parent, int depth, int *avails, int nbSons, t_map map);

void printNode(t_node *node);

void printTree(t_node *node, int depth);

void freeTree(t_node *node);

void findOptimalPath(t_node *node, t_map map, int currentCost, int *bestCost, t_node **bestLeaf, t_localisation loc);



#endif //TREE_H
