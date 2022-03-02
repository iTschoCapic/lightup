#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"
#include "queue.h"

bool test_game_load(void)
{
    game jeu_default = game_default();
    game jeu;
    jeu = game_load("default.txt");
    bool resultat = game_equal(jeu_default, jeu);
    game_delete(jeu);
    game_delete(jeu_default);
    return resultat;
}

bool test_game_solve(void){
    game jeu = game_load("../jeu.txt");
    return game_solve(jeu);
}

bool test_game_nb_solutions(void){
    game jeu = game_load("jeu.txt");
    unsigned int nb_sol = game_nb_solutions(jeu);
    //fprintf(stderr,"%u\n", nb_sol);
    return true;
}

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
        return EXIT_FAILURE;
    }
    // start test
    fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
    bool ok = false;
    if (strcmp("game_load", argv[1]) == 0)
        ok = test_game_load();
    if (strcmp("game_solve", argv[1]) == 0)
        ok = test_game_solve();
    if (strcmp("game_nb_solutions", argv[1]) == 0)
        ok = test_game_nb_solutions();
    else
    {
        fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
        return EXIT_FAILURE;
    }

    // print test result
    if (ok)
    {
        fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
        return EXIT_SUCCESS;
    }
    else
    {
        fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
        return EXIT_FAILURE;
    }
}