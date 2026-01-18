#include "maps.h" 
#include "controls.h" //need for Cam, which is needed for rendering
#include <stdlib.h> //needed for various things
#include <iostream> //needed for printTilemap
#include <fstream> //needed for loading tileset
#include <string> //needed for filenames
#include <cmath> //needed for pow

Tilemap* makeTilemap(uint w, uint h, uint l, u8 f, Tileset* ts) //return a randomized tilemap
{
    static Tilemap r; //result
    r.w=w; r.h=h;  r.l=l; //dimensions
    r.ts=ts; //assign argumented tileset pointer
    r.textureIdxMap = (u8***)malloc(sizeof(u8**[l])); //allocate *** array, texture indices
    r.variationIdxMap = (u8***)malloc(sizeof(u8**[l])); //allocate *** array, variation indices
    for(uint z=0; z<l; z++) //loop trough layers
    {
        r.textureIdxMap[z] = (u8**)malloc(sizeof(u8*[h])); //allocate ** array, texture indices
        r.variationIdxMap[z] = (u8**)malloc(sizeof(u8*[h])); //allocate ** array, variation indices
        for(uint y=0; y<h; y++) //loop trough rows
        {
            r.textureIdxMap[z][y] = (u8*)malloc(sizeof(u8[w])); //allocate * array, texture indices
            r.variationIdxMap[z][y] = (u8*)malloc(sizeof(u8[w])); //allocate * array, variation indices
            for(uint x=0; x<w;  x++) //loop trough coloumns
            {
                r.textureIdxMap[z][y][x] = rand() % 3; //fill randomly, 3 because the example tileset contains two tiles (third is air)  
                r.variationIdxMap[z][y][x] = 0; //fill with 0
            }
        }
    }
    return &r;

}
void unloadTilemap(Tilemap* map) //free allocated memory
{
    for(uint z=0; z<map->l; z++) //loop trough layers
    {
        for(uint y=0; y<map->h; y++) //loop trough rows
        {
            free(map->textureIdxMap[z][y]);   //free row
            free(map->variationIdxMap[z][y]); //free row
        }
        free(map->textureIdxMap[z]); //free layer
        free(map->variationIdxMap[z]); //free layer
    }   
    free(map->textureIdxMap); //free array
    free(map->variationIdxMap); //free array
}
void printTilemap(Tilemap* map, uint z)
{
    for(uint y=0; y<map->h; y++) //loop trough rows
    {
        for(uint x=0; x<map->w; x++) //loop trough coloumns
        {
            printf("%u:%u\t",map->textureIdxMap[z][y][x],map->variationIdxMap[z][y][x]); //print value at x y
        }
        printf("\n"); //new line
    }
}
void updateTilemap(Tilemap* map) //auto tiling
{
    u8 nb = 0; //neighbors
    u8 autoTileType; //determined by number of variants
    i8 xOffs[] = {-1,0,1,0}; // since only the 16 variant autotiling is supported in this project ->
    i8 yOffs[] = {0,1,0,-1}; // -> 4 different checks are enough, (8 neighbors - the diagonal ones)
    for(uint z=0; z<map->l; z++) //loop trough layers
    {
        for(uint y=0; y<map->h; y++) //loop trough rows
        {
            for(uint x=0; x<map->w; x++) //loop trough coloumns
            {
                //std::cout << "x: " << x << "\ty: " << y << std::endl;
                u8 tIdx = map->textureIdxMap[z][y][x]; //texture idx
                if(tIdx == map->ts->nTextures) continue; //air tile
                autoTileType = map->ts->nVariations[tIdx]; //same as number of variants
                if(autoTileType == 1) continue; //1 variant => no auto tiling
                nb=0; //initalize sum
                for(u8 i=0; i<4; i++)
                {
                    int checkX = x+xOffs[i]; //neighbor to check's x
                    int checkY = y+yOffs[i]; //neighbor to check's y
                    //std::cout << "x+: " << checkX << "\ty+: " << y+yOffs[i] << std::endl;
                
                    if
                    (
                        (checkX < 0) || (checkX == map->w) || //if neighbor is outside of tilemap, then the tile is counted as bordered in that direction
                        (checkY < 0) || (checkY == map->h) 
                    ) nb+=(u8)1 << i; //bordered in direction
                    else if(
                        (tIdx != map->textureIdxMap[z][checkY][checkX]) //if neighbor contains a different texture index, then the tile is counted as bordered in that direction
                    ) nb+=(u8)1 << i; //bordered in direction
                }
                map->variationIdxMap[z][y][x]=nbToInt(nb,autoTileType); //turn border data into index of variation
                
            }
        }
    }
}
u8 nbToInt(u8 nb, u8 type) //turns border data into index of variation
{
    switch (type) //only 16 variant autotiling is supported
    {
        case 16:    return nb; //border data byte is same as the index byte, since the way it is checked was made with 16 variant autotiling in mind
        case 1:     return 0;
    }
    return 0;
};
void renderTilemap(Tilemap* map, Cam& cam) //render
{
    for(uint z=0; z<map->l; z++) //loop trough layers
    {
        for(uint y=0; y<map->h; y++) //loop trough rows
        {
            for(uint x=0; x<map->w; x++) //loop trough coloumns
            {
                //printf("%u:%u\t",map->textureIdxMap[y][x],map->variationIdxMap[y][x]);
                
                u8 tIdx = map->textureIdxMap[z][y][x]; //texture index
                u8 vIdx = map->variationIdxMap[z][y][x]; //variant index

                if(tIdx==map->ts->nTextures) continue; //skip air tile
                DrawTextureEx 
                (
                    map->ts->textures[tIdx][vIdx], //get sprite
                    {
                        (float)(x-cam.pos.x)*(map->ts->tileSize*cam.zoom), //drawX = (tileX-camX)*tileSize*zoom
                        (float)(y-cam.pos.y)*(map->ts->tileSize*cam.zoom)  //drawY = (tileY-camY)*tileSize*zoom
                    }, 
                    0, //no rotation
                    cam.zoom, //scale with zoom
                    z==0 ? DARKGRAY : WHITE //dark if lower layer, to visualize layers simply
                );
                
            }
            //printf("\n");
        }
    }
}


