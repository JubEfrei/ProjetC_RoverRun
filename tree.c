
//
// Created by jubos on 12/11/2024.
//

#include <stdlib.h>
#include <stdio.h>
#include "tree.h"
#include "map.h"
#include "loc.h"
#include "moves.h"

        t_node *createNode(int value, int depth, int *availablemvt, t_node *parent, t_localisation loc, int status, int next_phase, int case_cost) {
    if (value < -1 || value > 6) { // Vérification des valeurs autorisées
        printf("Erreur : tentative de création d'un nœud avec une valeur invalide (%d).\n", value);
        return NULL; // Retourne NULL pour éviter d'ajouter un nœud incorrect
    }

    t_node *newNode = (t_node *)malloc(sizeof(t_node));
    if (newNode == NULL) {
        printf("Erreur d'allocation mémoire pour le nœud.\n");
        exit(1);
    }

    newNode->value = value;
    newNode->depth = depth;
    newNode->parent = parent;
    newNode->loc = loc;
    newNode->nbSons = 0;
    newNode->sons = NULL;
    newNode->availablemvt = availablemvt;
    newNode->status = status;
    newNode->next_phase = next_phase;
    newNode->case_cost = case_cost;

    printf("Nœud initialisé : valeur = %d, profondeur = %d, localisation = (%d, %d, %d).\n",
           value, depth, loc.pos.x, loc.pos.y, loc.ori);

    return newNode;
}






int *updateAvails(int *parentAvails, int currentChoice, int nbSons) {
    // Vérification des arguments d'entrée
    if (parentAvails == NULL || nbSons <= 0) {
        printf("Erreur : `parentAvails` est NULL ou `nbSons` est invalide (%d).\n", nbSons);
        return NULL;
    }

    // Allocation de mémoire pour le tableau des mouvements mis à jour
    int *newAvails = (int *)malloc((nbSons - 1) * sizeof(int));
    if (newAvails == NULL) {
        printf("Erreur d'allocation mémoire pour `newAvails`.\n");
        return NULL;
    }

    // Remplissage du tableau en excluant `currentChoice`
    int index = 0;
    for (int i = 0; i < nbSons; i++) {
        if (parentAvails[i] < 0 || parentAvails[i] > 6) {
            // Vérifie si un mouvement invalide est détecté
            printf("Erreur : mouvement invalide détecté dans `parentAvails` (%d) à l'index %d.\n", parentAvails[i], i);
            continue; // Ignorer les mouvements invalides
        }

        if (parentAvails[i] != currentChoice) {
            // Ajouter le mouvement au nouveau tableau s'il ne correspond pas à `currentChoice`
            newAvails[index++] = parentAvails[i];
        }
    }

    // Vérification post-traitement
    if (index != nbSons - 1) {
        printf("Attention : certains mouvements ont été ignorés lors de `updateAvails`. Index final = %d, attendu = %d.\n", index, nbSons - 1);
    }

    // Affichage des mouvements mis à jour (pour débogage)
    printf("Mouvements disponibles après suppression de %d : ", currentChoice);
    for (int i = 0; i < index; i++) {
        printf("%d ", newAvails[i]);
    }
    printf("\n");

    return newAvails;
}

void buildTree(t_node *parent, int depth, int *avails, int nbSons, t_map map) {
    if (depth == 5) {
        printf("Profondeur maximale atteinte pour le nœud de localisation (%d, %d, %d).\n",
               parent->loc.pos.x, parent->loc.pos.y, parent->loc.ori);
        return;
    }

    parent->sons = (t_node **)malloc(nbSons * sizeof(t_node *));
    if (parent->sons == NULL) {
        printf("Erreur d'allocation mémoire pour les fils.\n");
        exit(1);
    }
    parent->nbSons = nbSons;
    printf("Mémoire allouée pour %d fils du nœud à la profondeur %d.\n", nbSons, depth);

    for (int i = 0; i < nbSons; i++) {
        int currentMove = avails[i];
        if (currentMove < 0 || currentMove > 6) {
            printf("Erreur : mouvement invalide détecté (%d) à l'index %d.\n", currentMove, i);
            continue;
        }

        int newDepth = depth + 1;
        int status = (newDepth == 5) ? 1 : 0;

        t_localisation newLoc = parent->loc; // Copie locale de la localisation du parent
        updateLocalisation(&newLoc, currentMove);

        if (!isValidLocalisation(newLoc.pos, map.x_max, map.y_max)) {
            printf("Position invalide détectée pour le mouvement %d : (%d, %d).\n",
                   currentMove, newLoc.pos.x, newLoc.pos.y);
            continue; // Ignore ce mouvement
        }

        int *newAvails = updateAvails(avails, currentMove, nbSons);
        if (newAvails == NULL) {
            printf("Erreur dans l'allocation de `newAvails` pour le mouvement %d.\n", currentMove);
            continue;
        }

        int nodeCost = map.costs[newLoc.pos.x][newLoc.pos.y];
        int soilType = map.soils[newLoc.pos.x][newLoc.pos.y];

        if (soilType == CREVASSE) {
            printf("Le robot est tombé dans une crevasse.\n");
            parent->sons[i] = createNode(currentMove, newDepth, newAvails, parent, newLoc, 3, status, nodeCost);
            free(newAvails);
            continue;
        }

        if (soilType == BASE_STATION) {
            printf("Vous êtes arrivé à la base.\n");
            parent->sons[i] = createNode(currentMove, newDepth, newAvails, parent, newLoc, 2, status, nodeCost);
            free(newAvails);
            continue;
        }

        printf("Le nouveau nœud a comme coordonnées : (%d, %d), coût : %d.\n",
               newLoc.pos.x, newLoc.pos.y, nodeCost);

        parent->sons[i] = createNode(currentMove, newDepth, newAvails, parent, newLoc, status, parent->next_phase, nodeCost);
        buildTree(parent->sons[i], newDepth, newAvails, nbSons - 1, map);
        free(newAvails);
    }
}

