#include "model.h"

#ifdef __ANDROID__
#include <SDL.h>
#include <SDL_image.h>  // required to load transparent texture from PNG
#include <SDL_ttf.h>    // required to use TTF fonts
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#endif
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"

/* **************************************************************** */

#define FONT "fonts/mistral.ttf"
#define FONTSIZE 36
#define BACKGROUND "textures/mansion.png"
#define BOO "textures/boo.png"
#define SHYBOO "textures/shyboo.png"
#define KINGBOO "textures/kingboo.png"
#define LUIGI "textures/luigi.png"
#define MIST "textures/mist.png"
#define TITLE "textures/title.png"
#define RESTART "textures/restart.png"
#define SOLVE "textures/solve.png"
#define UNDO "textures/undo.png"
#define REDO "textures/redo.png"
#define HELP "textures/help.png"
#define SAVE "textures/save.png"
#define CREDITS "textures/credits.png"

#ifdef __ANDROID__
#define HELP_PANEL "textures/help_panel_android.png"
#else
#define HELP_PANEL "textures/help_panel.png"
#endif

struct Env_t
{
    /* PUT YOUR VARIABLES HERE */
    SDL_Texture *background;
    SDL_Texture *wallU;  // Wall unumbered
    // Numbered walls
    SDL_Texture *wall0;
    SDL_Texture *wall1;
    SDL_Texture *wall2;
    SDL_Texture *wall3;
    SDL_Texture *wall4;
    // Numbered walls with error flag
    SDL_Texture *wall0r;
    SDL_Texture *wall1r;
    SDL_Texture *wall2r;
    SDL_Texture *wall3r;
    SDL_Texture *wall4r;
    // Buttons textures
    SDL_Texture *restart;
    SDL_Texture *solve;
    SDL_Texture *undo;
    SDL_Texture *redo;
    SDL_Texture *help;
    SDL_Texture *save;

    SDL_Texture *lamp;
    SDL_Texture *lamp_error;  // lamp texture with error flag
    SDL_Texture *lamp_win;    // lamp texture when winning the game
    SDL_Texture *mark;
    SDL_Texture *lighted;  // lighted flag
    SDL_Texture *title;
    SDL_Texture *help_panel;
    SDL_Texture *credits;
    bool toggle_help;   // If true then the help panel is displayed
    SDL_Rect *cases;    // Array to store each case of the game and access them easily
    SDL_Rect *buttons;  // Same but for the buttons
    game game;
};

/* **************************************************************** */