u8 stou8(const std::string& s) //could find a built-in function to convert strings to uint8_t so i made it myself
{
    u8 r=0; //result
    u8 i=0; //track iteration
    u8 l = s.length()-1; //lenght of string
    for(char c : s) //loop trough characters
    {
        r+=(c - '0')*pow(10,l-i); //number*10^(digit index) (bits are from right to left, while arabic numbers are from left to right)
        i++; //increment tracker
    }
    return r;
}

Tileset* loadTileset(const std::string& folder) //load tileset contained in a folder
{
    static Tileset r; //result
    std::fstream dataFile("tilesets/"+folder+"/_dat.dat"); //load file containing data about the tile set
    std::string line; //stores read lines
    getline(dataFile,line); r.nTextures = stou8(line); //line 1: number of different tiles
    getline(dataFile,line); r.tileSize = stou8(line); //line 2: tile size
    //std::cout << r.nTextures << "-" << r.tileSize << std::endl;
    
    r.nVariations = (u8*)malloc(sizeof(u8[r.nTextures])); //allocate memory for * array
    r.textures = (Texture**)malloc(sizeof(Texture*[r.nTextures])); //allocate memory for ** array

    u8 i=0; //track iteration
    while(getline(dataFile,line)) //name of the tile (that appears in filenames)
    {                                                   
        std::string tileName = line; //store read tilename
        getline(dataFile,line);  //number of variants
        r.nVariations[i] = stou8(line); //store read number
        //std::cout<< r.nVariations[i] << std::endl;
        r.textures[i] = (Texture*)malloc(sizeof(Texture[r.nVariations[i]])); //allocate memory for * array
        for(u8 j=0; j<r.nVariations[i]; j++) //get each variant
        {
            std::string fileName = ("tilesets/"+folder+"/"+tileName+"-"+std::to_string(j)+".png"); //assemble filename
            r.textures[i][j] = LoadTexture(fileName.c_str()); //load texture
        }
        i++; //increment
        //std::cout << "loop\n";
    }
    return &r;
}
void unloadTileset(Tileset* tileset) //free textures and data related to the tileset
{
    
    for(u8 i=0; i<tileset->nTextures; i++) //loop trough tiles
    {
        for(u8 j=0; j<tileset->nVariations[i]; j++) //loop through variants
        {
            UnloadTexture(tileset->textures[i][j]); //unload texture
        }
        free(tileset->textures[i]); //free * array
    }
    free(tileset->nVariations); //free * array
    free(tileset->textures); //free ** array
}