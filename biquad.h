#ifndef BIQUAD_H
#define BIQUAD_H

#include <vector>
#include <cstring>
#include "jack_client.h"

class biquad {
public:
    biquad(std::vector<std::vector<sample_t>>& coefsIn);

    std::vector<float> numExtractor(int order);
    std::vector<float> denExtractor(int order);
    void getMaxOrder();
    std::vector<float> applyFilter(const std::vector<sample_t>& input, const std::vector<sample_t>& b, const std::vector<sample_t>& a);
    void process(jack_nframes_t nframes, const float *const in, float *const out);

private:
    std::vector<std::vector<sample_t>> coefs;
    int maxOrder;
};

#endif // BIQUAD_H
