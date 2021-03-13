
#include "Buzzer.h"
#include "cmsis_os.h"
#include "main.h"

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
	buzzer_evnt_t buzz_evnt;
	
	/* creation of Buzzer_queue */
  Buzzer_queue_Handle = osMessageQueueNew (10, sizeof(buzzer_evnt_t), &Buzzer_queue_attributes);
	
  for(;;)
  {
		osMessageQueueGet(Buzzer_queue_Handle, &buzz_evnt, 0, osWaitForever);
		
		if((buzz_evnt.total_buzz_on_off & (1 << TOTAL_BZR_ON_OFF_BIT)) == 1){
			if((buzz_evnt.ch_buzz_on_off & (1 << CH_BZR_ON_OFF_BIT)) == 1){
				BUZZER_ON();
			}else if((buzz_evnt.emerg_buzz_on_off & (1 << EMERG_BZR_ON_OFF_BIT)) == 1){
				BUZZER_ON();
				osDelay(10000);
				BUZZER_OFF();
				buzz_evnt.emerg_buzz_on_off &= ~(1 << EMERG_BZR_ON_OFF_BIT);
			}else{
				BUZZER_OFF();
			}
		}else{
			BUZZER_OFF();
		}
	
    osDelay(1);
  }
}



