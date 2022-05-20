#ifndef ROPE_H
#define ROPE_H

#include <vector>

class World;
extern World* world; // 可以直接读取全局信息。不要修改自己(world.rope)以外的全局信息。

enum ROPEDOT_STATE {
    ROPEDOT_ZERO, // 高度为0，并且现在不存在的绳子。
    ROPEDOT_ALIVE, // 高度不为零，且与老虎连在一起的绳子。
    ROPEDOT_DEAD  // 高度不为零，但与老虎之间被炸断了的绳子。
};

class RopeDot {
  public:
    double sita; // 0 ~ 2*pi
    double r; // Height.
    ROPEDOT_STATE status;
    int die_time; // 只有ROPEDOT_DEAD的点有这个属性，它记录了这个点变成死的以来过了多少帧。 0表示刚刚死或者活着
    RopeDot(int index); //index为[0, BLOCK_NUMBER)范围内的整数，表明点在的极角方向
    bool isALIVE();
};


class RopeInfo {
  public:
    std::vector<RopeDot> dots;
    std::vector<std::pair <int, int> > segments; //段区间是左闭右开的

    //初始化绳, 初始帧调用。请在initTiger()之后调用
    void initRope(); 
    /* 
      绳状态更新，每帧调用一次。请在调用updateTiger()后调用
    */
    void updateRope();

    /* 在爆炸音符爆炸的时候调用。这个函数需要炸掉索引为[left, right)的绳段（即dots[left].r, ... ,dots[right-1].r都归零。）
      Attention：请在调用updateRope()后使用
    */
    void breakRope(int left, int right); 

    //get the Rope's data
    void getRopeData(std::vector<std::pair <int,int> >& seg, std::vector<std::pair<std::pair <double, double>, ROPEDOT_STATE > >& PAs) const;
  private:
    void getSegs();
};


#endif