#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define SDL_FLAGS SDL_INIT_VIDEO

#define WINDOW_TITLE "Open Window"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

struct Game {
        SDL_Window *window;
        SDL_Renderer *renderer;
};

bool game_init_sdl(struct Game *g);
void game_free(struct Game *g);
void game_run(struct Game *g);

bool game_init_sdl(struct Game *g) {
    if (!SDL_Init(SDL_FLAGS)) {
        fprintf(stderr, "Error initializing SDL3: %s\n", SDL_GetError());
        return false;
    }

    g->window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!g->window) {
        fprintf(stderr, "Error creating Window: %s\n", SDL_GetError());
        return false;
    }

    g->renderer = SDL_CreateRenderer(g->window, NULL);
    if (!g->renderer) {
        fprintf(stderr, "Error creating Renderer: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

void game_free(struct Game *g) {
    if (g->renderer) {
        SDL_DestroyRenderer(g->renderer);
        g->renderer = NULL;
    }

    if (g->window) {
        SDL_DestroyWindow(g->window);
        g->window = NULL;
    }

    SDL_Quit();
}

void game_run(struct Game *g) {
    SDL_Delay(100);

    SDL_RenderClear(g->renderer);

    SDL_RenderPresent(g->renderer);

    SDL_Delay(5000);
}

int main(void) {
    bool exit_status = EXIT_FAILURE;

    struct Game game = {0};

    if (game_init_sdl(&game)) {
        game_run(&game);

        exit_status = EXIT_SUCCESS;
    }

    game_free(&game);

    return exit_status;
}
