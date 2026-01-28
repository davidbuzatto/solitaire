#pragma once

#include <stdbool.h>
#include "raylib/raylib.h"

typedef enum Suit {
    SUIT_H, // hearts
    SUIT_D, // diamonds
    SUIT_C, // club
    SUIT_S  // spades
} Suit;

typedef enum CardStackType {
    CARD_STACK_TYPE_AVAILABLE,
    CARD_STACK_TYPE_CHECKING,
    CARD_STACK_TYPE_OK_H,
    CARD_STACK_TYPE_OK_D,
    CARD_STACK_TYPE_OK_C,
    CARD_STACK_TYPE_OK_S,
    CARD_STACK_TYPE_TEMP
} CardStackType;

typedef struct Card {
    int value;
    Suit suit;
    Rectangle rect;
    Color color;
    bool flipped;
} Card;

typedef struct Deck {
    Card cards[52];
} Deck;

typedef struct CardStack {
    Card *cards[52];
    int top;
    Rectangle rect;
    CardStackType type;
} CardStack;