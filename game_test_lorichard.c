#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "queue.h"
#include "game_tools.h"

bool test_game_load(void){
    game jeu_default = game_default();
    game jeu ;
    jeu = game_load("default.txt");
    game_delete(jeu);
    if(!game_equal(jeu_default , jeu)){
        return false;
    }
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
