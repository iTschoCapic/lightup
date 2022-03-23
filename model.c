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

#define FONT "arial.ttf"
#define FONTSIZE 36
#define BACKGROUND "background.png"
#define MARIO "mario.png"

struct Env_t
{
    /* PUT YOUR VARIABLES HERE */
    SDL_Texture *background;
    SDL_Texture *wall;
    SDL_Texture *lamp;
    SDL_Texture *mark;
    SDL_Texture *title;
    SDL_Rect *cases;
    game jeu;
    int window_width , window_height ;
    SDL_Point ligne_depart,ligne_arrivee;
};

/* **************************************************************** */

Env *init(SDL_Window *win, SDL_Renderer *ren, int argc, char *argv[])
{
    Env *env = malloc(sizeof(struct Env_t));
    env->jeu = game_load(argv[1]);
    SDL_GetWindowSize(win, &env->window_width , &env->window_height);
    env->ligne_depart.x = env->window_width/(game_nb_rows(env->jeu)+2);
    env->ligne_depart.y = env->window_height/(game_nb_cols(env->jeu)+2);
    env->cases = malloc(sizeof(SDL_Rect)*game_nb_rows(env->jeu)*(game_nb_cols(env->jeu)));
    return env;
}

/* **************************************************************** */

void render(SDL_Window *win, SDL_Renderer *ren, Env *env)
{
    SDL_SetRenderDrawColor(ren,120,120,120,255);
    SDL_RenderClear(ren);
    SDL_SetRenderDrawColor(ren,255,255,255,255);
    SDL_GetWindowSize(win, &env->window_width , &env->window_height);
    int size = 0;
    if(env->window_height < env->window_width){
        size = env->window_height;
    }
    else
    {
        size = env->window_width;
    }
    env->ligne_depart.x = size / (game_nb_cols(env->jeu) + 2);
    env->ligne_arrivee.x = (size / (game_nb_cols(env->jeu) + 2)) * (game_nb_cols(env->jeu) + 1);
    int centrage = (env->window_width -(env->ligne_depart.x+env->ligne_arrivee.x))/2;

    //creation des cases 
    env->cases[0].x = size / (game_nb_cols(env->jeu) + 2)+centrage;
    env->cases[0].y = size/(game_nb_rows(env->jeu)+2);
    env->cases[0].w = size/(game_nb_cols(env->jeu)+2);
    env->cases[0].h = size/(game_nb_rows(env->jeu)+2);
    for(int i = 1; i != (game_nb_rows(env->jeu)*game_nb_cols(env->jeu)); i++)
    {
        env->cases[i].x = env->cases[i-1].x + env->cases[i-1].w;
        env->cases[i].y = env->cases[i-1].y;

        if (i % game_nb_cols(env->jeu) == 0)  // retour à la ligne
        {
            env->cases[i].x = size / (game_nb_cols(env->jeu) + 2)+centrage;
            env->cases[i].y = env->cases[i-1].y + env->cases[0].h;
        }
    }
    if(SDL_RenderFillRects(ren,env->cases,(game_nb_rows(env->jeu)*game_nb_cols(env->jeu))) <0)//Remplissage des cases blanches
    {
        printf("Erreur lors des remplissages de rectangles: %s", SDL_GetError());
        return;
    }
   
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);  // Couleur rouge
    // Lignes horizontales
    env->ligne_depart.x = size / (game_nb_cols(env->jeu) + 2)+centrage;
    env->ligne_arrivee.x = (size / (game_nb_cols(env->jeu) + 2)) * (game_nb_cols(env->jeu) + 1)+centrage;
    env->ligne_depart.y = size/(game_nb_rows(env->jeu)+2);
    env->ligne_arrivee.y = size/(game_nb_rows(env->jeu)+2);
    for (int i = 0; i != (game_nb_rows(env->jeu) + 1); i++)
    {
        SDL_RenderDrawLine(ren,env->ligne_depart.x, env->ligne_depart.y,env->ligne_arrivee.x,env->ligne_arrivee.y);
        env->ligne_depart.y += size/(game_nb_rows(env->jeu)+2);
        env->ligne_arrivee.y = env->ligne_depart.y;
    }
    // Lignes verticales
    env->ligne_depart.x = size/(game_nb_cols(env->jeu)+2)+centrage; 
    env->ligne_arrivee.x = size/(game_nb_cols(env->jeu)+2)+centrage;
    env->ligne_depart.y = size/(game_nb_rows(env->jeu)+2);
    env->ligne_arrivee.y = (size / (game_nb_rows(env->jeu) + 2)) * (game_nb_rows(env->jeu) + 1);
    for(int i = 0; i!=(game_nb_cols(env->jeu)+1); i++)
    {
        SDL_RenderDrawLine(ren,env->ligne_depart.x, env->ligne_depart.y,env->ligne_arrivee.x,env->ligne_arrivee.y);
        env->ligne_depart.x += size/(game_nb_cols(env->jeu)+2);
        env->ligne_arrivee.x = env->ligne_depart.x;
        
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
        Uint32 button = SDL_GetMouseState(&mouse.x, &mouse.y);
        int case_i = 0;
        int case_j = 0;
        if ((e->button.button) == 1)
        {
            if (game_check_move(env->jeu, case_i, case_j, S_LIGHTBULB))
            {
                game_play_move(env->jeu, case_i, case_j, S_LIGHTBULB);
            }
        }
        else if ((e->button.button) == 1)
        {
            if (game_check_move(env->jeu, case_i, case_j, S_MARK))
            {
                game_play_move(env->jeu, case_i, case_j, S_MARK);
            }
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
