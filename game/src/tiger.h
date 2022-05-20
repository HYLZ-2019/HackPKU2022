#ifndef TIGERH
#define TIGERH

class World;
extern World* world; // 可以直接读取全局信息。不要修改自己(world.tiger)以外的全局信息。

class Tiger {
  public:
    double sita; // 0 ~ 2*pi.
    double r; // height.
    int index;
    
    //初始化老虎。pos为当前帧老虎所在的极角方向，pos \in [0, 2Pi)
    void initTigher(double pos);
    /* 每帧调用一次，不负责读取用户的键盘输入。
      delH表示当前时间段内老虎的高度变化，更新老虎的状态。 
      如果老虎触地，它只能在地上，不能穿透地心再向下了。*/
    void updateTiger(double delH);
};


#endif