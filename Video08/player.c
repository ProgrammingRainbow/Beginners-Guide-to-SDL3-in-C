#include "player.h"
#include "bubble.h"

bool player_new(struct Player **player, SDL_Renderer *renderer) {
    *player = calloc(1, sizeof(struct Player));
    if (*player == NULL) {
        fprintf(stderr, "Error Calloc of New Player.\n");
        return false;
    }
    struct Player *p = *player;

    p->renderer = renderer;

    SDL_Surface *surface = IMG_Load("images/C-logo.png");
    if (!surface) {
        fprintf(stderr, "Error loading Surface.\n");
        return false;
    }

    SDL_Surface *bubble_surf =
        bubble_surface(surface, BUBBLE_RADIUS, WHITE_COLOR);
    if (!surface) {
        SDL_DestroySurface(surface);
        return false;
    }

    p->rect.w = (float)bubble_surf->w;
    p->rect.h = (float)bubble_surf->h;

    p->image = SDL_CreateTextureFromSurface(p->renderer, bubble_surf);
    SDL_DestroySurface(surface);
    SDL_DestroySurface(bubble_surf);
    if (!p->image) {
        fprintf(stderr, "Error creating Texture from Surface: %s\n",
                SDL_GetError());
        return false;
    }

    p->keystate = SDL_GetKeyboardState(NULL);

    return true;
}

void player_free(struct Player **player) {
    if (*player) {
        struct Player *p = *player;

        if (p->surface) {
            SDL_DestroySurface(p->surface);
            p->surface = NULL;
        }
        if (p->image) {
            SDL_DestroyTexture(p->image);
            p->image = NULL;
        }

        p->renderer = NULL;

        free(p);
        p = NULL;
        *player = NULL;

        printf("Free Player.\n");
    }
}

void player_update(struct Player *p) {
    if (p->keystate[SDL_SCANCODE_LEFT] || p->keystate[SDL_SCANCODE_A]) {
        p->rect.x -= PLAYER_VEL;
    }
    if (p->keystate[SDL_SCANCODE_RIGHT] || p->keystate[SDL_SCANCODE_D]) {
        p->rect.x += PLAYER_VEL;
    }
    if (p->keystate[SDL_SCANCODE_UP] || p->keystate[SDL_SCANCODE_W]) {
        p->rect.y -= PLAYER_VEL;
    }
    if (p->keystate[SDL_SCANCODE_DOWN] || p->keystate[SDL_SCANCODE_S]) {
        p->rect.y += PLAYER_VEL;
    }
}

void player_draw(const struct Player *p) {
    SDL_RenderTexture(p->renderer, p->image, NULL, &p->rect);
}
