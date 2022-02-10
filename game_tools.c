#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "queue.h"
#include "game_tools.h"

game game_load(char *filename){
    check_if_error(filename == NULL, "Address invalid !");
    FILE *fichier = fopen("filename" , "r");
    game jeu;
    int size = 3;
    int tab[size];
    int cmp =0;
    int c ;
    while( cmp != size-1){
        c = fgetc(fichier);
        if(c == EOF){
            check_if_error(true, "Address invalid !");
        }
        if(c != ' '){
            if(c > 0 || c <=10){
                tab[cmp] = c;
                cmp++;
            }
        }
    }
    jeu = game_new_empty_ext(tab[0] , tab[1], tab[2]);
    int height = 0 ;
    int width = 0 ;
    while( (c = fgetc(fichier)) != EOF){
        if(c == '*'){
            game_play_move(jeu , height , width , S_LIGHTBULB);
            width++;
        }
        if(c == '0'){
            game_play_move(jeu , height , width , S_BLACK0);
            width++;
        }
        if(c == '1'){
            game_play_move(jeu , height , width , S_BLACK1);
            width++;
        }
        if(c == '2'){
            game_play_move(jeu , height , width , S_BLACK2);
            width++;
        }
        if(c == '3'){
            game_play_move(jeu , height , width , S_BLACK3);
            width++;
        }
        if(c == '4'){
            game_play_move(jeu , height , width , S_BLACK4);
            width++;
        }
        if(c == 'w'){
            game_play_move(jeu , height , width , S_BLACKU);
            width++;
        }
        if(c == 'b'){
            
        }

    }
    return jeu;
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