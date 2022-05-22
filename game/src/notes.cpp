#include "screens.h"

extern World* world; 
// 可以直接读取全局信息。不要修改自己(world.notes)以外的全局信息。

Note::Note(int type, double sita, double r) : type(type), 
            sita(sita), r(r), alive(true), speed(NOTE_MIN_SPEED + world->currentStage * 2), del_speed(0), last_speed(0), 
                time(0), points(score[type]) {}

NormalNote::NormalNote(int type, double sita, double r) : Note(type, sita, r) {
}

FasterNote::FasterNote(int type, double sita, double r) : Note(type, sita, r) {
}

ExplosiveNote::ExplosiveNote(int type, double sita, double r)
     : Note(type, sita, r) {
    speed = NOTE_MIN_SPEED * 3 + world->currentStage;
}

void NotesInfo::addNotes(int type) {
    Note* cur;
    int left = world->tiger.index + 30;
    int right = left + BLOCK_NUMBER / 3 * 2;
    int temp = (GetRandomValue(left, right) % BLOCK_NUMBER);
    double sita = get_sita(temp);
    double r = random_number() % ((int)MAX_HEIGHT - 51) + 50;
    printf("%d, %d\n", world->tiger.index, temp);
    // printf("%lf, %lf\n", sita, r);
    switch(type) {    
        case 0:
            cur = new NormalNote(type, sita, r);
            break ;
        case 1:
            cur = new FasterNote(type, sita, r);
            break ;
        case 2:
            cur = new ExplosiveNote(type, sita, r);
            break ;
        case 3:
            cur = new NormalNote(type, sita, r);
            break ;
    }
    
    notes.push_back(cur);
}

void NormalNote::update_pos() {
    static int interval = FPS / time_stamp;
    static int r_interval = FPS / 2;
    // printf("NormalNote");
    if (time % r_interval == 0) {
        // printf("NormalNote");
        // delta = (random_number() & 1) ? 1 : -1;
        last_speed = del_speed;
        delta = (random_number() % 3) - 1;
        del_speed = (random_number() & 1) 
            ? random_speed() : -random_speed();
    }
    if (time % interval == 0) {
        int cur_sita = get_cur_sita();
        cur_sita = (cur_sita + delta + BLOCK_NUMBER) % BLOCK_NUMBER;
        sita = get_sita(cur_sita);
    }
    r += ((del_speed - last_speed) / (double)r_interval * 
        (time % r_interval + 1) + last_speed) / FPS;
    if (r < MIN_HEIGHT) 
        r = MIN_HEIGHT, del_speed = -del_speed, last_speed = -last_speed;
    if (r > MAX_HEIGHT) 
        r = MAX_HEIGHT, del_speed = -del_speed, last_speed = -last_speed;
    ++time;
}

void FasterNote::update_pos() {
    // printf("FasterNote");
    static int interval = FPS / time_stamp; 
    static int r_interval = FPS / 2;
    if (time % r_interval == 0) {
        if (random_number() & 1) {
            // 随机游走
            // delta = (random_number() & 1) ? 1 : -1;
            last_speed = del_speed;
            delta = (random_number() % 3) - 1;
            del_speed = (random_number() & 1) 
                ? random_speed() : -random_speed();
        } else {
            del_speed = (world->tiger.r > r) ? -random_speed() 
                : random_speed() ;
            double temp_sita = world->tiger.sita - sita;
            if (temp_sita < 0) temp_sita += 2.0 * PI;
                delta = (temp_sita > PI) ? 1 : -1;
        }
    }
    if (time % interval == 0) {
        int cur_sita = get_cur_sita();
        cur_sita = (cur_sita + delta + BLOCK_NUMBER) % BLOCK_NUMBER;
        sita = get_sita(cur_sita);
    }
    r += ((del_speed - last_speed) / (double)r_interval * 
        (time % r_interval + 1) + last_speed) / FPS;
    if (r < MIN_HEIGHT) 
        r = MIN_HEIGHT, del_speed = -del_speed, last_speed = -last_speed;
    if (r > MAX_HEIGHT) 
        r = MAX_HEIGHT, del_speed = -del_speed, last_speed = -last_speed;
    ++time;
}

