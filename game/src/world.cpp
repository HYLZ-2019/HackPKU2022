#include "world.h"
#include <cstdio>

/* 后端的所有信息。 */
World :: World() {
    points = 0;
    currentStage = 0;
    Earth_sita = 0;
    texture[0] = LoadTexture("resources/space.png");
    Image moon = LoadImage("resources/moon.png");
    ImageResize(&moon,EARTH_RADIUS,EARTH_RADIUS);
    texture[1] = LoadTextureFromImage(moon);
    printf("%d,%d\n",texture[1].width,texture[1].height);
    texture[2] = LoadTexture("resources/tiger.png");

    Image purple_note = LoadImage("resources/purple_note.png");
    ImageResize(&purple_note,NOTE_WIDTH,NOTE_HEIGHT);
    texture[3] = LoadTextureFromImage(purple_note);

    // texture[3] = LoadTexture("resources/");
    // Source rectangle (part of the texture to use for drawing)
    sourceRec = { 0.0f, 0.0f, (float)texture[1].width, (float)texture[1].height };
    // Destination rectangle (screen rectangle where drawing part of texture)
    destRec = { EARTH_POSX, EARTH_POSY, texture[1].width*2.0f, texture[1].height*2.0f };
    // Origin of the texture (rotation/scale point), it's relative to destination rectangle size
    origin = { (float)texture[1].width, (float)texture[1].height };
}

int World::pointsNeededForNextStage(int stage) {
    assert(stage < MAX_STAGE);
    return (stage+1) * 100;
}

/* 调用tiger, rope, notes的update*/
void World::updateWorld(){
    double deltaH = 0;
    //
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) 
        deltaH = MOVE_STEP_LENGTH - DOWN_STEP_LENGTH;
    else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
        deltaH = - MOVE_STEP_LENGTH - DOWN_STEP_LENGTH;
    else deltaH = -DOWN_STEP_LENGTH; 
    tiger.updateTiger(deltaH); 
    rope.updateRope();
    notes.updateNotes();      
    if (points > pointsNeededForNextStage(currentStage)){
        currentStage += 1;
    }
    Earth_sita++;
    return;
}