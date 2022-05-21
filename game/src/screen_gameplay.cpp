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

/*
            pointsP[k] = TransitionCoordinate(PolarAngels[j].first.first - world -> NorthPolarAngel, 
                                              PolarAngels[j].first.second + (float)EARTH_RADIUS*2/3-range_right*(float)(k)/(float)(kk)-range_left);
            pointsP[k].x += EARTH_POSX, pointsP[k].y += EARTH_POSY;
            
            pointsP1[k] = TransitionCoordinate(PolarAngels[j].first.first - world -> NorthPolarAngel, 
                                              PolarAngels[j].first.second + (float)EARTH_RADIUS*2/3-(range_right/2)*(float)(k)/(float)(kk)-range_left/2);
            pointsP1[k].x += EARTH_POSX, pointsP1[k].y += EARTH_POSY;

            pointsP2[k] = TransitionCoordinate(PolarAngels[j].first.first - world -> NorthPolarAngel, 
                                              PolarAngels[j].first.second + (float)EARTH_RADIUS*2/3-0*(float)(k)/(float)(kk));
            pointsP2[k].x += EARTH_POSX, pointsP2[k].y += EARTH_POSY;

            pointsP3[k] = TransitionCoordinate(PolarAngels[j].first.first - world -> NorthPolarAngel, 
                                              PolarAngels[j].first.second + (float)EARTH_RADIUS*2/3+(range_right/2)*(float)(k)/(float)(kk)+range_left/2);
            pointsP3[k].x += EARTH_POSX, pointsP3[k].y += EARTH_POSY;
            
            pointsP4[k] = TransitionCoordinate(PolarAngels[j].first.first - world -> NorthPolarAngel, 
                                              PolarAngels[j].first.second + (float)EARTH_RADIUS*2/3+range_right*(float)(k)/(float)(kk)+range_left);
            pointsP4[k].x += EARTH_POSX, pointsP4[k].y += EARTH_POSY;
*/

void Rela2EARTH(Vector2& v) {
    v.x += EARTH_POSX, v.y += EARTH_POSY;
}

void getPointInfo(double rangeL, double rangeR, int len, int index, PA_t p, 
                  Vector2& pointP, Vector2& pointP1, Vector2& pointP2, Vector2& pointP3, Vector2& pointP4) {
    p.second += EARTH_RADIUS * 2 / 3;
    double k = (double)index / len;
    Rela2EARTH(pointP  = TransitionCoordinate(p.first - world -> NorthPolarAngel, p.second - k * rangeR - rangeL));
    Rela2EARTH(pointP1 = TransitionCoordinate(p.first - world -> NorthPolarAngel, p.second - k * rangeR / 2 - rangeL / 2));
    Rela2EARTH(pointP2 = TransitionCoordinate(p.first - world -> NorthPolarAngel, p.second));
    Rela2EARTH(pointP3 = TransitionCoordinate(p.first - world -> NorthPolarAngel, p.second + k * rangeR / 2 + rangeL / 2));
    Rela2EARTH(pointP4 = TransitionCoordinate(p.first - world -> NorthPolarAngel, p.second + k * rangeR + rangeL));
}

Color getRopeColor(ROPEDOT_STATE status, int timer) { //给出绳粒子的状态, 返回绳粒子的颜色
    if (status == ROPEDOT_ALIVE) return RED;
    if (status == ROPEDOT_DEAD) return BLUE;
    
    return GRAY;
}

