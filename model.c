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

        if (i % game_nb_rows(env->jeu) == 0)  // retour à la ligne
        {
            env->cases[i].x = env->cases[0].x;
            env->cases[i].y = env->cases[i-1].y + env->cases[0].h;
        }
        env->cases[i].w = env->cases[0].w;
        env->cases[i].h = env->cases[0].h; 
    }
    if(SDL_RenderFillRects(ren,env->cases,game_nb_rows(env->jeu)*(game_nb_cols(env->jeu)))<0)//Remplissage des cases blanches
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
    /*Creation des boutons*/
    int button_size = (size/5)*0.8;
    int button_height = env->ligne_depart.y/2;
    int space = (size/5)*0.1;
    int first_last_space = (size/5)*0.25;
    int ligne = 2;

    for (int r = 0; r < 5; r++){
        if (r == 0 || r == 4){
            for (int i = 0; i < ligne; i++){
                int x_depart = first_last_space + r * button_size + r * space;
                int x_arrivee = x_depart + button_size;
                int y_depart = button_height + i * (button_height/2);
                int y_arrivee = y_depart;
                SDL_RenderDrawLine(ren, x_depart, y_depart, x_arrivee, y_arrivee);
                if (i == 0){
                    int x_arrivee_2 = x_depart;
                    int y_arrivee_2 = y_depart + button_height/2;
                    SDL_RenderDrawLine(ren, x_depart, y_depart, x_arrivee_2, y_arrivee_2);
                    int x_depart_2 = x_arrivee;
                    y_arrivee_2 = y_depart + button_height/2;
                    SDL_RenderDrawLine(ren, x_depart_2, y_depart, x_arrivee, y_arrivee_2);
                }
            }
        }
        else{
            for (int i = 0; i < ligne; i++){
                int x_depart = first_last_space + r * button_size + r * space;
                int x_arrivee = x_depart + button_size;
                int y_depart = button_height + i * (button_height/2);
                int y_arrivee = y_depart;
                SDL_RenderDrawLine(ren, x_depart, y_depart, x_arrivee, y_arrivee);
                if (i == 0){
                    int x_arrivee_2 = x_depart;
                    int y_arrivee_2 = y_depart + button_height/2;
                    SDL_RenderDrawLine(ren, x_depart, y_depart, x_arrivee_2, y_arrivee_2);
                    int x_depart_2 = x_arrivee;
                    y_arrivee_2 = y_depart + button_height/2;
                    SDL_RenderDrawLine(ren, x_depart_2, y_depart, x_arrivee, y_arrivee_2);
                }

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
        Uint32 button = SDL_GetMouseState(&mouse.x, &mouse.y);
        int grid_x = env->cases[0].x;
        int grid_y = env->cases[0].y;
        int grid_w = env->cases[0].w;
        int grid_h = env->cases[0].h;
        // We first check if the click is in the game grid
        if(mouse.x > grid_x && mouse.x < (grid_x)+(grid_w) && mouse.y > grid_y && mouse.y < (grid_y)+(grid_h)){
            // We find the coordinates (i,j) of the case in which we clicked
            int case_i = -1;
            int case_j = -1;
            int k = grid_y;
            while((k < mouse.y) && (k < grid_y+grid_h)){
                case_i++;
                k += grid_h;
            }
            k = grid_x;
            while((k < mouse.x) && (k < grid_x+grid_w)){
                case_j++;
                k += grid_w;
            }
            if ((e->button.button) == SDL_BUTTON_LEFT)  // If left click then lightbulb
            {
                if (game_check_move(env->jeu, case_i, case_j, S_LIGHTBULB))
                {
                    game_play_move(env->jeu, case_i, case_j, S_LIGHTBULB);
                }
            }
            else if ((e->button.button) == SDL_BUTTON_RIGHT)    // If right click then mark
            {
                if (game_check_move(env->jeu, case_i, case_j, S_MARK))
                {
                    game_play_move(env->jeu, case_i, case_j, S_MARK);
                }
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
