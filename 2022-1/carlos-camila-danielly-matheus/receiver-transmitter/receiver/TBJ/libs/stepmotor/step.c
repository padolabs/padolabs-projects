/*
 * step.c
 *
 *  Created on: Apr 8, 2022
 *      Author: gabrielly
 */

#include "step.h"


// Privates

uint8_t Steps4[4][4] = {{1, 0, 0, 0},
						{0, 1, 0, 0},
						{0, 0, 1, 0},
						{0, 0, 0, 1}};

void __step_turnoff(step_motor_t *obj){
	HAL_GPIO_WritePin(obj->pins.Orange.gpio, obj->pins.Orange.pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(obj->pins.Yellow.gpio, obj->pins.Yellow.pin,  GPIO_PIN_RESET);
	HAL_GPIO_WritePin(obj->pins.Pink.gpio, obj->pins.Pink.pin,  GPIO_PIN_RESET);
	HAL_GPIO_WritePin(obj->pins.Blue.gpio, obj->pins.Blue.pin, GPIO_PIN_RESET) ;
}

void __onestep (step_motor_t *obj){
	  if(obj->direction==STEP_DIRECTION_CW){
			HAL_GPIO_WritePin(obj->pins.Orange.gpio, obj->pins.Orange.pin,Steps4[obj->stepN][0]);
			HAL_GPIO_WritePin(obj->pins.Yellow.gpio, obj->pins.Yellow.pin,  Steps4[obj->stepN][1]);
			HAL_GPIO_WritePin(obj->pins.Pink.gpio, obj->pins.Pink.pin,  Steps4[obj->stepN][2]);
			HAL_GPIO_WritePin(obj->pins.Blue.gpio, obj->pins.Blue.pin, Steps4[obj->stepN][3]);
			/*HAL_Delay(obj->delay);*/

	  }
	  else if(obj->direction==STEP_DIRECTION_ACW){
			HAL_GPIO_WritePin(obj->pins.Orange.gpio, obj->pins.Orange.pin,Steps4[3-obj->stepN][0]);
			HAL_GPIO_WritePin(obj->pins.Yellow.gpio, obj->pins.Yellow.pin,  Steps4[3-obj->stepN][1]);
			HAL_GPIO_WritePin(obj->pins.Pink.gpio, obj->pins.Pink.pin,  Steps4[3-obj->stepN][2]);
			HAL_GPIO_WritePin(obj->pins.Blue.gpio, obj->pins.Blue.pin, Steps4[3-obj->stepN][3]);
			/*HAL_Delay(obj->delay);*/
	  }
	  obj->stepN++;
	  if(obj->stepN>=4)
		  obj->stepN = 0;
	}

// Publics

void step_init(step_motor_t *obj){
	if (obj != NULL){
		// reset all variables of objejct step
		obj->delay = 0;
		obj->direction = STEP_DIRECTION_NONE;
		obj->steps = 0;
		// turno off all gpios
		__step_turnoff(obj);
	}
}

void step_drive_steps(step_motor_t *obj, uint32_t step ,step_direction_e dir, uint16_t delay)
{
	if (obj != NULL){
		obj->delay = delay;
		obj->direction = dir;
		obj->steps = step;
		/*obj->stepN = 0;*/
		for (uint32_t k=0; k<step; k++){
		  __onestep(obj);
		}
		// turno off all gpios
	/*	__step_turnoff(obj);*/
	}
}



