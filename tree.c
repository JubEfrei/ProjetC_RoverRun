//
// Created by jubos on 12/11/2024.
//

#include <malloc.h>
#include <assert.h>
#include <stdio.h>
#include "tree.h"

t_node *createNode(int value, int depth, int *availablemvt, t_node *parent)
{
    t_node *node = (t_node *)malloc(sizeof(t_node));
    node->value = value;
    node->depth = depth;
    node->availablemvt = availablemvt;
    node->nbSons = 9 - depth;
    node->parent = parent;
    return node;
}
int *updateAvails(int *parentAvails, int currentChoice, int nbSons) {
    // Vérifie que les paramètres sont valides
    if (parentAvails == NULL || nbSons <= 0) {
        printf("Erreur: paramètres invalides pour updateAvails\n");
        return NULL;  // Retourne NULL pour signaler l'erreur
    }

    // Alloue de la mémoire pour le tableau des choix restants
    int *newAvails = (int *)malloc((nbSons) * sizeof(int));  // Taille égale à nbSons pour inclure -1
    if (newAvails == NULL) {
        printf("Erreur: échec de l'allocation mémoire pour updateAvails\n");
        exit(1);  // Termine le programme en cas d'erreur critique
    }

    // Remplit le tableau avec les choix restants
    int index = 0;
    for (int i = 0; i < nbSons; i++) {
        if (parentAvails[i] != currentChoice) {
            newAvails[index++] = parentAvails[i];
        }
    }

    // Ajoute le marqueur de fin (-1) pour terminer le tableau
    newAvails[index] = -1;

    // Retourne le nouveau tableau des choix restants
    return newAvails;
}




void buildTree(t_node *parent, int depth, int *avails, int nbSons) {
    // Vérifie si la profondeur maximale est atteinte
    if (depth == 5) {
        return;
    }

    // Vérifie que les paramètres d'entrée sont valides
    if (parent == NULL || avails == NULL || nbSons <= 0) {
        printf("Erreur: paramètres invalides pour buildTree\n");
        return;
    }

    // Alloue de la mémoire pour les nœuds fils du nœud parent
    parent->sons = (t_node **)malloc(nbSons * sizeof(t_node *));
    if (parent->sons == NULL) {
        printf("Erreur: échec de l'allocation mémoire pour les fils\n");
        exit(1);  // Fin immédiate en cas d'erreur critique
    }
    parent->nbSons = nbSons;

    for (int i = 0; i < nbSons; i++) {
        int move = avails[i];  // Choix actuel pour le nœud fils
        int newDepth = depth + 1;

        // Créer une copie des choix restants sans le choix actuel
        int *newAvails = updateAvails(avails, move, nbSons);


        // Créer le nœud fils
        parent->sons[i] = createNode(move, newDepth, newAvails, parent);
        if (parent->sons[i] == NULL) {
            printf("Erreur: échec de la création d'un nœud fils\n");
            free(newAvails);  // Libère la mémoire allouée pour newAvails
            exit(1);
        }

        // Appeler récursivement buildTree() pour construire le sous-arbre
        buildTree(parent->sons[i], newDepth, newAvails, nbSons - 1);

        // Libérer la mémoire de newAvails après utilisation
        free(newAvails);
    }
}
