#include <cmath>

const int FPS = 60; // 游戏的帧率

// TODO(wy) : 修改至你喜欢的大小
const int BLOCK_NUMBER = 720; // 2pi被分割成多少个角度

// TODO(xy) : 给出你觉得合适的判定点大小
const int NOTE_RADIUS = 10;//判定点大小

// 屏幕长款
const int screenWidth = 800;
const int screenHeight = 800;

// 地球半径
const int Earth_RADIUS = 400;

// 运动速度
const int SpeedMin = 3;
const int SpeedMax = 10;

// const double PI = acos(-1.0);

// 总共有几个关卡（一旦stage变成MAX_STAGE，就视为通关）
const int MAX_STAGE = 10;