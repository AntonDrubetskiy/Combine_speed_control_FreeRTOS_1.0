#ifndef _HC595_SER_TRANS_H
#define _HC595_SER_TRANS_H

#include "stm32f1xx_hal.h"



/***************************************************
* Данная библиотека применяется для отображения    *
* цифр на нескольких семимегментных индикаторах  *
* с помощью сдвиговых регистров 74HC595 путем      *
* последовательной передачи байтов в регистры,     *
* соединенные последовательно через выход Q7'      *
****************************************************/

typedef enum{
	HC595_OK = 0,
	HC595_CS_PIN_OK = 1,
	HC595_CS_PIN_ERR,
	HC595_CS_PORT_ERR,
	HC595_NUM_DISP_ERR,
	HC595_TRANSMIT_ERR,
	HC595_STR_LEN_ERR,
	HC595_STR_EQUAL_LEN_ERR,
}hc595_err_t;

// вывод символов на n семисегментных инд. с помощью n сдвиговых регистров 74HC595
hc595_err_t HC595_Set_Column_Symbols(SPI_HandleTypeDef *handle, char *str_clm, int8_t n, GPIO_TypeDef *cs_port, uint16_t cs_pin);


//#define CS_STROB() CS_RESET(); CS_SET()

#endif
