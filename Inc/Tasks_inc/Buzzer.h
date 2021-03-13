
#ifndef __BUZZER_H
#define __BUZZER_H

#include <stdbool.h>
#include "stm32f1xx_hal.h"

#define TOTAL_BZR_ON_OFF_BIT		0
#define CH_BZR_ON_OFF_BIT				1
#define EMERG_BZR_ON_OFF_BIT		2

typedef struct{
	unsigned total_buzz_on_off : 1;
	unsigned ch_buzz_on_off : 1;
	unsigned emerg_buzz_on_off : 1;
}buzzer_evnt_t;

void Buzzer_task(void *argument);

HAL_StatusTypeDef Buzzer_ev(buzzer_evnt_t* buzz_evnt);

#endif //__BUZZER_H
