#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"

int main(int argc, char* argv[])
{
    ;
    game current_game;
    if (argc >= 3 && argc <= 4)
    {
        current_game = game_load(argv[2]);
    }
    else
    {
        return EXIT_FAILURE;
    }
    if (strcmp("-s", argv[1]) == 0)
    {
        game_solve(current_game);
        if (argc == 4)
        {
            game_save(current_game, argv[3]);
        }
        else
        {
            game_print(current_game);
        }
    }
    else if (strcmp("-c", argv[1]) == 0)
    {
        int nb_sol = game_nb_solutions(current_game);
        if (argc == 4)
        {
            FILE* save = fopen(argv[3], "w");
            fprintf(save, "%d\n", nb_sol);
            fclose(save);
        }
        else
        {
            printf("This game has %d solution(s).\n", nb_sol);
        }
    }
    return EXIT_SUCCESS;
}
