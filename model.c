#include "model.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>  // required to load transparent texture from PNG
#include <SDL2/SDL_ttf.h>    // required to use TTF fonts
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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
    int grid_height, grid_width;
    SDL_Rect cases[49];
    SDL_Point ligne_depart,ligne_arrivee;
};

/* **************************************************************** */

Env *init(SDL_Window *win, SDL_Renderer *ren, int argc, char *argv[])
{
    Env *env = malloc(sizeof(struct Env_t));

    /* PUT YOUR CODE HERE TO INIT TEXTURES, ... */

    return env;
}

/* **************************************************************** */

void render(SDL_Window *win, SDL_Renderer *ren, Env *env)
{ /* PUT YOUR CODE HERE TO RENDER TEXTURES, ... */
    env->cases[0].x = env->cases[0].y = 0;
    env->cases[0].w = env->cases[0].h = 100;
    for(int i = 1; i != 35; i++)
    {
        env->cases[i].x = env->cases[i-1].x + 200;
        env->cases[i].y = env->cases[i-1].y;

        if(i%7 == 0) //retour à la ligne
        {
            env->cases[i].x = (i%7 == 0) ? 0 : 100;
            env->cases[i].y = env->cases[i-1].y + 100;
        }
        env->cases[i].w = env->cases[i].h = 100; //taille d'une case : 100 x 100
    }
    if(SDL_RenderFillRects(ren,env->cases,49) <0)//Remplissage des cases blanches
    {
        printf("Erreur lors des remplissages de rectangles: %s",SDL_GetError());
        return ;
    }
    // À présent, occupons nous des lignes
    // On ne peut pas utiliser la fonction SDL_RenderDrawLines
    // car celle-ci ne permet pas de créer des lignes indépendantes comme nous voulons le faire mais des chemins

    SDL_SetRenderDrawColor(ren,0,0,0,255);//Couleur rouge

    // Lignes horizontales
    env->ligne_depart.x = 100;
    env->ligne_arrivee.x = 800;
    env->ligne_depart.y = 0;
    for(int i = 0; i!=8; i++)
    {
      env->ligne_depart.y += 100;
      env->ligne_arrivee.y = env->ligne_depart.y;
      SDL_RenderDrawLine(ren,env->ligne_depart.x, env->ligne_depart.y,env->ligne_arrivee.x,env->ligne_arrivee.y);
    }
    // Lignes verticales
    env->ligne_depart.x = 0;
    env->ligne_depart.y = 100;
    env->ligne_arrivee.y = 800;
    for(int i = 0; i!=8; i++)
    {
      env->ligne_depart.x += 100;
      env->ligne_arrivee.x = env->ligne_depart.x;
      SDL_RenderDrawLine(ren,env->ligne_depart.x, env->ligne_depart.y,env->ligne_arrivee.x,env->ligne_arrivee.y);
    }
}

/* **************************************************************** */

bool process(SDL_Window *win, SDL_Renderer *ren, Env *env, SDL_Event *e)
{
    if (e->type == SDL_QUIT)
    {
        return true;
    }

    /* PUT YOUR CODE HERE TO PROCESS EVENTS */

    return false;
}

/* **************************************************************** */

void clean(SDL_Window *win, SDL_Renderer *ren, Env *env)
{
    /* PUT YOUR CODE HERE TO CLEAN MEMORY */

    free(env);
}

/* **************************************************************** */