Env *init(SDL_Window *win, SDL_Renderer *ren, int argc, char *argv[])
{
    Env *env = malloc(sizeof(struct Env_t));
    if (argv[1] != NULL)
    {
        env->game = game_load(argv[1]);
    }
    else
    {
        srand(time(NULL));
        env->game = game_random(7, 7, false, 10, false);
    }
    env->cases = malloc(sizeof(SDL_Rect) * game_nb_rows(env->game) * (game_nb_cols(env->game)));
    env->buttons = malloc(sizeof(SDL_Rect) * 6);

    // Loading of all the textures

    env->background = IMG_LoadTexture(ren, BACKGROUND);
    if (!env->background) ERROR("IMG_LoadTexture: %s\n", BACKGROUND);

    env->wallU = IMG_LoadTexture(ren, "textures/blockU.png");
    if (!env->wallU) ERROR("IMG_LoadTexture: %s\n", "textures/blockU.png");

    env->wall0 = IMG_LoadTexture(ren, "textures/block0.png");
    if (!env->wall0) ERROR("IMG_LoadTexture: %s\n", "textures/block0.png");
    env->wall1 = IMG_LoadTexture(ren, "textures/block1.png");
    if (!env->wall1) ERROR("IMG_LoadTexture: %s\n", "textures/block1.png");
    env->wall2 = IMG_LoadTexture(ren, "textures/block2.png");
    if (!env->wall2) ERROR("IMG_LoadTexture: %s\n", "textures/block2.png");
    env->wall3 = IMG_LoadTexture(ren, "textures/block3.png");
    if (!env->wall3) ERROR("IMG_LoadTexture: %s\n", "textures/block3.png");
    env->wall4 = IMG_LoadTexture(ren, "textures/block4.png");
    if (!env->wall4) ERROR("IMG_LoadTexture: %s\n", "textures/block4.png");

    env->wall0r = IMG_LoadTexture(ren, "textures/block0r.png");
    if (!env->wall0r) ERROR("IMG_LoadTexture: %s\n", "textures/block0r.png");
    env->wall1r = IMG_LoadTexture(ren, "textures/block1r.png");
    if (!env->wall1r) ERROR("IMG_LoadTexture: %s\n", "textures/block1r.png");
    env->wall2r = IMG_LoadTexture(ren, "textures/block2r.png");
    if (!env->wall2r) ERROR("IMG_LoadTexture: %s\n", "textures/block2r.png");
    env->wall3r = IMG_LoadTexture(ren, "textures/block3r.png");
    if (!env->wall3r) ERROR("IMG_LoadTexture: %s\n", "textures/block3r.png");
    env->wall4r = IMG_LoadTexture(ren, "textures/block4r.png");
    if (!env->wall4r) ERROR("IMG_LoadTexture: %s\n", "textures/block4r.png");

    env->lamp = IMG_LoadTexture(ren, BOO);
    if (!env->lamp) ERROR("IMG_LoadTexture: %s\n", BOO);
    env->lamp_error = IMG_LoadTexture(ren, SHYBOO);
    if (!env->lamp_error) ERROR("IMG_LoadTexture: %s\n", SHYBOO);
    env->lamp_win = IMG_LoadTexture(ren, KINGBOO);
    if (!env->lamp_win) ERROR("IMG_LoadTexture: %s\n", KINGBOO);
    env->lighted = IMG_LoadTexture(ren, MIST);
    if (!env->lighted) ERROR("IMG_LoadTexture: %s\n", MIST);
    env->mark = IMG_LoadTexture(ren, LUIGI);
    if (!env->mark) ERROR("IMG_LoadTexture: %s\n", LUIGI);

    env->restart = IMG_LoadTexture(ren, RESTART);
    if (!env->restart) ERROR("IMG_LoadTexture: %s\n", RESTART);
    env->solve = IMG_LoadTexture(ren, SOLVE);
    if (!env->solve) ERROR("IMG_LoadTexture: %s\n", SOLVE);
    env->undo = IMG_LoadTexture(ren, UNDO);
    if (!env->undo) ERROR("IMG_LoadTexture: %s\n", UNDO);
    env->redo = IMG_LoadTexture(ren, REDO);
    if (!env->redo) ERROR("IMG_LoadTexture: %s\n", REDO);
    env->help = IMG_LoadTexture(ren, HELP);
    if (!env->help) ERROR("IMG_LoadTexture: %s\n", HELP);
    env->save = IMG_LoadTexture(ren, SAVE);
    if (!env->save) ERROR("IMG_LoadTexture: %s\n", SAVE);

    env->title = IMG_LoadTexture(ren, TITLE);
    if (!env->title) ERROR("IMG_LoadTexture: %s\n", TITLE);
    env->help_panel = IMG_LoadTexture(ren, HELP_PANEL);
    if (!env->help_panel) ERROR("IMG_LoadTexture: %s\n", HELP_PANEL);
    env->toggle_help = false;
    env->credits = IMG_LoadTexture(ren, CREDITS);
    if (!env->credits) ERROR("IMG_LoadTexture: %s\n", CREDITS);

    return env;
}

/* **************************************************************** */

