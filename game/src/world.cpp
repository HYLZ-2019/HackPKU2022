#include "world.h"

/* 后端的所有信息。 */
World :: World() {
    points = 0;
    currentStage = 0;
}

int World::pointsNeededForNextStage(int stage) {
    assert(stage < MAX_STAGE);
    return (stage+1) * 100;
}

/* 调用tiger, rope, notes的update*/
void World::updateWorld(){
    double deltaH = 0;
    //
    tiger.updateTiger(deltaH); 
    rope.updateRope();
    notes.updateNotes();      
    if (points > pointsNeededForNextStage(currentStage)){
        currentStage += 1;
    }
    return;
}