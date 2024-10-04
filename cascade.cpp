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

// Forma transpuesta sin loop unrooling, el buffer se recorre n veces para las etapas
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

// Forma transpuesta con loop unrooling, el buffer se recorre 1 vez para las etapas, se trabaja muestra por muestra

void cascade::process(int nframes, const float *const __restrict in, float *const __restrict out)
{
    if (maxOrder == 3)
    {
        processThreeStages(nframes, in, out);
    }
    else
    {
        processTwoStages(nframes, in, out);
    }
}

void cascade::processThreeStages(int nframes, const float *const __restrict in, float *const __restrict out)
{
    float partialResults[3]; // Tamaño fijo para 3 etapas

    for (int i = 0; i < nframes; i++)
    {
        // Inicializar la entrada
        partialResults[0] = in[i];

        // Procesar etapas del filtro
        partialResults[1] = this->stages[0]->processOne(partialResults[0]);
        partialResults[2] = this->stages[1]->processOne(partialResults[1]);
        out[i] = this->stages[2]->processOne(partialResults[2]); // Almacena el resultado final directamente
    }
}

void cascade::processTwoStages(int nframes, const float *const __restrict in, float *const __restrict out)
{
    float partialResults[2]; // Tamaño fijo para 2 etapas

    for (int i = 0; i < nframes; i++)
    {
        // Inicializar la entrada
        partialResults[0] = in[i];

        // Procesar etapas del filtro
        partialResults[1] = this->stages[0]->processOne(partialResults[0]);
        out[i] = this->stages[1]->processOne(partialResults[1]); // Almacena el resultado final directamente
    }
}

