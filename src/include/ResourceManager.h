/**
 * @file ResourceManager.h
 * @author Prof. Dr. David Buzatto
 * @brief ResourceManager struct and function declarations.
 * 
 * @copyright Copyright (c) 2025
 */
#pragma once

#include "raylib/raylib.h"

typedef struct ResourceManager {
    Texture2D heartsSmall;
    Texture2D diamondsSmall;
    Texture2D clubSmall;
    Texture2D spadesSmall;
    Texture2D heartsBig;
    Texture2D diamondsBig;
    Texture2D clubBig;
    Texture2D spadesBig;
    Texture2D heartsGray;
    Texture2D diamondsGray;
    Texture2D clubGray;
    Texture2D spadesGray;
} ResourceManager;

/**
 * @brief Global ResourceManager instance.
 */
extern ResourceManager rm;

/**
 * @brief Load global game resources, linking them in the global instance of
 * ResourceManager called rm.
 */
void loadResourcesResourceManager( void );

/**
 * @brief Unload global game resources.
 */
void unloadResourcesResourceManager( void );