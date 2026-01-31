/**
 * @file GameWorld.h
 * @author Prof. Dr. David Buzatto
 * @brief GameWorld implementation.
 * 
 * @copyright Copyright (c) 2026
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
static CardStack *targetStack = NULL;
static Vector2 pressOffset;

static float stackVerticalSpacing = 15;
static float stackDiagonalSpacing = 0.2f;

static void prepareNewGame( GameWorld *gw );

static bool checkCompatibilityOkStack( Card *selected, Card *target );
static bool checkCompatibilityTempStack( Card *selected, Card *target );
static void reorganizeAllStacks( GameWorld *gw );
static Card *resolveSelectedCard( GameWorld *gw );

/**
 * @brief Creates a dinamically allocated GameWorld struct instance.
 */
GameWorld* createGameWorld( void ) {
    GameWorld *gw = (GameWorld*) malloc( sizeof( GameWorld ) );
    prepareNewGame( gw );
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

    if ( IsKeyPressed( KEY_R ) ) {
        prepareNewGame( gw );
    }

    if ( IsMouseButtonPressed( MOUSE_BUTTON_LEFT ) ) {

        selectedCard = resolveSelectedCard( gw );

        if ( selectedCard == NULL ) {

            // check and pick available cards
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

            // flip temp stack card
            for ( int i = 0; i < 7; i++ ) {
                CardStack *tempStack = gw->stacks[i+6];
                Card *topCard = peekCardStack( tempStack );
                if ( topCard != NULL && CheckCollisionPointRec( GetMousePosition(), topCard->rect ) ) {
                    topCard->flipped = false;
                }
            }

        }

    }

    if ( IsMouseButtonReleased( MOUSE_BUTTON_LEFT ) ) {

        if ( selectedCard != NULL ) {

            for ( int i = 0; i < 13; i++ ) {
                CardStack *s = gw->stacks[i];
                if ( CheckCollisionPointRec( GetMousePosition(), s->dropRect ) ) {
                    targetStack = s;
                    break;
                }
            }

            if ( targetStack != NULL ) {

                if ( targetStack->type == CARD_STACK_TYPE_TEMP ) {
                    if ( targetStack->top != -1 ) {
                        Card *topCard = peekCardStack( targetStack );
                        if ( checkCompatibilityTempStack( selectedCard, topCard ) ) {
                            CardStack *selectedCS = selectedCard->belongsTo;
                            if ( selectedCS != NULL ) { // always true
                                popCardStack( selectedCS );
                                pushCardStack( targetStack, selectedCard );
                            } else {
                                TraceLog( LOG_ERROR, "should not be here..." );
                            }
                        }
                    } else {
                        CardStack *selectedCS = selectedCard->belongsTo;
                        if ( selectedCS != NULL ) { // always true
                            popCardStack( selectedCS );
                            pushCardStack( targetStack, selectedCard );
                        } else {
                            TraceLog( LOG_ERROR, "should not be here..." );
                        }
                    }
                } else if ( ( selectedCard->suit == SUIT_H && targetStack->type == CARD_STACK_TYPE_OK_H ) || 
                            ( selectedCard->suit == SUIT_D && targetStack->type == CARD_STACK_TYPE_OK_D ) || 
                            ( selectedCard->suit == SUIT_C && targetStack->type == CARD_STACK_TYPE_OK_C ) || 
                            ( selectedCard->suit == SUIT_S && targetStack->type == CARD_STACK_TYPE_OK_S ) ) {
                    CardStack *selectedCS = selectedCard->belongsTo;
                    if ( targetStack->top == -1 || checkCompatibilityOkStack( selectedCard, peekCardStack( targetStack ) ) ) {
                        popCardStack( selectedCS );
                        pushCardStack( targetStack, selectedCard );
                    }
                }

            }

        }

        selectedCard = NULL;
        targetStack = NULL;
        reorganizeAllStacks( gw );

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
        drawCardStack( s, selectedCard );
    }

    if ( selectedCard != NULL ) {
        drawCard( selectedCard );
    }

    EndDrawing();

}

