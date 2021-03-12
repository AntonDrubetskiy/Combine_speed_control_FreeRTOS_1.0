#include "HC595_ser_trans.h"
#include <string.h>



#define CS_SET(port, pin) 		HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET) // разрешение на запись данных
#define CS_RESET(port, pin) 	HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET) // "защелка" данных с одновременным выводм их на выходы Q0-Q7

typedef enum {
	TRANSMIT_WAIT,
	TRANSMIT_COMPLETE,
	TRANSMIT_ERROR
}spi_tans_t;

/* transfer state */
__IO uint32_t wTransferState = TRANSMIT_WAIT;

static HAL_StatusTypeDef HC595_Set_Symb(SPI_HandleTypeDef *handle, char symb);


hc595_err_t HC595_Set_Column_Symbols(SPI_HandleTypeDef *handle, char *str_clm, int8_t n, GPIO_TypeDef *cs_port, uint16_t cs_pin)
{
	if(n < 0){
		return HC595_NUM_DISP_ERR;
	}
	if(IS_GPIO_PIN(cs_pin) != HC595_CS_PIN_OK){
		return HC595_CS_PIN_ERR;
	}
	if(cs_port == NULL){
		return HC595_CS_PORT_ERR;
	}
	
	static uint8_t i;

	CS_SET(cs_port, cs_pin);
	
	for(i = 0; i < n; i++){
		if(HC595_Set_Symb(handle, str_clm[i]) != HAL_OK){
			return HC595_TRANSMIT_ERR;
		}
		while(wTransferState == TRANSMIT_WAIT)
		{ }
	}
	
	CS_RESET(cs_port, cs_pin);
	
	return HC595_OK;
}




static HAL_StatusTypeDef HC595_Set_Symb(SPI_HandleTypeDef *spi_hndl, char symb)
{
	static uint8_t num;
	switch(symb)
	{
		case '0':
			num=0x3F;
			break;
		
		case '1':
			num=0x6;
			break;
		
		case '2':
			num=0x5B;
			break;
		
		case '3':
			num=0x4F;
			break;
		
		case '4':
			num=0x66;
			break;
		
		case '5':
			num=0x6D;
			break;
		
		case '6':
			num=0x7D;
			break;
		
		case'7':
			num=0x7;
			break;
		
		case '8':
			num=0x7F;
			break;
		
		case '9':
			num=0x6F;
			break;
		
		case 'a':
			num=0x6e;
			break;
		
		case 'b':
			num=0xfe;
			break;
		
		case 'c':
			num=0x72;
			break;
		
		case 'd':
			num=0x7e;
			break;
		
		case 'e':
			num=0xf2;
			break;
		
		case 'f':
			num=0x71;
			break;
		
		case 'g':
			num=0xfa;
			break;
		
		case 'h':
			num=0xec;
			break;
		
		case 'i':
			num=0x60;
			break;
		
		case 'j':
			num=0x1c;
			break;
		
		case 'k':
			num=0x6c;
			break;
		
		case 'l':
			num=0x70;
			break;
		
		case 'm':
			num=0x7e;
			break;
		
		case 'n':
			num=0x6e;
			break;
		
		case 'o':
			num=0x3F;
			break;
		
		case 'p':
			num=0xe6;
			break;
		
		case 'q':
			num=0x5e;
			break;
		
		case 'r':
			num=0x62;
			break;
		
		case 's':
			num=0xda;
			break;
		
		case 't':
			num=0xE;
			break;
		
		case 'u':
			num=0x7c;
			break;
		
		case 'v':
			num=0x5c;
			break;
		
		case 'w':
			num=0xb8;
			break;
		
		case 'x':
			num=0xec;
			break;
		
		case 'y':
			num=0x4c;
			break;
		
		case 'z':
			num=0xb6;
			break;
		
		case '-':
			num=0x40;
			break;
		
		default: // пустой символ
			num=0x00;
			break;
	}
	
	return HAL_SPI_Transmit_DMA(spi_hndl, &num, 1);
}


/**
  * @brief  Tx Transfer completed callback.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	wTransferState = TRANSMIT_COMPLETE;
}

/**
  * @brief  SPI error callbacks.
  * @param  hspi: SPI handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
  wTransferState = TRANSMIT_ERROR;
}