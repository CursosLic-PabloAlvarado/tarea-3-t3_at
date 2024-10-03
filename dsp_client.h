/**
 * passthrough_client.h
 *
 * Copyright (C) 2023-2024  Pablo Alvarado
 * EL5805 Procesamiento Digital de Señales
 * Escuela de Ingeniería Electrónica
 * Tecnológico de Costa Rica
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the authors nor the names of its contributors may be
 *    used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _DSP_CLIENT_H
#define _DSP_CLIENT_H

#include "jack_client.h"
#include "cascade.h"
#include <vector>
#include <cstring>
#include <cmath>

/**
 * Jack client class
 *
 * This class wraps some basic jack functionality.
 */
class dsp_client : public jack::client
{

public:
    // typedef jack::client::sample_t sample_t;

    /**
     * The default constructor performs some basic connections.
     */
    dsp_client();
    ~dsp_client();

    /**
    * Incrementa en una unidad el volumen de la salida.
    * 
    */
    void incrementVolume();
    /**
    * Disminuye en una unidad el volumen de la salida.
    * 
    */
    void decreaseVolume();
    /**
    * Aplica una normalización exponencial al volumen para notar más las subidas en volúmenes altos.
    * 
    */
    void configureVolume();
    /**
    * Establece los objetos cascade (el default y el del usuario si pone coeficientes) 
    * Recibe un vector bidimensional de vectores.
    */
    void configureFilter(std::vector<std::vector<sample_t>>& coefs);
    /**
    * Coloca el modo passthrough, reflejando directamente la entrada en la salida.
    * 
    */
    void deactivateModes();
    /**
    * Activa el modo con el filtro por defecto.
    * 
    */
    void activateFilterDefault();
    /**
    * Activa el modo con el filtro establecido por el usuario.
    * 
    */
    void activateFilter();

    bool filterOn;
    bool filterDefOn;
    float volume;
    float volumeMultiplier;
    float k_exp;
    cascade* filterAllStages;
    cascade* filterDefault;


    /**
     * Función que refleja a jack el procesamiento realizado a un buffer de nframes para la entrada y la salida.
     */
    virtual bool process(jack_nframes_t nframes,
                         const sample_t *const in,
                         sample_t *const out) override;
};

#endif
