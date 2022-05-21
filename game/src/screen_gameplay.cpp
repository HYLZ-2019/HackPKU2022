/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Gameplay Screen Functions Definitions (Init, Update, Draw, Unload)
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
#include "cstdio"

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

Vector2 TransitionCoordinate(double sita, double rho){
    return (Vector2){(float)(rho*cos(sita)), (float)(rho*sin(sita))};
}

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    // TODO: Initialize GAMEPLAY screen variables here!
    world = new World();
    framesCounter = 0;
    finishScreen = 0;
    world -> tiger.initTiger(0);
    world -> rope.initRope();
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    // TODO: Update GAMEPLAY screen variables here!
    if (world->points < 0){
        // Game over
        finishScreen = 1;
    }
    else if (world->currentStage == MAX_STAGE){
        // Passed
        finishScreen = 1;
    }
    else{
        world->updateWorld();
    }
    return;
}

void DrawRope(const World* world) {
    std::vector <std::pair<std::pair <double, double>, ROPEDOT_STATE > > PolarAngels;
    std::vector <std::pair <int,int> > seg;
    world -> rope.getRopeData(seg, PolarAngels);
    for (int i = 0, sze = seg.size(); i < sze; ++i) {
        int l = seg[i].first, r = seg[i].second;
        int numPoints = r - l;
        if (numPoints <= 0) numPoints += BLOCK_NUMBER;
        Vector2 *pointsP = (Vector2 *)malloc(numPoints * sizeof(Vector2));
        Vector2 *pointsP1 = (Vector2 *)malloc(numPoints * sizeof(Vector2));
        Vector2 *pointsP2 = (Vector2 *)malloc(numPoints * sizeof(Vector2));
        Vector2 *pointsP3 = (Vector2 *)malloc(numPoints * sizeof(Vector2));
        Vector2 *pointsP4 = (Vector2 *)malloc(numPoints * sizeof(Vector2));
        for (int j = l, k = 0; j != r; j = (j + 1) % BLOCK_NUMBER, ++k) {
            //pointsP[k] = TransitionCoordinate(PolarAngels[j].first, 
            //                                  PolarAngels[j].second);
            pointsP[k] = TransitionCoordinate(PolarAngels[j].first.first - world -> NorthPolarAngel, 
                                              PolarAngels[j].first.second + (float)EARTH_RADIUS*2/3);
            pointsP[k].x += EARTH_POSX, pointsP[k].y += EARTH_POSY;
            
            pointsP1[k] = TransitionCoordinate(PolarAngels[j].first.first - world -> NorthPolarAngel, 
                                              PolarAngels[j].first.second + (float)EARTH_RADIUS*2/3+20);
            pointsP1[k].x += EARTH_POSX, pointsP1[k].y += EARTH_POSY;
            
            pointsP2[k] = TransitionCoordinate(PolarAngels[j].first.first - world -> NorthPolarAngel, 
                                              PolarAngels[j].first.second + (float)EARTH_RADIUS*2/3+40);
            pointsP2[k].x += EARTH_POSX, pointsP2[k].y += EARTH_POSY;
            
            pointsP3[k] = TransitionCoordinate(PolarAngels[j].first.first - world -> NorthPolarAngel, 
                                              PolarAngels[j].first.second + (float)EARTH_RADIUS*2/3+60);
            pointsP3[k].x += EARTH_POSX, pointsP3[k].y += EARTH_POSY;
            
            pointsP4[k] = TransitionCoordinate(PolarAngels[j].first.first - world -> NorthPolarAngel, 
                                              PolarAngels[j].first.second + (float)EARTH_RADIUS*2/3+80);
            pointsP4[k].x += EARTH_POSX, pointsP4[k].y += EARTH_POSY;
        }
        /*for (int j = 0; j < numPoints - 1; ++j) { 
            if(PolarAngels[j].second == ROPEDOT_ZERO)DrawLineEx(pointsP[j], pointsP[j + 1], 10.0, GRAY);
            if(PolarAngels[j].second == ROPEDOT_ALIVE)DrawLineEx(pointsP[j], pointsP[j + 1], 10.0, RED);
            if(PolarAngels[j].second == ROPEDOT_DEAD)DrawLineEx(pointsP[j], pointsP[j + 1], 10.0, BLUE);
        }*/

        for (int j = l, k = 0; j != r; j = (j + 1) % BLOCK_NUMBER, ++k) {
            int type = PolarAngels[j].second;
            if ((j + 1) % BLOCK_NUMBER != r) {
                if(type == ROPEDOT_ZERO){
                    DrawLineEx(pointsP[k], pointsP[k + 1], 10.0, GRAY);
                    DrawLineEx(pointsP1[k], pointsP1[k + 1], 10.0, GRAY);
                    DrawLineEx(pointsP2[k], pointsP2[k + 1], 10.0, GRAY);
                    DrawLineEx(pointsP3[k], pointsP3[k + 1], 10.0, GRAY);
                    DrawLineEx(pointsP4[k], pointsP4[k + 1], 10.0, GRAY);
                }
                if(type == ROPEDOT_ALIVE){
                    DrawLineEx(pointsP[k], pointsP[k + 1], 10.0, RED);
                    DrawLineEx(pointsP1[k], pointsP1[k + 1], 10.0, RED);
                    DrawLineEx(pointsP2[k], pointsP2[k + 1], 10.0, RED);
                    DrawLineEx(pointsP3[k], pointsP3[k + 1], 10.0, RED);
                    DrawLineEx(pointsP4[k], pointsP4[k + 1], 10.0, RED);
                }
                if(type == ROPEDOT_DEAD){
                    DrawLineEx(pointsP[k], pointsP[k + 1], 10.0, BLUE);
                    DrawLineEx(pointsP1[k], pointsP1[k + 1], 10.0, BLUE);
                    DrawLineEx(pointsP2[k], pointsP2[k + 1], 10.0, BLUE);
                    DrawLineEx(pointsP3[k], pointsP3[k + 1], 10.0, BLUE);
                    DrawLineEx(pointsP4[k], pointsP4[k + 1], 10.0, BLUE);
                }
            }
        }
        //printf("++++++++++ DrawRope() seg %d numPoints = %d\n", i, numPoints);                        
        // Draw a line defining thickness
        //DrawLineStrip(pointsP, numPoints, RED);
        //DrawLine(int startPosX, int startPosY, int endPosX, int endPosY, Color color);
    }
    //printf("%d\n", seg.size());
}