static bool checkCompatibilityOkStack( Card *selected, Card *target ) {
    return selected != NULL && target != NULL && selected->suit == target->suit && selected->value == target->value + 1;
}

static bool checkCompatibilityTempStack( Card *selected, Card *target ) {
    return selected != NULL && target != NULL && selected->suitColor != target->suitColor && selected->value == target->value - 1;
}

static void reorganizeAllStacks( GameWorld *gw ) {

    reorganizeCardsPositionFromStackedCardStack( &gw->available, stackDiagonalSpacing );
    reorganizeCardsPositionFromStackedCardStack( &gw->checking, stackDiagonalSpacing );
    reorganizeCardsPositionFromStackedCardStack( &gw->okHearts, stackDiagonalSpacing );
    reorganizeCardsPositionFromStackedCardStack( &gw->okDiamonds, stackDiagonalSpacing );
    reorganizeCardsPositionFromStackedCardStack( &gw->okClub, stackDiagonalSpacing );
    reorganizeCardsPositionFromStackedCardStack( &gw->okSpades, stackDiagonalSpacing );

    reorganizeCardsPositionFromTempCardStack( &gw->temp1, stackVerticalSpacing );
    reorganizeCardsPositionFromTempCardStack( &gw->temp2, stackVerticalSpacing );
    reorganizeCardsPositionFromTempCardStack( &gw->temp3, stackVerticalSpacing );
    reorganizeCardsPositionFromTempCardStack( &gw->temp4, stackVerticalSpacing );
    reorganizeCardsPositionFromTempCardStack( &gw->temp5, stackVerticalSpacing );
    reorganizeCardsPositionFromTempCardStack( &gw->temp6, stackVerticalSpacing );
    reorganizeCardsPositionFromTempCardStack( &gw->temp7, stackVerticalSpacing );
    
}

static void prepareNewGame( GameWorld *gw ) {

    selectedCard = NULL;
    targetStack = NULL;

    Suit suits[] = { SUIT_H, SUIT_D, SUIT_C, SUIT_S };
    SuitColor suitsColor[] = { SUIT_COLOR_RED, SUIT_COLOR_RED, SUIT_COLOR_BLACK, SUIT_COLOR_BLACK };
    Color colors[] = { RED, RED, BLACK, BLACK };
    Vector2 dim = { 70, 100 };

    for ( int i = 0; i < 52; i++ ) {
        gw->deck.cards[i] = (Card) {
            .suit = suits[i/13],
            .suitColor = suitsColor[i/13],
            .value = i % 13 + 1,
            .rect = {
                0, 
                0, 
                dim.x,
                dim.y
            },
            .color = colors[i/13],
            .flipped = false,
            .belongsTo = NULL
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
    gw->stacks[13] = &gw->transfer;

    for ( int i = 0; i < 14; i++ ) {
        initCardStack( gw->stacks[i] );
    }

    gw->available.type = CARD_STACK_TYPE_AVAILABLE;
    gw->checking.type = CARD_STACK_TYPE_CHECKING;
    gw->transfer.type = CARD_STACK_TYPE_TRANSFER;

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

    reorganizeAllStacks( gw );

}

static Card *resolveSelectedCard( GameWorld *gw ) {

    for ( int i = 1; i < 13; i++ ) {
        CardStack *s = gw->stacks[i];
        for ( int i = s->top; i >= 0; i-- ) {
            Card *c = s->cards[i];
            if ( !c->flipped && CheckCollisionPointRec( GetMousePosition(), c->rect ) ) {
                pressOffset.x = GetMouseX() - c->rect.x;
                pressOffset.y = GetMouseY() - c->rect.y;
                return c;
            }
        }
    }

    return NULL;

}