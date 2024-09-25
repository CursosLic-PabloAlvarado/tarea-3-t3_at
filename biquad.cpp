#include "biquad.h"

// Constructor para inicializar los coeficientes del filtro
biquad::biquad(std::vector<std::vector<float>>& coefsIn) {
    this->coefs = coefsIn;

    for (int i=0; i<this->coefs.size(); i++){
        for (int j=0; j<this->coefs[i].size(); j++){
            std::cout << "Coeficiente cargado: " << this->coefs[i][j] << std::endl;
        }
    }
    // Obtener el número de etapas del filtro
    this->getMaxOrder();
    std::cout << "Orden del filtro cargado: " << this->maxOrder << std::endl;

}

// Extraer los coeficientes del numerador de una etapa específica
std::vector<float> biquad::numExtractor(int order) {
    std::vector<float> b(3);  // Inicializar el vector con tamaño 3
    for (int i = 0; i < 3; i++) {
        b[i] = this->coefs[order][i];
    }
    return b;
}

// Extraer los coeficientes del denominador de una etapa específica
std::vector<float> biquad::denExtractor(int order) {
    std::vector<float> a(3);  // Inicializar el vector con tamaño 3
    for (int i = 0; i < 3; i++) {
        a[i] = this->coefs[order][i+3];  // Ajuste de índice
    }
    return a;
}

// Obtener el máximo número de etapas del filtro (el orden del filtro)
void biquad::getMaxOrder() {
    this->maxOrder = this->coefs.size();  // Número de etapas es el tamaño del vector de coeficientes
}

std::vector<float> biquad::applyFilter(const std::vector<float>& input, const std::vector<float>& b, const std::vector<float>& a) {
    size_t N = input.size();
    size_t M = b.size();  // Tamaño de los coeficientes del numerador
    std::vector<float> output(N, 0);


    for (size_t n = 0; n < N; ++n) {
        // Convolución del numerador
        for (size_t i = 0; i < M; ++i) {
            if (n >= i) {
                output[n] += b[i] * input[n - i];
            }
        }

        // Convolución del denominador, omitiendo a[0]
        for (size_t j = 1; j < M; ++j) {  // Empieza desde a[1]
            if (n >= j) {
                output[n] -= a[j] * output[n - j];
            }
        }
    }

    return output;
}


// Procesar la señal de entrada en bloques
void biquad::process(int nframes, const float *const in, std::vector<float>& out) {
    std::vector<float> temp(nframes);

    // Copiar la señal de entrada a un vector temporal
    for (int i = 0; i < nframes; ++i) {
        temp[i] = in[i];
    }

    // Aplicar el filtro por cada etapa
    for (int i = 0; i < this->maxOrder; i++) {
        temp = this->applyFilter(temp, this->numExtractor(i), this->denExtractor(i));
    }

    for (int i = 0; i < nframes; i++) {
        out[i] = temp[i];
    }

    
}
