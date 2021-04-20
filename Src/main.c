

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "Normal_indic.h"
#include "Check_speed.h"
#include "Buzzer.h"
#include "Blink_indic.h"
#include <stdio.h>


#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))
#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define TRCENA          0x01000000


struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE *f) {
   if (DEMCR & TRCENA) {

while (ITM_Port32(0) == 0);
    ITM_Port8(0) = ch;
  }
  return(ch);
}


/* Private variables ---------------------------------------------------------*/


TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart1;


/* Definitions for Normal_Indic */
osThreadId_t Normal_Indic_Handle;
const osThreadAttr_t Normal_Indic_attributes = {
  .name = "Normal_Indic",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for Blink_Indic */
osThreadId_t Blink_Indic_Handle;
const osThreadAttr_t Blink_Indic_attributes = {
  .name = "Blink_Indic",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 1
};
/* Definitions for Buzzer */
osThreadId_t Buzzer_Handle;
const osThreadAttr_t Buzzer_attributes = {
  .name = "Buzzer",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 2
};
/* Definitions for Check_speed */
osThreadId_t Check_speed_Handle;
const osThreadAttr_t Check_speed_attributes = {
  .name = "Check_speed",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 2
};

static uint16_t count = 0;



/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART1_UART_Init(void);

static inline void calculate_speed(uint8_t *cnt_pls, int64_t *usec, int64_t *lst_tim_trn,
																			int64_t *tim_trn, uint16_t *trn_spd, speed_data_t *speed, uint8_t spd_ch);

static inline void set_brightness(void);



/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();

	
	//------------------------------------------------------------------
	// запуск таймеров и ШИМ
	//------------------------------------------------------------------
	// запуск таймеров
	HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_Base_Start_IT(&htim2);
	// запуск ШИМ
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	HAL_Delay(100);
	//==================================================================
	
	printf("START APPLICATION!\n\r");
	
  /* Init scheduler */
  osKernelInitialize();
	
	//__disable_irq();
	/* Create the thread(s) */
	/* creation of Check_speed_ */
  Check_speed_Handle = osThreadNew(Check_speed_task, NULL, &Check_speed_attributes);
  /* creation of Normal_Indic_ */
  Normal_Indic_Handle = osThreadNew(Normal_Indicate_task, NULL, &Normal_Indic_attributes);
  /* creation of Blink_Indic_ */
  Blink_Indic_Handle = osThreadNew(Blink_Indicate_task, NULL, &Blink_Indic_attributes);
  /* creation of Buzzer_ */
  Buzzer_Handle = osThreadNew(Buzzer_task, NULL, &Buzzer_attributes);

	//__enable_irq();
  /* Start scheduler */
  osKernelStart();
 
  while (1)
  { }
}

/*********************************************************************************************************************
*																							SYSTEM SETUP																													 *
**********************************************************************************************************************/

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}



/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 72;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 275;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65535;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 1000;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }

}


