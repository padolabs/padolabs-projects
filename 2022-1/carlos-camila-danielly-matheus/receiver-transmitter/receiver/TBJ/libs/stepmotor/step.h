/*
 * step.h
 *
 *  Created on: Apr 8, 2022
 *      Author: gabrielly
 */

#ifndef STEPMOTOR_STEP_H_
#define STEPMOTOR_STEP_H_

#include "stm32g0xx.h"

#include <stdint.h>
#include <stddef.h>

/*
 * Enumerate
 */
typedef enum{
	STEP_DIRECTION_NONE,
	STEP_DIRECTION_ACW,
	STEP_DIRECTION_CW
}step_direction_e ;


/*
 * Structs and Unions
 */

typedef struct{
	GPIO_TypeDef *gpio;
	uint32_t pin;
}_gpios_t;

typedef struct{
	struct{
		float AnglePerStep;
	}cfg;
	struct{
		_gpios_t Yellow;
		_gpios_t Blue;
		_gpios_t Orange;
		_gpios_t Pink;
	}pins;
	uint32_t steps;
	uint16_t delay;
	uint8_t stepN;
	step_direction_e direction;
}step_motor_t;


// Prototypes
void step_init(step_motor_t *obj);

void step_drive_steps(step_motor_t *obj, uint32_t step ,step_direction_e dir, uint16_t delay);



#endif /* STEP_MOTOR_STEP_H_ */


