#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SDL_FLAGS (SDL_INIT_VIDEO | SDL_INIT_AUDIO)
#define MIXER_FLAGS MIX_INIT_OGG

#define WINDOW_TITLE "Sound and Music"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define TEXT_SIZE 80
#define TEXT_COLOR (SDL_Color){255, 255, 255, 255}
#define TEXT_STR "SDL"
#define TEXT_VEL 3

#define SPRITE_VEL 5

struct Game {
        SDL_Window *window;
        SDL_Renderer *renderer;
        bool is_running;
        SDL_Event event;
        SDL_Texture *background;
        TTF_Font *text_font;
        SDL_FRect text_rect;
        SDL_Texture *text_image;
        float text_xvel;
        float text_yvel;
        SDL_Texture *sprite_image;
        SDL_FRect sprite_rect;
        const bool *keystate;
        Mix_Chunk *c_sound;
        Mix_Chunk *sdl_sound;
        Mix_Music *music;
};

bool game_load_media(struct Game *g);
bool game_init_sdl(struct Game *g);
bool game_new(struct Game *g);
void game_free(struct Game *g);
void game_text_update(struct Game *g);
void game_sprite_update(struct Game *g);
void game_render_color(struct Game *g);
void game_toggle_mute(void);
void game_events(struct Game *g);
void game_update(struct Game *g);
void game_draw(const struct Game *g);
bool game_run(struct Game *g);

bool game_init_sdl(struct Game *g) {
    if (!SDL_Init(SDL_FLAGS)) {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return false;
    }

    if (!TTF_Init()) {
        fprintf(stderr, "Error initializing SDL_ttf: %s\n", SDL_GetError());
        return false;
    }

    MIX_InitFlags mix_init = Mix_Init(MIXER_FLAGS);
    if ((mix_init & MIXER_FLAGS) != MIXER_FLAGS) {
        fprintf(stderr, "Error initializing SDL_mixer: %s\n", SDL_GetError());
        return false;
    }

    SDL_AudioSpec audiospec;
    audiospec.freq = MIX_DEFAULT_FREQUENCY;
    audiospec.format = MIX_DEFAULT_FORMAT;
    audiospec.channels = MIX_DEFAULT_CHANNELS;

    if (!Mix_OpenAudio(0, &audiospec)) {
        fprintf(stderr, "Error Opening Audio: %s\n", SDL_GetError());
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
        TTF_RenderText_Blended(g->text_font, TEXT_STR, 0, TEXT_COLOR);
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

    g->sprite_image = IMG_LoadTexture(g->renderer, "images/C-logo.png");
    if (!g->sprite_image) {
        fprintf(stderr, "Error creating Texture: %s\n", SDL_GetError());
        return false;
    }

    if (!SDL_GetTextureSize(g->sprite_image, &g->sprite_rect.w,
                            &g->sprite_rect.h)) {
        fprintf(stderr, "Error getting texture size: %s\n", SDL_GetError());
        return false;
    }

    g->c_sound = Mix_LoadWAV("sounds/C.ogg");
    if (!g->c_sound) {
        fprintf(stderr, "Error loading Chunk: %s\n", SDL_GetError());
        return false;
    }

    g->sdl_sound = Mix_LoadWAV("sounds/SDL.ogg");
    if (!g->sdl_sound) {
        fprintf(stderr, "Error loading Chunk: %s\n", SDL_GetError());
        return false;
    }

    g->music = Mix_LoadMUS("music/freesoftwaresong-8bit.ogg");
    if (!g->music) {
        fprintf(stderr, "Error loading Music: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

bool game_new(struct Game *g) {
    g->text_xvel = TEXT_VEL;
    g->text_yvel = TEXT_VEL;
    g->keystate = SDL_GetKeyboardState(NULL);
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

    if (g->sprite_image) {
        SDL_DestroyTexture(g->sprite_image);
        g->sprite_image = NULL;
    }

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
        g->text_xvel = -TEXT_VEL;
        Mix_PlayChannel(-1, g->sdl_sound, 0);
    }
    if (g->text_rect.x < 0) {
        g->text_xvel = TEXT_VEL;
        Mix_PlayChannel(-1, g->sdl_sound, 0);
    }
    if (g->text_rect.y + g->text_rect.h > WINDOW_HEIGHT) {
        g->text_yvel = -TEXT_VEL;
        Mix_PlayChannel(-1, g->sdl_sound, 0);
    }
    if (g->text_rect.y < 0) {
        g->text_yvel = TEXT_VEL;
        Mix_PlayChannel(-1, g->sdl_sound, 0);
    }
}

void game_sprite_update(struct Game *g) {
    if (g->keystate[SDL_SCANCODE_LEFT] || g->keystate[SDL_SCANCODE_A]) {
        g->sprite_rect.x -= SPRITE_VEL;
    }
    if (g->keystate[SDL_SCANCODE_RIGHT] || g->keystate[SDL_SCANCODE_D]) {
        g->sprite_rect.x += SPRITE_VEL;
    }
    if (g->keystate[SDL_SCANCODE_UP] || g->keystate[SDL_SCANCODE_W]) {
        g->sprite_rect.y -= SPRITE_VEL;
    }
    if (g->keystate[SDL_SCANCODE_DOWN] || g->keystate[SDL_SCANCODE_S]) {
        g->sprite_rect.y += SPRITE_VEL;
    }
}

void game_render_color(struct Game *g) {
    SDL_SetRenderDrawColor(g->renderer, (Uint8)rand() % 256,
                           (Uint8)rand() % 256, (Uint8)rand() % 256, 255);
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
    game_text_update(g);
    game_sprite_update(g);
}

void game_draw(const struct Game *g) {
    SDL_RenderClear(g->renderer);

    SDL_RenderTexture(g->renderer, g->background, NULL, NULL);
    SDL_RenderTexture(g->renderer, g->text_image, NULL, &g->text_rect);
    SDL_RenderTexture(g->renderer, g->sprite_image, NULL, &g->sprite_rect);

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

int main(void) {
    bool exit_status = EXIT_FAILURE;

    struct Game game = {0};

    if (game_new(&game)) {
        if (game_run(&game)) {
            exit_status = EXIT_SUCCESS;
        }
    }

    game_free(&game);

    return exit_status;
}