// Fonction pour afficher un nœud
void printNode(t_node *node) {
    if (!node) return;
    printf("Nœud : valeur = %d, profondeur = %d, localisation = (%d, %d, %d), coût = %d, status = %d\n",
           node->value, node->depth, node->loc.pos.x, node->loc.pos.y, node->loc.ori, node->case_cost, node->status);
}
// Fonction pour afficher l'arbre (parcours préfixe)
void printTree(t_node *node, int level) {
    if (!node) return;
    for (int i = 0; i < level; i++) printf("  ");
    printNode(node);
    for (int i = 0; i < node->nbSons; i++) {
        printTree(node->sons[i], level + 1);
    }
}







void freeTree(t_node *node) {
    if (node == NULL) {
        return;
    }

    // Libérer les fils
    for (int i = 0; i < node->nbSons; i++) {
        freeTree(node->sons[i]);
    }

    // Libérer les ressources du nœud
    free(node->sons);
    free(node->availablemvt);
    free(node);
}

void copyLocalisation(t_localisation *dest, t_localisation *src) {

    if (dest == NULL || src == NULL) {

        return;

    }

    dest->pos.x = src->pos.x;

    dest->pos.y = src->pos.y;

    dest->ori = src->ori;

}


void findOptimalPath(t_node *node, t_map map, int currentCost, int *bestCost, t_node **bestLeaf, t_localisation loc) {
    if (node == NULL) {
        return;
    }

    // Vérification de la validité du mouvement
    if (node->value < -1 || node->value >= 7) {
        printf("Erreur : valeur de mouvement invalide détectée dans le nœud (%d).\n", node->value);
        return;
    }

    // Calcul du coût du terrain
    int terrainCost = map.costs[loc.pos.y][loc.pos.x];
    currentCost += terrainCost;

    printf("Exploration du noeud : valeur = %d, coût actuel = %d, localisation = (%d, %d, %d).\n",
           node->value, currentCost, loc.pos.x, loc.pos.y, loc.ori);

    if (node->nbSons == 0) { // C'est une feuille
        printf("Feuille atteinte : coût = %d, localisation = (%d, %d, %d).\n",
               currentCost, loc.pos.x, loc.pos.y, loc.ori);
        if (currentCost < *bestCost) {
            printf("Mise à jour du meilleur coût : ancien = %d, nouveau = %d.\n", *bestCost, currentCost);
            *bestCost = currentCost;
            *bestLeaf = node;
        }
        return;
    }

    // Parcours des fils
    for (int i = 0; i < node->nbSons; i++) {
        t_node *child = node->sons[i];
        t_localisation newLoc = move(loc, (t_move)child->value);

        // Vérification de localisation valide
        if (!isValidLocalisation(newLoc.pos, map.x_max, map.y_max)) {
            printf("Position invalide lors du mouvement %d.\n", child->value);
            continue;
        }

        findOptimalPath(child, map, currentCost, bestCost, bestLeaf, newLoc);
    }
}

// Fonction pour afficher le chemin optimal
void printOptimalPath(t_node *bestLeaf) {
    if (bestLeaf == NULL) {
        printf("Aucun chemin optimal trouvé.\n");
        return;
    }

    printf("Chemin optimal (coût total = %d):\n", bestLeaf->case_cost);
    t_node *current = bestLeaf;

    // Stocker les mouvements dans un tableau pour les afficher dans l'ordre correct
    int path[100]; // Assurez-vous que ce tableau est assez grand
    int index = 0;

    while (current != NULL) {
        path[index++] = current->value;
        current = current->parent;
    }

    // Affichage du chemin en partant de la racine
    for (int i = index - 1; i >= 0; i--) {
        printf("%d ", path[i]);
    }
    printf("\n");
}

