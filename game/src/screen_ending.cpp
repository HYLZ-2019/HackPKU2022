/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Ending Screen Functions Definitions (Init, Update, Draw, Unload)
*
*   Copyright (c) 2014-2022 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#include "raylib.h"
#include "screens.h"

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;
static GameResults result;

Texture gameoverPic;
Texture youwinPic;
Texture fireworks;

//----------------------------------------------------------------------------------
// Ending Screen Functions Definition
//----------------------------------------------------------------------------------

// Ending Screen Initialization logic
void InitEndingScreen(GameResults res)
{
    // TODO: Initialize ENDING screen variables here!
    result = res;
    framesCounter = 0;
    finishScreen = 0;
    gameoverPic = LoadTexture("resources/gameover.png");
    youwinPic = LoadTexture("resources/youwin.png");
    fireworks = LoadTexture("resources/Firework.png");
}

// Ending Screen Update logic
void UpdateEndingScreen(void)
{
    // TODO: Update ENDING screen variables here!
    framesCounter += 1;
    // Press enter or tap to return to TITLE screen
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        finishScreen = 1;
        PlaySound(fxCoin);
    }
}

// Ending Screen Draw logic
void DrawEndingScreen(void)
{
    // TODO: Draw ENDING screen here!
    bool win = result.currentStage == MAX_STAGE;
    if (win) {
        Rectangle source_rec = {0.0f, 0.0f, (float)youwinPic.width, (float)youwinPic.height};
        Rectangle dest_rec = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        DrawTexturePro(youwinPic, source_rec, dest_rec, {0.0f, 0.0f}, 0.0f, WHITE);
        DrawText(TextFormat("%d", result.points), 950, 220, 60, ORANGE);
        DrawText(TextFormat("%02d:%02d", ((int)result.usedTime)/60, (int)(result.usedTime)%60), 950, 315, 60, ORANGE);
    
        // Firework 1: [0, 60], Firework 2: [30, 90]
        int fwtime = framesCounter % 90;
        Rectangle fwd_1 = {30.0, 20.0, 300.0, 300.0};
        Rectangle fwd_2 = {400.0, 190.0, 200.0, 200.0};
        Rectangle fwd_3 = {800.0, 30.0, 150.0, 150.0};
        Rectangle fws_1 = {float(256.0*((fwtime/2)%6)), float(256.0*((fwtime/2)/6)), 256.0, 256.0};
        Rectangle fws_2 = {float(256.0*(((fwtime-15)/2)%6)), float(256.0*(((fwtime-15)/2)/6)), 256.0, 256.0};
        Rectangle fws_3 = {float(256.0*(((fwtime-30)/2)%6)), float(256.0*(((fwtime-30)/2)/6)), 256.0, 256.0};
        if (0 <= fwtime && fwtime < 60){
            DrawTexturePro(fireworks, fws_1, fwd_1, {0.0f, 0.0f}, 0.0f, WHITE);
        }
        if (15 <= fwtime && fwtime < 75){
            DrawTexturePro(fireworks, fws_2, fwd_2, {0.0f, 0.0f}, 0.0f, WHITE);
        }
        if (30 <= fwtime && fwtime < 90){
            DrawTexturePro(fireworks, fws_3, fwd_3, {0.0f, 0.0f}, 0.0f, WHITE);
        }
    
    }
    else {
        Rectangle source_rec = {0.0f, 0.0f, (float)gameoverPic.width, (float)gameoverPic.height};
        Rectangle dest_rec = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        DrawTexturePro(gameoverPic, source_rec, dest_rec, {0.0f, 0.0f}, 0.0f, WHITE);
        DrawText(TextFormat("%d", result.points), 950, 150, 60, BLACK);
        DrawText(TextFormat("%d", result.maxpoints), 950, 245, 60, BLACK);
        DrawText(TextFormat("%02d:%02d", ((int)result.usedTime)/60, (int)(result.usedTime)%60), 950, 335, 60, BLACK);

    }
}

// Ending Screen Unload logic
void UnloadEndingScreen(void)
{
    // TODO: Unload ENDING screen variables here!
}

// Ending Screen should finish?
int FinishEndingScreen(void)
{
    return finishScreen;
}