void render(SDL_Window *win, SDL_Renderer *ren, Env *env)
{
    int w, h;
    SDL_GetWindowSize(win, &w, &h);
    SDL_Point ligne_depart, ligne_arrivee;
    ligne_depart.x = w / (game_nb_rows(env->game) + 2);
    ligne_depart.y = h / (game_nb_cols(env->game) + 2);
    SDL_Rect rect;

    SDL_RenderCopy(ren, env->background, NULL, NULL);
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    SDL_GetWindowSize(win, &w, &h);
    int size = 0;
    if (h < w)
    {
        size = h;
    }
    else
    {
        size = w;
    }
    ligne_depart.x = size / (game_nb_cols(env->game) + 2);
    ligne_arrivee.x = (ligne_depart.x) * (game_nb_cols(env->game) + 1);
    int centrage = (w - (ligne_depart.x + ligne_arrivee.x)) / 2;

    // creation des cases
    env->cases[0].x = size / (game_nb_cols(env->game) + 2) + centrage;
    env->cases[0].y = 1.5 * size / (game_nb_rows(env->game) + 2);
    env->cases[0].w = size / (game_nb_cols(env->game) + 2);
    env->cases[0].h = size / (game_nb_rows(env->game) + 2);
    for (int i = 1; i != (game_nb_rows(env->game) * game_nb_cols(env->game)); i++)
    {
        env->cases[i].x = env->cases[i - 1].x + env->cases[i - 1].w;
        env->cases[i].y = env->cases[i - 1].y;

        if (i % game_nb_cols(env->game) == 0)  // retour à la ligne
        {
            env->cases[i].x = env->cases[0].x;
            env->cases[i].y = env->cases[i - 1].y + env->cases[0].h;
        }
        env->cases[i].w = env->cases[0].w;
        env->cases[i].h = env->cases[0].h;
    }
    bool is_won = game_is_over(env->game);
    for (int i = 0; i < game_nb_rows(env->game); i++)
    {
        for (int j = 0; j < game_nb_cols(env->game); j++)
        {
            int case_nb = game_nb_cols(env->game) * i + j;
            int black_nb = -1;
            square state = game_get_state(env->game, i, j);
            if (game_is_black(env->game, i, j))
            {
                SDL_Texture *wall = env->wallU;
                black_nb = game_get_black_number(env->game, i, j);
                if (!game_has_error(env->game, i, j))
                {
                    switch (black_nb)
                    {
                        case -1:
                            break;
                        case 0:
                            wall = env->wall0;
                            break;
                        case 1:
                            wall = env->wall1;
                            break;
                        case 2:
                            wall = env->wall2;
                            break;
                        case 3:
                            wall = env->wall3;
                            break;
                        case 4:
                            wall = env->wall4;
                            break;
                    }
                }
                else
                {
                    switch (black_nb)
                    {
                        case -1:
                            break;
                        case 0:
                            wall = env->wall0r;
                            break;
                        case 1:
                            wall = env->wall1r;
                            break;
                        case 2:
                            wall = env->wall2r;
                            break;
                        case 3:
                            wall = env->wall3r;
                            break;
                        case 4:
                            wall = env->wall4r;
                            break;
                    }
                }
                SDL_RenderCopy(ren, wall, NULL, &(env->cases[case_nb]));
            }
            else
            {
                if (state == S_LIGHTBULB)
                {
                    if (game_has_error(env->game, i, j))
                        SDL_RenderCopy(ren, env->lamp_error, NULL, &(env->cases[case_nb]));
                    else if (is_won)
                        SDL_RenderCopy(ren, env->lamp_win, NULL, &(env->cases[case_nb]));
                    else
                        SDL_RenderCopy(ren, env->lamp, NULL, &(env->cases[case_nb]));
                }
                else
                {
                    if (game_is_lighted(env->game, i, j)) SDL_RenderCopy(ren, env->lighted, NULL, &(env->cases[case_nb]));
                    if (state == S_MARK) SDL_RenderCopy(ren, env->mark, NULL, &(env->cases[case_nb]));
                }
            }
        }
    }

    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);  // Lignes en blanc
    // Lignes horizontales
    ligne_depart.x = size / (game_nb_cols(env->game) + 2) + centrage;
    ligne_arrivee.x = (size / (game_nb_cols(env->game) + 2)) * (game_nb_cols(env->game) + 1) + centrage;
    ligne_depart.y = 1.5 * size / (game_nb_rows(env->game) + 2);
    ligne_arrivee.y = 1.5 * size / (game_nb_rows(env->game) + 2);
    for (int i = 0; i != (game_nb_rows(env->game) + 1); i++)
    {
        SDL_RenderDrawLine(ren, ligne_depart.x, ligne_depart.y, ligne_arrivee.x, ligne_arrivee.y);
        ligne_depart.y += size / (game_nb_rows(env->game) + 2);
        ligne_arrivee.y = ligne_depart.y;
    }
    // Lignes verticales
    ligne_depart.x = size / (game_nb_cols(env->game) + 2) + centrage;
    ligne_arrivee.x = size / (game_nb_cols(env->game) + 2) + centrage;
    ligne_depart.y = 1.5 * size / (game_nb_rows(env->game) + 2);
    ligne_arrivee.y = (size / (game_nb_rows(env->game) + 2)) * (game_nb_rows(env->game) + 1) + env->cases[0].h / 2;
    for (int i = 0; i != (game_nb_cols(env->game) + 1); i++)
    {
        SDL_RenderDrawLine(ren, ligne_depart.x, ligne_depart.y, ligne_arrivee.x, ligne_arrivee.y);
        ligne_depart.x += size / (game_nb_cols(env->game) + 2);
        ligne_arrivee.x = ligne_depart.x;
    }
    /*Creation des boutons*/
    int button_size = (size / 6) * 0.8;
    int button_height = ligne_depart.y / 2;
    int space = (size / 6) * 0.1;

    env->buttons[0].h = button_height / 2;
    env->buttons[0].w = button_size;
    env->buttons[0].x = w / 2 - 3 * env->buttons[0].w - space * 2.5;
    env->buttons[0].y = env->cases[0].y * 0.95 - env->buttons[0].h;
    SDL_RenderCopy(ren, env->restart, NULL, &(env->buttons[0]));
    for (int r = 1; r < 6; r++)
    {
        env->buttons[r].x = env->buttons[r - 1].x + button_size + space;
        env->buttons[r].y = env->buttons[r - 1].y;
        env->buttons[r].h = env->buttons[r - 1].h;
        env->buttons[r].w = env->buttons[r - 1].w;
        switch (r)
        {
            case 1:
                SDL_RenderCopy(ren, env->solve, NULL, &(env->buttons[r]));
                break;
            case 2:
                SDL_RenderCopy(ren, env->undo, NULL, &(env->buttons[r]));
                break;
            case 3:
                SDL_RenderCopy(ren, env->redo, NULL, &(env->buttons[r]));
                break;
            case 4:
                SDL_RenderCopy(ren, env->save, NULL, &(env->buttons[r]));
                break;
            case 5:
                SDL_RenderCopy(ren, env->help, NULL, &(env->buttons[r]));
                break;
        }
    }

    // Placement du titre
    rect.h = env->cases[0].y * 0.7;
    rect.w = rect.h * 4;
    rect.x = w / 2 - rect.w / 2;
    rect.y = 0;
    SDL_RenderCopy(ren, env->title, NULL, &rect);

    // Placement des crédits
    rect.h = env->cases[0].h / 2;
    rect.w = rect.h * 9;
    rect.x = w / 2 - rect.w / 2;
    rect.y = ligne_arrivee.y + 3;
    SDL_RenderCopy(ren, env->credits, NULL, &rect);

    // Placement du panneau d'aide
    if (env->toggle_help)
    {
        rect.y = env->cases[0].y;
        rect.h = h - rect.y;
        rect.w = 2 * rect.h / 3;
        rect.x = w - rect.w;
        if (rect.x < 0)
        {
            rect.x = 0;
            rect.w = w;
        }
        SDL_RenderCopy(ren, env->help_panel, NULL, &rect);
    }
}

