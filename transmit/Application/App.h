/*
 * App.h
 *
 *  Created on: 22 de mar de 2022
 *      Author: nathan
 */

#ifndef APP_H_
#define APP_H_

#include<stdint.h>

#include"main.h"

#include "string.h"

#include "nrf24.h"

enum senha{INCORRETA, CORRETA};

void app_init();
void app_run();
char read_keypad (void);

#endif /* APP_H_ */