/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ST_CP_GPIO_Port, ST_CP_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : Input_1_Pin Input_2_Pin Input_3_Pin */
  GPIO_InitStruct.Pin = Input_1_Pin|Input_2_Pin|Input_3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : Input_4_Pin Input_5_Pin */
  GPIO_InitStruct.Pin = Input_4_Pin|Input_5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : Buzzer_Pin */
  GPIO_InitStruct.Pin = Buzzer_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Buzzer_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : ST_CP_Pin */
  GPIO_InitStruct.Pin = ST_CP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ST_CP_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : Disp_OFF_Pin */
  GPIO_InitStruct.Pin = Disp_OFF_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(Disp_OFF_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Brightness_Pin Buzzer_OFF_Pin */
  GPIO_InitStruct.Pin = Brightness_Pin|Buzzer_OFF_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

  HAL_NVIC_SetPriority(EXTI3_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

/************************************************************************************************************
*																					END SYSTEM SETUP																									*
*************************************************************************************************************/

 /**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM4 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM4) {
    HAL_IncTick();
  }
	
	if (htim->Instance == TIM1) {
		if(count == UINT16_MAX){
			count = 0;
		}
		else{
			count++;
		}
  }
	
	if(htim->Instance == TIM2){
		Increase_Check_Speed_TIM_counter();
		check_blink_indic();
	}
}


/**---------------------------------------------------------------
  * @brief функция возврата микросекунд с начала работы программы
  * @param None
  * @retval long, количество микросекунд с начала работы программы
------------------------------------------------------------------*/
int64_t micros(void)
{
	return (TIM1->CNT + count * UINT16_MAX);
}
//---------------------------------------------------------------
//---------------------------------------------------------------



/**---------------------------------------------------------------
  * @brief Callback function
	* @param GPIO_Pin на котором сработало прерывание
  * @retval None
------------------------------------------------------------------*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	static uint8_t cnt_pulse[SPEED_CHANNEL_MAX];
	static uint8_t cnt_turn[SPEED_CHANNEL_MAX];
	static int64_t microsec, last_tim_turn[SPEED_CHANNEL_MAX], time_turn[SPEED_CHANNEL_MAX];
	static uint16_t turn_speed[SPEED_CHANNEL_MAX];
	static speed_data_t speed_data[SPEED_CHANNEL_MAX];
	static speed_data_t buzzer_data = {.buzzer_state = true};
	
	switch (GPIO_Pin)
		{
			// прерывание по первому каналу измерения частоты вращения
			case Input_1_Pin:
				calculate_speed(cnt_pulse, &microsec, last_tim_turn, time_turn, turn_speed, speed_data, SPEED_CHANNEL_1);
			break;
			
			// прерывание по второму каналу измерения частоты вращения
			case Input_2_Pin:
				calculate_speed(cnt_pulse, &microsec, last_tim_turn, time_turn, turn_speed, speed_data, SPEED_CHANNEL_2);
			break;
			
			// прерывание по третьему каналу измерения частоты вращения
			case Input_3_Pin:
				calculate_speed(cnt_pulse, &microsec, last_tim_turn, time_turn, turn_speed, speed_data, SPEED_CHANNEL_3);
			break;

			// прерывание по четвертому каналу измерения частоты вращения
			case Input_4_Pin:
				calculate_speed(cnt_pulse, &microsec, last_tim_turn, time_turn, turn_speed, speed_data, SPEED_CHANNEL_4);
			break;

			// прерывание по пятому каналу измерения частоты вращения
			case Input_5_Pin:
				calculate_speed(cnt_pulse, &microsec, last_tim_turn, time_turn, turn_speed, speed_data, SPEED_CHANNEL_5);
			break;
			
			// обработка нажатия кнопки уменьшения яркости дисплеев
			case Brightness_Pin:
				set_brightness();
			break;
			
				// обработка нажатия кнопки выключения звукового сигнала
			case Buzzer_OFF_Pin:
				buzzer_data.buzzer_state = !buzzer_data.buzzer_state;
				buzzer_data.speed_ch = SPEED_CHANNEL_MAX;
				Speed_evt(&buzzer_data);
			break;
		}
}
//---------------------------------------------------------------
//---------------------------------------------------------------



static inline void calculate_speed(uint8_t *cnt_pls, int64_t *usec, int64_t *lst_tim_trn,
																			int64_t *tim_trn, uint16_t *trn_spd, speed_data_t *speed, uint8_t spd_ch)
{
	if(cnt_pls[spd_ch]==4) // один оборот - 4 импульса датчика
	{	
			*usec=micros();
			tim_trn[spd_ch]=*usec-lst_tim_trn[spd_ch];
			lst_tim_trn[spd_ch]=*usec;
			trn_spd[spd_ch]=60000000/tim_trn[spd_ch];
			speed[spd_ch].rotate=trn_spd[spd_ch];
			*usec=micros();
			lst_tim_trn[spd_ch]=*usec;
			speed[spd_ch].speed_ch = spd_ch;
			if(Speed_evt(&speed[spd_ch]) != HAL_OK){
				//printf("Speed_evt %i ERROR!\n\r", spd_ch + 1);
			}
		cnt_pls[spd_ch]=0;
	}
	cnt_pls[spd_ch]++;
}


static inline void set_brightness(void)
{
	static uint8_t count_brightness;
	static uint16_t brightness;
	
	if(count_brightness==4){
		count_brightness=0;
	}else{
		count_brightness++;
	}
			
	switch(count_brightness)
	{
		case 0:
			brightness=BRIGHTNESS_100;
			break;
		
		case 1:
			brightness=BRIGHTNESS_60;
			break;
		
		case 2:
			brightness=BRIGHTNESS_30;
			break;
		
		case 3:
			brightness=BRIGHTNESS_10;
			break;
		
		case 4:
			brightness=BRIGHTNESS_5;
			break;
	}
	
	Indic_brightness_evt(&brightness);
}



/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{ }

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
