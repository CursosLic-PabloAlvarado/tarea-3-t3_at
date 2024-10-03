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
    /**
    * Hace las llamadas a funciones del biquad para procesar un vector de los registros de la etapa dada, según el vector de entrada.
    */
    __m128 subProcessVector(int stage, __m128 inputVec);

private:
    std::vector<biquad*> stages; 
    std::vector<std::vector<float>> coefs;
    int maxOrder;
};

#endif // CASCADE_H
