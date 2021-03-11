
#include "Blink_indic.h"
#include "cmsis_os.h"
#include "main.h"


/**
* @brief Function implementing the Blink_Indic_ thread.
* @param argument: Not used
* @retval None
*/
void Blink_Indicate_task(void *argument)
{

  for(;;)
  {
    osDelay(1);
  }
}
