#ifndef WORLDH
#define WORLDH
#include "screens.h"
#include "tiger.h"
#include "rope.h"
#include "notes.h"
#include "wolf.h"
#include <map>

/* 后端的所有信息。 */

class World {
  public:
    Tiger tiger;
    RopeInfo rope;
    NotesInfo notes;
    Wolf wolf;
    int points; // 清零时game over
    int maxpoints; // 被打爆之前的最高分
    int currentStage;
    time_t beginTime;

    double NorthPolarAngel;
    
    // 地球旋转角
    float Earth_sita;
    Texture texture[TEXTURE_CNT];
    enum {
      MOON,
      SPACE,
      TIGER,
      NOTE_PURPLE,
      NOTE_ORANGE,
      NOTE_BLUE,
      NOTE_WOLF,
      WOLF
    }; // 所有图片的编号

    Rectangle sourceRec;
    Rectangle destRec;
    Vector2 origin;  
    World();
    
    int pointsNeededForNextStage(int stage);

    /* 调用tiger, rope, notes的update*/
    void updateWorld();
};


#endif