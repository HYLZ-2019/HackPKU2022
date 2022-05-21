#include "world.h"
#include <cstdio>

/* 后端的所有信息。 */
World :: World() {
    points = 0;
    maxpoints = 0;
    currentStage = 0;
    Earth_sita = 0;
    beginTime = time(0);
    // 是弧度制！
    NorthPolarAngel = 0;
    texture[World::SPACE] = LoadTexture("resources/space.png");
    //texture[0] = LoadTexture("resources/space.png");
    Image moon = LoadImage("resources/Moon_new.png");
    ImageResize(&moon,EARTH_RADIUS*2/3,EARTH_RADIUS*2/3);
    //texture[1] = LoadTextureFromImage(moon);
    texture[World::MOON] = LoadTextureFromImage(moon);
    // printf("%d,%d\n",texture[1].width,texture[1].height);
    //texture[2] = LoadTexture("resources/tiger.png");
    texture[World::EXPLOSION] = LoadTexture("resources/explosion.png");
    texture[World::TIGER] = LoadTexture("resources/tiger.png");

    Image purple_note = LoadImage("resources/purple_note.png");
    ImageResize(&purple_note,NOTE_WIDTH,NOTE_HEIGHT);
    //texture[3] = LoadTextureFromImage(purple_note);
    texture[World::NOTE_PURPLE] = LoadTextureFromImage(purple_note);

    Image orange_note = LoadImage("resources/orange_note.png");
    ImageResize(&orange_note,NOTE_WIDTH,NOTE_HEIGHT);
    texture[World::NOTE_ORANGE] = LoadTextureFromImage(orange_note);
    //texture[4] = LoadTextureFromImage(orange_note);
    
    Image blue_note = LoadImage("resources/blue_note.png");
    ImageResize(&blue_note,NOTE_WIDTH,NOTE_HEIGHT);
    //texture[5] = LoadTextureFromImage(blue_note);
    texture[World::NOTE_BLUE] = LoadTextureFromImage(blue_note);

    Image pink_note = LoadImage("resources/pink_note.png");
    ImageResize(&pink_note,NOTE_WIDTH,NOTE_HEIGHT);
    texture[World::NOTE_PINK] = LoadTextureFromImage(pink_note);

    Image red_note = LoadImage("resources/red_note.png");
    ImageResize(&red_note,NOTE_WIDTH,NOTE_HEIGHT);
    texture[World::NOTE_RED] = LoadTextureFromImage(red_note);

    Image wolf_note = LoadImage("resources/wolf_note.png");
    ImageResize(&wolf_note,NOTE_WIDTH,NOTE_HEIGHT);
    //texture[6] = LoadTextureFromImage(wolf_note);
    texture[World::NOTE_WOLF] = LoadTextureFromImage(wolf_note);

    Image wolf = LoadImage("resources/wolf.png");
    ImageResize(&wolf,NOTE_WIDTH,NOTE_HEIGHT);
    //texture[6] = LoadTextureFromImage(wolf_note);
    texture[World::WOLF] = LoadTextureFromImage(wolf);
    // Image staff = LoadImage("resources/staff.png");
    // ImageResize(&staff,NOTE_WIDTH,NOTE_HEIGHT);
    // // texture[7] = LoadTextureFromImage(staff);
    // texture[World::NOTE_WOLF] = LoadTextureFromImage(staff);

    // texture[3] = LoadTexture("resources/");
    // Source rectangle (part of the texture to use for drawing)
    sourceRec = { 0.0f, 0.0f, (float)texture[World::MOON].width, (float)texture[World::MOON].height };
    // Destination rectangle (screen rectangle where drawing part of texture)
    destRec = { EARTH_POSX, EARTH_POSY, texture[World::MOON].width*2.0f, texture[World::MOON].height*2.0f };
    // Origin of the texture (rotation/scale point), it's relative to destination rectangle size
    origin = { (float)texture[World::MOON].width, (float)texture[World::MOON].height };
}

int World::pointsNeededForNextStage(int stage) {
    assert(stage < MAX_STAGE);
    return (stage + 1) * 100;
}

/* 调用tiger, rope, notes的update*/
void World::updateWorld() {
    double deltaH = 0;
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
        if (currentStage == 1 || currentStage == 5 || currentStage == 8)  {
            wolf.init_wolf(120);
        }
    }
    wolf.update_wolf();
    if (points > maxpoints){
        maxpoints = points;
    }
    Earth_sita-=0.1;
    NorthPolarAngel += 2 * PI / BLOCK_NUMBER;
    return;
}