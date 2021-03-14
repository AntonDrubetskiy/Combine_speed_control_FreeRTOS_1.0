
#include "Check_speed.h"
#include "Normal_indic.h"
#include "Buzzer.h"
#include "cmsis_os.h"
#include "main.h"
#include "Flash.h"
#include <stdio.h>


#define ONE_SEC						4
#define MIN_SPEED					60

volatile uint32_t TIM_cntr[SPEED_CHANNEL_MAX];
volatile bool latch[SPEED_CHANNEL_MAX] = {false};
static buzzer_evnt_t buzz_evnt = {.total_buzz_on_off = BIT_BZR_ON};
static indic_data_t indic_evnt;

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
	printf("sizeof(speed_data_t) =  %u\n\r", sizeof(speed_data_t));
	speed_data_t check_speed_data;
	
	
	buzzer_evnt_t buzz_check[SPEED_CHANNEL_MAX] = {{.total_buzz_on_off = BIT_BZR_ON},
																									{.total_buzz_on_off = BIT_BZR_ON},	
																									{.total_buzz_on_off = BIT_BZR_ON},
																									{.total_buzz_on_off = BIT_BZR_ON},
																								};

	uint32_t speed[SPEED_CHANNEL_MAX] = {0};
	
	uint16_t control_speed[SPEED_CHANNEL_MAX] = {0};
	
	// обновление частот врашения при которых срабатывает тревога
	Flash_Read(control_speed, SPEED_CHANNEL_MAX);
	
  for(;;)
  {
		osMessageQueueGet(Speed_queue_Handle, &check_speed_data, 0, osWaitForever);

		//printf("Speed_ch %u\n\r", check_speed_data.speed_ch);
		switch(check_speed_data.speed_ch){
			//------------------------------------
			case SPEED_CHANNEL_1:
				speed[SPEED_CHANNEL_1] = check_speed_data.rotate;
	
				if(speed[SPEED_CHANNEL_1] < MIN_SPEED){
					indic_evnt.ch_1_on = 1;
					sprintf(indic_evnt.speed, "---");
				}else{
					sprintf(indic_evnt.speed, "%u", speed[SPEED_CHANNEL_1]);
					TIM_cntr[SPEED_CHANNEL_1] = 0;
					if(speed[SPEED_CHANNEL_1] < control_speed[SPEED_CHANNEL_1]){
						indic_evnt.ch_1_on = 0;
						buzz_check[SPEED_CHANNEL_1].ch_buzz_on_off = BIT_BZR_ON;
					}else{
						indic_evnt.ch_1_on = 1;
						buzz_check[SPEED_CHANNEL_1].ch_buzz_on_off = BIT_BZR_OFF;
					}
				}
				indic_evnt.speed_ch = SPEED_CHANNEL_1;
				latch[SPEED_CHANNEL_1] = true;
				//printf("Speed_check %u = %s\n\r", indic_evnt.speed_ch + 1, indic_evnt.speed);
				break;
			
			//------------------------------------
			case SPEED_CHANNEL_2:
				speed[SPEED_CHANNEL_2] = check_speed_data.rotate;
	
				if(speed[SPEED_CHANNEL_2] < MIN_SPEED){
					indic_evnt.ch_1_on = 1;
					sprintf(indic_evnt.speed, "---");
				}else{
					sprintf(indic_evnt.speed, "%u", speed[SPEED_CHANNEL_2]);
					TIM_cntr[SPEED_CHANNEL_2] = 0;
					if(speed[SPEED_CHANNEL_2] < control_speed[SPEED_CHANNEL_2]){
						indic_evnt.ch_1_on = 0;
						buzz_check[SPEED_CHANNEL_2].ch_buzz_on_off = BIT_BZR_ON;
					}else{
						indic_evnt.ch_1_on = 1;
						buzz_check[SPEED_CHANNEL_2].ch_buzz_on_off = BIT_BZR_OFF;
					}
				}
				indic_evnt.speed_ch = SPEED_CHANNEL_2;
				latch[SPEED_CHANNEL_2] = true;
				//printf("Speed_check %u = %s\n\r", indic_evnt.speed_ch + 1, indic_evnt.speed);
				break;
				
			//------------------------------------
			case SPEED_CHANNEL_3:
				speed[SPEED_CHANNEL_3] = check_speed_data.rotate;
	
				if(speed[SPEED_CHANNEL_3] < MIN_SPEED){
					indic_evnt.ch_1_on = 1;
					sprintf(indic_evnt.speed, "---");
				}else{
					sprintf(indic_evnt.speed, "%u", speed[SPEED_CHANNEL_3]);
					TIM_cntr[SPEED_CHANNEL_3] = 0;
					if(speed[SPEED_CHANNEL_3] < control_speed[SPEED_CHANNEL_3]){
						indic_evnt.ch_1_on = 0;
						buzz_check[SPEED_CHANNEL_3].ch_buzz_on_off = BIT_BZR_ON;
					}else{
						indic_evnt.ch_1_on = 1;
						buzz_check[SPEED_CHANNEL_3].ch_buzz_on_off = BIT_BZR_OFF;
					}
				}
				indic_evnt.speed_ch = SPEED_CHANNEL_3;
				latch[SPEED_CHANNEL_3] = true;
				//printf("Speed_check %u = %s\n\r", indic_evnt.speed_ch + 1, indic_evnt.speed);
				break;
				
			//------------------------------------
			case SPEED_CHANNEL_4:
				speed[SPEED_CHANNEL_4] = check_speed_data.rotate;
	
				if(speed[SPEED_CHANNEL_4] < MIN_SPEED){
					indic_evnt.ch_1_on = 1;
					sprintf(indic_evnt.speed, "---");
				}else{
					sprintf(indic_evnt.speed, "%u", speed[SPEED_CHANNEL_4]);
					TIM_cntr[SPEED_CHANNEL_4] = 0;
					if(speed[SPEED_CHANNEL_4] < control_speed[SPEED_CHANNEL_4]){
						indic_evnt.ch_1_on = 0;
						buzz_check[SPEED_CHANNEL_4].ch_buzz_on_off = BIT_BZR_ON;
					}else{
						indic_evnt.ch_1_on = 1;
						buzz_check[SPEED_CHANNEL_4].ch_buzz_on_off = BIT_BZR_OFF;
					}
				}
				indic_evnt.speed_ch = SPEED_CHANNEL_4;
				latch[SPEED_CHANNEL_4] = true;
				//printf("Speed_check %u = %s\n\r", indic_evnt.speed_ch + 1, indic_evnt.speed);
				break;
				
			//------------------------------------
			case SPEED_CHANNEL_5:
				speed[SPEED_CHANNEL_5] = check_speed_data.rotate;
	
				if(speed[SPEED_CHANNEL_5] < MIN_SPEED){
					indic_evnt.ch_1_on = 1;
					sprintf(indic_evnt.speed, "---");
				}else{
					sprintf(indic_evnt.speed, "%u", speed[SPEED_CHANNEL_5]);
					TIM_cntr[SPEED_CHANNEL_5] = 0;
					if(speed[SPEED_CHANNEL_5] < control_speed[SPEED_CHANNEL_5]){
						indic_evnt.ch_1_on = 0;
						buzz_check[SPEED_CHANNEL_5].ch_buzz_on_off = BIT_BZR_ON;
					}else{
						indic_evnt.ch_1_on = 1;
						buzz_check[SPEED_CHANNEL_5].ch_buzz_on_off = BIT_BZR_OFF;
					}
				}
				indic_evnt.speed_ch = SPEED_CHANNEL_5;
				latch[SPEED_CHANNEL_5] = true;
				//printf("Speed_check %u = %s\n\r", indic_evnt.speed_ch + 1, indic_evnt.speed);
				break;
				
			//------------------------------------
			case SPEED_CHANNEL_MAX:
				buzz_evnt.total_buzz_on_off = check_speed_data.buzzer_state;
				break;
		}
	
		Indic_evt(&indic_evnt);
		
		if(buzz_check[SPEED_CHANNEL_1].ch_buzz_on_off == BIT_BZR_ON ||
				buzz_check[SPEED_CHANNEL_2].ch_buzz_on_off == BIT_BZR_ON ||
				buzz_check[SPEED_CHANNEL_3].ch_buzz_on_off == BIT_BZR_ON ||
				buzz_check[SPEED_CHANNEL_4].ch_buzz_on_off == BIT_BZR_ON ||
				buzz_check[SPEED_CHANNEL_5].ch_buzz_on_off == BIT_BZR_ON){
				buzz_evnt.ch_buzz_on_off = BIT_BZR_ON;
			}else{
				buzz_evnt.ch_buzz_on_off = BIT_BZR_OFF;
			}
		
		Buzzer_ev(&buzz_evnt);
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
	
	if(TIM_cntr[SPEED_CHANNEL_1] == ONE_SEC && latch[SPEED_CHANNEL_1]){
		buzz_evnt.emerg_buzz_on_off = BIT_BZR_ON;
		indic_evnt.ch_1_on = 1;
		indic_evnt.speed_ch = SPEED_CHANNEL_1;
		sprintf(indic_evnt.speed, "---");
		printf("TIM_cntr[SPEED_CHANNEL_1] == ONE_SEC\n\r");
		Indic_evt(&indic_evnt);
		Buzzer_ev(&buzz_evnt);
	}
	if(TIM_cntr[SPEED_CHANNEL_2] == ONE_SEC && latch[SPEED_CHANNEL_2]){
		buzz_evnt.emerg_buzz_on_off = BIT_BZR_ON;
		indic_evnt.ch_2_on = 1;
		indic_evnt.speed_ch = SPEED_CHANNEL_2;
		sprintf(indic_evnt.speed, "---");
		printf("TIM_cntr[SPEED_CHANNEL_2] == ONE_SEC\n\r");
		Indic_evt(&indic_evnt);
		Buzzer_ev(&buzz_evnt);
	}
	if(TIM_cntr[SPEED_CHANNEL_3] == ONE_SEC && latch[SPEED_CHANNEL_3]){
		buzz_evnt.emerg_buzz_on_off = BIT_BZR_ON;
		indic_evnt.ch_3_on = 1;
		indic_evnt.speed_ch = SPEED_CHANNEL_3;
		printf("TIM_cntr[SPEED_CHANNEL_3] == ONE_SEC\n\r");
		sprintf(indic_evnt.speed, "---");
		Indic_evt(&indic_evnt);
		Buzzer_ev(&buzz_evnt);
	}
	if(TIM_cntr[SPEED_CHANNEL_4] == ONE_SEC && latch[SPEED_CHANNEL_4]){
		buzz_evnt.emerg_buzz_on_off = BIT_BZR_ON;
		indic_evnt.ch_4_on = 1;
		indic_evnt.speed_ch = SPEED_CHANNEL_4;
		printf("TIM_cntr[SPEED_CHANNEL_4] == ONE_SEC\n\r");
		sprintf(indic_evnt.speed, "---");
		Indic_evt(&indic_evnt);
		Buzzer_ev(&buzz_evnt);
	}
	if(TIM_cntr[SPEED_CHANNEL_5] == ONE_SEC && latch[SPEED_CHANNEL_5]){
		buzz_evnt.emerg_buzz_on_off = BIT_BZR_ON;
		indic_evnt.ch_5_on = 1;
		indic_evnt.speed_ch = SPEED_CHANNEL_5;
		printf("TIM_cntr[SPEED_CHANNEL_5] == ONE_SEC\n\r");
		sprintf(indic_evnt.speed, "---");
		Indic_evt(&indic_evnt);
		Buzzer_ev(&buzz_evnt);
	}

}
