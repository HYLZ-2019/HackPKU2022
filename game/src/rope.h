#ifndef ROPE_H
#define ROPE_H
#include <vector>

class RopeInfo {
  public:
    std::vector<RopeDot> dots;

    /* 每帧调用一次。*/
    void normalUpdate(){
        
    }

    /* 在爆炸音符爆炸的时候调用。这个函数需要炸掉索引为[left, right)的绳段（即dots[left].r, ... ,dots[right-1].r都归零。）*/
    void breakRope(int left, int right){

    }
};

class RopeDot {
  public:
    double sita; // 0 ~ 2*pi
    double r; // Height.
    enum {
        ROPEDOT_ZERO, // 高度为0、贴在地面上的绳子。
        ROPEDOT_ALIVE, // 高度不为零，且与老虎连在一起的绳子。
        ROPEDOT_DEAD  // 高度不为零，但与老虎之间被炸断了的绳子。
    } status;
    int die_time; // 只有ROPEDOT_DEAD的点有这个属性，它记录了这个点变成死的以来过了多少帧。
};


#endif