#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"

int main()
{
    game current_game = game_default();
    while (!game_is_over(current_game))
    {
        game_print(current_game);
        uint i, j;
        for (i = 0; i < game_nb_rows(current_game); i++)
        {
            for (j = 0; j < game_nb_cols(current_game); j++)
            {
                if (game_has_error(current_game, i, j))
                {
                    square s_tmp = game_get_state(current_game, i, j);  // Vérification et affichage des erreurs case par case
                    if (s_tmp == S_LIGHTBULB)
                    {
                        printf("Error at lightbulb (%u,%u)\n", i, j);
                    }
                    else
                    {
                        printf("Error at wall (%u,%u)\n", i, j);
                    }
                }
            }
        }
        printf("Enter your command: [h for help]\n");
        char cmd;
        int ret = scanf(" %c", &cmd);  // Scan de la commande
        if (ret == EOF)
        {
            printf("End of File!\n");
            return EXIT_SUCCESS;
        }
        else if (cmd == 'h')
        {
            printf("Commands : \n");
            printf("  - Type 'l <i> <j>' to place a lightbulb at coordinates (i,j).\n");
            printf("  - Type 'm <i> <j>' to place a mark at coordinates (i,j).\n");
            printf("  - Type 'b <i> <j>' to empty the square at coordinates (i,j).\n");
            printf("  - Type 'z' to undo the last move.\n");
            printf("  - Type 'y' to redo the last move.\n");
            printf("  - Type 'r' to restart.\n");
            printf("  - Type 'q' to quit.\n");
        }
        else if (cmd == 'r')
        {  // Restart
            printf(">Game restarted");
            game_restart(current_game);
        }
        else if (cmd == 'q')
        {
            current_game = game_default_solution();
            game_print(current_game);  // Abandon + affichage de la solution
            printf("shame\n");
            game_delete(current_game);
            return EXIT_SUCCESS;
        }
        else if (cmd == 'z')
        {
            printf("Last move undone.\n");
            game_undo(current_game);
        }
        else if (cmd == 'y')
        {
            printf("Last move redone.\n");
            game_redo(current_game);
        }
        else if (cmd == 'l' || cmd == 'm' || cmd == 'b')
        {
            ret = scanf("%u %u", &i, &j);  // Si la commande est un placement, alors récupération des coordonnées
            if (ret == EOF)
            {
                printf("End of File!\n");
                return EXIT_SUCCESS;
            }
            else if (ret < 1)
            {
                printf("Invalid command\nType h for help\n");
            }
            else
            {
                if (i >= DEFAULT_SIZE || j >= DEFAULT_SIZE)
                {  // Vérification des coordonnées
                    printf(
                        "Coordinates are out of the grid. Please enter a value "
                        "between 0 and 6.\n");
                }
                else
                {
                    if (game_check_move(current_game, i, j, game_get_state(current_game, i, j)))
                    {
                        if (cmd == 'l')
                        {
                            game_play_move(current_game, i, j, S_LIGHTBULB);
                            printf("Lightbulb placed at square (%u,%u)\n", i, j);
                        }
                        if (cmd == 'm')
                        {  // Actualisation de la case concernée en fonction de
                           // la commande
                            game_play_move(current_game, i, j, S_MARK);
                            printf("Marked placed at square (%u,%u)\n", i, j);
                        }
                        if (cmd == 'b')
                        {
                            game_play_move(current_game, i, j, S_BLANK);
                            printf("Blank placed at square (%u,%u)\n", i, j);
                        }
                    }
                }
            }
        }
        else
        {
            printf("Invalid command.\nType h for help.\n");
        }
        printf("\n");
        char clear;  // Efface le buffer afin d'ignorer toute accumulation de caractères à l'intérieur menant à une suite de commandes invoulues à cause du scanf.
        while ((clear = getchar()) != '\n' && clear != EOF)
        {
        }
    }
    game_print(current_game);
    game_delete(current_game);
    printf("congratulation\n");
    return EXIT_SUCCESS;
}
