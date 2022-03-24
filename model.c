#include "model.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>  // required to load transparent texture from PNG
#include <SDL2/SDL_ttf.h>    // required to use TTF fonts
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_ext.h"
#include "game_tools.h"

/* **************************************************************** */

#define FONT "textures/arial.ttf"
#define FONTSIZE 36
#define BACKGROUND "textures/background.png"
#define MARIO "textures/mario.png"

struct Env_t
{
    /* PUT YOUR VARIABLES HERE */
    SDL_Texture *background;
    SDL_Texture *wall;
    SDL_Texture *wall0;
    SDL_Texture *wall1;
    SDL_Texture *wall2;
    SDL_Texture *wall3;
    SDL_Texture *wall4;
    SDL_Texture *lamp;
    SDL_Texture *mark;
    SDL_Texture *title;
    SDL_Rect *cases;
    game jeu;
    int window_width, window_height;
    SDL_Point ligne_depart, ligne_arrivee;
};

/* **************************************************************** */

Env *init(SDL_Window *win, SDL_Renderer *ren, int argc, char *argv[])
{
    Env *env = malloc(sizeof(struct Env_t));
    env->jeu = game_load(argv[1]);
    SDL_GetWindowSize(win, &env->window_width, &env->window_height);
    env->ligne_depart.x = env->window_width / (game_nb_rows(env->jeu) + 2);
    env->ligne_depart.y = env->window_height / (game_nb_cols(env->jeu) + 2);
    env->cases = malloc(sizeof(SDL_Rect) * game_nb_rows(env->jeu) * (game_nb_cols(env->jeu)));
    // env->wall = malloc(5 * sizeof(SDL_Texture));

    env->wall0 = IMG_LoadTexture(ren, "textures/0.png");
    if (!env->wall0) ERROR("IMG_LoadTexture: %s\n", "textures/0.png");
    env->wall1 = IMG_LoadTexture(ren, "textures/1.png");
    if (!env->wall1) ERROR("IMG_LoadTexture: %s\n", "textures/1.png");
    env->wall2 = IMG_LoadTexture(ren, "textures/2.png");
    if (!env->wall2) ERROR("IMG_LoadTexture: %s\n", "textures/2.png");
    env->wall3 = IMG_LoadTexture(ren, "textures/3.png");
    if (!env->wall3) ERROR("IMG_LoadTexture: %s\n", "textures/3.png");
    env->wall4 = IMG_LoadTexture(ren, "textures/4.png");
    if (!env->wall4) ERROR("IMG_LoadTexture: %s\n", "textures/4.png");
    env->lamp = IMG_LoadTexture(ren, MARIO);
    if (!env->lamp) ERROR("IMG_LoadTexture: %s\n", MARIO);
    return env;
}

/* **************************************************************** */

