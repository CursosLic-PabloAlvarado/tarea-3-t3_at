#include "cascade.h"

cascade::cascade(std::vector<std::vector<float>> &coefsIn)
{
    this->maxOrder = coefsIn.size();

    std::cout << "Orden del filtro" << this->maxOrder << std::endl;
    this->stages.resize(this->maxOrder);
    this->coefs = coefsIn;

    for (int i = 0; i < this->maxOrder; i++)
    {
        std::cout << "Etapa " << i << std::endl;
        this->stages[i] = new biquad(this->coefs[i]);
    }
}

/*
void cascade::process(int nframes, const float *const in, float * const out){


    //Procesar encadenando muestra por muestra

    std::vector<float> partialResults(this->maxOrder+1);

    for (int i=0; i<nframes ; i++){
        partialResults[0] = in[i];

        if (this->maxOrder == 3){
            partialResults[1] = this->stages[0]->processOne(partialResults[0]);
            partialResults[2] = this->stages[1]->processOne(partialResults[1]);
            partialResults[3] = this->stages[2]->processOne(partialResults[2]);
        }else{
            partialResults[1] = this->stages[0]->processOne(partialResults[0]);
            partialResults[2] = this->stages[1]->processOne(partialResults[1]);
        }


        out[i] = partialResults[this->maxOrder];
    }


}

*/
void cascade::process(int nframes, const float * __restrict in, float * __restrict out)
{
    const int simdWidth = 8; // Número de floats que AVX puede procesar en paralelo

    int i = 0;
    
    // Procesamiento con AVX para bloques de 8 muestras
    for (; i <= nframes - simdWidth; i += simdWidth)
    {
        // Cargar 8 muestras de entrada
        __m256 inputVec = _mm256_loadu_ps(&in[i]);

        // Inicializamos el vector de resultados parciales
        __m256 resultVec = inputVec;

        // Procesar cada etapa biquad
        for (int j = 0; j < this->maxOrder; ++j)
        {
            // Variables para almacenar el estado anterior de cada biquad
            __m256 b0Vec = _mm256_set1_ps(this->stages[j]->b0);
            __m256 b1Vec = _mm256_set1_ps(this->stages[j]->b1);
            __m256 b2Vec = _mm256_set1_ps(this->stages[j]->b2);
            __m256 a1Vec = _mm256_set1_ps(this->stages[j]->a1);
            __m256 a2Vec = _mm256_set1_ps(this->stages[j]->a2);

            // Estado del filtro (w1 y w2)
            __m256 w1_pastVec = this->stages[j]->w1_pastVec;
            __m256 w2_pastVec = this->stages[j]->w2_pastVec;

            // Aplicar el filtro biquad transpuesto para 8 muestras
            __m256 outputVec = _mm256_add_ps(_mm256_mul_ps(b0Vec, resultVec), w1_pastVec);
            w1_pastVec = _mm256_add_ps(_mm256_sub_ps(_mm256_mul_ps(b1Vec, resultVec), _mm256_mul_ps(a1Vec, outputVec)), w2_pastVec);
            w2_pastVec = _mm256_sub_ps(_mm256_mul_ps(b2Vec, resultVec), _mm256_mul_ps(a2Vec, outputVec));

            // Guardar el estado actualizado
            this->stages[j]->w1_pastVec = w1_pastVec; // Usar la primera muestra de w1_pastVec
            this->stages[j]->w2_pastVec = w2_pastVec; // Usar la primera muestra de w2_pastVec

            // Actualizamos el vector de resultados parciales
            resultVec = outputVec;
        }

        // Guardar el resultado procesado
        _mm256_storeu_ps(&out[i], resultVec);
    }
}
