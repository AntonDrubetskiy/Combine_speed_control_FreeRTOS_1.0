
#ifndef __CHECK_SPEED_H
#define __CHECK_SPEED_H

#include "stm32f1xx_hal.h"
#include <stdbool.h>


typedef enum{
	SPEED_CHANNEL_1 = 0,
	SPEED_CHANNEL_2,
	SPEED_CHANNEL_3,
	SPEED_CHANNEL_4,
	SPEED_CHANNEL_5,
	SPEED_CHANNEL_MAX,
}num_channel_t;

typedef struct{
	uint16_t rotate[10];
	uint8_t speed_ch;
	bool flag_buzzer_off;
} speed_data_t;

void Check_speed_task(void *argument);

void Increase_Check_Speed_TIM_counter(void);

HAL_StatusTypeDef Speed_evt(speed_data_t *speed);

#endif //__CHECK_SPEED_H
