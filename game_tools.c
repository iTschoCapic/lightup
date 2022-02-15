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