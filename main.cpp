/**
 * Copyright (c) 2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <stdio.h>


//#include "mbed_mem_trace.h"

// Application helpers
#include "./Dust/hpma115/hpma115.h"
#include "LoraWAN.h"

using namespace sixtron;

#define MAX_BUFFER_SIZE 32
#define BAUD_RATE 9600
#define BUFFER_SIZE 32

/**
 * Dummy pin for dummy sensor
 */
#define PC_9                            0

// Max payload size can be LORAMAC_PHY_MAXPAYLOAD.
// This example only communicates with much shorter messages (<30 bytes).
// If longer messages are used, these buffers must be changed accordingly.


/**
 * Dust sensor class object
 */
HPMA115 sensor(P1_UART_TX,P1_UART_RX);

/**
 * LoRa class object
 */
LoraWAN lora;

static void callbalck_sensor_dust(uint8_t* buffer_to_transmit){
    hpma115_data_t data;
    sensor.read_measurement(&data);
    size_t len_buffer = sprintf((char*)buffer_to_transmit, "{\"Pm 2.5\":%d,\"Pm 10\":%d, \"Pm 1;0\":%d, \"Pm 4;0\":%d }", data.pm2_5, data.pm10, data.pm1_0, data.pm4_0);
}

/**
 * Entry point for application
 */
int main(void)
{
    DigitalOut pin(LORA_ANTSW_PWR,1);

    uint8_t coef;
    HPMA115::ErrorType err;

    err = sensor.stop_measurement();
    assert(err == HPMA115::ErrorType::Ok);

    err = sensor.stop_autosend();
    assert(err == HPMA115::ErrorType::Ok);

    err = sensor.set_adjust_coef(200);
    assert(err == HPMA115::ErrorType::Ok);

    err = sensor.read_adjust_coef(&coef);
    assert(err == HPMA115::ErrorType::Ok);
    assert(coef == 200);

    err = sensor.set_adjust_coef(100);
    assert(err == HPMA115::ErrorType::Ok);

    err = sensor.start_measurement();
    assert(err == HPMA115::ErrorType::Ok);

    // setup tracing
    setup_trace();

    //print_memory_info()

    lora.init_callback(callbalck_sensor_dust);
    lora. init_lora();
    lora.connect_lora();
    
    return 0;
}
// EOF
