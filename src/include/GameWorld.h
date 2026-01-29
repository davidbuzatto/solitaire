/**
 * @file GameWorld.h
 * @author Prof. Dr. David Buzatto
 * @brief GameWorld struct and function declarations.
 * 
 * @copyright Copyright (c) 2026
 */
#pragma once

#include "Types.h"
#include "Deck.h"

typedef struct GameWorld {

    Deck deck;

    CardStack available;
    CardStack checking;

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

    CardStack *stacks[13];

} GameWorld;

/**
 * @brief Creates a dinamically allocated GameWorld struct instance.
 */
GameWorld* createGameWorld( void );

/**
 * @brief Destroys a GameWindow object and its dependecies.
 */
void destroyGameWorld( GameWorld *gw );

/**
 * @brief Reads user input and updates the state of the game.
 */
void updateGameWorld( GameWorld *gw, float delta );

/**
 * @brief Draws the state of the game.
 */
void drawGameWorld( GameWorld *gw );