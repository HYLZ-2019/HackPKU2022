#ifndef WOLFH
#define WOLFH

class Wolf {
    public:
        double sita, r; // 极坐标描述位置(这里给出的r是离地高度)
        int life; // 存活时间
        int speed; // 速度底线（根据游戏进程常量处理）
        int time; // 狼自己的时间戳
        double l_sita, l_r; // 技能一的位置
        bool flag1; // 技能一放置
        double s_sita, s_r; 
        bool flag2;
        bool ready;
        bool ready2;
        int time1, time2;
        bool alive; // 是否在场
        int delta;                                                            
        double del_speed;
        double last_speed;
        double angle1;
        double angle2;

        Wolf() : time(0), alive(false), flag1(false), flag2(false), 
            del_speed(0), last_speed(0) {
        }

        double random_speed();

        void update_pos() ;

        int get_cur_sita() ;

        void init_wolf(int t);

        void wolf_skill_1();

        void wolf_skill_2();

        void update_wolf();

} ;

#endif