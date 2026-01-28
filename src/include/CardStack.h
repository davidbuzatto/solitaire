#pragma once

#include "CardStack.h"
#include "Card.h"

void initCardStack( CardStack *s );
void pushCardStack( CardStack *s, Card *c );
Card *popCardStack( CardStack *s );
Card *peekCardStack( CardStack *s );

void drawCardStack( CardStack *s );
void updateCardsPositionFromCardStack( CardStack *s, float verticalSpacing );
void updateCardsPositionFromCardStackAvailable( CardStack *s, float diagonalSpacing );
void updateCardsPositionFromCardStackChecking( CardStack *s, float diagonalSpacing );
