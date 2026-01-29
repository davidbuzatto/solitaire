/**
 * @file ResourceManager.c
 * @author Prof. Dr. David Buzatto
 * @brief ResourceManager implementation.
 * 
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>

#include "ResourceManager.h"
#include "raylib/raylib.h"

ResourceManager rm = { 0 };

void loadResourcesResourceManager( void ) {
    rm.heartsSmall = LoadTexture( "resources/images/heartsSmall.png" );
    rm.diamondsSmall = LoadTexture( "resources/images/diamondsSmall.png" );
    rm.clubSmall = LoadTexture( "resources/images/clubSmall.png" );
    rm.spadesSmall = LoadTexture( "resources/images/spadesSmall.png" );
    rm.heartsBig = LoadTexture( "resources/images/heartsBig.png" );
    rm.diamondsBig = LoadTexture( "resources/images/diamondsBig.png" );
    rm.clubBig = LoadTexture( "resources/images/clubBig.png" );
    rm.spadesBig = LoadTexture( "resources/images/spadesBig.png" );
    rm.heartsGray = LoadTexture( "resources/images/heartsGray.png" );
    rm.diamondsGray = LoadTexture( "resources/images/diamondsGray.png" );
    rm.clubGray = LoadTexture( "resources/images/clubGray.png" );
    rm.spadesGray = LoadTexture( "resources/images/spadesGray.png" );
}

void unloadResourcesResourceManager( void ) {
    UnloadTexture( rm.heartsSmall );
    UnloadTexture( rm.diamondsSmall );
    UnloadTexture( rm.clubSmall );
    UnloadTexture( rm.spadesSmall );
    UnloadTexture( rm.heartsBig );
    UnloadTexture( rm.diamondsBig );
    UnloadTexture( rm.clubBig );
    UnloadTexture( rm.spadesBig );
    UnloadTexture( rm.heartsGray );
    UnloadTexture( rm.diamondsGray );
    UnloadTexture( rm.clubGray );
    UnloadTexture( rm.spadesGray );
}