#pragma once

#include <stdbool.h>
#include "raylib/raylib.h"

typedef struct Card Card;
typedef struct CardStack CardStack;

typedef enum Suit {
    SUIT_H, // hearts
    SUIT_D, // diamonds
    SUIT_C, // club
    SUIT_S  // spades
} Suit;

typedef enum SuitColor {
    SUIT_COLOR_RED,
    SUIT_COLOR_BLACK
} SuitColor;

typedef enum CardStackType {
    CARD_STACK_TYPE_AVAILABLE,
    CARD_STACK_TYPE_CHECKING,
    CARD_STACK_TYPE_TRANSFER,
    CARD_STACK_TYPE_OK_H,
    CARD_STACK_TYPE_OK_D,
    CARD_STACK_TYPE_OK_C,
    CARD_STACK_TYPE_OK_S,
    CARD_STACK_TYPE_TEMP
} CardStackType;

struct Card {
    int value;
    Suit suit;
    SuitColor suitColor;
    Rectangle rect;
    Color color;
    bool flipped;
    CardStack *belongsTo;
};

struct CardStack {
    Card *cards[52];
    int top;
    Rectangle rect;
    Rectangle dropRect;
    CardStackType type;
};

typedef struct Deck {
    Card cards[52];
} Deck;

typedef struct GameWorld {

    Deck deck;

    CardStack available;
    CardStack checking;
    CardStack transfer;

    CardStack okHearts;
    CardStack okDiamonds;
    CardStack okClub;
    CardStack okSpades;

    CardStack temp1;
    CardStack temp2;
    CardStack temp3;
    CardStack temp4;
    CardStack temp5;
    CardStack temp6;
    CardStack temp7;

    CardStack *stacks[14];

} GameWorld;