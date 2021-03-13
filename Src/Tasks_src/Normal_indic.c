
#include "Normal_indic.h"
#include "cmsis_os.h"
#include "HC595_ser_trans.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include "Check_speed.h"

#define NUM_COLUMN			(3)

SPI_HandleTypeDef hspi2;
DMA_HandleTypeDef hdma_spi2_tx;

// подключение разрядов
#define DIGIT_1_ON(b) 		TIM3->CCR3=b // 1-й
#define DIGIT_2_ON(b)			TIM3->CCR2=b // 2-й
#define DIGIT_3_ON(b) 		TIM3->CCR1=b // 3-й
#define ALL_DIGIT_OFF() 	TIM3->CCR1=0, TIM3->CCR2=0, TIM3->CCR3=0 // все разряды отключены


/* Definitions for Indic_queue */
osMessageQueueId_t Indic_queue_Handle;
const osMessageQueueAttr_t Indic_queue_attributes = {
  .name = "Indic_queue"
};

/* Definitions for Brightness_queue */
osMessageQueueId_t Brightness_queue_Handle;
const osMessageQueueAttr_t Brightness_queue_attributes = {
  .name = "Brightness_queue"
};

// Indic_evt
HAL_StatusTypeDef Indic_evt(indic_data_t *indic)
{
	if(osMessageQueuePut(Indic_queue_Handle, indic, NULL, 0) != osOK){
		return HAL_ERROR;
	}
	return HAL_OK;
}

// Indic_brightness_evt
HAL_StatusTypeDef Indic_brightness_evt(uint16_t *bright)
{
	if(osMessageQueuePut(Brightness_queue_Handle, bright, NULL, 0) != osOK){
		return HAL_ERROR;
	}
	return HAL_OK;
}

static void MX_DMA_Init(void);
static void MX_SPI2_Init(void);

static inline void create_indic_strings(char **str_speed, char **str_indic, char *str_off);

/**
  * @brief  Function implementing the Normal_Indic_ thread.
  * @param  argument: Not used 
  * @retval None
  */
void Normal_Indicate_task(void *argument)
{
	MX_DMA_Init();
	MX_SPI2_Init();
	/* creation of Indic_queue */
  Indic_queue_Handle = osMessageQueueNew(10, sizeof(indic_data_t), &Indic_queue_attributes);
	/* creation of Brightness_queue */
  Brightness_queue_Handle = osMessageQueueNew(10, sizeof(uint16_t), &Brightness_queue_attributes);
	
	uint8_t i;
	uint16_t brightness = BRIGHTNESS_100;
	indic_data_t indic_data;
	char str_off[] = "off";
	char *str_speed[SPEED_CHANNEL_MAX];
	char *str_indic_column[NUM_COLUMN];
	
	create_indic_strings(str_speed, str_indic_column, str_off);

  for(;;)
  {
		if(osMessageQueueGet(Indic_queue_Handle, &indic_data, NULL, 0) == osOK){
			sprintf(str_speed[indic_data.speed_ch], "%u", indic_data.speed);
			
			for(i = 0; i < NUM_COLUMN; i++){
				str_indic_column[i][indic_data.speed_ch] = str_speed[indic_data.speed_ch][i];
			}
		}
    
		osMessageQueueGet(Brightness_queue_Handle, &brightness, NULL, 0);
		
		for(i = 0; i < NUM_COLUMN; i++){
			
			ALL_DIGIT_OFF();
			
			HC595_Set_Column_Symbols(&hspi2, (char*)str_indic_column[i], SPEED_CHANNEL_MAX, ST_CP_GPIO_Port, ST_CP_Pin);

			// индикация разряда
			switch(i)
			{
				case 0:
					DIGIT_1_ON(brightness); // сотни
					break;
				case 1:
					DIGIT_2_ON(brightness); // десятки
					break;
				case 2:
					DIGIT_3_ON(brightness); // единицы
					break;
			}
			osDelay(5);	
		}
  }
}


static inline void create_indic_strings(char **str_speed, char **str_indic, char *str_off)
{
	uint8_t i, j;
	for(i = 0; i < SPEED_CHANNEL_MAX; i++){
		str_speed[i] = calloc(NUM_COLUMN, sizeof(char));
		
		for(j = 0; j < NUM_COLUMN; j++){
			str_speed[i][j] = str_off[j];
		}
	}
	
	for(i = 0; i < NUM_COLUMN; i++){
		str_indic[i] = calloc(SPEED_CHANNEL_MAX, sizeof(char));
	}
	
	for(i = 0; i < NUM_COLUMN; i++){
		for(j = 0; j < SPEED_CHANNEL_MAX; j++){
			str_indic[i][j] = str_speed[j][i];
		}
	}
}


/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

}
