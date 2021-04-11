
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

static inline void check_speed(uint32_t* spd, uint16_t* ctrl_speed, speed_data_t* speed_data, buzzer_evnt_t* bzr_chck);

/**
* @brief Function implementing the Check_speed_ thread.
* @param argument: Not used
* @retval None
*/
void Check_speed_task(void *argument)
{
	/* creation of Speed_queue */
  Speed_queue_Handle = osMessageQueueNew(100, sizeof(speed_data_t), &Speed_queue_attributes);
	
	if(Speed_queue_Handle == NULL){
		while(1){
			printf("Speed_queue_Handle = NULL\n\r");
			osDelay(1000);
		}
	}
	
	printf("sizeof(speed_data_t) =  %u\n\r", sizeof(speed_data_t));
	speed_data_t check_speed_data;
	
	
	buzzer_evnt_t buzz_check[SPEED_CHANNEL_MAX] = {{.total_buzz_on_off = BIT_BZR_ON},
																									{.total_buzz_on_off = BIT_BZR_ON},	
																									{.total_buzz_on_off = BIT_BZR_ON},
																									{.total_buzz_on_off = BIT_BZR_ON},
																								};

	uint32_t speed[SPEED_CHANNEL_MAX] = {0};
	uint16_t control_speed[SPEED_CHANNEL_MAX] = {0};
	bool rst_flag_check_spd = false;

	
	// обновление частот врашения при которых срабатывает тревога
	Flash_Read(control_speed, SPEED_CHANNEL_MAX);
#ifdef DEBUG
	printf("control_speed[%u] = %u\n\r", SPEED_CHANNEL_1, control_speed[SPEED_CHANNEL_1]);
	printf("control_speed[%u] = %u\n\r", SPEED_CHANNEL_2, control_speed[SPEED_CHANNEL_2]);
	printf("control_speed[%u] = %u\n\r", SPEED_CHANNEL_3, control_speed[SPEED_CHANNEL_3]);
	printf("control_speed[%u] = %u\n\r", SPEED_CHANNEL_4, control_speed[SPEED_CHANNEL_4]);
	printf("control_speed[%u] = %u\n\r", SPEED_CHANNEL_5, control_speed[SPEED_CHANNEL_5]);
#endif
  for(;;)
  {
		osMessageQueueGet(Speed_queue_Handle, &check_speed_data, 0, osWaitForever);
#ifdef DEBUG
		//printf("Speed_ch %u\n\r", check_speed_data.speed_ch);
#endif
		switch(check_speed_data.speed_ch){
			//------------------------------------
			case SPEED_CHANNEL_1:
					check_speed(speed, control_speed, &check_speed_data, buzz_check);
				break;	
			//------------------------------------
			case SPEED_CHANNEL_2:
					check_speed(speed, control_speed, &check_speed_data, buzz_check);
				break;
			//------------------------------------
			case SPEED_CHANNEL_3:
					check_speed(speed, control_speed, &check_speed_data, buzz_check);
				break;
			//------------------------------------
			case SPEED_CHANNEL_4:
					check_speed(speed, control_speed, &check_speed_data, buzz_check);
				break;
			//------------------------------------
			case SPEED_CHANNEL_5:
					check_speed(speed, control_speed, &check_speed_data, buzz_check);
				break;
			//------------------------------------
			case SPEED_CHANNEL_MAX:
				buzz_evnt.total_buzz_on_off = check_speed_data.buzzer_state;
				Buzzer_ev(&buzz_evnt);
				break;
		}
	
		if(buzz_check[SPEED_CHANNEL_1].ch_buzz_on_off == BIT_BZR_ON ||
				buzz_check[SPEED_CHANNEL_2].ch_buzz_on_off == BIT_BZR_ON ||
				buzz_check[SPEED_CHANNEL_3].ch_buzz_on_off == BIT_BZR_ON ||
				buzz_check[SPEED_CHANNEL_4].ch_buzz_on_off == BIT_BZR_ON ||
				buzz_check[SPEED_CHANNEL_5].ch_buzz_on_off == BIT_BZR_ON){
					buzz_evnt.ch_buzz_on_off = BIT_BZR_ON;
					rst_flag_check_spd = true;
					Buzzer_ev(&buzz_evnt);
			}else if(rst_flag_check_spd == true){
				buzz_evnt.ch_buzz_on_off = BIT_BZR_OFF;
				Buzzer_ev(&buzz_evnt);
				rst_flag_check_spd = false;
			}
    osDelay(1);
  }
}



