#ifndef TIGERH
#define TIGERH

#include "screens.h"

extern World* world; // 可以直接读取全局信息。不要修改自己(world.tiger)以外的全局信息。

class Tiger {
  public:
    double sita; // 0 ~ 2*pi.
    double r; // height.
    
    /* 每帧调用一次。要负责读取用户的键盘输入，并更新老虎的状态。 */
    void updateTiger(){

    }
};


#endif