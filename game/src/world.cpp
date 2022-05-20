#include "world.h"

void World::updateWorld() {
    tiger.updateTiger();
    rope.updateRope();
    notes.updateNotes();      
    if (points > pointsNeededForNextStage(currentStage)){
        currentStage += 1;
    }
    Earth_sita++;
    return ;
}