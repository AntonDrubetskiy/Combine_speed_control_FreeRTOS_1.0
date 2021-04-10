
#include "Buzzer.h"
#include "cmsis_os.h"
#include "main.h"
#include <stdio.h>

// включение/отключение звукового сигнала
#define BUZZER_ON() 			HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_RESET) // включение
#define BUZZER_OFF()			HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_SET) // отключение


/* Definitions for Buzzer_queue */
osMessageQueueId_t Buzzer_queue_Handle;
const osMessageQueueAttr_t Buzzer_queue_attributes = {
  .name = "Buzzer_queue"
};


HAL_StatusTypeDef Buzzer_ev(buzzer_evnt_t* buzz_evnt)
{
	if(osMessageQueuePut(Buzzer_queue_Handle, buzz_evnt, 0, 0) != osOK){
		return HAL_ERROR;
	}
	return HAL_OK;
}


/**
* @brief Function implementing the Buzzer_ thread.
* @param argument: Not used
* @retval None
*/
void Buzzer_task(void *argument)
{
	BUZZER_OFF();
	buzzer_evnt_t buzz_evnt;
	/* creation of Buzzer_queue */
  Buzzer_queue_Handle = osMessageQueueNew (10, sizeof(buzzer_evnt_t), &Buzzer_queue_attributes);
	
	if(Buzzer_queue_Handle == NULL){
		while(1){
			printf("Buzzer_queue_Handle = NULL\n\r");
			osDelay(1000);
		}
	}
	
  for(;;)
  {	
		osMessageQueueGet(Buzzer_queue_Handle, &buzz_evnt, 0, osWaitForever);
		
		printf("buzz_evnt.total_buzz_on_off = %i\r", buzz_evnt.total_buzz_on_off);
		printf("buzz_evnt.ch_buzz_on_off = %i\r", buzz_evnt.ch_buzz_on_off);
		printf("buzz_evnt.emerg_buzz_on_off = %i\n\r", buzz_evnt.emerg_buzz_on_off);
		
		if(buzz_evnt.total_buzz_on_off == BIT_BZR_ON){
			if(buzz_evnt.ch_buzz_on_off == BIT_BZR_ON){
				BUZZER_ON();
				printf("BUZZER_ON\n\r");
			}else if(buzz_evnt.emerg_buzz_on_off == BIT_BZR_ON){
				BUZZER_ON();
				printf("BUZZER_ON\n\r");
				osDelay(1000);
				BUZZER_OFF();
				printf("BUZZER_OFF\n\r");
				buzz_evnt.emerg_buzz_on_off = BIT_BZR_OFF;
			}else{
				BUZZER_OFF();
				printf("BUZZER_OFF\n\r");
			}
		}else{
			BUZZER_OFF();
			printf("BUZZER_OFF\n\r");
		}
    osDelay(1);
  }
}



