#include "screens.h"

extern World* world; 
// 可以直接读取全局信息。不要修改自己(world.notes)以外的全局信息。

Note::Note(int type, double sita, double r) : type(type), 
            sita(sita), r(r), alive(true), speed(SpeedMin + world->currentStage)
                , time(0), points(score[type]) {}

ExplosiveNote::ExplosiveNote(int type, double sita, double r) {
            Note(type, sita, r);
            speed = SpeedMin * 2 + world->currentStage;
}

void NotesInfo::addNotes(int type) {
    
}

void NormalNote::update_pos() {
    static int interval = FPS / time_stamp;
    if (time % FPS == 0) {
        delta = (random_number() & 1) ? 1 : -1;
        del_speed = (random_number() & 1) 
            ? random_speed() : -random_speed();
    }
    if (time % interval == 0) {
        int cur_sita = get_cur_sita();
        cur_sita = (cur_sita + delta + BLOCK_NUMBER) % BLOCK_NUMBER;
        sita = get_sita(cur_sita);
    }
    r += del_speed / (double)FPS;
    if (r < 0) r = 0, del_speed = -del_speed;
    ++time;
}

void FasterNote::update_pos() {
    static int interval = FPS / time_stamp, cur_sita, delta;
    if (time % FPS == 0) {
        if (random_number() & 1) {
            // 随机游走
            delta = (random_number() & 1) ? 1 : -1;
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
    r += del_speed / (double)FPS;
    if (r < 0) r = 0, del_speed = -del_speed;
    ++time;
}

void ExplosiveNote::update_pos() {
    static int interval = FPS / time_stamp, cur_sita, delta;
    if (time % FPS == 0) {
        // 随机游走
        delta = (random_number() & 1) ? 1 : -1;
        del_speed = (random_number() & 1) ? random_speed()
            : -random_speed();
    }
    if (time % (interval / 2) == 0) {
        int cur_sita = get_cur_sita();
        cur_sita = (cur_sita + delta + BLOCK_NUMBER) % BLOCK_NUMBER;
        sita = get_sita(cur_sita);
    }
    r += del_speed / (double)FPS;
    if (r < 0) r = 0, del_speed = -del_speed;
    ++time;
}

void NotesInfo::updateNotes() {
    std::vector<Note*> cur_notes;
    for ( ; !notes.empty();) {
        Note *e = notes.back();
        notes.pop_back();
        if (e->get_collision()) {
            world->points += e->points;
            delete e;
        } else if (e->out_of_range()) {
            delete e;
        } else {
            e->update_pos();
            cur_notes.push_back(e);
        }
    }
    if (time % FPS == 0) {
        int ran = random_number() % MAX_STAGE;
        if (ran < world->currentStage / 4) addNotes(2);
        else if (ran < MAX_STAGE / 2) addNotes(0);
        else if (ran < MAX_STAGE / 3 * 2) addNotes(1);
        else if (ran < MAX_STAGE / 5 * 4) addNotes(3);
    }
    ++time;
    return;
}