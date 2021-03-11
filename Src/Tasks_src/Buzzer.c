
#include "Buzzer.h"
#include "cmsis_os.h"
#include "main.h"

static bool buzzer_flag;


/* Definitions for Buzzer_queue */
osMessageQueueId_t Buzzer_queue_Handle;
const osMessageQueueAttr_t Buzzer_queue_attributes = {
  .name = "Buzzer_queue"
};


/**
* @brief Function implementing the Buzzer_ thread.
* @param argument: Not used
* @retval None
*/
void Buzzer_task(void *argument)
{

	
	/* creation of Buzzer_queue */
  Buzzer_queue_Handle = osMessageQueueNew (1, sizeof(uint16_t), &Buzzer_queue_attributes);
	
  for(;;)
  {
    osDelay(1);
  }
}