void ShowSTATE(const World* world) {
    DrawText(TextFormat("POINTS: %d, CURRENT_STAGE:%d", world -> points, world -> currentStage), 
             50, 20, 20, RED);
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(const World* world, Shader shader)
{
    // TODO: Draw GAMEPLAY screen here!
    ClearBackground(RAYWHITE);

        BeginShaderMode(shader);

            DrawTexture(world->texture[0], world->texture[0].width, 0, WHITE);
            DrawTexture(world->texture[0], 0, 0, WHITE);
            DrawTexture(world->texture[0], 0, world->texture[0].height, WHITE);
            DrawTexture(world->texture[0], world->texture[0].width, world->texture[0].height, WHITE);
            DrawTexture(world->texture[0], 0, 2*world->texture[0].height, WHITE);
            DrawTexture(world->texture[0], world->texture[0].width, 2*world->texture[0].height, WHITE);

        EndShaderMode();
        // printf("*************,%d",world->Earth_sita);
            DrawTexturePro(world->texture[1], world->sourceRec, world->destRec, world->origin, -(float)world -> NorthPolarAngel*RAD2DEG, WHITE);


            DrawRope(world);
            ShowSTATE(world);

            Rectangle frameRec = {0.0f,0.0f,(float)world->texture[2].width/6, (float)world->texture[2].height};
            frameRec.x = (float)(world->tiger.position)*(float)world->texture[2].width/6;
            Vector2 tiger_origin = TransitionCoordinate(world->tiger.sita,world->tiger.r+EARTH_RADIUS);
            Rectangle destRec = { EARTH_POSX, EARTH_POSY, (float)world->texture[2].width/6, (float)world->texture[2].height };
            DrawTexturePro(world->texture[2], frameRec, destRec, (Vector2){(float)world->texture[2].width/12,(float)(world->tiger.r+EARTH_RADIUS)}, 0,WHITE);
            // world->tiger->sita;
            // world->tiger->r;(float)world->texture[2].width/3,(float)world->texture[2].height*2
            // world->tiger->pos
            // printf("hhhhhhhh!\n");
            for(int i = 0; i < world->notes.notes.size(); i++){
                // world->notes.notes[i].sita;
                // // world->notes.notes[i].r;
                // printf("%lf\n",world->notes.notes[i]->r);
                int frameType = world->notes.notes[i]->type + 3;
                Rectangle frameRec = {0.0f,0.0f,(float)world->texture[frameType].width, (float)world->texture[frameType].height};
                // Vector2 tiger_origin = TransitionCoordinate(world->notes.notes[i]->sita,world->notes.notes[i]->r+EARTH_RADIUS);
                Rectangle destRec = { EARTH_POSX, EARTH_POSY, (float)world->texture[frameType].width/6, (float)world->texture[frameType].height };
                DrawTexturePro(world->texture[frameType], frameRec, destRec, (Vector2){(float)world->texture[frameType].width/2,(float)(world->notes.notes[i]->r+(float)EARTH_RADIUS*2.0/3+30)}, 
              (-(float)world->NorthPolarAngel + world->notes.notes[i]->sita+PI*7/12)*RAD2DEG,WHITE);
                
                Vector2 tiger_origin = TransitionCoordinate(world->notes.notes[i]->sita- world -> NorthPolarAngel, world->notes.notes[i]->r + (float)EARTH_RADIUS*2/3);
                tiger_origin.x += EARTH_POSX, tiger_origin.y += EARTH_POSY;
                DrawCircle(tiger_origin.x,tiger_origin.y,20,GREEN);
            }





    // DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), PURPLE);
    // DrawTextEx(font, "GAMEPLAY SCREEN / YYYY", (Vector2){ 20, 10 }, font.baseSize*3.0f, 4, MAROON);
    // DrawText("PRESS ENTER or TAP to JUMP to ENDING SCREEN", 130, 220, 20, MAROON);

}

void DrawStartScreen(const World* world)
{
    DrawRectangle(0,0, GetScreenHeight(),GetScreenHeight(),PURPLE);
}

void DrawFinishScreen(const World* world)
{
    DrawRectangle(0,0, GetScreenHeight(),GetScreenHeight(),PURPLE);
}

// Gameplay Screen Unload logic
GameResults UnloadGameplayScreen(void)
{
    // TODO: Unload GAMEPLAY screen variables here!
    // 返回一个实例
    GameResults res;
    res.points = world->points;
    res.maxpoints = world->maxpoints;
    res.currentStage = world->currentStage;
    time_t currentTime = time(0);
    res.usedTime = difftime(currentTime, world->beginTime);
    delete world;
    return res;
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    return finishScreen;
}