/* **************************************************************** */

bool process(SDL_Window *win, SDL_Renderer *ren, Env *env, SDL_Event *e)
{
    int w, h;
    SDL_GetWindowSize(win, &w, &h);
    SDL_Point click;  // Will store the coordinates of the click, no matter if it is with mouse of finger
    if (e->type == SDL_QUIT)
    {
        return true;
    }
    /* Android events */
    else if (e->type == SDL_FINGERDOWN || e->type == SDL_MOUSEBUTTONDOWN)  // If there is a click (mouse or finger)
    {
        if (e->type == SDL_MOUSEBUTTONDOWN)  // If mouse click
        {
            SDL_GetMouseState(&click.x, &click.y);
        }
#ifdef __ANDROID__
        else if (e->type == SDL_FINGERDOWN)  // If finger touch
        {
            click.x = e->tfinger.x;
            click.y = e->tfinger.y;
        }
#endif
        for (int i = 0; i < game_nb_rows(env->game); i++)
        {
            for (int j = 0; j < game_nb_cols(env->game); j++)
            {
                // We check if the click is in one of the case of the grid
                if (SDL_PointInRect(&click, &(env->cases[game_nb_cols(env->game) * i + j])))
                {
                    // If so, we play a lamp or a mark in this case
                    square state = game_get_state(env->game, i, j);
#ifdef __ANDROID__
                    if (state == S_BLANK)
                        game_play_move(env->game, i, j, S_LIGHTBULB);  // We cycle Lightbulb -> Mark -> Blank in the case at each touch
                    else if (state == S_LIGHTBULB)
                        game_play_move(env->game, i, j, S_MARK);
                    else if (state == S_MARK)
                        game_play_move(env->game, i, j, S_BLANK);
#endif
#ifndef __ANDROID__
                    if ((e->button.button) == SDL_BUTTON_LEFT)  // If left click then lightbulb
                    {
                        if (state == S_LIGHTBULB)
                            game_play_move(env->game, i, j, S_BLANK);
                        else if (state == S_BLANK)
                            game_play_move(env->game, i, j, S_LIGHTBULB);
                    }
                    else if ((e->button.button) == SDL_BUTTON_RIGHT)  // If right click then mark
                    {
                        if (state == S_MARK)
                            game_play_move(env->game, i, j, S_BLANK);
                        else if (state == S_BLANK)
                            game_play_move(env->game, i, j, S_MARK);
                    }
#endif
                }
            }
        }
        // We check if the click is in one of the buttons
        if (SDL_PointInRect(&click, &(env->buttons[0])))
            game_restart(env->game);

        else if (SDL_PointInRect(&click, &(env->buttons[1])))
            game_solve(env->game);

        else if (SDL_PointInRect(&click, &(env->buttons[2])))
            game_undo(env->game);

        else if (SDL_PointInRect(&click, &(env->buttons[3])))
            game_redo(env->game);

        else if (SDL_PointInRect(&click, &(env->buttons[4])))
        {
            time_t now = time(NULL);
            struct tm tm_now = *localtime(&now);       // Save the game with the format 'saveJJMMAAAA_HHMMSS'
            char s_now[sizeof "saveJJMMAAAA_HHMMSS"];  // Using the date and the time
            strftime(s_now, sizeof s_now, "save%d%m%Y_%H%M%S", &tm_now);
            game_save(env->game, s_now);
        }

        else if (SDL_PointInRect(&click, &(env->buttons[5])))
            env->toggle_help = !(env->toggle_help);  // Will toggle the help panel
    }
    // Keyboard shortcut for buttons
    else if (e->type == SDL_KEYDOWN)
    {
        switch (e->key.keysym.sym)
        {
            case SDLK_r:
                game_restart(env->game);
                break;
            case SDLK_q:
                game_solve(env->game);
                break;
            case SDLK_z:
                game_undo(env->game);
                break;
            case SDLK_y:
                game_redo(env->game);
                break;
            case SDLK_s:
                game_save(env->game, "game");
                break;
            case SDLK_h:
                env->toggle_help = !(env->toggle_help);
                break;
            case SDLK_ESCAPE:
                return true;
                break;
        }
    }

    return false; /* don't quit */
}