void render(SDL_Window *win, SDL_Renderer *ren, Env *env)
{
    SDL_SetRenderDrawColor(ren, 120, 120, 120, 255);
    SDL_RenderClear(ren);
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    SDL_GetWindowSize(win, &env->window_width, &env->window_height);
    int size = 0;
    if (env->window_height < env->window_width)
    {
        size = env->window_height;
    }
    else
    {
        size = env->window_width;
    }
    env->ligne_depart.x = size / (game_nb_cols(env->jeu) + 2);
    env->ligne_arrivee.x = (size / (game_nb_cols(env->jeu) + 2)) * (game_nb_cols(env->jeu) + 1);
    int centrage = (env->window_width - (env->ligne_depart.x + env->ligne_arrivee.x)) / 2;

    // creation des cases
    env->cases[0].x = size / (game_nb_cols(env->jeu) + 2) + centrage;
    env->cases[0].y = size / (game_nb_rows(env->jeu) + 2);
    env->cases[0].w = size / (game_nb_cols(env->jeu) + 2);
    env->cases[0].h = size / (game_nb_rows(env->jeu) + 2);
    for (int i = 1; i != (game_nb_rows(env->jeu) * game_nb_cols(env->jeu)); i++)
    {
        env->cases[i].x = env->cases[i - 1].x + env->cases[i - 1].w;
        env->cases[i].y = env->cases[i - 1].y;
        env->cases[i].w = env->cases[0].w;
        env->cases[i].h = env->cases[0].h;

        if (i % game_nb_rows(env->jeu) == 0)  // retour à la ligne
        {
            env->cases[i].x = env->cases[0].x;
            env->cases[i].y = env->cases[i - 1].y + env->cases[0].h;
        }
        env->cases[i].w = env->cases[0].w;
        env->cases[i].h = env->cases[0].h;
    }
    for (int i = 0; i < game_nb_rows(env->jeu); i++)
    {
        for (int j = 0; j < game_nb_cols(env->jeu); j++)
        {
            int case_nb = game_nb_cols(env->jeu) * i + j;
            int black_nb = -1;
            square state = game_get_state(env->jeu, i, j);
            square flags = game_get_flags(env->jeu, i, j);
            if (game_is_black(env->jeu, i, j))
            {
                SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
                SDL_RenderFillRect(ren, &(env->cases[case_nb]));
                black_nb = game_get_black_number(env->jeu, i, j);
                switch (black_nb)
                {
                    case -1:
                        break;
                    case 0:
                        SDL_RenderCopy(ren, env->wall0, NULL, &(env->cases[case_nb]));
                        break;
                    case 1:
                        SDL_RenderCopy(ren, env->wall1, NULL, &(env->cases[case_nb]));
                        break;
                    case 2:
                        SDL_RenderCopy(ren, env->wall2, NULL, &(env->cases[case_nb]));
                        break;
                    case 3:
                        SDL_RenderCopy(ren, env->wall3, NULL, &(env->cases[case_nb]));
                        break;
                    case 4:
                        SDL_RenderCopy(ren, env->wall4, NULL, &(env->cases[case_nb]));
                        break;
                }
            }
            else
            {
                if (state == S_LIGHTBULB)
                {
                    SDL_SetRenderDrawColor(ren, 255, 255, 0, 255);
                    SDL_RenderFillRect(ren, &(env->cases[case_nb]));
                    SDL_RenderCopy(ren, env->lamp, NULL, &(env->cases[case_nb]));
                }
                else if (state == S_MARK)
                {
                    SDL_SetRenderDrawColor(ren, 0, 0, 120, 255);
                    SDL_RenderFillRect(ren, &(env->cases[case_nb]));
                }
                else if (game_is_lighted(env->jeu, i, j))
                {
                    SDL_SetRenderDrawColor(ren, 255, 255, 0, 255);
                    SDL_RenderFillRect(ren, &(env->cases[case_nb]));
                }
                else
                {
                    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
                    SDL_RenderFillRect(ren, &(env->cases[case_nb]));
                }
            }
        }
    }
    /*if(SDL_RenderFillRects(ren,env->cases,(game_nb_rows(env->jeu)*game_nb_cols(env->jeu))) <0)//Remplissage des cases blanches
    {
        printf("Erreur lors des remplissages de rectangles: %s", SDL_GetError());
        return;
    }*/

    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);  // Couleur rouge
    // Lignes horizontales
    env->ligne_depart.x = size / (game_nb_cols(env->jeu) + 2) + centrage;
    env->ligne_arrivee.x = (size / (game_nb_cols(env->jeu) + 2)) * (game_nb_cols(env->jeu) + 1) + centrage;
    env->ligne_depart.y = size / (game_nb_rows(env->jeu) + 2);
    env->ligne_arrivee.y = size / (game_nb_rows(env->jeu) + 2);
    for (int i = 0; i != (game_nb_rows(env->jeu) + 1); i++)
    {
        SDL_RenderDrawLine(ren, env->ligne_depart.x, env->ligne_depart.y, env->ligne_arrivee.x, env->ligne_arrivee.y);
        env->ligne_depart.y += size / (game_nb_rows(env->jeu) + 2);
        env->ligne_arrivee.y = env->ligne_depart.y;
    }
    // Lignes verticales
    env->ligne_depart.x = size / (game_nb_cols(env->jeu) + 2) + centrage;
    env->ligne_arrivee.x = size / (game_nb_cols(env->jeu) + 2) + centrage;
    env->ligne_depart.y = size / (game_nb_rows(env->jeu) + 2);
    env->ligne_arrivee.y = (size / (game_nb_rows(env->jeu) + 2)) * (game_nb_rows(env->jeu) + 1);
    for (int i = 0; i != (game_nb_cols(env->jeu) + 1); i++)
    {
        SDL_RenderDrawLine(ren, env->ligne_depart.x, env->ligne_depart.y, env->ligne_arrivee.x, env->ligne_arrivee.y);
        env->ligne_depart.x += size / (game_nb_cols(env->jeu) + 2);
        env->ligne_arrivee.x = env->ligne_depart.x;
    }
    /*Creation des boutons*/
    int button_size = (size / 5) * 0.8;
    int button_height = env->ligne_depart.y / 2;
    int space = (size / 5) * 0.1;
    int first_last_space = (size / 5) * 0.25;
    int ligne = 2;
    SDL_Rect rect;

    for (int r = 0; r < 5; r++)
    {
        for (int i = 0; i < ligne; i++)
        {
            int x_depart = first_last_space + r * button_size + r * space + centrage;
            int x_arrivee = x_depart + button_size;
            int y_depart = button_height + i * (button_height / 2);
            int y_arrivee = y_depart;
            SDL_RenderDrawLine(ren, x_depart, y_depart, x_arrivee, y_arrivee);
            if (i == 0)
            {
                rect.x = x_depart;
                rect.y = y_depart;
                int x_arrivee_2 = x_depart;
                int y_arrivee_2 = y_depart + button_height / 2;
                SDL_RenderDrawLine(ren, x_depart, y_depart, x_arrivee_2, y_arrivee_2);
                int x_depart_2 = x_arrivee;
                y_arrivee_2 = y_depart + button_height / 2;
                SDL_RenderDrawLine(ren, x_depart_2, y_depart, x_arrivee, y_arrivee_2);
                rect.w = button_size;
                rect.h = button_height / 2;
                SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
                SDL_RenderFillRect(ren, &rect);
                SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
            }
        }
    }
}

