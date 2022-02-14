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
    //check_if_error(filename == NULL, "Address invalid !");
    FILE *fichier = fopen( filename , "r");
    if(!fichier) {
        perror("File opening failed");
        return NULL;
    }
    game jeu;
    int tab[3];
    fscanf(fichier , "%d %d %d" , &tab[0] , &tab[1] , &tab[2]);
    jeu = game_new_empty_ext(tab[0] , tab[1], tab[2]);
    char chaine[7];
    int height = 0;
    while( fscanf(fichier , " %c%c%c%c%c%c%c" , &chaine[0] , &chaine[1], &chaine[2] ,&chaine[3] ,&chaine[4] ,&chaine[5] ,&chaine[6]) != EOF){
        for(int i = 0 ; i < 7 ; i++){
            if(chaine[i] == '*'){
                game_set_square(jeu , height , i , S_LIGHTBULB);
            }
            if(chaine[i] == '0'){
                game_set_square(jeu , height , i , S_BLACK0);
            }
            if(chaine[i] == '1'){
                game_set_square(jeu , height , i , S_BLACK1);
            }
            if(chaine[i] == '2'){
                game_set_square(jeu , height , i , S_BLACK2);
            }
            if(chaine[i] == '3'){
                game_set_square(jeu , height , i , S_BLACK3);
            }
            if(chaine[i] == '4'){
                game_set_square(jeu , height , i , S_BLACK4);
            }
            if(chaine[i] == 'w'){
                game_set_square(jeu , height , i , S_BLACKU);
            }
            if(chaine[i] == 'b'){
                game_set_square(jeu , height , i , S_BLANK);
            }
        }
        height++;
    }
    fclose(fichier);
    game_update_flags(jeu);
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