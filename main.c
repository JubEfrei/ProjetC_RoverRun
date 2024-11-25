#include <stdio.h>
#include <stdlib.h>
#include "map.h"
#include "tree.h"
#include "loc.h"
#include "moves.h"

#define NB_MOVES 9

int main() {
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

    // Initialisation des choix disponibles pour les tests
    t_move * avails = getRandomMoves(NB_MOVES);
    int nbSons = NB_MOVES;  // Nombre de choix disponibles

    // Test de createNode
    printf("Test de createNode...\n");
    t_orientation base_orientation = NORTH;
    t_localisation base_loc = loc_init(0, 0, base_orientation);
    t_node *root = createNode(-1, 0, avails, NULL, &base_loc);  // Création de la racine
    if (root != NULL) {
        printf("Création de la racine réussie : valeur = %d, profondeur = %d\n", root->value, root->depth);
    }

    // Test de updateAvails
    printf("\nTest de updateAvails...\n");
    int currentChoice = 3;  // Retirer le choix "3"
    t_move *newAvails = updateAvails(avails, currentChoice, nbSons);
    if (newAvails != NULL) {
        printf("Mouvements disponibles après updateAvails : ");
        for (int i = 0; newAvails[i] != -1; i++) {
            printf("%d ", newAvails[i]);
        }
        printf("-1\n");
        free(newAvails);  // Libérer la mémoire allouée
    }

    // Test de buildTree
    printf("\nTest de buildTree...\n");
    buildTree(root, 0, avails, nbSons);
    printf("Construction de l'arbre terminée.\n");

    printf("\nAffichage de l'arbre :\n");
    printTree(root, 0);


    return 0;
}
