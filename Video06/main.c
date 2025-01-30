#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SDL_FLAGS SDL_INIT_VIDEO

#define WINDOW_TITLE "Moving Text & Icon"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define TEXT_SIZE 80

struct Game {
        SDL_Window *window;
        SDL_Renderer *renderer;
        bool is_running;
        SDL_Event event;
        SDL_Texture *background;
        TTF_Font *text_font;
        SDL_Color text_color;
        SDL_FRect text_rect;
        SDL_Texture *text_image;
        float text_xvel;
        float text_yvel;
};

bool game_load_media(struct Game *g);
bool game_init_sdl(struct Game *g);
bool game_new(struct Game *g);
void game_free(struct Game *g);
void game_text_update(struct Game *g);
void game_render_color(struct Game *g);
void game_events(struct Game *g);
void game_update(struct Game *g);
void game_draw(const struct Game *g);
void game_run(struct Game *g);

bool game_init_sdl(struct Game *g) {
    if (!SDL_Init(SDL_FLAGS)) {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return false;
    }

    if (!TTF_Init()) {
        fprintf(stderr, "Error initializing SDL_ttf: %s\n", SDL_GetError());
        return false;
    }

    g->window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!g->window) {
        fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
        return false;
    }

    g->renderer = SDL_CreateRenderer(g->window, NULL);
    if (!g->renderer) {
        fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
        return false;
    }

    SDL_Surface *icon_surf = IMG_Load("images/C-logo.png");
    if (!icon_surf) {
        fprintf(stderr, "Error loading Surface: %s\n", SDL_GetError());
        return false;
    }
    SDL_SetWindowIcon(g->window, icon_surf);
    SDL_DestroySurface(icon_surf);

    return true;
}

bool game_load_media(struct Game *g) {
    g->background = IMG_LoadTexture(g->renderer, "images/background.png");
    if (!g->background) {
        fprintf(stderr, "Error creating Texture: %s\n", SDL_GetError());
        return false;
    }

    g->text_font = TTF_OpenFont("fonts/freesansbold.ttf", TEXT_SIZE);
    if (!g->text_font) {
        fprintf(stderr, "Error creating Font: %s\n", SDL_GetError());
        return false;
    }

    SDL_Surface *surface =
        TTF_RenderText_Blended(g->text_font, "SDL", 0, g->text_color);
    if (!surface) {
        fprintf(stderr, "Error creating Surface: %s\n", SDL_GetError());
        return false;
    }
    g->text_rect.w = (float)surface->w;
    g->text_rect.h = (float)surface->h;
    g->text_image = SDL_CreateTextureFromSurface(g->renderer, surface);
    SDL_DestroySurface(surface);
    if (!g->text_image) {
        fprintf(stderr, "Error creating Texture: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

bool game_new(struct Game *g) {
    g->text_color = (SDL_Color){255, 255, 255, 255};
    g->text_rect = (SDL_FRect){100, 0, 0, 0};
    g->text_xvel = 3;
    g->text_yvel = 3;
    g->is_running = true;

    if (!game_init_sdl(g)) {
        return false;
    }

    if (!game_load_media(g)) {
        return false;
    }

    srand((unsigned)time(NULL));

    return true;
}

void game_free(struct Game *g) {
    if (g->text_image) {
        SDL_DestroyTexture(g->text_image);
        g->text_image = NULL;
    }

    if (g->text_font) {
        TTF_CloseFont(g->text_font);
        g->text_font = NULL;
    }

    if (g->background) {
        SDL_DestroyTexture(g->background);
        g->background = NULL;
    }

    if (g->renderer) {
        SDL_DestroyRenderer(g->renderer);
        g->renderer = NULL;
    }

    if (g->window) {
        SDL_DestroyWindow(g->window);
        g->window = NULL;
    }

    TTF_Quit();
    SDL_Quit();

    printf("all clean!\n");
}

void game_text_update(struct Game *g) {
    g->text_rect.x += g->text_xvel;
    g->text_rect.y += g->text_yvel;
    if (g->text_rect.x + g->text_rect.w > WINDOW_WIDTH) {
        g->text_xvel = -3;
    }
    if (g->text_rect.x < 0) {
        g->text_xvel = 3;
    }
    if (g->text_rect.y + g->text_rect.h > WINDOW_HEIGHT) {
        g->text_yvel = -3;
    }
    if (g->text_rect.y < 0) {
        g->text_yvel = 3;
    }
}

void game_render_color(struct Game *g) {
    SDL_SetRenderDrawColor(g->renderer, (Uint8)rand() % 256,
                           (Uint8)rand() % 256, (Uint8)rand() % 256, 255);
}

void game_events(struct Game *g) {
    while (SDL_PollEvent(&g->event)) {
        switch (g->event.type) {
        case SDL_EVENT_QUIT:
            g->is_running = false;
            break;
        case SDL_EVENT_KEY_DOWN:
            switch (g->event.key.scancode) {
            case SDL_SCANCODE_ESCAPE:
                g->is_running = false;
                break;
            case SDL_SCANCODE_SPACE:
                game_render_color(g);
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}

void game_update(struct Game *g) { game_text_update(g); }

void game_draw(const struct Game *g) {
    SDL_RenderClear(g->renderer);

    SDL_RenderTexture(g->renderer, g->background, NULL, NULL);
    SDL_RenderTexture(g->renderer, g->text_image, NULL, &g->text_rect);

    SDL_RenderPresent(g->renderer);
}

void game_run(struct Game *g) {
    while (g->is_running) {

        game_events(g);

        game_update(g);

        game_draw(g);

        SDL_Delay(16);
    }
}

int main(void) {
    bool exit_status = EXIT_FAILURE;

    struct Game game = {0};

    if (game_new(&game)) {

        game_run(&game);

        exit_status = EXIT_SUCCESS;
    }

    game_free(&game);

    return exit_status;
}
