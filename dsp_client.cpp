/**
 * passthrough_client.cpp
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

#include "dsp_client.h"



dsp_client::dsp_client() : jack::client()
{
    this->volume = 1.0f;
    this->volumeMultiplier = 5.0f;
    this->filterOn = false;
    this->k_exp = 0.5f;
    this->temp.resize(1024);
}

dsp_client::~dsp_client()
{
}

void dsp_client::configureVolume()
{
    this->volume = 7 * ((exp(k_exp * this->volumeMultiplier) - exp(k_exp)) / (exp(10 * k_exp - exp(k_exp))));
}

// Aumenta el exponente para aumentar el volumen
void dsp_client::incrementVolume()
{
    if (this->volumeMultiplier < 10.0f)
    {
        this->volumeMultiplier = this->volumeMultiplier + 1.0f;
        this->configureVolume();
    }
}

// Disminuye el exponente para disminuir el volumen
void dsp_client::decreaseVolume()
{
    if (this->volumeMultiplier > 0.0f)
    {
        this->volumeMultiplier = this->volumeMultiplier - 1.0f;
        this->configureVolume();
    }
}

void dsp_client::configureFilter(std::vector<std::vector<sample_t>> &coefs)
{
    this->configureVolume();
    this->filterBiquad = new biquad(coefs);
}

void dsp_client::activateFilter()
{

    filterOn = true;
}

void dsp_client::activateFilterDefault()
{

    filterOn = true;
}

void dsp_client::deactivateModes()
{
    filterOn = false;
}

/**
 * The process callback for this JACK application is called in a
 * special realtime thread once for each audio cycle.
 *
 * This client does nothing more than copy data from its input
 * port to its output port. It will exit when stopped by
 * the user (e.g. using Ctrl-C on a unix-ish operating system)
 */
bool dsp_client::process(jack_nframes_t nframes,
                         const sample_t *const in,
                         sample_t *const out)
{


    if (filterOn)
    {

        this->filterBiquad->process(nframes, in, this->temp);

        for (jack_nframes_t i = 0; i < nframes; i++)
        {

            out[i] = this->temp[i] * this->volume;
        }
    }else{
        for (jack_nframes_t i = 0; i < nframes; i++)
        {

            out[i] = in[i] * this->volume;
        }
    }

    return true;
}
