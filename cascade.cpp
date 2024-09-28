#include "cascade.h"

cascade::cascade(std::vector<std::vector<float>>& coefsIn){

    this->coefs = coefsIn;
    for (int i=0; i <this->coefs.size(); i++){
        this->stages = new biquad(coefsIn[i]);
    }

    this->maxOrder = coefsIn.size();
}


void cascade::process(int nframes, const float *const in, float * const out){

    float* temp = new float[nframes];
    float* tempOut = new float[nframes];

    memcpy(temp, in, sizeof(float)*nframes);
    //Procesar en cascada-serie
    for (i=0; i<this->maxOrder; i++){
        this->stages[i]->process(nframes, temp, tempOut);
        memcpy(temp, tempOut, sizeof(float)*nframes);
    }
    memcpy(out, temp, sizeof(float)*nframes);

    //Procesar encadenando muestra por muestra
    /*  
    std::vector<float> partialResults(this->maxOrder+1);
    partialResults[0] = in[i];
    for (int i=0; i<nframes ; i++){
        for (int j=0; j<this->maxOrder; j++){
            this->stages[i]->processOne(partialResults[j], partialResults[j+1]);
        }
        output[i] = partialResults[this->maxOrder];
    }
    */
    

}