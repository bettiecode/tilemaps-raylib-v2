#ifndef CONTROLS_H
#define CONTROLS_H

#include "typedefs.h" //typedefs
#include <raylib.h> //for Vector2 and input

typedef struct { //camera struct
    Vector2 pos = {0,0}; //in tiles, (pos.x==1: camera is one tile to the right)
    float zoom = 1; //render size of tiles scales linearly with cam.zoom 
} Cam;

typedef struct { //mouse struct, contains data about mouse input
    Vector2 pos; //in pixels, on the window
    bool mb1; //left click down
    bool mb2; //right click down
    float wheel; //scrollwheel
} Mouse;

typedef struct { //keybinds struct, contains which keys' states the keystates struct shouls track
    int camMoveModeX = KEY_LEFT_CONTROL;  // these bools determine whether the scrollwheel's ->
    int camMoveModeY = KEY_LEFT_SHIFT;    // -> movement should move the camera in the x and y axis
} Keybinds; //inspired by photopea's camera movement with mouse wheel

typedef struct { //keystates struct, contains state(down or up) of bound keys
    bool camMoveModeX; //should camera be moved across the y axis
    bool camMoveModeY; //should camera be moved across the y axis
} Keystates;

typedef struct { //settings struct, contains constants
    float scrollVel = 1.0f; //camera movement speed multiplier
    float zoomVel = 0.25f; //zoom speed multiplier
} Settings;

//see explanations of these functions in implementation "controls.cpp"
void getInput(Mouse& mouse, Keystates& ks, Keybinds& kb);
void controlCam(Cam& cam, Mouse& mouse,Keystates& ks, Keybinds& kb, Settings& s, u8 tileSize, u8 w, u8 h); 
Vector2 getMouseRelPos(Mouse& mouse, Cam& cam, u8 tileSize, u8 w, u8 h);

#endif