
#ifndef __NORMAL_INDIC_H
#define __NORMAL_INDIC_H

#include <stdint.h>
#include <stdbool.h>
#include "stm32f1xx_hal.h"

typedef struct{
	uint16_t speed;
	uint8_t speed_ch;
	unsigned ch_1_on : 1;
	unsigned ch_2_on : 1;
	unsigned ch_3_on : 1;
	unsigned ch_4_on : 1;
	unsigned ch_5_on : 1;
}indic_data_t;

// уровни яркости дисплеев
#define BRIGHTNESS_100		1000 // 100%
#define BRIGHTNESS_60 		600 // 60%
#define BRIGHTNESS_30 		300 // 30%
#define BRIGHTNESS_10			100 // 10%
#define BRIGHTNESS_5			50 // 10%

void Normal_Indicate_task(void *argument);

HAL_StatusTypeDef Indic_evt(indic_data_t *indic);
HAL_StatusTypeDef Indic_brightness_evt(uint16_t *b);

#endif //__NORMAL_INDIC_H
