#include <stdbool.h>

#include "raylib/raylib.h"

#include "ResourceManager.h"
#include "Types.h"
#include "Card.h"

static char *values[] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
static Texture2D *suitsTSmall[4];
static Texture2D *suitsTBig[4];
static bool initialized = false;

void drawCard( Card *c ) {
    
    if ( !initialized ) {
        suitsTSmall[0] = &rm.heartsSmall;
        suitsTSmall[1] = &rm.diamondsSmall;
        suitsTSmall[2] = &rm.clubSmall;
        suitsTSmall[3] = &rm.spadesSmall;
        suitsTBig[0] = &rm.heartsBig;
        suitsTBig[1] = &rm.diamondsBig;
        suitsTBig[2] = &rm.clubBig;
        suitsTBig[3] = &rm.spadesBig;
        initialized = true;
    }

    if ( c->flipped ) {
        DrawRectangleRounded( (Rectangle) { c->rect.x, c->rect.y, c->rect.width, c->rect.height }, 0.2, 10, BLUE );
        DrawRectangleRounded( (Rectangle) { c->rect.x + 5, c->rect.y + 5, c->rect.width - 10, c->rect.height - 10 }, 0.2, 10, WHITE );
        DrawRectangleRounded( (Rectangle) { c->rect.x + 10, c->rect.y + 10, c->rect.width - 20, c->rect.height - 20 }, 0.2, 10, BLUE );
    } else {
        DrawRectangleRounded( (Rectangle) { c->rect.x, c->rect.y, c->rect.width, c->rect.height }, 0.2, 10, WHITE );

        DrawText( values[c->value], c->rect.x + 3, c->rect.y, 30, c->color );
        DrawText( values[c->value], c->rect.x + c->rect.width - MeasureText(values[c->value], 30) - 3, c->rect.y + c->rect.height - 28, 30, c->color );

        Texture2D *s = suitsTSmall[c->suit];
        Texture2D *b = suitsTBig[c->suit];

        DrawTexture( *s, c->rect.x + c->rect.width - s->width - 3, c->rect.y + 3, WHITE );
        DrawTexture( *s, c->rect.x + 3, c->rect.y + c->rect.height - s->height - 4, WHITE );
        DrawTexture( *b, c->rect.x + c->rect.width / 2 - b->width / 2, c->rect.y + c->rect.height / 2 - b->height / 2, WHITE );
    }

    DrawRectangleRoundedLinesEx( (Rectangle) { c->rect.x, c->rect.y, c->rect.width, c->rect.height }, 0.2, 10, 2, BLACK );

}