void ExplosiveNote::update_pos() {
    // printf("E)xplosiveNote");
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
    if (r < MIN_HEIGHT) 
        r = MIN_HEIGHT, del_speed = -del_speed, last_speed = -last_speed;
    if (r > MAX_HEIGHT) 
        r = MAX_HEIGHT, del_speed = -del_speed, last_speed = -last_speed;
    ++time;
}

void NotesInfo::updateNotes() {
    std::vector<Note*> cur_notes;
    cur_notes.clear();
    for ( ; !notes.empty();) {
        Note *e = notes.back();
        notes.pop_back();
        if (e->get_collision()) {
            world->points += e->points;
            (e->type == 3) ?  PlaySound(fxWeird) : PlaySound(fxCoin);
            delete e;
        } else if (e->out_of_range()) {
            delete e;
        } else {
            e->update_pos();
            if (e->type == 2) {
                if (e->break_rope()) {
                    delete e;
                    continue ;
                } 
            }
            cur_notes.push_back(e);
        }
    }
    while (!cur_notes.empty()) {
        notes.push_back(cur_notes.back());
        cur_notes.pop_back();
    }
    static int x = 0;
    if (time % FPS == 0 || (IsKeyDown(KEY_ZERO))) {
        int ran = random_number() % (MAX_STAGE * 100);
        if (ran < world->currentStage * 30) addNotes(2);
        else if (ran < MAX_STAGE * 50) addNotes(0);
        else if (ran < MAX_STAGE * 75) addNotes(1);
        else if (world->currentStage) addNotes(3);
        else addNotes(0);
        x = (x - 1 + FPS) % FPS;
    }
    ++time;
    return;
}

const int CHECK_SIZE = 15;

bool Note::get_collision() {
    int cur = get_cur_sita();
    int l = (cur - CHECK_SIZE + BLOCK_NUMBER) % BLOCK_NUMBER;
    int r = (cur + CHECK_SIZE + BLOCK_NUMBER) % BLOCK_NUMBER;
    for (int i = l; i != r; i = (i + 1) % BLOCK_NUMBER) {
        RopeDot cur_dot = world->rope.dots[i];
        if (!cur_dot.isALIVE()) continue ;
        int pos = (i - cur_dot.sl + BLOCK_NUMBER) % BLOCK_NUMBER;
        int len = (cur_dot.sr - cur_dot.sl + BLOCK_NUMBER) % BLOCK_NUMBER;
        if (get_dis(cur_dot.sita, cur_dot.r) < (double)NOTE_RADIUS + 30.00 * 
            (double)pos / (double)len) {
            return true;
        }
    }
    return false;
}

bool ExplosiveNote::break_rope() {
    if (time % (FPS * NOTE_LANTENCY) != 0 || time == 0) {
        return false; 
    }
    if (time % (FPS * NOTE_LANTENCY) != 0 || time == 0) return false; 

    PlaySound(fxBoom);

    //接下来要算一个可靠的爆炸区间
    int l = world->rope.segments[0].first;
    int r = world->rope.segments[0].second;
    int len = r - l;
    if (len < 0) len += BLOCK_NUMBER;
    if (len <= 1) { //相当于现在没有ALIVE线，只有老虎在的那一个点
        return true; //没有实际爆炸效果
    }

    if (r < l) r += BLOCK_NUMBER; 
    int blowL = (l + random_number() % (len - 1));
    int blowR = std::min(blowL + EXPLOSION_RANGE, r);

    world->rope.breakRope(blowL % BLOCK_NUMBER, blowR % BLOCK_NUMBER);
    return true;
}