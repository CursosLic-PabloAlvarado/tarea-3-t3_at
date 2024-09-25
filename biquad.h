#ifndef BIQUAD_H
#define BIQUAD_H

#include <vector>
#include <cstring>

class biquad {
public:
    biquad(std::vector<std::vector<float>>& coefsIn);

    std::vector<float> numExtractor(int order);
    std::vector<float> denExtractor(int order);
    void getMaxOrder();
    std::vector<float> applyFilter(const std::vector<float>& input, const std::vector<float>& b, const std::vector<float>& a);
    void process(jack_nframes_t nframes, const float *const in, float *const out);

private:
    std::vector<std::vector<float>> coefs;
    int maxOrder;
};

#endif // BIQUAD_H