/* PUT YOUR CODE HERE TO PROCESS EVENTS */

/* **************************************************************** */

void clean(SDL_Window *win, SDL_Renderer *ren, Env *env)
{
    /* PUT YOUR CODE HERE TO CLEAN MEMORY */

    // Clean all the textures
    SDL_DestroyTexture(env->background);
    SDL_DestroyTexture(env->wallU);
    SDL_DestroyTexture(env->wall0);
    SDL_DestroyTexture(env->wall1);
    SDL_DestroyTexture(env->wall2);
    SDL_DestroyTexture(env->wall3);
    SDL_DestroyTexture(env->wall4);
    SDL_DestroyTexture(env->wall0r);
    SDL_DestroyTexture(env->wall1r);
    SDL_DestroyTexture(env->wall2r);
    SDL_DestroyTexture(env->wall3r);
    SDL_DestroyTexture(env->wall4r);
    SDL_DestroyTexture(env->restart);
    SDL_DestroyTexture(env->undo);
    SDL_DestroyTexture(env->solve);
    SDL_DestroyTexture(env->redo);
    SDL_DestroyTexture(env->save);
    SDL_DestroyTexture(env->help);
    SDL_DestroyTexture(env->lamp);
    SDL_DestroyTexture(env->lamp_error);
    SDL_DestroyTexture(env->lamp_win);
    SDL_DestroyTexture(env->lighted);
    SDL_DestroyTexture(env->mark);
    SDL_DestroyTexture(env->title);
    SDL_DestroyTexture(env->help_panel);
    SDL_DestroyTexture(env->credits);

    if (env->cases) free(env->cases);
    if (env->buttons) free(env->buttons);
    game_delete(env->game);

    free(env);
}

/* **************************************************************** */
