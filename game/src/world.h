#ifndef WORLDH
#define WORLDH
#include "screens.h"
#include "tiger.h"
#include "rope.h"
#include "notes.h"

/* 后端的所有信息。 */

class World {
  public:
    Tiger tiger;
    RopeInfo rope;
    NotesInfo notes;
    int points; // 清零时game over
    int currentStage;
    
    // 地球旋转角
    int Earth_sita;
    Texture texture[Texture_number];
    Rectangle sourceRec;
    Rectangle destRec;
    Vector2 origin;  
    World();
    
    int pointsNeededForNextStage(int stage);

    /* 调用tiger, rope, notes的update*/
    void updateWorld();
};

#endif