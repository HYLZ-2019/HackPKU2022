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
    return (Vector2){(float)(rho*sin(sita)), (float)(-rho*cos(sita))};
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
    if (status == ROPEDOT_DEAD) {
        printf("%lf\n",timer);
        return Fade(RED,(float)(100-timer)/(100));
    }
    
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

        free(pointsP);
        free(pointsP1);
        free(pointsP2);
        free(pointsP3);
        free(pointsP4);
    }
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

            // 画老虎
            Texture tiger = world->texture[World::TIGER];
            Rectangle frameRec = {(float)world->tiger.position*(tiger.width/6), 0.0f, float(tiger.width/6.0), (float)tiger.height};
            Rectangle destRec = {EARTH_POSX, EARTH_POSY, TIGER_WIDTH, TIGER_HEIGHT};
            DrawTexturePro(tiger, frameRec, destRec, (Vector2){(float)(TIGER_WIDTH/2),(float)(world->tiger.r+tiger.height/2 + EARTH_RADIUS*2.0/3)}, 0,WHITE);

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
                        if (note->time < FPS*(NOTE_LANTENCY-3)){
                            // 离爆炸还有三秒以上
                            pic = world->texture[World::NOTE_RED];
                        }
                        else if (note->time >= FPS*(NOTE_LANTENCY-3)){
                            // 闪烁警示
                            if ((note->time/10)%2==0){
                                pic = world->texture[World::NOTE_PINK];
                            }
                            else{
                                pic = world->texture[World::NOTE_RED];
                            }
                        }
                        break;
                    case 3:
                        // WolfNote (Also of type NormalNote)
                        pic = world->texture[World::NOTE_WOLF];
                        break;
                    assert(false);
                }
                Rectangle frameRec = {0.0f,0.0f,(float)pic.width, (float)pic.height};
                // Vector2 tiger_origin = TransitionCoordinate(world->notes.notes[i]->sita,world->notes.notes[i]->r+EARTH_RADIUS);
                Rectangle destRec = { EARTH_POSX, EARTH_POSY, NOTE_WIDTH, NOTE_HEIGHT };
                DrawTexturePro(pic, frameRec, destRec, (Vector2){(float)(pic.width/2),(float)(note->r+pic.height/2+EARTH_RADIUS*2.0/3)}, 
              (-(float)world->NorthPolarAngel + note->sita)*RAD2DEG,WHITE);

                Vector2 origin = TransitionCoordinate(note->sita - world->NorthPolarAngel, note->r + (float)EARTH_RADIUS*2/3);
                origin.x += EARTH_POSX, origin.y += EARTH_POSY;
                DrawCircle(origin.x,origin.y,20,GREEN);
                // Vector2 tiger_origin = TransitionCoordinate(note->sita - world->NorthPolarAngel, note->r + (float)EARTH_RADIUS*2/3);
                // tiger_origin.x += EARTH_POSX, tiger_origin.y += EARTH_POSY;
                // DrawCircle(tiger_origin.x,tiger_origin.y,20,GREEN);

                if (note->type == 2){
                // 最后1秒，闪烁叠加爆炸
                    if (note->time >= FPS*(NOTE_LANTENCY-1)){
                        int frame = note->time - (FPS*NOTE_LANTENCY - FPS*1);
                        int frames_per_pic = (FPS*1) / 25;
                        int cur_frame = frame / frames_per_pic;
                        Texture explode = world->texture[World::EXPLOSION];
                        Rectangle exs = { float((cur_frame%5)*204.8), float((cur_frame/5)*204.8), float(204.8), float(204.8)};
                        Rectangle exd = { EARTH_POSX, EARTH_POSY, (float)200, (float)200 };
                        DrawTexturePro(explode, exs, exd, (Vector2){(float)(200/2),(float)(note->r+200/2+(float)EARTH_RADIUS*2.0/3)}, 
              (-(float)world->NorthPolarAngel + note->sita)*RAD2DEG,WHITE);
                    }
                }
            }

            if(world->wolf.alive){
                // world->wolf.sita;
                Texture pic = world->texture[World::WOLF];
                Wolf wolf = world->wolf;//wolf;
                // printf("%lf,%lf\n",wolf.sita,wolf.r);
                Rectangle frameRec = {0.0f,0.0f,(float)pic.width, (float)pic.height};
                Rectangle destRec = { EARTH_POSX, EARTH_POSY, (float)pic.width, (float)pic.height };
                DrawTexturePro(pic, frameRec, destRec, (Vector2){(float)pic.width/2,(float)(wolf.r+pic.height/2+(float)EARTH_RADIUS*2.0/3)}, 
                (-(float)world->NorthPolarAngel + wolf.sita)*RAD2DEG,WHITE);
                if(wolf.flag1){
                    // printf("wolf is on  the = %lf, %lf\n", wolf.x, wolf.y);
                    Vector2 t = TransitionCoordinate(wolf.l_sita - wolf.angle, wolf.l_r + (float)EARTH_RADIUS*2/3);
                        t.x += EARTH_POSX, t.y += EARTH_POSY;
                    DrawCircle(t.x, t.y ,(float)WOLF_SKILL1_RADIUS,Fade(BLACK,0.2+0.8*(float)wolf.time1/(float)(15*FPS)));
                }
                if(wolf.flag2){
                    Vector2 s,t;
                    s = TransitionCoordinate(wolf.sita - world->NorthPolarAngel, wolf.r + (float)EARTH_RADIUS*2/3);
                    s.x += EARTH_POSX, s.y += EARTH_POSY;
                    if(wolf.ready){
                        t = TransitionCoordinate(wolf.s_sita - world->NorthPolarAngel, wolf.s_r + (float)EARTH_RADIUS*2/3);
                        t.x += EARTH_POSX, t.y += EARTH_POSY;
                    }
                    else{
                        t = TransitionCoordinate(world->tiger.sita - world->NorthPolarAngel, world->tiger.r + (float)EARTH_RADIUS*2/3);
                        t.x += EARTH_POSX, t.y += EARTH_POSY;
                    }
                    DrawLineV(s,t,YELLOW);
                }
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