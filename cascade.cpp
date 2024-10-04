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

void cascade::process(int nframes, const float *const in, float *const out) {
    // Procesar encadenando muestra por muestra
    float partialResults[4]; // Tamaño fijo para maxOrder = 3

    for (int i = 0; i < nframes; i++) {
        // Inicializar la entrada
        partialResults[0] = in[i];

        // Procesar etapas del filtro
        if (this->maxOrder == 3) {
            partialResults[1] = this->stages[0]->processOne(partialResults[0]);
            partialResults[2] = this->stages[1]->processOne(partialResults[1]);
            out[i] = this->stages[2]->processOne(partialResults[2]); // Almacena el resultado final directamente
        } else {
            partialResults[1] = this->stages[0]->processOne(partialResults[0]);
            out[i] = this->stages[1]->processOne(partialResults[1]); // Almacena el resultado final directamente
        }
    }
}





__m128 cascade::subProcessVector(int stage, __m128 inputVec)
{


    __m128 outputVec = this->stages[stage]->processVectorial(inputVec);

    return outputVec;
}

//Forma transpuesta con loop unrooling, procesamiento vectorial, el buffer se recorre 1 vez para las etapas, se trabaja en 4 muestras con shifts


/*  

void cascade::process(int nframes, const float *__restrict in, float *__restrict out)
{
    const int simdWidth = 4;

    int i = 0;

    __m128 resultVec1;
    __m128 resultVec2;
    __m128 resultVec3;

    // Procesar bloques de 4 muestras usando SIMD
    for (; i <= nframes - simdWidth; i += simdWidth)
    {
        // Cargar 4 muestras de entrada
        __m128 inputVec = _mm_loadu_ps(&in[i]);

        // Aplicar los filtros en cascada
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
            resultVec3 = resultVec2;  // No se aplica el tercer filtro
        }

        // Guardar el resultado procesado
        _mm_storeu_ps(&out[i], resultVec3);
    }
}
*/
