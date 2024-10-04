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
    /**
    * Constructor de la clase que recibe un vector bidimensional con los coeficientes de las múltiples etapas del filtro.
    */
    cascade(std::vector<std::vector<float>>& coefsIn);
    /**
    * Procesa las n etapas del sistema, considerando nframes de entrada y salida.
    */
    void process(int nframes, const float * __restrict in, float * __restrict out);
    void processThreeStages(int nframes, const float *const __restrict in, float *const __restrict out);
    void processTwoStages(int nframes, const float *const __restrict in, float *const __restrict out);

private:
    std::vector<biquad*> stages; 
    std::vector<std::vector<float>> coefs;
    int maxOrder;
};

#endif // CASCADE_H
