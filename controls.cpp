#include "controls.h"
#include <raymath.h> //needed for Vector2Subtract

void getInput(Mouse& mouse, Keystates& ks, Keybinds& kb) //get keystates, and update mouse attributes
{
    //mouse
    mouse.pos = GetMousePosition(); //get mouse position
    mouse.mb1 = IsMouseButtonDown(0); //check left click
    mouse.mb2 = IsMouseButtonDown(1); //check right click
    mouse.wheel = GetMouseWheelMove(); //get scrollwheel movement
    
    //keyboard
    ks.camMoveModeX=IsKeyDown(kb.camMoveModeX); //cam move mode x
    ks.camMoveModeY=IsKeyDown(kb.camMoveModeY); //cam move mode y
}
void controlCam(Cam& cam,Mouse& mouse,  Keystates& ks, Keybinds& kb, Settings& s, u8 tileSize, u8 w, u8 h) //zoom and pos update
{
    Vector2 mp1 = getMouseRelPos(mouse,cam,tileSize,w,h); //save mouse's position on the tilemap (not the screen!) before zoom
    if(ks.camMoveModeX+ks.camMoveModeY == 0) //if true, then zoom
    {
        cam.zoom += mouse.wheel*s.zoomVel; //amplify zoom value
        if(cam.zoom<=0) cam.zoom=s.zoomVel; //prevent zoom value from reaching 0, capped at zoomVel
    }
    Vector2 mp2 = getMouseRelPos(mouse,cam,tileSize,w,h); //get mouse's position on the tilemap (not the screen!) after zoom
    cam.pos+=Vector2Subtract(mp1,mp2); //move the camera so the tile under the mouse remains the same, ->
                                       // -> this is needed to make the zoom "go" toward the mouse instead of the corner of the screen
    cam.pos.x-=ks.camMoveModeX*mouse.wheel*s.scrollVel; //move across x axis, if bound key down
    cam.pos.y-=ks.camMoveModeY*mouse.wheel*s.scrollVel; //move across y axis, if bound key down
}

Vector2 getMouseRelPos(Mouse& mouse, Cam& cam, u8 tileSize, u8 w, u8 h) //mouse's screen positon -> tilemap (relative) position
{
    Vector2 start = 
    {
        (float)(0-cam.pos.x)*(tileSize*cam.zoom), //same formula that is used in the rendering function, with x=0
        (float)(0-cam.pos.y)*(tileSize*cam.zoom)  //same formula that is used in the rendering function, with y=0
    };
    Vector2 end =
    {
        (float)(w-cam.pos.x)*(tileSize*cam.zoom), //same formula that is used in the rendering function, with x=w
        (float)(h-cam.pos.y)*(tileSize*cam.zoom)  //same formula that is used in the rendering function, with y=h
    };
    Vector2 relPos =
    {
        ((mouse.pos.x-start.x)/(end.x-start.x))*w, //w*(offset from startX)/(distance from startX to endX)
        ((mouse.pos.y-start.y)/(end.y-start.y))*h  //h*(offset from startY)/(distance from startY to endY)
    };
    return relPos; 
}
