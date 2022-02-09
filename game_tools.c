#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"

game game_load(char *filename){
    return game_default();
}

void game_save(cgame g, char *filename){
    assert(g);
    assert(filename);
    FILE* save = fopen(filename, "w");
    assert(save);
    int nb_rows = game_nb_rows(g);
    int nb_cols = game_nb_cols(g);
    fprintf(save, "%d %d %d\n", nb_rows, nb_cols, game_is_wrapping(g));
    for(int i = 0; i < nb_cols; i++){
        for(int j = 0; j < nb_rows; j++){
            square state = game_get_state(g,i,j);
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
                number = game_get_black_number(g,i,j);
                if(number == -1){
                    fprintf(save, "w");
                }
                else{
                    fprintf(save, "%d", number);
                }
                break;
            }
        }
        fprintf(save, "\n");
    }
    fclose(save);
}