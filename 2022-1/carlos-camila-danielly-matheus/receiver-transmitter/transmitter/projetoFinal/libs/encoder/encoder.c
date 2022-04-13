/*
 * encoder.c
 *
 *  Created on: Apr 1, 2022
 *      Author: carlos
 */

/*
 * includes
 */

#include "encoder.h"

/*
 * Privates
 */



/*
 * Publics
 */

void encoder_init(encoder_t *obj, TIM_HandleTypeDef *timer, uint32_t channel){
	if (obj != NULL){
		obj->tim = timer;
		obj->counter = 0;
		obj->channel = channel;
		HAL_TIM_Encoder_Start_IT(obj->tim, obj->channel);
		__HAL_TIM_SET_COUNTER(obj->tim, 0);
	}
}

uint32_t encoder_get_value(encoder_t *obj){
	uint32_t v;
	if (obj != NULL){
		v = __HAL_TIM_GET_COUNTER(obj->tim);
		if(v > 100){
			__HAL_TIM_SET_COUNTER(obj -> tim,100);
		}
		obj->counter = v;
		return v;
	}
}

/*
 * Externs
 */
