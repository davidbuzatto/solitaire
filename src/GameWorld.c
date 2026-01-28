/**
 * @file GameWorld.h
 * @author Prof. Dr. David Buzatto
 * @brief GameWorld implementation.
 * 
 * @copyright Copyright (c) 2025
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "GameWorld.h"
#include "Card.h"
#include "CardStack.h"
#include "ResourceManager.h"

#include "raylib/raylib.h"
//#include "raylib/raymath.h"
//#define RAYGUI_IMPLEMENTATION    // to use raygui, comment these three lines.
//#include "raylib/raygui.h"       // other compilation units must only include
//#undef RAYGUI_IMPLEMENTATION     // raygui.h

static Card *selectedCard = NULL;
static Vector2 pressOffset;

static float stackVerticalSpacing = 10;
static float stackDiagonalSpacing = 0.2f;

/**
 * @brief Creates a dinamically allocated GameWorld struct instance.
 */
GameWorld* createGameWorld( void ) {

    GameWorld *gw = (GameWorld*) malloc( sizeof( GameWorld ) );

    Suit suits[] = { SUIT_H, SUIT_D, SUIT_C, SUIT_S };
    Color colors[] = { RED, RED, BLACK, BLACK };
    Vector2 dim = { 70, 100 };

    for ( int i = 0; i < 52; i++ ) {
        gw->deck.cards[i] = (Card) {
            .suit = suits[i/13],
            .value = i % 13,
            .rect = {
                0, 
                0, 
                dim.x,
                dim.y
            },
            .color = colors[i/13],
            .flipped = false
        };
    }

    gw->stacks[0] = &gw->available;
    gw->stacks[1] = &gw->checking;
    gw->stacks[2] = &gw->okHearts;
    gw->stacks[3] = &gw->okDiamonds;
    gw->stacks[4] = &gw->okClub;
    gw->stacks[5] = &gw->okSpades;
    gw->stacks[6] = &gw->temp1;
    gw->stacks[7] = &gw->temp2;
    gw->stacks[8] = &gw->temp3;
    gw->stacks[9] = &gw->temp4;
    gw->stacks[10] = &gw->temp5;
    gw->stacks[11] = &gw->temp6;
    gw->stacks[12] = &gw->temp7;

    for ( int i = 0; i < 13; i++ ) {
        initCardStack( gw->stacks[i] );
    }

    gw->available.type = CARD_STACK_TYPE_AVAILABLE;
    gw->checking.type = CARD_STACK_TYPE_CHECKING;

    int counts[] = { 1, 2, 3, 4, 5, 6, 7 };
    int k = 0;

    for ( int i = 0; i < 7; i++ ) {
        for ( int j = 0; j < counts[i]; j++ ) {
            pushCardStack( gw->stacks[i+6], &gw->deck.cards[k++] );
        }
    }

    for ( ; k < 52; k++ ) {
        pushCardStack( &gw->available, &gw->deck.cards[k] );
    }

    int topMargin = 30;
    int leftMargin = 30;
    int horizontalSpacing = 30;
    int verticalSpacing = 30;

    for ( int i = 0; i < 7; i++ ) {
        CardStack *s = gw->stacks[i+6];
        s->type = CARD_STACK_TYPE_TEMP;
        s->rect.x = leftMargin + dim.x * i + horizontalSpacing * i;
        s->rect.y = topMargin + dim.y + verticalSpacing;
        s->rect.width = dim.x;
        s->rect.height = dim.y;
        updateCardsPositionFromCardStack( s, stackVerticalSpacing );
    }

    gw->available.rect.x = leftMargin;
    gw->available.rect.y = topMargin;
    gw->available.rect.width = dim.x;
    gw->available.rect.height = dim.y;
    updateCardsPositionFromCardStackAvailable( &gw->available, stackDiagonalSpacing );

    gw->checking.rect.x = leftMargin + dim.x + horizontalSpacing;
    gw->checking.rect.y = topMargin;
    gw->checking.rect.width = dim.x;
    gw->checking.rect.height = dim.y;
    updateCardsPositionFromCardStackChecking( &gw->checking, stackDiagonalSpacing );

    for ( int i = 0; i < 4; i++ ) {
        CardStack *s = gw->stacks[i+2];
        s->type = i+2;
        s->rect.x = leftMargin + dim.x * (i+3) + horizontalSpacing * (i+3);
        s->rect.y = topMargin;
        s->rect.width = dim.x;
        s->rect.height = dim.y;
    }

    return gw;

}

/**
 * @brief Destroys a GameWindow object and its dependecies.
 */
void destroyGameWorld( GameWorld *gw ) {
    free( gw );
}

/**
 * @brief Reads user input and updates the state of the game.
 */
void updateGameWorld( GameWorld *gw, float delta ) {

    if ( IsMouseButtonPressed( MOUSE_BUTTON_LEFT ) ) {

        selectedCard = NULL;

        for ( int i = 0; i < 52; i++ ) {
            Card *c = &gw->deck.cards[i];
            if ( !c->flipped && CheckCollisionPointRec( GetMousePosition(), c->rect ) ) {
                selectedCard = c;
                pressOffset.x = GetMouseX() - c->rect.x;
                pressOffset.y = GetMouseY() - c->rect.y;
                break;
            }
        }

        if ( CheckCollisionPointRec( GetMousePosition(), gw->available.rect ) ) {
            if ( gw->available.top != -1 ) {
                Card *c = popCardStack( &gw->available );
                pushCardStack( &gw->checking, c );
                updateCardsPositionFromCardStackAvailable( &gw->available, stackDiagonalSpacing );
                updateCardsPositionFromCardStackChecking( &gw->checking, stackDiagonalSpacing );
            } else {
                while ( gw->checking.top != -1 ) {
                    Card *c = popCardStack( &gw->checking );
                    pushCardStack( &gw->available, c );
                }
                updateCardsPositionFromCardStackAvailable( &gw->available, stackDiagonalSpacing );
                updateCardsPositionFromCardStackChecking( &gw->checking, stackDiagonalSpacing );
            }
        }

    }

    if ( IsMouseButtonReleased( MOUSE_BUTTON_LEFT ) ) {
        selectedCard = NULL;
    }

    if ( selectedCard != NULL ) {
        selectedCard->rect.x = GetMouseX() - pressOffset.x;
        selectedCard->rect.y = GetMouseY() - pressOffset.y;
    }

}

/**
 * @brief Draws the state of the game.
 */
void drawGameWorld( GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( DARKGREEN );

    for ( int i = 0; i < 13; i++ ) {
        CardStack *s = gw->stacks[i];
        drawCardStack( s );
        if ( s->top != -1 ) {
            for ( int j = 0; j <= s->top; j++ ) {
                Card *c = s->cards[j];
                if ( selectedCard != c ) {
                    drawCard( c );
                }
            }
        }
    }

    if ( selectedCard != NULL ) {
        drawCard( selectedCard );
    }

    EndDrawing();

}