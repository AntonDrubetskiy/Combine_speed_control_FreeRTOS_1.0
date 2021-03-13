
#include "Check_speed.h"
#include "Normal_indic.h"
#include "cmsis_os.h"
#include "main.h"
#include "Flash.h"
#include <stdio.h>


#define COUNT_1_SECOND		4
#define MIN_SPEED					60

volatile uint16_t TIM_cntr[SPEED_CHANNEL_MAX];

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

	uint8_t i;
	uint16_t speed = 0;
	indic_data_t indic_evnt;
	uint16_t control_speed[SPEED_CHANNEL_MAX] = {0};
	
	// обновление частот врашения при которых срабатывает тревога
	Flash_Read(control_speed, SPEED_CHANNEL_MAX);
	
  for(;;)
  {
		osMessageQueueGet(Speed_queue_Handle, &check_speed_data, 0, osWaitForever);
		
		switch(check_speed_data.speed_ch){
			case SPEED_CHANNEL_1:
			for(i=0; i<10; i++){
				speed += check_speed_data.rotate[i];
			}			
			speed = speed/10;
			sprintf(indic_evnt.speed, "%u", speed);
			
			indic_evnt.speed_ch = SPEED_CHANNEL_1;
			
			if(speed < control_speed[SPEED_CHANNEL_1]){
				indic_evnt.ch_1_on &= ~(1 << SPEED_CHANNEL_1);
			}else{
				indic_evnt.ch_1_on |= (1 << SPEED_CHANNEL_1);
			}
			
			if(speed < MIN_SPEED || TIM_cntr[SPEED_CHANNEL_1]){
				indic_evnt.ch_1_on |= (1 << SPEED_CHANNEL_1);
				sprintf(indic_evnt.speed, "---");
			}else{
				
			}
			
			Indic_evt(&indic_evnt);
			speed = 0;
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



void Increase_Check_Speed_TIM_counter(void)
{
	TIM_cntr[SPEED_CHANNEL_1]++;
	TIM_cntr[SPEED_CHANNEL_2]++;
	TIM_cntr[SPEED_CHANNEL_3]++;
	TIM_cntr[SPEED_CHANNEL_4]++;
	TIM_cntr[SPEED_CHANNEL_5]++;
}
