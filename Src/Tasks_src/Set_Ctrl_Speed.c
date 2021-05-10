
#include "Set_Ctrl_Speed.h"
#include "Check_speed.h"
#include "cmsis_os.h"
#include "main.h"
#include <stdbool.h>

/**
* @brief Function implementing the Set_Ctrl_Speed thread.
* @param argument: Not used
* @retval None
*/
void Set_Ctrl_Speed_task(void *argument)
{
	bool latch_btn_flag = true;
	bool btn_state = false;
	uint8_t btn_count = 0;
	HAL_StatusTypeDef check_set_speed = HAL_OK;
	GPIO_PinState state_set_speed_pin = GPIO_PIN_RESET;
	
  for(;;)
  {
		state_set_speed_pin = HAL_GPIO_ReadPin(Disp_OFF_GPIO_Port, Disp_OFF_Pin);
		if((state_set_speed_pin == GPIO_PIN_SET) && (latch_btn_flag == true)){
			btn_count++;
			if(btn_count >= 20){
				check_set_speed = set_control_speed();
				if(check_set_speed == HAL_OK){
					latch_btn_flag = false;
				}
			}
		}else if(state_set_speed_pin == GPIO_PIN_RESET){
			latch_btn_flag = true;
			btn_count = 0;
		}
    osDelay(100);
  }
}
