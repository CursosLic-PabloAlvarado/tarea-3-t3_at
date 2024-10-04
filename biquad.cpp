#include "biquad.h"

// Constructor para inicializar los coeficientes del filtro
biquad::biquad(std::vector<float> &coefsIn)
{
    this->coefs = coefsIn;
    /*
    for (int i = 0; i < this->coefs.size(); i++)
    {
        std::cout << "Coeficiente cargado: " << this->coefs[i] << "fila " << i << std::endl;
    }
    */

    // this->a.resize(3);
    // this->b.resize(3);

    this->denExtractor();
    this->numExtractor();

    // Variables para almacenar el estado anterior de cada biquad
    this->b0Vec = _mm_set1_ps(this->b0);
    this->b1Vec = _mm_set1_ps(this->b1);
    this->b2Vec = _mm_set1_ps(this->b2);
    this->a1Vec = _mm_set1_ps(this->a1);
    this->a2Vec = _mm_set1_ps(this->a2);

    this->w1_pastVec = _mm_set1_ps(0);
    this->w2_pastVec = _mm_set1_ps(0);

    this->w1_past_point = new float[4];
    this->w2_past_point = new float[4];

    this->x0 = 0.0;
    this->x1 = 0.0;
    this->x2 = 0.0;
    this->y1 = 0.0;
    this->y2 = 0.0;
    this->w1_past = 0.0;
    this->w2_past = 0.0;
}

// Extraer los coeficientes del numerador de una etapa específica
void biquad::numExtractor()
{
    /*
    for (int i = 0; i < 3; i++)
    {
        this->b[i] = this->coefs[i];
    }
    */
    b0 = this->coefs[0];
    b1 = this->coefs[1];
    b2 = this->coefs[2];
}

// Extraer los coeficientes del denominador de una etapa específica
void biquad::denExtractor()
{
    /*
    for (int i = 0; i < 3; i++)
    {
        this->a[i] = this->coefs[i + 3]; // Ajuste de índice
    }
    */
    a0 = this->coefs[3];
    a1 = this->coefs[4];
    a2 = this->coefs[5];
}

void biquad::applyFilter(const float *const input, float *const output, int nFrames)
{

    for (size_t n = 0; n < nFrames; ++n)
    {

        // Forma directa
        /*
        output[n] = this->b[0] * input[n] + this->b[1] * x1 + this->b[2] * x2 - this->a[1] * y1 - this->a[2] * y2;
        this->x1 = input[n];
        this->x2 = this->x1;
        this->y1 = output[n];
        this->y2 = this->y1;
        */

        // Forma transpuesta

        output[n] = this->b0 * input[n] + this->w1_past;
        this->w1_past = this->b1 * input[n] - this->a1 * output[n] + this->w2_past;
        this->w2_past = this->b2 * input[n] - this->a2 * output[n];
    }
}

// Procesar la señal de entrada en bloques
void biquad::process(int nframes, const float *const in, float *const out)
{

    this->applyFilter(in, out, nframes);
}


__m128 biquad::processVectorial(__m128 __restrict vectorIn)
{
    __m128 outputVec;
    int j;
    if (this->firstTime)
    {

        float * inputArray = new float[4];
        float * outputArray = new float[4];

        _mm_storeu_ps(&inputArray[0], vectorIn);

        for (int i = 0; i < 4; i++)
        {
            outputArray[i] = this->b0 * inputArray[i] + this->w1_past_point[i];
            
            if (i+1 == 4){
                j = 0;
            }else{
                j = i+1;
            }

            this->w1_past_point[j] = this->b1 * inputArray[i] - this->a1 * outputArray[i] + this->w2_past_point[i];
            this->w2_past_point[j] = this->b2 * inputArray[i] - this->a2 * outputArray[i];
            
        }

        outputVec = _mm_loadu_ps(&outputArray[0]);

        this->w1_pastVec = _mm_loadu_ps(&w1_past_point[0]);
        this->w2_pastVec = _mm_loadu_ps(&w2_past_point[0]);


    }



    return outputVec; // Retornar el vector de salida
}