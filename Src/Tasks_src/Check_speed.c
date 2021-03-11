
#include "Check_speed.h"
#include "cmsis_os.h"
#include "main.h"

// включение/отключение звукового сигнала
#define BUZZER_ON() 			HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_RESET) // включение
#define BUZZER_OFF()			HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_SET) // отключение



/* Definitions for Speed_queue */
osMessageQueueId_t Speed_queue_Handle;
const osMessageQueueAttr_t Speed_queue_attributes = {
  .name = "Speed_queue"
};


HAL_StatusTypeDef Speed_evt(speed_data_t *speed)
{
	if(osMessageQueuePut(Speed_queue_Handle, speed, 0, 0) != osOK){
		return HAL_ERROR;
	}
	return HAL_OK;
}



/**
* @brief Function implementing the Check_speed_ thread.
* @param argument: Not used
* @retval None
*/
void Check_speed_task(void *argument)
{
	/* creation of Speed_queue */
  Speed_queue_Handle = osMessageQueueNew(10, sizeof(speed_data_t), &Speed_queue_attributes);
	speed_data_t check_speed_data;

	
	
  for(;;)
  {
		osMessageQueueGet(Speed_queue_Handle, &check_speed_data, 0, osWaitForever);
		
		switch(check_speed_data.speed_ch){
			case SPEED_CHANNEL_1:
				
				break;
			
			case SPEED_CHANNEL_2:
				break;
			
			case SPEED_CHANNEL_3:
				break;
			
			case SPEED_CHANNEL_4:
				break;
			
			case SPEED_CHANNEL_5:
				break;
		}
		
    osDelay(1);
  }
}
