#ifndef NOTESH
#define NOTESH
#include "raylib.h"
#include "constant.h"
#include <vector>
#include <ctime>
#include <random>

const int time_stamp = 15;
const int score[4] = {5, 10, 20, -5};

inline int random_number() {
    return GetRandomValue(0, 147483647);
}

inline double get_sita(int pos) {
    return  2.0 * PI * pos / BLOCK_NUMBER;
}

class Note {
    public:
        int type; //音符类型
        double sita, r; // 极坐标描述位置(这里给出的r是离地高度)
        bool alive; // 是否存活
        // TODO: 待调参
        int speed; // 速度底线（根据游戏进程常量处理）
        int time; // 音符自己的时间戳
        int points; // 得分情况
        int delta;                                                             //警告：未初始化
        int del_speed;                                                         //警告：未初始化
        Note() {}
        Note(int type, double sita, double r);

        inline double random_speed() {
<<<<<<< HEAD
            return (double)(random_number() % (NOTE_MID_SPEED - speed - 1)     
=======
            return (double)(random_number() % (NOTE_HIGH_SPEED - speed - 1)
>>>>>>> 5e5785aa39f7005b94a35f603d5d6a5d03049070
                 + speed);
        }

        inline double get_dis(double dot_sita, double dot_r) {
            return sqrt(r * r + dot_r * dot_r - 2.0 * r * 
                dot_r * cos(sita - dot_sita));
        }

        inline int get_cur_sita() {
            return floor(sita * BLOCK_NUMBER / 2.0 / PI);
        }

        virtual inline void update_pos() {
            printf("Note");
            return ;
        }

        inline bool break_rope() {
            return false;
        }

        bool get_collision() ;

        bool out_of_range() {
            return r > MAX_HEIGHT || r < 0;
        }
};

class NotesInfo {
  public:
    std::vector<Note*> notes;
    int time;
    
    /* 更新所有音符的状态（包括生成新音符，改变已有音符的位置，消灭过期音符等） */
    NotesInfo() : time(0) {}

    virtual void updateNotes() ;
    void addNotes(int type) ; 
} ;

/* 普通音符 */
class NormalNote : public Note {
    public:
        NormalNote() {}
        NormalNote(int type, double sita, double r);

        virtual void update_pos();
};

class FasterNote : public Note {
    public:
        FasterNote() {}
        FasterNote(int type, double sita, double r) ;
        virtual void update_pos();

}; 

class ExplosiveNote : public Note {
    public:
        ExplosiveNote() {}
        ExplosiveNote(int type, double sita, double r) ;

        inline double random_speed() {
            return (double)(random_number() % (NOTE_HIGH_SPEED - speed - 1)
                 + speed);
        }

        void update_pos();
        bool break_rope();
};

#endif