#ifndef MAPS_H
#define MAPS_H

#include "typedefs.h" 
#include <raylib.h> //needed for texture loading
#include "controls.h" //needed for rendering
#include <string> //needed for filenames

struct Tileset //array of arrays of textures + tile texture size
{
    Texture2D** textures; //2d array containing textures
    u8 nTextures; //length of "textures" array, tile types
                  //max: 255 different tiles, one has to be reserved for air(empty) tiles,
    u8* nVariations; //lengths of arrays (contained by the "textures" array), different alignments of a tile, also determines autotiling type
    u8 tileSize; //size of tile textures in pixels
};

struct Tilemap //tileset pointer, and two 3d arrays containing indices for it 
{
    u8*** textureIdxMap; //tile type, texture type
    u8*** variationIdxMap; //variant, alignment of tile 
    uint w,h,l; //width, height, layers
    Tileset* ts; //pointer to tileset
};

//see explanations of these functions in implementation "maps.cpp"
Tilemap* makeTilemap(uint w, uint h, uint l, u8 f, Tileset* ts);
void unloadTilemap(Tilemap* map);
void printTilemap(Tilemap* map, uint z);
void updateTilemap(Tilemap* map);
u8 nbToInt(u8 nb, u8 type);
void renderTilemap(Tilemap* map, Cam& cam);

u8 stou8(const std::string& s);
Tileset* loadTileset(const std::string& folder);
void unloadTileset(Tileset* tileset);

#endif