//
// Created by jubos on 12/11/2024.
//

#include <malloc.h>
#include <assert.h>
#include <stdio.h>
#include "tree.h"


t_node *createNode(int value, int depth, t_move *availablemvt, t_node *parent, t_localisation *loc)
{
    t_node *node = (t_node *)malloc(sizeof(t_node));
    node->value = value;
    node->depth = depth;
    node->availablemvt = availablemvt;
    node->nbSons = 9 - depth;
    node->parent = parent;
    node->loc = loc;
    return node;
}
t_move *updateAvails(t_move *parentAvails, t_move currentChoice, int nbSons) {

    int i, j = 0;
    t_move *newAvails = (t_move *)malloc(nbSons * sizeof(t_move));
    if (newAvails == NULL) {
        printf("Erreur d'allocation mémoire pour updateAvails\n");
        exit(1);  // Terminer en cas d'erreur critique
    }

    for (i = 0; i < nbSons; i++) {
        if (parentAvails[i] != currentChoice) {
            newAvails[j] = parentAvails[i];
            j++;
        }
    }

    return newAvails;
}


void buildTree(t_node *parent, int depth, t_move *avails, int nbSons) {
    // Vérifie si la profondeur maximale est atteinte
    if (depth == 5) {
        return;  // Si on atteint la profondeur 5, on arrête la récursion
    }

    // Vérification de la validité des paramètres
    if (parent == NULL || avails == NULL || nbSons <= 0) {
        printf("Erreur: paramètres invalides pour buildTree\n");
        return;
    }

    // Alloue de la mémoire pour les nœuds fils du nœud parent
    parent->sons = (t_node **)malloc(nbSons * sizeof(t_node *));
    if (parent->sons == NULL) {
        printf("Erreur d'allocation mémoire pour les fils\n");
        exit(1);  // Terminer en cas d'erreur critique
    }
    parent->nbSons = nbSons;

    // Parcours des choix disponibles et création des sous-arbres
    for (int i = 0; i < nbSons; i++) {
        t_move move = avails[i];  // Choix actuel pour le nœud fils
        int newDepth = depth + 1;
        t_localisation *new_loc = parent->loc;

        updateLocalisation(new_loc, move);

        // Créer une copie des choix restants sans le choix actuel
        t_move *newAvails = updateAvails(avails, move, nbSons);

        // Créer le nœud fils
        parent->sons[i] = createNode(move, newDepth, newAvails, parent, new_loc);

        // Appeler récursivement buildTree() pour construire le sous-arbre
        buildTree(parent->sons[i], newDepth, newAvails, nbSons - 1);  // nbSons doit diminuer à chaque niveau

        // Libérer la mémoire de newAvails après utilisation
        free(newAvails);
    }
}



void printTreeValues(t_node *root) {
    if (root->depth == 5) {
        printf("End tree\n");
        return;  // Cas de base : si le nœud est NULL, on ne fait rien
    }


    // Affiche la valeur du nœud actuel
    printf("Valeur du nœud: %d\n", root->value);

    // Parcours récursif des fils
    printf("n = %d\n", root->nbSons);
    for (int i = 0; i < root->nbSons; i++) {
        printf("d1 = %d\n", root->depth);
        printTreeValues(root->sons[i]);  // Appel récursif pour chaque fils
        printf("d2 = %d\n", root->depth);
    }
}

void printTree(t_node *root, int level) {
    if (root == NULL) return;  // Si le nœud est NULL, on ne fait rien

    // Affiche les informations du nœud avec une indentation correspondant à la profondeur
    for (int i = 0; i < level; i++) {
        printf("  ");  // Ajoute des espaces pour indiquer la profondeur
    }
    printf("Nœud: valeur = %d, profondeur = %d, nbSons = %d\n", root->value, root->depth, root->nbSons);

    // Affiche les mouvements disponibles pour ce nœud
    for (int i = 0; i < level; i++) {
        printf("  ");  // Même indentation pour aligner les informations
    }
    printf("  Mouvements disponibles : ");
    if (root->availablemvt != NULL) {
        for (int i = 0; root->availablemvt[i] != -1; i++) {
            printf("%d ", root->availablemvt[i]);
        }
    }
    printf("-1\n");  // Indique la fin de la liste des mouvements disponibles

    // Parcours récursif des fils
    for (int i = 0; i < root->nbSons; i++) {
        printTree(root->sons[i], level + 1);  // Appelle récursivement pour chaque fils
    }
}

