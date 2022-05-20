#ifndef WORLDH
#define WORLDH
#include "screens.h"


/* 后端的所有信息。 */
class World {
  public:
    Tiger tiger;
    RopeInfo rope;
    NotesInfo notes;
    int points; // 清零时game over
    int currentStage;

    World(){
        points = 0;
        currentStage = 0;
    }
    
    /* 调用tiger, rope, notes的update*/
    void updateWorld(){
        return;
    }
};

#endif