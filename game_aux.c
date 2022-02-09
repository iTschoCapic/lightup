#include "game_aux.h"

#include <stdio.h>
#include <stdlib.h>

void game_print(cgame g)
{
    uint i, j; /*i est verticale et j est horizontale*/
    printf("   0123456\n   -------\n");
    for (i = 0; i < DEFAULT_SIZE; i++)
    {
        printf("%u |", i);
        for (j = 0; j < DEFAULT_SIZE; j++)
        {
            if (game_is_blank(g, i, j))
            {
                if (game_is_lighted(g, i, j))
                {
                    printf(".");  // Case vide éclairée
                }
                else
                {
                    printf(" ");  // Case vide
                }
            }
            else if (game_is_black(g, i, j))
            {
                uint n = game_get_black_number(g, i, j);
                if (n == -1)
                {
                    printf("w");  // Case noire
                }
                else
                {
                    printf("%u", n);  // Case noire avec chiffre
                }
            }
            else if (game_is_lightbulb(g, i, j))
            {
                printf("*");  // Ampoule
            }
            else if (game_is_marked(g, i, j))
            {
                printf("-");  // Marque
            }
        }
        printf("|\n");
    }
    printf("   -------\n");
}

game game_default(void)
{
    game default_game = game_new_empty();
    game_set_square(default_game, 0, 2, S_BLACK1);
    game_set_square(default_game, 1, 2, S_BLACK2);
    game_set_square(default_game, 2, 5, S_BLACKU);
    game_set_square(default_game, 2, 6, S_BLACK2);  // Création d'un jeu vide, puis remplissage des cases qui doivent l'être
    game_set_square(default_game, 4, 0, S_BLACK1);  // d'après le modèle de jeu de base
    game_set_square(default_game, 4, 1, S_BLACKU);
    game_set_square(default_game, 5, 4, S_BLACK2);
    game_set_square(default_game, 6, 4, S_BLACKU);
    return default_game;
}

game game_default_solution(void)
{
    game default_solution = game_default();  // Reprise du modèle de base
    game_set_square(default_solution, 0, 0, S_LIGHTBULB);
    game_set_square(default_solution, 0, 3, S_LIGHTBULB);
    game_set_square(default_solution, 1, 1, S_LIGHTBULB);
    game_set_square(default_solution, 1, 6, S_LIGHTBULB);
    game_set_square(default_solution, 2, 2, S_LIGHTBULB);  // Placement des lampes
    game_set_square(default_solution, 3, 6, S_LIGHTBULB);
    game_set_square(default_solution, 4, 4, S_LIGHTBULB);
    game_set_square(default_solution, 5, 0, S_LIGHTBULB);
    game_set_square(default_solution, 5, 5, S_LIGHTBULB);
    game_set_square(default_solution, 6, 1, S_LIGHTBULB);
    game_update_flags(default_solution);  // Mise à jour pour activer les flags F_LIGHTED
    return default_solution;
}