#ifndef BIQUAD_H
#define BIQUAD_H

#include <vector>
#include <cstring>
#include "jack_client.h"
#include <cstdlib>
#include <iostream>


class biquad {
public:
    biquad(){}
    biquad(std::vector<float>& coefsIn);
    
    void numExtractor();
    void denExtractor();
    void getMaxOrder();
    void applyFilter(const float *const input, float *const output, int nFrames);
    void process(int nframes, const float *const in, float *const out);
    float processOne(float input);
    //void processEight(float * __restrict in, float * __restrict out);

    float a0, a1, a2;
    float b0, b1, b2;

    float x0;
    float x1;
    float x2;
    float y1;
    float y2;
    float w2_past;
    float w1_past;

    __m256 w1_pastVec;
    __m256 w2_pastVec 

private:
    std::vector<float> coefs;
    int maxOrder;
    std::vector<float> b;
    std::vector<float> a;
};

#endif // BIQUAD_H