void DrawRope(const World* world) {
    std::vector <std::pair <double, double> > p;
    //std::vector <std::pair<std::pair <double, double>, ROPEDOT_STATE > > PolarAngels;
    std::vector <std::pair <int,int> > seg;
    world -> rope.getRopeData(seg, p);
    //if (seg.size() > 1) printf("SegSize = %d\n", seg.size());
    for (int i = 0, sze = seg.size(); i < sze; ++i) {
        int l = seg[i].first, r = seg[i].second;
        int numPoints = r - l;
        if (numPoints <= 0) numPoints += BLOCK_NUMBER;
        Vector2 *pointsP  = (Vector2 *)malloc(numPoints * sizeof(Vector2));
        Vector2 *pointsP1 = (Vector2 *)malloc(numPoints * sizeof(Vector2));
        Vector2 *pointsP2 = (Vector2 *)malloc(numPoints * sizeof(Vector2));
        Vector2 *pointsP3 = (Vector2 *)malloc(numPoints * sizeof(Vector2));
        Vector2 *pointsP4 = (Vector2 *)malloc(numPoints * sizeof(Vector2));
        
        double rangeL = std::min(30.0, p[l].second), rangeR = 30;
        for (int j = l, k = 0; j != r; j = (j + 1) % BLOCK_NUMBER, ++k) 
            getPointInfo(rangeL, rangeR, numPoints, k, p[j], 
                         pointsP[k], pointsP1[k], pointsP2[k], pointsP3[k], pointsP4[k]);
        
        int centreJ = 0;
        for (int j = l, k = 0; j != r; j = (j + 1) % BLOCK_NUMBER, ++k) {
            if(k == numPoints / 2) centreJ = j;
            ROPEDOT_STATE status;
            int timer;
            (world -> rope).Index2Type(j, status, timer);

          
            if ((j + 1) % BLOCK_NUMBER != r) {
                Color col = getRopeColor(status, timer);
                float thick = 4.0;
                DrawLineEx(pointsP[k],  pointsP[k + 1],  thick, col);
                DrawLineEx(pointsP1[k], pointsP1[k + 1], thick, col);
                DrawLineEx(pointsP2[k], pointsP2[k + 1], thick, col);
                DrawLineEx(pointsP3[k], pointsP3[k + 1], thick, col);
                DrawLineEx(pointsP4[k], pointsP4[k + 1], thick, col);
            }
        }
    }
    //if (seg.size() > 1) printf("\n");
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

            DrawTexture(world->texture[World::SPACE], world->texture[World::SPACE].width, 0, WHITE);
            DrawTexture(world->texture[World::SPACE], 0, 0, WHITE);
            DrawTexture(world->texture[World::SPACE], 0, world->texture[World::SPACE].height, WHITE);
            DrawTexture(world->texture[World::SPACE], world->texture[World::SPACE].width, world->texture[World::SPACE].height, WHITE);
            DrawTexture(world->texture[World::SPACE], 0, 2*world->texture[World::SPACE].height, WHITE);
            DrawTexture(world->texture[World::SPACE], world->texture[World::SPACE].width, 2*world->texture[World::SPACE].height, WHITE);

        EndShaderMode();
        // printf("*************,%d",world->Earth_sita);
            DrawTexturePro(world->texture[World::MOON], world->sourceRec, world->destRec, world->origin, -(float)world -> NorthPolarAngel*RAD2DEG, WHITE);


            DrawRope(world);
            ShowSTATE(world);

            Rectangle frameRec = {0.0f,0.0f,(float)world->texture[World::TIGER].width/6, (float)world->texture[World::TIGER].height};
            frameRec.x = (float)(world->tiger.position)*(float)world->texture[World::TIGER].width/6;
            Vector2 tiger_origin = TransitionCoordinate(world->tiger.sita,world->tiger.r+EARTH_RADIUS);
            Rectangle destRec = { EARTH_POSX, EARTH_POSY, (float)world->texture[World::TIGER].width/6, (float)world->texture[World::TIGER].height };
            DrawTexturePro(world->texture[World::TIGER], frameRec, destRec, (Vector2){(float)world->texture[World::TIGER].width/12,(float)(world->tiger.r+EARTH_RADIUS-50)}, 0,WHITE);

            for(int i = 0; i < world->notes.notes.size(); i++){
                // world->notes.notes[i].sita;
                // // world->notes.notes[i].r;
                // printf("%lf\n",world->notes.notes[i]->r);
                Note* note = world->notes.notes[i];
                Texture pic;
                switch (note->type) {
                    case 0:
                        // NormalNote
                        pic = world->texture[World::NOTE_ORANGE];
                        break;
                    case 1:
                        // FasterNote
                        pic = world->texture[World::NOTE_PURPLE];
                        break;
                    case 2:
                        // ExplosiveNote
                        pic = world->texture[World::NOTE_BLUE];
                        break;
                    case 3:
                        // WolfNote (Also of type NormalNote)
                        pic = world->texture[World::NOTE_WOLF];
                        break;
                    assert(false);
                }
                Rectangle frameRec = {0.0f,0.0f,(float)pic.width, (float)pic.height};
                // Vector2 tiger_origin = TransitionCoordinate(world->notes.notes[i]->sita,world->notes.notes[i]->r+EARTH_RADIUS);
                Rectangle destRec = { EARTH_POSX, EARTH_POSY, (float)pic.width/6, (float)pic.height };
                DrawTexturePro(pic, frameRec, destRec, (Vector2){(float)pic.width/2,(float)(note->r+(float)EARTH_RADIUS*2.0/3+30)}, 
              (-(float)world->NorthPolarAngel + note->sita+PI*7/12)*RAD2DEG,WHITE);
                
                Vector2 tiger_origin = TransitionCoordinate(note->sita - world->NorthPolarAngel, note->r + (float)EARTH_RADIUS*2/3);
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