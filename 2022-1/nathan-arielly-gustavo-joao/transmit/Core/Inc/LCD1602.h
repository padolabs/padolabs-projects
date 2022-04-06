/*
 * LCD1602.h
 *
 *  Created on: Apr 5, 2022
 *      Author: nathan
 */

/*
 * lcd1602.h
 *
 *  Created on: Jan 21, 2020
 *      Author: Controllerstech
 */

#ifndef INC_LCD1602_H_
#define INC_LCD1602_H_

#include "stm32g0xx.h"

/*
 * Enumerates
 */
typedef enum{
	LCD_RS,
	LCD_RW,
	LCD_E,
	LCD_D4,
	LCD_D5,
	LCD_D6,
	LCD_D7,

	LCD_QTD
}lcd_pin_e;


/*
 * Function Prototypes
 */

void lcd_init (void);   // inicializar lcd

void lcd_send_cmd (char cmd);  // enviar comando para o lcd

void lcd_send_data (char data);  // enviar dados para o lcd

void lcd_send_string (char *str);  // enviar string para o lcd

void lcd_attr_gpio(lcd_pin_e lcd, GPIO_TypeDef *gpio, uint16_t pin);

void lcd_put_cur(int row, int col);  // coloca o cursor na linha de posição inserida (0 ou 1), col (0-15);

void lcd_clear (void);//Limpar a tela do lcd

#endif /* INC_LCD1602_H_ */
