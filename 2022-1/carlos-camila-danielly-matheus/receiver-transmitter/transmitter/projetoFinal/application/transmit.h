/*
 * transmit.h
 *
 *  Created on: 29 de mar de 2022
 *      Author: carlos
 */

#ifndef TRANSMIT_H_
#define TRANSMIT_H_

/*
 * Includes
 */

#include <stdint.h>
#include <string.h>
#include "stm32g0xx.h"
#include "nrf24.h"
#include "tim.h"
#include "encoder.h"
#include "liquidcrystal_i2c.h"
/*
 * function prototypes
 */

void transmit_init();

void encoder_run();

void transmit_run();


#endif /* TRANSMIT_H_ */