void Increase_Check_Speed_TIM_counter(void)
{
	static bool reset_flag;
	
	TIM_cntr[SPEED_CHANNEL_1]++;
	TIM_cntr[SPEED_CHANNEL_2]++;
	TIM_cntr[SPEED_CHANNEL_3]++;
	TIM_cntr[SPEED_CHANNEL_4]++;
	TIM_cntr[SPEED_CHANNEL_5]++;
	
	if(TIM_cntr[SPEED_CHANNEL_1] == ONE_SEC && latch[SPEED_CHANNEL_1]){
		reset_flag = true;
		indic_evnt.ch_1_on = 1;
		indic_evnt.speed_ch = SPEED_CHANNEL_1;
		sprintf(indic_evnt.speed, "---");
#ifdef DEBUG
		printf("TIM_cntr[SPEED_CHANNEL_1] == ONE_SEC\n\r");
#endif
		Indic_evt(&indic_evnt);
	}
	if(TIM_cntr[SPEED_CHANNEL_2] == ONE_SEC && latch[SPEED_CHANNEL_2]){
		reset_flag = true;
		indic_evnt.ch_2_on = 1;
		indic_evnt.speed_ch = SPEED_CHANNEL_2;
		sprintf(indic_evnt.speed, "---");
#ifdef DEBUG
		printf("TIM_cntr[SPEED_CHANNEL_2] == ONE_SEC\n\r");
#endif
		Indic_evt(&indic_evnt);
	}
	if(TIM_cntr[SPEED_CHANNEL_3] == ONE_SEC && latch[SPEED_CHANNEL_3]){
		reset_flag = true;
		indic_evnt.ch_3_on = 1;
		indic_evnt.speed_ch = SPEED_CHANNEL_3;
#ifdef DEBUG
		printf("TIM_cntr[SPEED_CHANNEL_3] == ONE_SEC\n\r");
#endif
		sprintf(indic_evnt.speed, "---");
		Indic_evt(&indic_evnt);
	}
	if(TIM_cntr[SPEED_CHANNEL_4] == ONE_SEC && latch[SPEED_CHANNEL_4]){
		reset_flag = true;
		indic_evnt.ch_4_on = 1;
		indic_evnt.speed_ch = SPEED_CHANNEL_4;
#ifdef DEBUG
		printf("TIM_cntr[SPEED_CHANNEL_4] == ONE_SEC\n\r");
#endif
		sprintf(indic_evnt.speed, "---");
		Indic_evt(&indic_evnt);
	}
	if(TIM_cntr[SPEED_CHANNEL_5] == ONE_SEC && latch[SPEED_CHANNEL_5]){
		reset_flag = true;
		indic_evnt.ch_5_on = 1;
		indic_evnt.speed_ch = SPEED_CHANNEL_5;
#ifdef DEBUG
		printf("TIM_cntr[SPEED_CHANNEL_5] == ONE_SEC\n\r");
#endif
		sprintf(indic_evnt.speed, "---");
		Indic_evt(&indic_evnt);
	}
	
	if(reset_flag == true){
		buzz_evnt.emerg_buzz_on_off = BIT_BZR_ON;
		Buzzer_ev(&buzz_evnt);
		buzz_evnt.emerg_buzz_on_off = BIT_BZR_OFF;
		reset_flag = false;
	}

}


static inline void check_speed(uint32_t* spd, uint16_t* ctrl_speed, speed_data_t* speed_data, buzzer_evnt_t* bzr_chck)
{
				spd[speed_data->speed_ch] = speed_data->rotate;
				if(spd[speed_data->speed_ch] < MIN_SPEED){
					indic_evnt.ch_1_on = 1;
					sprintf(indic_evnt.speed, "---");
				}else{
					sprintf(indic_evnt.speed, "%u", spd[speed_data->speed_ch]);
					TIM_cntr[speed_data->speed_ch] = 0;
					if(spd[speed_data->speed_ch] < ctrl_speed[speed_data->speed_ch]){
						indic_evnt.ch_1_on = 0;
						bzr_chck[speed_data->speed_ch].ch_buzz_on_off = BIT_BZR_ON;
					}else{
						indic_evnt.ch_1_on = 1;
						bzr_chck[speed_data->speed_ch].ch_buzz_on_off = BIT_BZR_OFF;
					}
				}
				indic_evnt.speed_ch = speed_data->speed_ch;
				latch[speed_data->speed_ch] = true;
				Indic_evt(&indic_evnt);
#ifdef DEBUG
				//printf("Speed_check %u = %s\n\r", indic_evnt.speed_ch + 1, indic_evnt.speed);
#endif
}
