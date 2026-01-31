#include <stdlib.h>
#include <stdbool.h>

#include "CardStack.h"
#include "Card.h"
#include "ResourceManager.h"

static Texture2D *grayTextures[4];
static bool initialized = false;
static void updateDropRect( CardStack *s );

void initCardStack( CardStack *s ) {
    s->top = -1;
}

void pushCardStack( CardStack *s, Card *c ) {
    if ( s->top < 52 ) {
        s->cards[++(s->top)] = c;
        c->belongsTo = s;
    }
}

Card *popCardStack( CardStack *s ) {
    if ( s->top != -1 ) {
        s->cards[s->top]->belongsTo = NULL;
        return s->cards[(s->top)--];
    }
    return NULL;
}

Card *peekCardStack( CardStack *s ) {
    if ( s->top != -1 ) {
        return s->cards[s->top];
    }
    return NULL;
}

void drawCardStack( CardStack *s, Card *excludeFromDrawing ) {

    if ( !initialized ) {
        grayTextures[0] = &rm.heartsGray;
        grayTextures[1] = &rm.diamondsGray;
        grayTextures[2] = &rm.clubGray;
        grayTextures[3] = &rm.spadesGray;
        initialized = true;
    }

    DrawRectangleRoundedLinesEx( s->rect, 0.2, 10, 2, LIGHTGRAY );

    switch ( s->type ) {
        case CARD_STACK_TYPE_AVAILABLE:
            DrawRing( 
                (Vector2) {
                    s->rect.x + s->rect.width / 2,
                    s->rect.y + s->rect.height / 2
                }, 
                s->rect.width / 2 - 10 - 4, 
                s->rect.width / 2 - 10, 
                0, 
                360, 
                50, 
                LIGHTGRAY
            );
            break;
        case CARD_STACK_TYPE_CHECKING:
        case CARD_STACK_TYPE_TRANSFER:
            break;
        case CARD_STACK_TYPE_OK_H:
        case CARD_STACK_TYPE_OK_D:
        case CARD_STACK_TYPE_OK_C:
        case CARD_STACK_TYPE_OK_S:
            DrawTexture( 
                *(grayTextures[s->type-2]),
                s->rect.x + s->rect.width / 2 - grayTextures[s->type-2]->width / 2,
                s->rect.y + s->rect.height / 2 - grayTextures[s->type-2]->height / 2,
                WHITE
            );
            break;
        case CARD_STACK_TYPE_TEMP:
            break;
    }

    for ( int i = 0; i <= s->top; i++ ) {
        if ( s->cards[i] != excludeFromDrawing ) {
            drawCard( s->cards[i] );
        }
    }

    //DrawRectangleLinesEx( s->dropRect, 2, ORANGE );

}

void updateCardsPositionFromCardStack( CardStack *s, float verticalSpacing ) {

    for ( int i = 0; i <= s->top; i++ ) {
        s->cards[i]->rect.x = s->rect.x;
        s->cards[i]->rect.y = s->rect.y + verticalSpacing * i;
        if ( i < s->top ) {
            s->cards[i]->flipped = true;
        }
    }

    updateDropRect( s );

}

void updateCardsPositionFromCardStackAvailable( CardStack *s, float diagonalSpacing ) {

    for ( int i = 0; i <= s->top; i++ ) {
        s->cards[i]->rect.x = s->rect.x - diagonalSpacing * i;
        s->cards[i]->rect.y = s->rect.y - diagonalSpacing * i;
        s->cards[i]->flipped = true;
    }

    updateDropRect( s );

}

void updateCardsPositionFromCardStackChecking( CardStack *s, float diagonalSpacing ) {

    for ( int i = 0; i <= s->top; i++ ) {
        s->cards[i]->rect.x = s->rect.x - diagonalSpacing * i;
        s->cards[i]->rect.y = s->rect.y - diagonalSpacing * i;
        s->cards[i]->flipped = false;
    }

    updateDropRect( s );

}

void reorganizeCardsPositionFromTempCardStack( CardStack *s, float verticalSpacing ) {

    for ( int i = 0; i <= s->top; i++ ) {
        s->cards[i]->rect.x = s->rect.x;
        s->cards[i]->rect.y = s->rect.y + verticalSpacing * i;
    }

    updateDropRect( s );

}

void reorganizeCardsPositionFromStackedCardStack( CardStack *s, float diagonalSpacing ) {

    for ( int i = 0; i <= s->top; i++ ) {
        s->cards[i]->rect.x = s->rect.x - diagonalSpacing * i;
        s->cards[i]->rect.y = s->rect.y - diagonalSpacing * i;
    }

    updateDropRect( s );

}

static void updateDropRect( CardStack *s ) {

    s->dropRect.x = s->rect.x;
    s->dropRect.y = s->rect.y;
    s->dropRect.width = s->rect.width;

    if ( s->top != -1 ) {
        s->dropRect.height = s->cards[s->top]->rect.y + s->cards[s->top]->rect.height - s->rect.y;
    } else {
        s->dropRect.height = s->rect.height;
    }

}