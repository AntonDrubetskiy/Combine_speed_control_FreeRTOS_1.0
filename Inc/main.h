/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Input_1_Pin GPIO_PIN_0
#define Input_1_GPIO_Port GPIOA
#define Input_1_EXTI_IRQn EXTI0_IRQn
#define Input_2_Pin GPIO_PIN_1
#define Input_2_GPIO_Port GPIOA
#define Input_2_EXTI_IRQn EXTI1_IRQn
#define Input_3_Pin GPIO_PIN_2
#define Input_3_GPIO_Port GPIOA
#define Input_3_EXTI_IRQn EXTI2_IRQn
#define Input_4_Pin GPIO_PIN_3
#define Input_4_GPIO_Port GPIOA
#define Input_4_EXTI_IRQn EXTI3_IRQn
#define Input_5_Pin GPIO_PIN_4
#define Input_5_GPIO_Port GPIOA
#define Input_5_EXTI_IRQn EXTI4_IRQn
#define Digit_3_Pin GPIO_PIN_6
#define Digit_3_GPIO_Port GPIOA
#define Digit_2_Pin GPIO_PIN_7
#define Digit_2_GPIO_Port GPIOA
#define Digit_1_Pin GPIO_PIN_0
#define Digit_1_GPIO_Port GPIOB
#define Buzzer_Pin GPIO_PIN_1
#define Buzzer_GPIO_Port GPIOB
#define ST_CP_Pin GPIO_PIN_12
#define ST_CP_GPIO_Port GPIOB
#define Disp_OFF_Pin GPIO_PIN_8
#define Disp_OFF_GPIO_Port GPIOA
#define DBG_TX_Pin GPIO_PIN_6
#define DBG_TX_GPIO_Port GPIOB
#define DBG_RX_Pin GPIO_PIN_7
#define DBG_RX_GPIO_Port GPIOB
#define Brightness_Pin GPIO_PIN_8
#define Brightness_GPIO_Port GPIOB
#define Brightness_EXTI_IRQn EXTI9_5_IRQn
#define Buzzer_OFF_Pin GPIO_PIN_9
#define Buzzer_OFF_GPIO_Port GPIOB
#define Buzzer_OFF_EXTI_IRQn EXTI9_5_IRQn
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
