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
    game_restart(sol);
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

/* ************************************************************************** */

uint nb_neigh_lightbulbs(cgame g, uint i, uint j)
{
    assert(g);
    int nb_rows = game_nb_rows(g);
    int nb_cols = game_nb_cols(g);
    assert(i < nb_rows);
    assert(j < nb_cols);

    int i_up = i - 1;
    int i_down = i + 1;
    int j_left = j - 1;
    int j_right = j + 1;
    if (game_is_wrapping(g))
    {
        i_up = (i_up + nb_rows) % nb_rows;
        i_down = (i_down + nb_rows) % nb_rows;
        j_left = (j_left + nb_cols) % nb_cols;
        j_right = (j_right + nb_cols) % nb_cols;
    }

    uint count = 0;
    if ((i_up >= 0) && game_is_lightbulb(g, i_up, j)) count++;
    if ((i_down < nb_rows) && game_is_lightbulb(g, i_down, j)) count++;
    if ((j_left >= 0) && game_is_lightbulb(g, i, j_left)) count++;
    if ((j_right < nb_cols) && game_is_lightbulb(g, i, j_right)) count++;

    return count;
}

/* ************************************************************************** */

uint nb_unlit_squares(cgame g)
{
    int nb = 0;
    for (uint i = 0; i < game_nb_rows(g); i++)
        for (uint j = 0; j < game_nb_cols(g); j++)
            if (!game_is_lighted(g, i, j) && game_is_blank(g, i, j)) nb++;
    return nb;
}

/* ************************************************************************** */

/**
 * Create a random game with a given size and number of walls
 *
 * @param nb_rows the number of rows of the game
 * @param nb_cols the number of columns of the game
 * @param wrapping wrapping option
 * @param nb_walls the number of walls to add
 * @param with_solution if true, the game contains the solution, otherwise only walls
 *
 * @return the generated random game
 */

game game_random(uint nb_rows, uint nb_cols, bool wrapping, uint nb_walls, bool with_solution)
{
    assert(nb_walls <= nb_rows * nb_cols);

    // step 0: create an empty game
    game g = game_new_empty_ext(nb_rows, nb_cols, wrapping);

    // step 1: add random black walls
    uint k = 0;
    while (k < nb_walls)
    {
        uint i = rand() % nb_rows;
        uint j = rand() % nb_cols;
        if (!game_is_black(g, i, j))
        {
            game_set_square(g, i, j, S_BLACKU);
            k++;
        }
    }
    game_update_flags(g);

    // step 2: add lightbulbs until every squares are lighted
    uint nb_unlit = nb_unlit_squares(g);
    while (nb_unlit != 0)
    {
        uint random_unlit_num = rand() % nb_unlit;
        uint num = 0;
        for (uint i = 0; i < game_nb_rows(g); i++)
            for (uint j = 0; j < game_nb_cols(g); j++)
            {
                if (!game_is_lighted(g, i, j) && game_is_blank(g, i, j))
                {
                    if (num == random_unlit_num)
                    {
                        game_set_square(g, i, j, S_LIGHTBULB);
                        game_update_flags(g);
                    }
                    num++;
                }
            }
        nb_unlit = nb_unlit_squares(g);
    }

    // step 3 : set some black wall numbers
    for (uint i = 0; i < game_nb_rows(g); i++)
        for (uint j = 0; j < game_nb_cols(g); j++)
        {
            if (game_is_black(g, i, j))
            {
                if (rand() % 2 == 0)
                {
                    int nb_lightbulbs = nb_neigh_lightbulbs(g, i, j);
                    game_set_square(g, i, j, S_BLACK + nb_lightbulbs);
                }
            }
        }

    // check
    assert(game_is_over(g));

    if (!with_solution) game_restart(g);
    return g;
}

/* ************************************************************************** */
