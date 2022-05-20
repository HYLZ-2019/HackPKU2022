#ifndef WORLDH
#define WORLDH
#include "screens.h"


/* 后端的所有信息。 */
class World {
  public:
    Tiger tiger;
    RopeInfo rope;
    NotesInfo notes;
    
    /* 调用tiger, rope, notes的update*/
    void updateWorld(){
        return;
    }
};

#endif