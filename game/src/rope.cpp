#include "constant.h"
#include "world.h"
#include "rope.h"

//初始化绳粒子
RopeDot :: RopeDot(int index) {
    sita = 2.0 * PI * index / BLOCK_NUMBER;
    r = 0;
    status = ROPEDOT_ZERO;
    die_time = 0;
    return;
};

bool RopeDot :: isALIVE() {
    return status == ROPEDOT_ALIVE;
}

//初始化绳, 初始帧调用。请在initTiger()之后调用
void RopeInfo :: initRope() {
    dots.clear();
    for (int i = 0; i < BLOCK_NUMBER; ++i) 
        dots.push_back(RopeDot(i));
    int index = world -> tiger.index;
    dots[index].status = ROPEDOT_ALIVE;
    
    segments.clear();
    segments.push_back(std :: make_pair(index, (index + 1) % BLOCK_NUMBER));
}

/* 
绳状态更新，每帧调用一次，在调用updateTiger()后调用。
*/
RopeDot NextDot(const RopeDot& dL, const RopeDot &dC, const RopeDot& dR) { //不会更改status
    RopeDot nxt = dC;
    if (nxt.status == ROPEDOT_ZERO) return nxt;
    
    int cnt = 0;
    double sumR = 0;
    if (dL.status == ROPEDOT_ALIVE) ++cnt, sumR += dL.r;
    if (dC.status == ROPEDOT_ALIVE) ++cnt, sumR += dC.r;
    if (dR.status == ROPEDOT_ALIVE) ++cnt, sumR += dR.r;

    double avg;
    if (cnt > 0) avg = sumR / cnt;
    else avg = dC.r;

    nxt.r = std :: max(avg - ROPE_DOWN_STEP, (double)0);
    if (nxt.status == ROPEDOT_DEAD) nxt.die_time = nxt.die_time + 1;
    return nxt;
}

void RopeInfo :: updateRope(){ 
    int index = world -> tiger.index;
    dots[index].status = ROPEDOT_ALIVE;
    dots[index].die_time = -1;
    dots[index].r = world -> tiger.r;
    
    std::vector <RopeDot> vec;
    for (int i = 0; i < BLOCK_NUMBER; ++i) {
        if (i == index) continue;
        vec.push_back(NextDot(dots[(i + BLOCK_NUMBER - 1) % BLOCK_NUMBER], 
                              dots[i], 
                              dots[(i + 1) % BLOCK_NUMBER]));
    }
    for (int i = 0, j = 0; i < BLOCK_NUMBER; ++i) {
        if (i == index) continue;
        dots[i] = vec[j]; ++j;
        if (dots[i].r == 0 && dots[i].status == ROPEDOT_DEAD) { //消失Case 1: 没有牵引的线触地;
            dots[i].status = ROPEDOT_ZERO;
        }
    }

    int End = index;
    while (dots[End].status == ROPEDOT_ALIVE) End = (End + BLOCK_NUMBER - 1) % BLOCK_NUMBER;
    for (int i = (End + 1) % BLOCK_NUMBER; i != index; i = (i + 1) % BLOCK_NUMBER) {
        if (dots[i].r == 0) {//消失Case 2: 有牵引的线的接地端连续的0触地
            dots[i].status = ROPEDOT_ZERO; 
        }
        else {
            dots[(i - 1 + BLOCK_NUMBER) % BLOCK_NUMBER].status = ROPEDOT_ALIVE;
            break;
        }
    }

    getSegs();
    return;
}

bool InRange(int x, int l, int r) {
    if (l <= r) {
        return l <= x && x < r;
    }

    if (x >= l) return true;
    if (x < r) return true;

    return false;
}

void RopeInfo :: getSegs() {
    int index = world -> tiger.index;

    segments.clear();
    for (int z = 0; z < BLOCK_NUMBER; ++z) if (dots[z].status == ROPEDOT_ZERO) {
        if (dots[(z + 1) % BLOCK_NUMBER].status == ROPEDOT_ZERO) continue;
        for (int i = (z + 1) % BLOCK_NUMBER; ; i = (i + 1) % BLOCK_NUMBER) {
            if (dots[i].status == ROPEDOT_ZERO) {
                segments.push_back(std::make_pair((z + 1) % BLOCK_NUMBER, i));
                int sze = segments.size();
                bool isALIVE = InRange(index, (z + 1) % BLOCK_NUMBER, i);
                if (isALIVE) swap(segments[0], segments[sze - 1]);
                break;
            }
            dots[i].status = ROPEDOT_DEAD;
            dots[i].die_time = 0;
        }
    }

    for (int i = index; ; i = (i + BLOCK_NUMBER - 1) % BLOCK_NUMBER) {
        if (dots[i].status == ROPEDOT_ZERO) break;
        dots[i].status = ROPEDOT_ALIVE;
    }
}

//请在调用updateRope()后使用
void RopeInfo :: breakRope(int left, int right) { //请在调用updateRope()后使用
    for (int i = 0; i < BLOCK_NUMBER; ++i) 
        if (InRange(i, left, right)) {
            dots[i].r = 0;
            dots[i].status = ROPEDOT_ZERO;
        }
    
    getSegs();
}

//get the Rope's data
void RopeInfo :: getRopeData(std::vector<std::pair <int,int> >& seg, std::vector<std::pair<std::pair <double, double>, ROPEDOT_STATE > >& PAs) const {
    seg = segments;
    PAs.clear();
    for (int i = 0; i < BLOCK_NUMBER; ++i) 
        PAs.push_back(std::make_pair(std::make_pair(dots[i].sita, dots[i].r), dots[i].status));
    return;
}