#ifndef CASCADE_H
#define CASCADE_H

#include <vector>
#include <cstring>
#include "jack_client.h"
#include <cstdlib>
#include <iostream>
#include "biquad.h"
#include <immintrin.h>


class cascade {
public:
    cascade(std::vector<std::vector<float>>& coefsIn);
    void process(int nframes, const float * __restrict in, float * __restrict out);
    __m256 subProcessVector(int stage, __m256 inputVec);

private:
    std::vector<biquad*> stages; 
    std::vector<std::vector<float>> coefs;
    int maxOrder;
};

#endif // CASCADE_H
