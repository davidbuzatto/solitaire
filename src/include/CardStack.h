#pragma once

#include <stdbool.h>

#include "CardStack.h"
#include "Card.h"

void initCardStack( CardStack *s );
void pushCardStack( CardStack *s, Card *c );
Card *popCardStack( CardStack *s );
Card *peekCardStack( CardStack *s );

void drawCardStack( CardStack *s, Card *excludeFromDrawing );
void updateCardsPositionFromCardStack( CardStack *s, float verticalSpacing );
void updateCardsPositionFromCardStackAvailable( CardStack *s, float diagonalSpacing );
void updateCardsPositionFromCardStackChecking( CardStack *s, float diagonalSpacing );

void reorganizeCardsPositionFromTempCardStack( CardStack *s, float verticalSpacing );
void reorganizeCardsPositionFromStackedCardStack( CardStack *s, float diagonalSpacing );

void logCardStack( CardStack *s, const char *stackName );
