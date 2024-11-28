#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "map.h"
#include "moves.h"


// Fonction principale de test

int main() {
    printf("Chargement de la carte...\n");

    t_map map = createMapFromFile("..\\maps\\example1.map");
    printf("Map created with dimensions %d x %d\n", map.y_max, map.x_max);
    for (int i = 0; i < map.y_max; i++)
    {
        for (int j = 0; j < map.x_max; j++)
        {
            printf("%d ", map.soils[i][j]);
        }
        printf("\n");
    }
    // printf the costs, aligned left 5 digits
    for (int i = 0; i < map.y_max; i++)
    {
        for (int j = 0; j < map.x_max; j++)
        {
            printf("%-5d ", map.costs[i][j]);
        }
        printf("\n");
    }
    displayMap(map);

    printf("Carte chargée avec succès.\n");

    // Initialisation des mouvements disponibles
    int avails[] = {F_10, F_20, F_30, B_10, T_LEFT, T_RIGHT, U_TURN};

    printf("Initialisation de l'arbre...\n");

    // Création de la racine de l'arbre
    t_localisation rootLoc = {1, 1, NORTH};
    t_node *root = createNode(-1, 0, avails, NULL, rootLoc, 0, 0, 0);
    printNode(root);

    printf("Construction de l'arbre...\n");
    buildTree(root, 0, avails, 7, map);

    int bestCost = 10000;
    t_node *bestLeaf = NULL;
    findOptimalPath(root, map, 0, &bestCost, &bestLeaf, rootLoc);
    printf("Tests terminés.\n");

    printf("Le meilleur chemin arrive à la case en %d, %d avec une case de cout : %d", bestLeaf->loc.pos.x, bestLeaf->loc.pos.y, bestCost);


    return 0;
}