#ifndef CONSTANT_H
#define CONSTANT_H

#include <cmath>

const int FPS = 60; // 游戏的帧率

// TODO: 需要调参
const int BLOCK_NUMBER = 720; // 2pi被分割成多少个角度

const double MOVE_STEP_LENGTH = 3; //老虎的上升/下降速度
const double DOWN_STEP_LENGTH = 1;

const double MAX_HEIGHT = 300;  //高度上限

const double ROPE_DOWN_STEP = 1; //绳子的自然下坠速度

// TODO(xy) : 给出你觉得合适的判定点大小
const int NOTE_RADIUS = 10;//判定点大小

// 屏幕长款
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 900;

// 地球config
const int EARTH_RADIUS = 400;
const int EARTH_POSX = SCREEN_WIDTH/2;
const int EARTH_POSY = SCREEN_HEIGHT*2/3;

// note config
const int NOTE_WIDTH = 50;
const int NOTE_HEIGHT = 50;



// 纹理个数
const int TEXTURE_CNT = 8;

const int MOVE_SPEED = 120;

// 运动速度
  // 常规音符
const int NOTE_MIN_SPEED = 3;
const int NOTE_MID_SPEED = 10;
  // 爆炸音符
const int NOTE_HIGH_SPEED = 20;

// 总共有几个关卡（一旦stage变成MAX_STAGE，就视为通关）
const int MAX_STAGE = 10;

#endif