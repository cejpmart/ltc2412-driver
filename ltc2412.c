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

#include "ltc2412.h"

enum { LTC2412_nEOC = 0x80 };
enum { LTC2412_nCH0_CH1 = 0x40 };
enum { LTC2412_SIG = 0x20 };
enum { LTC2412_MSB = 0x10 };
enum { LTC2412_MSBYTE_MASK = 0x1f };

int ltc2412_readout(ltc2412_readout_t* data_out) {
    // Kindly refer to https://www.analog.com/media/en/technical-documentation/data-sheets/2412fa.pdf

    uint8_t bytes[4];

    ltc2412_spi_receive(&bytes[0], sizeof(bytes));

    if ((bytes[0] & LTC2412_nEOC) == 0) {
        data_out->conversion_finished = 1;
    }
    else {
        data_out->conversion_finished = 0;
    }

    if ((bytes[0] & LTC2412_nCH0_CH1) == 0) {
        data_out->channel = 0;
    }
    else {
        data_out->channel = 1;
    }

    if ((bytes[0] & LTC2412_SIG) && (bytes[0] & LTC2412_MSB)) {
        data_out->overflow = 1;
        data_out->value_ext = LTC2412_MAX_READING;
    }
    else if ((bytes[0] & LTC2412_SIG) == 0 && (bytes[0] & LTC2412_MSB) == 0) {
        data_out->overflow = 1;
        data_out->value_ext = LTC2412_MIN_READING;
    }
    else {
        data_out->overflow = 0;

        data_out->value_ext = (bytes[0] & LTC2412_MSBYTE_MASK) << 24
                             | bytes[1] << 16
                             | bytes[2] << 8
                             | bytes[3];

        if (bytes[0] & LTC2412_MSB) {
            data_out->value_ext |= 0xe0000000;
        }
    }

    return 0;
}
