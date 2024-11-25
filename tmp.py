void buildTree(t_node *parent, int depth, t_move *avails, int nbSons, t_map *map) {
    // Vérifie si la profondeur maximale est atteinte
    if (depth == 5) {
        return;  // Si on atteint la profondeur 5, on arrête la récursion
    }
    printf("%d\n", depth);
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
        int status = 0;
        t_localisation *new_loc = parent->loc;

        if (map->soils[new_loc->pos.x][new_loc->pos.y] == ERG)
        {
            switch (move)
            {
                case F_10:
                    break;
                case B_10:
                    break;
                case F_20:
                    updateLocalisation(new_loc, F_10);
                    break;
                case F_30:
                    updateLocalisation(new_loc, F_20);
                    break;
                default:
                    updateLocalisation(new_loc, move);
                    break;
            }
        }
        else
        {
            updateLocalisation(new_loc, move);
        }

        int new_value = map->costs[new_loc->pos.x][new_loc->pos.y];
        printf("cost is %d and pos is %d %d\n", new_value, new_loc->pos.x, new_loc->pos.y);

        // Créer une copie des choix restants sans le choix actuel
        t_move *newAvails = updateAvails(avails, move, nbSons);

        // Créer le nœud fils
        if (newDepth == 5)
        {
            status = 1;
        }

        if (isValidLocalisation(new_loc->pos, map->x_max, map->y_max) == 1){
            if (map->soils[new_loc->pos.x][new_loc->pos.y] == REG)
            {
                parent->sons[i] = createNode(new_value, newDepth, newAvails, parent, new_loc, 4, status);

                // Appeler récursivement buildTree() pour construire le sous-arbre
                buildTree(parent->sons[i], newDepth, newAvails, nbSons - 1, map);  // nbSons doit diminuer à chaque niveau

                // Libérer la mémoire de newAvails après utilisation
                free(newAvails);
            }
            else
            {
                if (map->soils[new_loc->pos.x][new_loc->pos.y] == CREVASSE)
                {
                    printf("Le robot est tombé dans une crevasse.\n");
                    parent->sons[i] = createNode(new_value, newDepth, newAvails, parent, new_loc, parent->next_move, 3);

                    free(newAvails);
                }
                else
                {
                    if (map->soils[new_loc->pos.x][new_loc->pos.y] == BASE_STATION)
                    {
                        parent->sons[i] = createNode(new_value, newDepth, newAvails, parent, new_loc, parent->next_move, 2);
                        printf("Le rover est arrivé à destination!!!\n");
                        free(newAvails);
                    }
                    else
                    {
                        parent->sons[i] = createNode(new_value, newDepth, newAvails, parent, new_loc, parent->next_move, status);

                        // Appeler récursivement buildTree() pour construire le sous-arbre
                        buildTree(parent->sons[i], newDepth, newAvails, nbSons - 1, map);  // nbSons doit diminuer à chaque niveau

                        // Libérer la mémoire de newAvails après utilisation
                        free(newAvails);
                    }
                }
            }
        }
        else
        {
            printf("La position n'est pas valide\n");
            parent->sons[i] = createNode(10000, newDepth, newAvails, parent, new_loc, parent->next_move, 3);
            free(newAvails);

        }
    }
}
