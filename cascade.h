#ifndef CASCADE_H
#define CASCADE_H

#include <vector>
#include <cstring>
#include "jack_client.h"
#include <cstdlib>
#include <iostream>
#include "biquad.h"

class cascade {
public:
    cascade(std::vector<std::vector<float>>& coefsIn);
    void process(int nframes, const float *const in, float *const out);

private:
    std::vector<biquad*> stages(3); 
    std::vector<std::vector<float>> coefs;
    int maxOrder;
};

#endif // CASCADE_H
