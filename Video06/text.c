#include "text.h"

void blit_symmetric_points(SDL_Surface *text_surf, SDL_Surface *target_surf,
                           int radius, int x, int y);
bool text_create_bubble(struct Text *t, SDL_Color outer_color,
                        SDL_Color inner_color, const char *str, int size,
                        int radius);

bool text_new(struct Text **text, SDL_Renderer *renderer) {
    *text = calloc(1, sizeof(struct Text));
    if (*text == NULL) {
        fprintf(stderr, "Error in Calloc of New Text.\n");
        return false;
    }
    struct Text *t = *text;

    t->renderer = renderer;

    t->font = TTF_OpenFont("fonts/freesansbold.ttf", TEXT_SIZE);
    if (!t->font) {
        fprintf(stderr, "Error Opening Font: %s\n", SDL_GetError());
        return false;
    }

    if (!text_create_bubble(t, OUTER_COLOR, INNER_COLOR, TEXT_STR, TEXT_SIZE,
                            10)) {
        return false;
    }

    t->x_vel = TEXT_VEL;
    t->y_vel = TEXT_VEL;

    return true;
}

void text_free(struct Text **text) {
    if (*text) {
        struct Text *t = *text;

        if (t->image) {
            SDL_DestroyTexture(t->image);
            t->image = NULL;
        }
        if (t->target_surf) {
            SDL_DestroySurface(t->target_surf);
            t->target_surf = NULL;
        }
        if (t->text_surf) {
            SDL_DestroySurface(t->text_surf);
            t->text_surf = NULL;
        }
        if (t->font) {
            TTF_CloseFont(t->font);
            t->font = NULL;
        }

        t->renderer = NULL;

        free(t);
        t = NULL;
        *text = NULL;

        printf("Text free.\n");
    }
}

void blit_symmetric_points(SDL_Surface *text_surf, SDL_Surface *target_surf,
                           int radius, int x, int y) {
    SDL_BlitSurface(
        text_surf, NULL, target_surf,
        &(SDL_Rect){radius + x, radius + y, text_surf->w, text_surf->h});
    SDL_BlitSurface(
        text_surf, NULL, target_surf,
        &(SDL_Rect){radius + x, radius - y, text_surf->w, text_surf->h});
    SDL_BlitSurface(
        text_surf, NULL, target_surf,
        &(SDL_Rect){radius - x, radius + y, text_surf->w, text_surf->h});
    SDL_BlitSurface(
        text_surf, NULL, target_surf,
        &(SDL_Rect){radius - x, radius - y, text_surf->w, text_surf->h});
    SDL_BlitSurface(
        text_surf, NULL, target_surf,
        &(SDL_Rect){radius + y, radius + x, text_surf->w, text_surf->h});
    SDL_BlitSurface(
        text_surf, NULL, target_surf,
        &(SDL_Rect){radius + y, radius - x, text_surf->w, text_surf->h});
    SDL_BlitSurface(
        text_surf, NULL, target_surf,
        &(SDL_Rect){radius - y, radius + x, text_surf->w, text_surf->h});
    SDL_BlitSurface(
        text_surf, NULL, target_surf,
        &(SDL_Rect){radius - y, radius - x, text_surf->w, text_surf->h});
}

bool text_create_bubble(struct Text *t, SDL_Color outer_color,
                        SDL_Color inner_color, const char *str, int size,
                        int radius) {
    t->text_surf = TTF_RenderText_Blended(t->font, TEXT_STR, 0, outer_color);
    if (!t->text_surf) {
        fprintf(stderr, "Error creating outer text Surface: %s\n",
                SDL_GetError());
        return false;
    }

    int padding = radius * 2;
    t->target_surf =
        SDL_CreateSurface(t->text_surf->w + padding, t->text_surf->h + padding,
                          SDL_PIXELFORMAT_ARGB32);
    if (!t->target_surf) {
        fprintf(stderr, "Error creating Target Surface: %s\n", SDL_GetError());
        return false;
    }

    t->rect.w = t->target_surf->w;
    t->rect.h = t->target_surf->h;

    // // Polar Coordinates Trigonometry Algorithm
    // for (int index = 0; index < (int)(2 * M_PI * radius); index++) {
    //     double rad = (double)index / radius;
    //     int x = (int)(cos(rad) * radius) + radius;
    //     int y = (int)(sin(rad) * radius) + radius;
    //     SDL_BlitSurface(t->text_surf, NULL, t->target_surf,
    //                     &(SDL_Rect){x, y, t->text_surf->w, t->text_surf->h});
    // }

    // Bresenham's Circle Drawing Algorithm
    // https://www.geeksforgeeks.org/bresenhams-circle-drawing-algorithm/
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;
    blit_symmetric_points(t->text_surf, t->target_surf, radius, x, y);
    while (y >= x) {
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
        blit_symmetric_points(t->text_surf, t->target_surf, radius, x, y);
    }

    SDL_DestroySurface(t->text_surf);
    t->text_surf = NULL;
    t->text_surf = TTF_RenderText_Blended(t->font, str, 0, inner_color);
    if (!t->text_surf) {
        fprintf(stderr, "Error creating inner text Surface: %s\n",
                SDL_GetError());
        return false;
    }

    SDL_BlitSurface(
        t->text_surf, NULL, t->target_surf,
        &(SDL_Rect){radius, radius, t->text_surf->w, t->text_surf->h});

    t->image = SDL_CreateTextureFromSurface(t->renderer, t->target_surf);
    if (!t->image) {
        fprintf(stderr, "Error creating Texture from Surface: %s\n",
                SDL_GetError());
        return false;
    }

    return true;
}

void text_update(struct Text *t) {
    t->rect.x += t->x_vel;
    t->rect.y += t->y_vel;
    if (t->rect.x + t->rect.w > WINDOW_WIDTH) {
        t->x_vel = -TEXT_VEL;
    }
    if (t->rect.x < 0) {
        t->x_vel = TEXT_VEL;
    }
    if (t->rect.y + t->rect.h > WINDOW_HEIGHT) {
        t->y_vel = -TEXT_VEL;
    }
    if (t->rect.y < 0) {
        t->y_vel = TEXT_VEL;
    }
}

void text_draw(const struct Text *t) {
    SDL_RenderTexture(t->renderer, t->image, NULL, &t->rect);
}
