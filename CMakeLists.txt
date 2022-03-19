cmake_minimum_required(VERSION 3.0)
project(lightup C)

include(CTest)
enable_testing()

set(CMAKE_C_FLAGS "-Wall -std=c99")            # options de compilation génériques
set(CMAKE_C_FLAGS_DEBUG "-g -D DEBUG -O0 --coverage")   # options de compilation en mode Debug
set(CMAKE_C_FLAGS_RELEASE "-O3")               # options de compilation en mode Release

## use a CMake3 package to solve SDL2 dependencies
find_package(SDL2 REQUIRED)
set(SDL2_ALL_INC ${SDL2_INCLUDE_DIRS})
set(SDL2_ALL_LIBS ${SDL2_LIBRARIES} -lSDL2_ttf -lSDL2_image) # add extra SDL2 libraries

#Executable tests:
add_executable(game_text game_text.c)
add_executable(game_solve game_solve.c)

add_executable(game_sdl game_sdl.c model.c)

add_executable(game_test_maaudrain game_test_maaudrain.c)
add_executable(game_test_lorichard game_test_lorichard.c)
add_executable(game_test_abdouldiallo game_test_abdouldiallo.c)
add_executable(game_test_arledoux game_test_arledoux.c)
add_executable(game_test_rmachecourtb game_test_rmachecourtb.c)

#Libraries:
add_library(game game.c game_aux.c game_tools.c queue.c)

target_link_libraries(game_text game ) 
target_link_libraries(game_test_abdouldiallo game ) 
target_link_libraries(game_test_maaudrain game ) 
target_link_libraries(game_test_arledoux game )
target_link_libraries(game_test_lorichard game )
target_link_libraries(game_test_rmachecourtb game)
target_link_libraries(game_solve game)
target_link_libraries(game_sdl ${SDL2_ALL_LIBS} m game)

#Tests dummy
add_test(test_maaudrain_dummy ./game_test_maaudrain dummy)
add_test(test_abdouldiallo_dummy ./game_test_abdouldiallo dummy)
add_test(test_arledoux_dummy ./game_test_arledoux dummy)


#Tests Maxime:
add_test(test_maaudrain_game_new ./game_test_maaudrain game_new)
add_test(test_maaudrain_game_new_ext ./game_test_maaudrain game_new_ext)
add_test(test_maaudrain_game_new_empty ./game_test_maaudrain game_new_empty)
add_test(test_maaudrain_game_new_empty_ext ./game_test_maaudrain game_new_empty_ext)
add_test(test_maaudrain_game_copy ./game_test_maaudrain game_copy)
add_test(test_maaudrain_game_equal ./game_test_maaudrain game_equal)
add_test(test_maaudrain_game_delete ./game_test_maaudrain game_delete)
add_test(test_maaudrain_game_set_square ./game_test_maaudrain game_set_square)
add_test(test_maaudrain_game_get_square ./game_test_maaudrain game_get_square)
add_test(test_maaudrain_game_print ./game_test_maaudrain game_print)


#Tests Abdoulaye:
add_test(test_abdouldiallo_game_sol ./game_test_abdouldiallo test_game_solution)
add_test(test_abdouldiallo_game_state ./game_test_abdouldiallo test_get_state)
add_test(test_abdouldiallo_game_flags ./game_test_abdouldiallo test_get_flags)
add_test(test_abdouldiallo_game_is_blank ./game_test_abdouldiallo test_is_blank)
add_test(test_abdouldiallo_game_is_lightbulb ./game_test_abdouldiallo test_is_lightbulb)
add_test(test_abdouldiallo_game_is_black ./game_test_abdouldiallo test_is_black)
add_test(test_abdouldiallo_game_black_number ./game_test_abdouldiallo test_get_black_number)
add_test(test_abdouldiallo_game_is_marked ./game_test_abdouldiallo test_is_marked)
add_test(test_abdouldiallo_game_undo ./game_test_abdouldiallo test_undo)
add_test(test_abdouldiallo_game_redo ./game_test_abdouldiallo test_redo)



#Tests Arthur:
add_test(test_arledoux_game_is_lighted ./game_test_arledoux game_is_lighted)
add_test(test_arledoux_game_has_error ./game_test_arledoux game_has_error)
add_test(test_arledoux_game_check_move ./game_test_arledoux game_check_move)
add_test(test_arledoux_game_play_move ./game_test_arledoux game_play_move)
add_test(test_arledoux_game_update_flags ./game_test_arledoux game_update_flags)
add_test(test_arledoux_game_is_over ./game_test_arledoux game_is_over)
add_test(test_arledoux_game_restart ./game_test_arledoux game_restart)
add_test(test_arledoux_game_default ./game_test_arledoux game_default)
add_test(test_arledoux_game_nb_rows ./game_test_arledoux game_nb_rows)
add_test(test_arledoux_game_nb_cols ./game_test_arledoux game_nb_cols)
add_test(test_arledoux_game_is_wrapping ./game_test_arledoux game_is_wrapping)

#Tests Loïc ;
#add_test(test_lorichard_game_load ./game_test_lorichard game_load)
add_test(test_lorichard_game_solve ./game_test_lorichard game_solve)
add_test(test_lorichard_game_nb_solutions ./game_test_lorichard game_nb_solutions)

#Tests Renaud:
add_test(test_rmachecourtb_game_load ./game_test_rmachecourtb game_load)
add_test(test_rmachecourtb_game_save ./game_test_rmachecourtb game_save)

## copy useful data files in build dir
foreach(file "background.png" "mario.png" "arial.ttf")
  file(COPY ${file} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endforeach(file)
