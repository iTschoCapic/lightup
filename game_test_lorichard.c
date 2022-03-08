#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"
#include "queue.h"

#define ASSERT(expr)                                                                        \
    do                                                                                      \
    {                                                                                       \
        if ((expr) == 0)                                                                    \
        {                                                                                   \
            fprintf(stderr, "[%s:%d] Assertion '%s' failed!\n", __FILE__, __LINE__, #expr); \
            abort();                                                                        \
        }                                                                                   \
    } while (0)

bool test_game_load(void)
{
    game jeu_default = game_default();
    game jeu;
    jeu = game_load("../games/default.txt");
    bool resultat = game_equal(jeu_default, jeu);
    game_delete(jeu);
    game_delete(jeu_default);
    return resultat;
}

bool test_game_solve(void)
{
    game g1 = game_default();
    game g2 = game_default_solution();
    game_solve(g1);
    ASSERT(game_equal(g1, g2));
    game g3 = game_load("../games/impossible.txt");
    game g4 = game_copy(g3);
    game_solve(g3);
    ASSERT(game_equal(g3, g4));
    game_delete(g1);
    game_delete(g2);
    game_delete(g3);
    game_delete(g4);
    return true;
}

bool test_game_nb_solutions(void)
{
    game jeu = game_load("../games/3x3.txt");
    unsigned int nb_sol = game_nb_solutions(jeu);
    ASSERT(nb_sol == 4);
    game_delete(jeu);
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
    else if (strcmp("game_solve", argv[1]) == 0)
        ok = test_game_solve();
    else if (strcmp("game_nb_solutions", argv[1]) == 0)
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