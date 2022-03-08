#include "game_tools.h"

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "queue.h"

game game_load(char *filename)
{
    // check_if_error(filename == NULL, "Address invalid !");
    FILE *fichier = fopen(filename, "r");
    if (!fichier)
    {
        perror("File opening failed");
        return NULL;
    }
    int tab[3];
    fscanf(fichier, "%d %d %d\n", &tab[0], &tab[1], &tab[2]);
    game jeu = game_new_empty_ext(tab[0], tab[1], tab[2]);
    for (int i = 0; i < game_nb_rows(jeu); i++)
    {
        for (int j = 0; j < game_nb_cols(jeu); j++)
        {
            char c = fgetc(fichier);
            if (c == '*')
            {
                game_set_square(jeu, i, j, S_LIGHTBULB);
            }
            if (c == '0')
            {
                game_set_square(jeu, i, j, S_BLACK0);
            }
            if (c == '1')
            {
                game_set_square(jeu, i, j, S_BLACK1);
            }
            if (c == '2')
            {
                game_set_square(jeu, i, j, S_BLACK2);
            }
            if (c == '3')
            {
                game_set_square(jeu, i, j, S_BLACK3);
            }
            if (c == '4')
            {
                game_set_square(jeu, i, j, S_BLACK4);
            }
            if (c == 'w')
            {
                game_set_square(jeu, i, j, S_BLACKU);
            }
            if (c == 'b')
            {
                game_set_square(jeu, i, j, S_BLANK);
            }
            if (c == '-')
            {
                game_set_square(jeu, i, j, S_MARK);
            }
        }
        fgetc(fichier);
    }
    fclose(fichier);
    game_update_flags(jeu);
    return jeu;
}

void game_save(cgame g, char *filename)
{
    assert(g);
    assert(filename);
    FILE *save = fopen(filename, "w");
    assert(save);
    int nb_rows = game_nb_rows(g);
    int nb_cols = game_nb_cols(g);
    fprintf(save, "%d %d %d\n", nb_rows, nb_cols, game_is_wrapping(g));
    for (int i = 0; i < nb_rows; i++)
    {
        for (int j = 0; j < nb_cols; j++)
        {
            square state = game_get_state(g, i, j);
            int number;
            switch (state)
            {
                case S_BLANK:
                    fprintf(save, "b");
                    break;
                case S_LIGHTBULB:
                    fprintf(save, "*");
                    break;
                case S_MARK:
                    fprintf(save, "-");
                    break;
                default:
                    number = game_get_black_number(g, i, j);
                    if (number == -1)
                    {
                        fprintf(save, "w");
                    }
                    else
                    {
                        fprintf(save, "%d", number);
                    }
                    break;
            }
        }
        fprintf(save, "\n");
    }
    fclose(save);
}

/**
 * @brief Auxiliary recursive function for game_solve
 *
 * @param g The game to solve
 * @param pos The current position in the grid
 * @param len The number of cases in the game
 * @return true if there is a solution
 * @return false otherwise
 */
bool solve_rec(game g, int pos, int len)
{
    if (pos == len)
    {
        return game_is_over(g);
    }
    int nb_cols = game_nb_cols(g);
    int nb_rows = game_nb_rows(g);
    int row = pos / nb_cols;
    int col = pos % nb_cols;
    bool flag = false;
    if (game_is_black(g, row, col) || game_is_lighted(g, row, col))
    {
        return solve_rec(g, pos + 1, len);
    }
    game_play_move(g, row, col, S_LIGHTBULB);
    for (int i = 0; i < nb_rows; i++)
    {
        for (int j = 0; j < nb_cols; j++)
        {
            if (game_has_error(g, i, j))
            {
                game_play_move(g, row, col, S_BLANK);
                return solve_rec(g, pos + 1, len);
            }
        }
    }
    flag = solve_rec(g, pos + 1, len);
    if (!flag)
    {
        game_play_move(g, row, col, S_BLANK);
        flag = solve_rec(g, pos + 1, len);
    }
    return flag;
}

bool game_solve(game g)
{
    game sol = game_copy(g);
    int nb_cols = game_nb_cols(g);
    int nb_rows = game_nb_rows(g);
    int len = nb_cols * nb_rows;
    if (solve_rec(sol, 0, len))
    {
        for (int i = 0; i < nb_rows; i++)
        {
            for (int j = 0; j < nb_cols; j++)
            {
                game_set_square(g, i, j, game_get_square(sol, i, j));
            }
        }
        game_delete(sol);
        return true;
    }
    game_delete(sol);
    return false;
}

/**
 * @brief Auxiliary recursive function for game_nb_solutions
 *
 * @param g a game (will be modified)
 * @param pos The current position in the grid
 * @param len The number of cases in the game
 * @param count The pointer that stores the number of solutions
 */
void nb_sol_rec(game g, int pos, int len, int *count)
{
    if (pos == len)
    {
        if (game_is_over(g))
        {
            (*count)++;
            game_print(g);
        }
        return;
    }
    int nb_cols = game_nb_cols(g);
    int nb_rows = game_nb_rows(g);
    int row = pos / nb_cols;
    int col = pos % nb_cols;
    if (game_is_black(g, row, col) || game_is_lighted(g, row, col))
    {
        nb_sol_rec(g, pos + 1, len, count);
        return;
    }
    game_play_move(g, row, col, S_LIGHTBULB);
    for (int i = 0; i < nb_rows; i++)
    {
        for (int j = 0; j < nb_cols; j++)
        {
            if (game_has_error(g, i, j))
            {
                game_play_move(g, row, col, S_BLANK);
                nb_sol_rec(g, pos + 1, len, count);
                return;
            }
        }
    }
    nb_sol_rec(g, pos + 1, len, count);
    game_play_move(g, row, col, S_BLANK);
    nb_sol_rec(g, pos + 1, len, count);
    return;
}

uint game_nb_solutions(cgame g)
{
    int cpt = 0;
    game sol = game_copy(g);
    game_restart(sol);
    int nb_cols = game_nb_cols(sol);
    int nb_rows = game_nb_rows(sol);
    int len = nb_cols * nb_rows;
    nb_sol_rec(sol, 0, len, &cpt);
    game_delete(sol);
    return cpt;
}