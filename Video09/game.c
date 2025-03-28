#include "game.h"
#include "init_sdl.h"
#include "load_media.h"

void game_render_color(struct Game *g);
void game_toggle_mute(void);
void game_events(struct Game *g);
void game_update(struct Game *g);
void game_draw(const struct Game *g);

bool game_new(struct Game **game) {
    *game = calloc(1, sizeof(struct Game));
    if (*game == NULL) {
        fprintf(stderr, "Error Calloc of New Game.\n");
        return false;
    }
    struct Game *g = *game;

    if (!game_init_sdl(g)) {
        return false;
    }

    if (!game_load_media(g)) {
        return false;
    }

    if (!text_new(&g->text, g->renderer, g->sdl_sound)) {
        return false;
    }
    if (!player_new(&g->player, g->renderer)) {
        return false;
    }

    g->is_running = true;

    srand((unsigned)time(NULL));

    return true;
}

void game_free(struct Game **game) {
    if (*game) {
        struct Game *g = *game;
        Mix_HaltMusic();
        Mix_HaltChannel(-1);

        if (g->music) {
            Mix_FreeMusic(g->music);
            g->music = NULL;
        }
        if (g->sdl_sound) {
            Mix_FreeChunk(g->sdl_sound);
            g->sdl_sound = NULL;
        }
        if (g->c_sound) {
            Mix_FreeChunk(g->c_sound);
            g->c_sound = NULL;
        }

        if (g->player) {
            player_free(&g->player);
        }
        if (g->text) {
            text_free(&g->text);
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

        Mix_Quit();
        TTF_Quit();
        SDL_Quit();

        free(g);

        g = NULL;
        *game = NULL;

        printf("All Clean!\n");
    }
}

void game_render_color(struct Game *g) {
    SDL_SetRenderDrawColor(g->renderer, (Uint8)rand(), (Uint8)rand(),
                           (Uint8)rand(), 255);
    Mix_PlayChannel(-1, g->c_sound, 0);
}

void game_toggle_mute(void) {
    if (Mix_PausedMusic()) {
        Mix_ResumeMusic();
    } else {
        Mix_PauseMusic();
    }
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
            case SDL_SCANCODE_M:
                game_toggle_mute();
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

void game_update(struct Game *g) {
    text_update(g->text);
    player_update(g->player);
}

void game_draw(const struct Game *g) {
    SDL_RenderClear(g->renderer);

    SDL_RenderTexture(g->renderer, g->background, NULL, NULL);
    text_draw(g->text);
    player_draw(g->player);

    SDL_RenderPresent(g->renderer);
}

bool game_run(struct Game *g) {
    if (!Mix_PlayMusic(g->music, -1)) {
        fprintf(stderr, "Error Playing Music: %s\n", SDL_GetError());
        return false;
    }

    while (g->is_running) {
        game_events(g);

        game_update(g);

        game_draw(g);

        SDL_Delay(16);
    }

    return true;
}
