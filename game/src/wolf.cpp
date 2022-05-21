#include "screens.h"

extern World* world;

double Wolf::random_speed() {
    return (double)(random_number() % (WOLF_MAX_SPEED - 
        speed - 1) + speed);
}

int Wolf::get_cur_sita() {
    return floor(sita * BLOCK_NUMBER / 2.0 / PI);
}

void Wolf::update_pos() {
    static int interval = FPS / time_stamp;
    static int r_interval = FPS / 2;
    if (time % r_interval == 0) {
        // delta = (random_number() & 1) ? 2 : -2;
        last_speed = del_speed;
        delta = (random_number() % 5) - 2;  //delta sita
        del_speed = (random_number() & 1) ? random_speed()
            : -random_speed();
    }
    if (time % (interval / 2) == 0) {
        int cur_sita = get_cur_sita();
        cur_sita = (cur_sita + delta + BLOCK_NUMBER) % BLOCK_NUMBER;
        sita = get_sita(cur_sita);
    }
    r += ((del_speed - last_speed) / (double)r_interval * 
        (time % r_interval + 1) + last_speed) / FPS;
    if (r < 0) r = 0, del_speed = -del_speed, last_speed = -last_speed;
    if (r > MAX_HEIGHT) 
        r = MAX_HEIGHT, del_speed = -del_speed, last_speed = -last_speed;
}

void Wolf::init_wolf(int t) {
    // 切换到狼的bgm
    SetMusicVolume(music, 2.0f);
    music = wolfMusic;
    time = 0, alive = true;
    sita = get_sita(random_number() % BLOCK_NUMBER);
    r = random_number() % ((int)MAX_HEIGHT - 10) + 5;
    life = t, speed = WOLF_MIN_SPEED + world->currentStage * 2;
    flag1 = flag2 = false;
}

void Wolf::wolf_skill_1() {
    l_sita = sita, l_r = r, angle = world->NorthPolarAngel, 
        flag1 = true, time1 = 0;
}

void Wolf::wolf_skill_2() {
    flag2 = true, ready = false, time2 = 0;
}

void Wolf::update_wolf() {
    if (alive == false) return ;
    // printf("enter_wolf\n");
    if (time / FPS >= life && !flag1 && !flag2) {
        alive = false;
        // 狼消失后，切换回老虎的bgm
        SetMusicVolume(music, 1.0f);
        music = tigerMusic;
        return ;
    }
    // printf("enter_wolf!\n");
    if (flag1) {
        ++time1;
        if (time1 == 15 * FPS + 1) {
            flag1 = false;
        }
    }
    // printf("enter_wolf1!\n");
    if (flag2) {
        ++time2;
        if (!ready) {
            if (time2 == FPS * 2) {
                ready = true;
                s_sita = world->tiger.sita;
                s_r = world->tiger.r;
            }    
        } else {
            if (time2 == FPS * 4) flag2 = false;
        }
    }
    // printf("enter_wolf2!\n");
    if (time % FPS == 0) {
        printf("%d, %d\n", flag1, flag2);
    }
    if (time % (FPS * 30) == 0 && time) {
        printf("skill_1\n");
        wolf_skill_1();
    } else if (time % (FPS * 20) == 0 && time) {
        printf("skill_2\n");
        wolf_skill_2();
    } else {
        if (!flag2) update_pos();
    }
    ++time;
}