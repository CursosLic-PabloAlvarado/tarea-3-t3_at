#ifndef BIQUAD_H
#define BIQUAD_H

#include <vector>
#include <cstring>
#include "jack_client.h"
#include <cstdlib>
#include <iostream>
#include <immintrin.h>

class biquad
{
public:
    /**
     * Constructor por defecto
     */
    biquad() {}
    /**
     * Constructor utilizado, recibe un vector de coeficientes de la matriz de la etapa bi cuadrática.
     */
    biquad(std::vector<float> &coefsIn);
    /**
     * Asigna a las variables b los coeficientes del numerador.
     */
    void numExtractor();
    /**
     * Asigna a las variables a los coeficientes del denominador.
     */
    void denExtractor();

    /**
     * Función interna auxiliar de process para aplicar el filtro según los datos recibidos. Actúa secuencialmente con n buffers.
     */
    void applyFilter(const float *const input, float *const output, int nFrames);
    /**
     * Función para procesar nframes cantidad de muestras de la variable in hacia la variable out según el filtro cargado.
     */
    void process(int nframes, const float *const in, float *const out);
    /**
     * Función auxiliar de process para procesar una sola muestra
     */
    inline __attribute__((always_inline)) float processOne(const float input)
    {

        // Calcular la salida
        const float output = this->b0 *  input + this->w1_past;

        // Actualizar los estados pasados
        this->w1_past = this->b1 * input - this->a1 * output + this->w2_past;
        this->w2_past = this->b2 * input - this->a2 * output;

        return output;
    }
    /**
     * Función auxiliar de process para procesar vectores de muestras haciendo uso de registros
     */
    __m128 processVectorial(__m128 __restrict vectorIn);

    bool firstTime;

    float a0, a1, a2;
    float b0, b1, b2;

    float x0;
    float x1;
    float x2;
    float y1;
    float y2;
    float w2_past;
    float w1_past;

    float *w2_past_point;
    float *w1_past_point;

    __m128 b0Vec;
    __m128 b1Vec;
    __m128 b2Vec;
    __m128 a1Vec;
    __m128 a2Vec;

    __m128 w1_pastVec;
    __m128 w2_pastVec;

private:
    std::vector<float> coefs;
    std::vector<float> b;
    std::vector<float> a;
};

#endif // BIQUAD_H
