#include <string.h>
#include "Flash.h"


#define DATA_PAGE_BEGIN       ADDR_FLASH_PAGE_63
#define DATA_PAGE_END         ADDR_FLASH_PAGE_63 + FLASH_PAGE_SIZE

HAL_StatusTypeDef Flash_Write(uint16_t* data, uint8_t mas_len)
{
		HAL_StatusTypeDef ret;
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t Address = 0, PageError = 0;
		uint32_t flash_data;

    ret = HAL_FLASH_Unlock();
    if(ret != HAL_OK) goto exit;

    EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = DATA_PAGE_BEGIN;
    EraseInitStruct.NbPages = (DATA_PAGE_END - DATA_PAGE_BEGIN) / FLASH_PAGE_SIZE;

    ret = HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);
    if(ret != HAL_OK) goto exit;

    Address = DATA_PAGE_BEGIN;
   
		for(int8_t i = 0; i<mas_len; i++)
		{
			flash_data=(uint32_t)data[i];
			
			ret = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)(Address+i*4), flash_data);    
			if(ret != HAL_OK) return ret;
		}
		
    ret = HAL_FLASH_Lock();
		exit:
    return ret;
}

void Flash_Read(uint16_t* data, uint8_t mas_len)
{
    uint32_t *source_adr = (uint32_t *)(DATA_PAGE_BEGIN);
		uint32_t tmp;
	
		for(int8_t i=0; i<mas_len;i++)
		{
			tmp = *(__IO uint32_t*)(source_adr+i);
			data[i] = (uint16_t) tmp;
		}	
}

HAL_StatusTypeDef Flash_Erase(void)
{
		HAL_StatusTypeDef ret;
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t PageError = 0;

    ret = HAL_FLASH_Unlock();
    if(ret != HAL_OK) return ret;

    EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = DATA_PAGE_BEGIN;
    EraseInitStruct.NbPages = (DATA_PAGE_END - DATA_PAGE_BEGIN) / FLASH_PAGE_SIZE;

    ret = HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);
    if(ret != HAL_OK) return ret;

    ret = HAL_FLASH_Lock();
    return ret;
}
