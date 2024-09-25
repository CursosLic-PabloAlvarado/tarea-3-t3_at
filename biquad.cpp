#include "biquad.h"

// Constructor para inicializar los coeficientes del filtro
biquad::biquad(std::vector<std::vector<sample_t>>& coefsIn) {
    this->coefs = coefsIn;
}

// Extraer los coeficientes del numerador de una etapa específica
std::vector<sample_t> biquad::numExtractor(int order) {
    std::vector<sample_t> a(3);  // Inicializar el vector con tamaño 3
    for (int i = 0; i < 3; i++) {
        a[i] = this->coefs[order][i];
    }
    return a;
}

// Extraer los coeficientes del denominador de una etapa específica
std::vector<sample_t> biquad::denExtractor(int order) {
    std::vector<sample_t> b(3);  // Inicializar el vector con tamaño 3
    for (int i = 3; i < 6; i++) {
        b[i] = this->coefs[order][i];  // Ajuste de índice
    }
    return b;
}

// Obtener el máximo número de etapas del filtro (el orden del filtro)
void biquad::getMaxOrder() {
    this->maxOrder = this->coefs.size();  // Número de etapas es el tamaño del vector de coeficientes
}

// Aplicar el filtro a una señal de entrada utilizando los coeficientes de numerador y denominador
std::vector<sample_t> biquad::applyFilter(const std::vector<sample_t>& input, const std::vector<sample_t>& b, const std::vector<sample_t>& a) {
    size_t N = input.size();
    size_t M = 3;  // Tamaño de los coeficientes del numerador
    size_t L = 3;  // Tamaño de los coeficientes del denominador
    std::vector<sample_t> output(N, 0);

    for (size_t n = 0; n < N; ++n) {
        // Convolución del numerador
        for (size_t i = 0; i < M; ++i) {
            if (n >= i) {
                output[n] += b[i] * input[n - i];
                output[n] -= a[j] * output[n - j];
            }
        }
    }

    return output;
}

// Procesar la señal de entrada en bloques
void biquad::process(jack_nframes_t nframes, const float *const in, float *const out) {
    std::vector<float> temp(nframes);

    // Copiar la señal de entrada a un vector temporal
    for (jack_nframes_t i = 0; i < nframes; ++i) {
        temp[i] = in[i];
    }

    // Obtener el número de etapas del filtro
    this->getMaxOrder();

    // Aplicar el filtro por cada etapa
    for (int i = 0; i < this->maxOrder; i++) {
        temp = this->applyFilter(temp, this->numExtractor(i), this->denExtractor(i));
    }

    // Copiar la señal procesada a la salida
    memcpy(out, temp.data(), sizeof(float) * nframes);
}