/* **************************************************************** */

bool process(SDL_Window *win, SDL_Renderer *ren, Env *env, SDL_Event *e)
{
    int w, h;
    SDL_GetWindowSize(win, &w, &h);

    /* generic events */
    if (e->type == SDL_QUIT)
    {
        return true;
    }
    /* Android events */
#ifdef __ANDROID__
    else if (e->type == SDL_FINGERDOWN)
    {
        env->mario_x = e->tfinger.x * w; /* tfinger.x, normalized in [0..1] */
        env->mario_y = e->tfinger.y * h; /* tfinger.y, normalized in [0..1] */
    }
    /* other events */
#else
    else if (e->type == SDL_MOUSEBUTTONDOWN)
    {
        SDL_Point mouse;
        SDL_GetMouseState(&mouse.x, &mouse.y);
        int grid_x = env->cases[0].x;
        int grid_y = env->cases[0].y;
        int grid_w = env->cases[0].w;
        int grid_h = env->cases[0].h;

        /*I had to copy the following to check if our click is in a button or not. In order to do this, I needed some measurements such as the size of the window and the size of the button.*/
        int size;
        if (env->window_height < env->window_width)
        {
            size = env->window_height;
        }
        else
        {
            size = env->window_width;
        }
        int centrage = (env->window_width - (env->ligne_depart.x + env->ligne_arrivee.x)) / 2;
        int button_size = (size / 5) * 0.8;
        int button_height = env->ligne_depart.y / 2;
        int space = (size / 5) * 0.1;
        int first_last_space = (size / 5) * 0.25;
        int button1_pos_x_start = first_last_space + centrage;
        int button1_pos_x_end = button1_pos_x_start + button_size;
        int button1_pos_y_start = button_height;
        int button1_pos_y_end = button1_pos_y_start + (button_height / 2);
        int button2_start = button1_pos_x_end + space;
        int button2_end = (button1_pos_x_end + button1_pos_y_end) + space + button_size;
        int button3_start = button2_start + space + button_size;
        int button3_end = button2_end + space + button_size;
        int button4_start = button3_start + space + button_size;
        int button4_end = button3_end + space + button_size;
        int button5_start = button4_start + space + button_size;
        int button5_end = button4_end + space + button_size;

        // We first check if the click is in the game grid
        if (mouse.x > grid_x && mouse.x < (grid_x) + (game_nb_cols(env->jeu)) * (grid_w) && mouse.y > grid_y && mouse.y < (grid_y) + (game_nb_rows(env->jeu)) * (grid_h))
        {
            // We find the coordinates (i,j) of the case in which we clicked
            int case_i = -1;
            int case_j = -1;
            int k = grid_y;
            while (k < mouse.y)
            {
                case_i++;
                k += grid_h;
            }
            k = grid_x;
            while (k < mouse.x)
            {
                case_j++;
                k += grid_w;
            }
            if ((e->button.button) == SDL_BUTTON_LEFT)  // If left click then lightbulb
            {
                square state = game_get_state(env->jeu, case_i, case_j);
                if (state == S_LIGHTBULB)
                    game_play_move(env->jeu, case_i, case_j, S_BLANK);
                else if (state == S_BLANK)
                    game_play_move(env->jeu, case_i, case_j, S_LIGHTBULB);
            }
            else if ((e->button.button) == SDL_BUTTON_RIGHT)  // If right click then mark
            {
                square state = game_get_state(env->jeu, case_i, case_j);
                if (state == S_MARK)
                    game_play_move(env->jeu, case_i, case_j, S_BLANK);
                else if (state == S_BLANK)
                    game_play_move(env->jeu, case_i, case_j, S_MARK);
            }
        }
        else if (mouse.x > button1_pos_x_start + button1_pos_y_start && mouse.x < button1_pos_x_end + button1_pos_y_end)
        {
            printf("Restart");
            game_restart(env->jeu);
        }
        else if (mouse.x > button2_start && mouse.x < button2_end)
        {
            printf("Solve");
            game_solve(env->jeu);
        }
        else if (mouse.x > button3_start && mouse.x < button3_end)
        {
            printf("Undo");
            game_undo(env->jeu);
        }
        else if (mouse.x > button4_start && mouse.x < button4_end)
        {
            printf("Redo");
            game_redo(env->jeu);
        }
        else if (mouse.x > button5_start && mouse.x < button5_end)
        {
            printf("Help");
            // game_help();
        }
    }
    else if (e->type == SDL_KEYDOWN)
    {
        /*switch (e->key.keysym.sym)
        {
            case SDLK_LEFT:
                env->bomb_x -= 10;
                break;
            case SDLK_RIGHT:
                env->bomb_x += 10;
                break;
            case SDLK_UP:
                env->bomb_y -= 10;
                break;
            case SDLK_DOWN:
                env->bomb_y += 10;
                break;
            case SDLK_ESCAPE:
                return true;
                break;
        }*/
    }
#endif

    return false; /* don't quit */
}

/* PUT YOUR CODE HERE TO PROCESS EVENTS */

/* **************************************************************** */

void clean(SDL_Window *win, SDL_Renderer *ren, Env *env)
{
    /* PUT YOUR CODE HERE TO CLEAN MEMORY */

    free(env);
}

/* **************************************************************** */
