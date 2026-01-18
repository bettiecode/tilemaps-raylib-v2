#include "typedefs.h"
#include <raylib.h> //needed critically
#include "constants.h" //needed for all its contents
#include "controls.h" //import
#include "maps.h" //import
#include <iostream> //needed for debugging
#include <ctime> //needed for seeding rand

Tileset* pTs; //global tileset pointer
Tilemap* map; //global tilemap pointer
Cam cam; //global camera
Mouse mouse; //global mouse
Keybinds keybinds; //global keybinds
Keystates keystates; //global keystates
Settings settings; //global settings

class Game //class that contains the methods that are called each frame
{
    public:
    const char* title; //window title
    int fps; //frames per second

    void init() //initialize
    {
        InitWindow(WIN_WIDTH,WIN_HEIGHT,title); //make window (and allow texture loading)
        SetTargetFPS(fps); //set fps
    }
    
    void uninit() //end program
    {
        unloadTileset(pTs); //unload tileset
        unloadTilemap(map); //unload tilemap
        CloseWindow(); //close window
    }

    void input() //get input
    {
        getInput(mouse,keystates,keybinds); //get input
    }

    void update() //update
    {
        controlCam(cam,mouse,keystates,keybinds,settings,map->ts->tileSize,map->w,map->h); //update camera
        //since this build doesn't contain the functionality to edit the tilemap, it is only needed to be updated after randomization
    }

    void render() //render, draw
    {
        BeginDrawing(); //begin
            ClearBackground(BG_COLOR); //clear
            renderTilemap(map,cam); //render tilemap
            /*
            DrawCircle(WIN_WIDTH/2-cam.pos.x,WIN_HEIGHT/2-cam.pos.y,64*cam.zoom,WHITE);
            */
            DrawText(std::to_string((int)cam.pos.x).c_str(),4,4,16,WHITE); //see cam x for debug
            DrawText(std::to_string((int)cam.pos.y).c_str(),4+4*16+4,4,16,WHITE); //see cam y for debug
            DrawText(std::to_string(cam.zoom).c_str(),4,20,16,WHITE); //see zoom for debug
            
        EndDrawing(); //end
    }

    Game(const char* _title, int _fps) //constructor
    {
        title = _title;
        fps = _fps;
    }

}; Game game((const char*)"Tilemaps Raylib C++ V2",60); //global game object

int main()
{   
    srand(time(NULL)); //seed rand
    
    game.init(); //initalize game

    pTs = loadTileset("snow"); //load tileset
    map = makeTilemap(64,48,2,0,pTs); //make randomized tilemap
    printTilemap(map,0); //print layer 0 for debug
    printf("----\n"); //print separator
    updateTilemap(map); //autotile
    printTilemap(map,0); //print layer 0 after updating for debug
    
    while(!WindowShouldClose()) //game loop
    {
        game.input(); //get input
        game.update(); //update
        game.render(); //render, draw
    }    
    
    game.uninit(); //end program

    return 0; //exit with no errors
}
