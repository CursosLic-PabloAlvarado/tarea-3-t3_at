#include "cascade.h"

cascade::cascade(std::vector<std::vector<float>> &coefsIn)
{
    this->maxOrder = coefsIn.size();

    // std::cout << "Orden del filtro" << this->maxOrder << std::endl;
    this->stages.resize(this->maxOrder);
    this->coefs = coefsIn;

    for (int i = 0; i < this->maxOrder; i++)
    {
        // std::cout << "Etapa " << i << std::endl;
        this->stages[i] = new biquad(this->coefs[i]);
    }
}


//Forma transpuesta sin loop unrooling, el buffer se recorre n veces para las etapas
/*  
void cascade::process(int nframes, const float *const in, float *const out)
{

    // Procesar encadenando muestra por muestra

    std::vector<float *> partialResults(this->maxOrder + 1);

    partialResults[0] = new float[nframes];
    partialResults[1] = new float[nframes];


    if (this->maxOrder == 3)
    {

        this->stages[0]->process(nframes, in, partialResults[0]);
        this->stages[1]->process(nframes, partialResults[0], partialResults[1]);
        this->stages[2]->process(nframes, partialResults[1],out);
    }
    else
    {
        this->stages[0]->process(nframes, in, partialResults[0]);
        this->stages[1]->process(nframes, partialResults[0], out);
    }
}

*/

//Forma transpuesta con loop unrooling, el buffer se recorre 1 vez para las etapas, se trabaja muestra por muestra
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



__m128 cascade::subProcessVector(int stage, __m128 inputVec)
{
    /*
    // Variables para almacenar el estado anterior de cada biquad
    __m128 b0Vec = _mm_set1_ps(this->stages[stage]->b0);
    __m128 b1Vec = _mm_set1_ps(this->stages[stage]->b1);
    __m128 b2Vec = _mm_set1_ps(this->stages[stage]->b2);
    __m128 a1Vec = _mm_set1_ps(this->stages[stage]->a1);
    __m128 a2Vec = _mm_set1_ps(this->stages[stage]->a2);

    // Estado del filtro (w1 y w2)
    __m128 w1_pastVec = this->stages[stage]->w1_pastVec;
    __m128 w2_pastVec = this->stages[stage]->w2_pastVec;

    // Aplicar el filtro biquad transpuesto para 8 muestras
    __m128 outputVec = _mm_add_ps(_mm_mul_ps(b0Vec, inputVec), w1_pastVec);
    w1_pastVec = _mm_add_ps(_mm_sub_ps(_mm_mul_ps(b1Vec, inputVec), _mm_mul_ps(a1Vec, outputVec)), w2_pastVec);
    w2_pastVec = _mm_sub_ps(_mm_mul_ps(b2Vec, inputVec), _mm_mul_ps(a2Vec, outputVec));

    // Guardar el estado actualizado
    this->stages[stage]->w1_pastVec = w1_pastVec; // Usar la primera muestra de w1_pastVec
    this->stages[stage]->w2_pastVec = w2_pastVec; // Usar la primera muestra de w2_pastVec

    */

    __m128 outputVec = this->stages[stage]->processVectorial(inputVec);

    return outputVec;
}

//Forma transpuesta con loop unrooling, procesamiento vectorial, el buffer se recorre 1 vez para las etapas, se trabaja en 4 muestras con shifts

void cascade::process(int nframes, const float *__restrict in, float *__restrict out)
{
    const int simdWidth = 4;

    int i = 0;

    __m128 resultVec1;
    __m128 resultVec2;
    __m128 resultVec3;


    for (; i <= nframes - simdWidth; i += simdWidth)
    {
        // Cargar 8 muestras de entrada
        __m128 inputVec = _mm_loadu_ps(&in[i]);

        if (this->maxOrder == 3)
        {
            resultVec1 = this->subProcessVector(0, inputVec);
            resultVec2 = this->subProcessVector(1, resultVec1);
            resultVec3 = this->subProcessVector(2, resultVec2);
        }
        else
        {
            resultVec1 = this->subProcessVector(0, inputVec);
            resultVec2 = this->subProcessVector(1, resultVec1);
            resultVec3 = resultVec2;
        }

        // Guardar el resultado procesado
        _mm_storeu_ps(&out[i], resultVec3);
    }
}
