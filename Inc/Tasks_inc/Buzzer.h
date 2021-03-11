
#ifndef __BUZZER_H
#define __BUZZER_H

#include <stdbool.h>
#include "stm32f1xx_hal.h"

void Buzzer_task(void *argument);

HAL_StatusTypeDef Buzzer_ev(bool flag);

#endif //__BUZZER_H
