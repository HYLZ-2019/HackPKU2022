#include "screens.h"

extern World* world;
    
//初始化老虎。pos为当前帧老虎所在的极角方向，pos \in [0, 2Pi)
void Tiger :: initTiger(double pos) {
    sita = pos;
    r = 0;
    position = 0;
    index = floor(pos * BLOCK_NUMBER / (2 * PI));
    times = 0;
    return;
}

/* 每帧调用一次，不负责读取用户的键盘输入。
    delH表示当前时间段内老虎的高度变化，更新老虎的状态。 
    如果老虎触地，它只能在地上，不能穿透地心再向下了。*/
void Tiger :: updateTiger(double delH) {
    if (++index == BLOCK_NUMBER) index = 0;
    sita = 2.0 * PI * index / BLOCK_NUMBER;

    r += delH;
    if (r < 0) r = 0;
    if (r > MAX_HEIGHT) r = MAX_HEIGHT;

    if (world->wolf.flag2 && world->wolf.ready2 && 
        std::fabs(world->wolf.s_r - r) < TIGER_HEIGHT / 2)
            world->points -= 10;

    if(times%6==0) position = (position+1)%6;
    times ++;
    return;
}
