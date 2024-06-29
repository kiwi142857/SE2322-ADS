#pragma once
#include <cmath>

namespace HNSWLab {
    int M = 20; // 30
    int M_max = 30; //30
    int ef_construction = 100;  //100
    int ef_search = 100;  //100
    double mult_ = 1 / log(1.0 * M);
    bool isMultiThread = true;
}