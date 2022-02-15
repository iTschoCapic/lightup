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

bool test_game_load(char *filename)
{
    game g = game_load(filename);
    ASSERT(g);
    game g2 = game_default();
    ASSERT(g2);
    ASSERT(game_equal(g, g2));
    game_delete(g);
    game_delete(g2);
    return true;
}

bool test_game_save(char *filename)
{
    game g = game_new_empty_ext(7, 7, false);
    ASSERT(g);
    game_save(g, filename);
    ASSERT(filename != NULL);
    game g2 = game_new_empty_ext(7, 7, false);
    ASSERT(game_equal(g, g2));
    game_delete(g);
    game_delete(g2);
    remove("test_save.txt");
    return true;
}

int main(int argc, char *argv[])
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
        ok = test_game_load(argv[1]);
    else if (strcmp("game_save", argv[1]) == 0)
        ok = test_game_save(argv[1]);
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