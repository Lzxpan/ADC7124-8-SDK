/*
 * Ad7124.c
 *
 *  Created on: Mar 24, 2025
 *      Author: stevepeng
 */
#include "Ad7124.h"
#include <stdio.h>

/**
 * @brief  AD7124复位   连续写入64个1
 * @param  无
 * @retval 无
 */
void AD7124_Reset(void)
{
    uint8_t pData[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    AD7124_CS_LOW
    HAL_SPI_Transmit(&AD7124_SPI, pData, 8, AD7124_SPI_TIMEOUT);
    AD7124_CS_HIGHT
}



/**
 * @brief   AD7124写寄存器
 * @param   传感器地址
 * @param   写入长度
 * @param   写入数组地址
 * @retval  成功返回0 其他失败
 */
HAL_StatusTypeDef AD7124_Write_Register(uint8_t addr, uint8_t *data, uint16_t data_size)
{
    uint8_t pData[data_size + 1];
    uint8_t ret;
    pData[0] = AD7124_COMM_REG_WEN | AD7124_COMM_REG_WR | AD7124_COMM_REG_RA(addr);
    for (uint16_t i = 1; i <= data_size; i++) {
        pData[i] = data[i - 1];
    }
    AD7124_CS_LOW
    ret = HAL_SPI_Transmit(&AD7124_SPI, pData, (data_size + 1), AD7124_SPI_TIMEOUT);
    while(ret != HAL_OK);
    AD7124_CS_HIGHT
    return ret;
}



/**
 * @brief   AD7124读寄存器
 * @param   传感器地址
 * @param   读取长度
 * @param   存储数组地址
 * @retval  成功返回0 其他失败
 */
HAL_StatusTypeDef AD7124_Read_Register(uint8_t addr, uint8_t *data, uint16_t data_size)
{
    uint8_t pData = AD7124_COMM_REG_WEN | AD7124_COMM_REG_RD | AD7124_COMM_REG_RA(addr);
    uint8_t ret;
    AD7124_CS_LOW
    ret = HAL_SPI_Transmit(&AD7124_SPI, &pData, 1, AD7124_SPI_TIMEOUT);
    while(ret != HAL_OK);

    ret = HAL_SPI_Receive(&AD7124_SPI, data, data_size, AD7124_SPI_TIMEOUT);
    while(ret != HAL_OK);
    AD7124_CS_HIGHT
    return ret;
}



/**
 * @brief  AD7124初始化
 * @param  无
 * @retval 无
 */
void Ad7124_Init(void)
{
    uint8_t pData[3];
    pData[0] = (uint8_t)(0x06C0 >> 8);//07C1
    pData[1] = (uint8_t)0x06C0;
    AD7124_Reset(); // ADC复位
    HAL_Delay(AD7124_DELAY_TIME);
    AD7124_Write_Register(AD7124_ADC_CTRL_REG, pData, 2); // 配置ADC控制寄存器
    HAL_Delay(AD7124_DELAY_TIME);

    //設定腳位與通道
    pData[0] = (uint8_t)(0x8093 >> 8);//80F3
    pData[1] = (uint8_t)0x8093;
    AD7124_Write_Register(AD7124_CH0_MAP_REG, pData, 2); // 配置通道寄存器，AIN4 CH0
	HAL_Delay(AD7124_DELAY_TIME);

	pData[0] = (uint8_t)(0x80D3 >> 8);//80F3
	pData[1] = (uint8_t)0x80D3;
	AD7124_Write_Register(AD7124_CH1_MAP_REG, pData, 2); // 配置通道寄存器，AIN6 CH1
	HAL_Delay(AD7124_DELAY_TIME);

	pData[0] = (uint8_t)(0x80F3 >> 8);//80F3
	pData[1] = (uint8_t)0x80F3;
	AD7124_Write_Register(AD7124_CH2_MAP_REG, pData, 2); // 配置通道寄存器，AIN7 CH2
	HAL_Delay(AD7124_DELAY_TIME);
	//通道設定結尾
    pData[0] = (uint8_t)(0x0140 >> 8);//0050
    pData[1] = (uint8_t)0x0140;
    AD7124_Write_Register(AD7124_CFG0_REG, pData, 2); // 配置配置寄存器
    HAL_Delay(AD7124_DELAY_TIME);
    pData[0] = (uint8_t)(0x160180 >> 16);//000013
    pData[1] = (uint8_t)(0x160180 >> 8);
    pData[2] = (uint8_t)(0x160180);
    AD7124_Write_Register(AD7124_FILT0_REG, pData, 3); // 配置滤波器寄存器
    HAL_Delay(AD7124_DELAY_TIME);
}

/**
 * @brief  AD7124读取ADC值
 * @param  无
 * @retval 无
 */
uint32_t Ad7124_Read_Adc(void)
{
    uint8_t rData[4];
    uint32_t rData32;
    //float ADC_Data;

    AD7124_Read_Register(AD7124_DATA_REG, rData, 4);
    //rData32  = (rData[0] << 24) | (rData[1] << 16) | rData[2] << 8 | rData[3];
    rData32  = (rData[0] << 24) | (rData[1] << 16) | (rData[2] << 8) | rData[3];
    //ADC_Data = (rData32 * 3.3) / 16777215;
    //printf("%f\r\n", ADC_Data);
    //return ADC_Data;
    return rData32;
}

/**
 * @brief  AD7124電壓值轉換，使用此函式搭配FreeRTOS，因為回傳浮點數，所以task的記憶體(stack_size)要開到256以上，以免出錯
 * @param  无
 * @retval 无
 */
float AD7124_Conversion(uint32_t rData32)
{
	float ADC_Data;
	rData32 = rData32 >> 8;
	ADC_Data = (rData32 * 3.3) / 16777215;
	return ADC_Data;
}

/**
 * @brief  AD7124取得當前通道
 * @param  无
 * @retval 无
 */

uint8_t AD7124_GetCH(uint32_t rData32)
{
	rData32 = rData32 & 0x03;
	switch(rData32)
	{
		case 0 :
			return 0x00;
			break;

		case 1 :
			return 0x01;
			break;

		case 2 :
			return 0x02;
			break;

		case 3 :
			return 0x03;
			break;

		case 4 :
			return 0x04;
			break;

		case 5 :
			return 0x05;
			break;

		case 6 :
			return 0x06;
			break;

		case 7 :
			return 0x07;
			break;

		case 8 :
			return 0x08;
			break;

		default :
			return 0xff;
			break;
	}
}
