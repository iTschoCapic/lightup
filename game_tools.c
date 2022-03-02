#include "game_tools.h"

#include <assert.h>
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

bool solve_rec(game g, int pos, int len)
{
    int nb_cols = game_nb_cols(g);
    int nb_rows = game_nb_rows(g);
    for (int i = 0; i < nb_rows; i++)
    {
        for (int j = 0; j < nb_cols; j++)
        {
            if (game_has_error(g, i, j))
            {
                return false;
            }
        }
    }
    // stop recursive calls
    if (pos == len)
    {
        game_update_flags(g);
        if (game_is_over(g))
        {
            return true;
        }
        return false;
    }
    int row = pos / nb_cols;
    int col = pos % nb_cols;

    if (game_is_black(g, row, col))
    {
        return solve_rec(g, pos + 1, len);
    }
    if (game_is_lighted(g, row, col))
    {
        return solve_rec(g, pos + 1, len);
    }
    //
    game_set_square(g, row, col, S_BLANK);
    bool flag = solve_rec(g, pos + 1, len);

    if (!flag)
    {
        if (!game_is_lighted(g, row, col))
        {
            game_set_square(g, row, col, S_LIGHTBULB);
            flag = solve_rec(g, pos + 1, len);
        }
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
    return false;
}

uint game_nb_solutions(cgame g)
{
    uint cmp = 0;
    return cmp;
}