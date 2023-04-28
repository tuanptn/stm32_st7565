/*
* @Author: nhantt
* @Date:   2020-02-02
* @Last Modified by:   nhantt
* @Last Modified time: 2020-02-02
*/

#ifndef __ST7567_H__
#define __ST7567_H__

#include "stm32f1xx_hal.h"
#include "u8g2.h"

extern u8g2_t u8g2;

#define LCD_WIDTH	132
#define LCD_HEIGHT	64

#define GLCD_CLK()		__HAL_RCC_GPIOC_CLK_ENABLE()
#define GLCD_PORT		GPIOC
#define GLCD_BL_PIN		GPIO_PIN_9
#define GLCD_DB0_PIN	GPIO_PIN_11
#define GLCD_DB1_PIN	GPIO_PIN_12
#define GLCD_DB2_PIN	GPIO_PIN_5
#define GLCD_DB3_PIN	GPIO_PIN_4
#define GLCD_DB4_PIN	GPIO_PIN_3
#define GLCD_DB5_PIN	GPIO_PIN_2
#define GLCD_DB6_PIN	GPIO_PIN_1
#define GLCD_DB7_PIN	GPIO_PIN_0
#define GLCD_E_PIN		GPIO_PIN_10

#define GLCD_CTR_PORT	GPIOB
#define GLCD_CS0_PIN	GPIO_PIN_15
#define GLCD_RST_PIN	GPIO_PIN_14		// Reset
#define GLCD_RS_PIN		GPIO_PIN_13
#define GLCD_RW_PIN		GPIO_PIN_12


#define BL_On()		HAL_GPIO_WritePin(GLCD_PORT, GLCD_BL_PIN, RESET)
#define BL_Off()	HAL_GPIO_WritePin(GLCD_PORT, GLCD_BL_PIN, SET)

void ST7567_Init(void);
void ST7567_Refresh(void);
void Set_Backlight(uint8_t percent);
void Backlight_Off(void);
void Backlight_On();
#endif