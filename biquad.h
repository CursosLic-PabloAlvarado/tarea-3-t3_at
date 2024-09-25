#ifndef BIQUAD_H
#define BIQUAD_H

#include <vector>
#include <cstring>
#include "jack_client.h"

class biquad {
public:
    biquad(std::vector<std::vector<float>>& coefsIn);

    std::vector<float> numExtractor(int order);
    std::vector<float> denExtractor(int order);
    void getMaxOrder();
    std::vector<float> applyFilter(const std::vector<float>& input, const std::vector<float>& b, const std::vector<float>& a);
    void process(int nframes, const float *const in,std::vector<float> out);

private:
    std::vector<std::vector<float>> coefs;
    int maxOrder;
};

#endif // BIQUAD_H
