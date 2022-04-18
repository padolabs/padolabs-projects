/*
 * encoder.h
 *
 *  Created on: Apr 1, 2022
 *      Author: carlos
 */

#ifndef ENCODER_ENCODER_H_
#define ENCODER_ENCODER_H_

/*
 * includes
 */

#include <stdint.h>
#include "stm32g0xx.h"
#include "main.h"

/*
 * Structs and unions
 */

typedef struct{
	TIM_HandleTypeDef *tim;
	uint32_t channel;
	uint32_t counter;
}encoder_t;

/*
 * function prototypes
 */

void encoder_init(encoder_t *obj, TIM_HandleTypeDef *timer, uint32_t channel);

uint32_t encoder_get_value(encoder_t *obj);



#endif /* ENCODER_ENCODER_H_ */

