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
    SDL_Rect *buttons;
    game jeu;
    int window_width, window_height;
    SDL_Point ligne_depart, ligne_arrivee;
};

/* **************************************************************** */

Env *init(SDL_Window *win, SDL_Renderer *ren, int argc, char *argv[])
{
    Env *env = malloc(sizeof(struct Env_t));
    if(argv[1] != NULL){
        env->jeu = game_load(argv[1]);
    }
    else{
        env->jeu = game_load("../games/default");
    }
    SDL_GetWindowSize(win, &env->window_width, &env->window_height);
    env->ligne_depart.x = env->window_width / (game_nb_rows(env->jeu) + 2);
    env->ligne_depart.y = env->window_height / (game_nb_cols(env->jeu) + 2);
    env->cases = malloc(sizeof(SDL_Rect) * game_nb_rows(env->jeu) * (game_nb_cols(env->jeu)));
    env->buttons = malloc(sizeof(SDL_Rect)*6);
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
    int button_size = (size/6)*0.8;
    int button_height = env->ligne_depart.y/2;
    int space = (size/6)*0.1;
    int first_last_space = (size/6)*0.25;

    for (int r = 0; r < 6; r++){
        env->buttons[r].x = first_last_space + centrage + r * space + r * button_size;
        env->buttons[r].y = button_height;
        env->buttons[r].h = button_height/2;
        env->buttons[r].w = button_size;
        SDL_RenderDrawRect(ren, &(env->buttons[r]));
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_RenderFillRect(ren, &(env->buttons[r]));
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    }

}

/* **************************************************************** */

bool process(SDL_Window *win, SDL_Renderer *ren, Env *env, SDL_Event *e)
{
    int w, h;
    SDL_GetWindowSize(win, &w, &h);
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

        /*I had to copy the following to check if our click is in a button or not. In order to do this, I needed some measurements such as the size of the window and the size of the button.*/
        int size;
        if(env->window_height < env->window_width){
            size = env->window_height;
        }
        else
        {
            size = env->window_width;
        }
        
        //printf("%d %d %d\n", (env->buttons[0]),(env->buttons[4]), mouse.x);

        /*int button1_pos_x_start = first_last_space + centrage;
        int button1_pos_x_end = button1_pos_x_start + button_size;
        int button1_pos_y_start = button_height;
        int button1_pos_y_end = button1_pos_y_start + (button_height/2);

        int button2_pos_x_start = button1_pos_x_end + space;
        int button2_pos_x_end = (button1_pos_x_end + button1_pos_y_end) + space + button_size;

        int button3_pos_x_start = button2_start + space + button_size;
        int button3_pos_x_end = button2_end + space + button_size;
        int button4_pos_x_start = button3_start + space + button_size;
        int button4_pos_x_end = button3_end + space + button_size;
        int button5_pos_x_start = button4_start + space + button_size;
        int button5_pos_x_end = button4_end + space + button_size;*/
        

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
            }else{
                if (game_check_move(env->jeu, case_i, case_j, S_MARK))
                {
                    game_play_move(env->jeu, case_i, case_j, S_MARK);
                }
            }
        }else if (SDL_PointInRect(&mouse, &(env->buttons[0]))){
            printf("Restart");
            game_restart(env->jeu);
        }/*else if (mouse.x > button2_start && mouse.x < (button2_start + button_size) && mouse.y > button2_start+(button_height/2) && mouse.y < (button2_start + button_size + (button_height/2))){
            printf("Solve");
            game_solve(env->jeu);
        }else if (mouse.x > button3_start && mouse.x < button3_end){
            printf("Undo");
            game_undo(env->jeu);
        }else if (mouse.x > button4_start && mouse.x < button4_end){
            printf("Redo");
            game_redo(env->jeu);
        }*/else if (SDL_PointInRect(&mouse, &(env->buttons[4]))){
            printf("Help");
            //game_help();
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
