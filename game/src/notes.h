#ifndef NOTESH
#define NOTESH
#include "screens.h"
#include <vector>

extern World* world; // 可以直接读取全局信息。不要修改自己(world.notes)以外的全局信息。

class Note;

class NotesInfo {
  public:
    std::vector<Note*> notes;
    
    /* 更新所有音符的状态（包括生成新音符，改变已有音符的位置，消灭过期音符等） */
    void updateNotes(){
        return;
    }

    void addNotes(){
        return;
    }
};

/* 所有音符的基类 */
class Note {
  public:
    // 位置
    double sita;
    double r;
    // 吸收后能得多少分
    int points;
    
    virtual void update();
};

/* 普通音符 */
class NormalNote : Note {
    void update(){
        return;
    }
};

class ExplosiveNote : Note {
    void update(){
        return;
    }
};

#endif