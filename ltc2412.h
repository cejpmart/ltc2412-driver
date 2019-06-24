/*
eForce FSE Accumulator Management System

Written by Martin Cejp

Copyright (c) 2019 eForce FEE Prague Formula

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef LTC2412_H
#define LTC2412_H

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    unsigned int conversion_finished : 1;   // other values are only meaningful if this is nonzero
    unsigned int channel : 1;
    unsigned int overflow : 1;              // input voltage overflow; value saturated to LTC2412_{MIN/MAX}_READING

    int value_ext;                          // 29-bit sign-extended result
} ltc2412_readout_t;

enum { LTC2412_MAX_READING =  0x0fffffff }; // corresponds to 0.5 Vref - 1 LSB
enum { LTC2412_MIN_READING = -0x10000000 }; // corresponds to -0.5 Vref

// SPI callback -- provide this function
void ltc2412_spi_receive(uint8_t* buffer, size_t num_bytes);

/**
 * @param data_out
 * @return 0 on success, otherwise propagated error value
 */
int ltc2412_readout(ltc2412_readout_t* data_out);

#ifdef __cplusplus
}
#endif

#endif
