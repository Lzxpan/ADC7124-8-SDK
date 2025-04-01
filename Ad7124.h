#ifndef __AD7124_H__
#define __AD7124_H__
 
#include "main.h"
/* AD7124 Register Map */
#define AD7124_COMM_REG     0x00
#define AD7124_STATUS_REG   0x00
#define AD7124_ADC_CTRL_REG 0x01
#define AD7124_DATA_REG     0x02
#define AD7124_IO_CTRL1_REG 0x03
#define AD7124_IO_CTRL2_REG 0x04
#define AD7124_ID_REG       0x05
#define AD7124_ERR_REG      0x06
#define AD7124_ERREN_REG    0x07
#define AD7124_CH0_MAP_REG  0x09
#define AD7124_CH1_MAP_REG  0x0A
#define AD7124_CH2_MAP_REG  0x0B
#define AD7124_CH3_MAP_REG  0x0C
#define AD7124_CH4_MAP_REG  0x0D
#define AD7124_CH5_MAP_REG  0x0E
#define AD7124_CH6_MAP_REG  0x0F
#define AD7124_CH7_MAP_REG  0x10
#define AD7124_CH8_MAP_REG  0x11
#define AD7124_CH9_MAP_REG  0x12
#define AD7124_CH10_MAP_REG 0x13
#define AD7124_CH11_MAP_REG 0x14
#define AD7124_CH12_MAP_REG 0x15
#define AD7124_CH13_MAP_REG 0x16
#define AD7124_CH14_MAP_REG 0x17
#define AD7124_CH15_MAP_REG 0x18
#define AD7124_CFG0_REG     0x19
#define AD7124_CFG1_REG     0x1A
#define AD7124_CFG2_REG     0x1B
#define AD7124_CFG3_REG     0x1C
#define AD7124_CFG4_REG     0x1D
#define AD7124_CFG5_REG     0x1E
#define AD7124_CFG6_REG     0x1F
#define AD7124_CFG7_REG     0x20
#define AD7124_FILT0_REG    0x21
#define AD7124_FILT1_REG    0x22
#define AD7124_FILT2_REG    0x23
#define AD7124_FILT3_REG    0x24
#define AD7124_FILT4_REG    0x25
#define AD7124_FILT5_REG    0x26
#define AD7124_FILT6_REG    0x27
#define AD7124_FILT7_REG    0x28
#define AD7124_OFFS0_REG    0x29
#define AD7124_OFFS1_REG    0x2A
#define AD7124_OFFS2_REG    0x2B
#define AD7124_OFFS3_REG    0x2C
#define AD7124_OFFS4_REG    0x2D
#define AD7124_OFFS5_REG    0x2E
#define AD7124_OFFS6_REG    0x2F
#define AD7124_OFFS7_REG    0x30
#define AD7124_GAIN0_REG    0x31
#define AD7124_GAIN1_REG    0x32
#define AD7124_GAIN2_REG    0x33
#define AD7124_GAIN3_REG    0x34
#define AD7124_GAIN4_REG    0x35
#define AD7124_GAIN5_REG    0x36
#define AD7124_GAIN6_REG    0x37
#define AD7124_GAIN7_REG    0x38
#define AD7124_REGISTER_NUM 57
 
/* Communication Register bits */
#define AD7124_COMM_REG_WEN   (0 << 7)
#define AD7124_COMM_REG_WR    (0 << 6)
#define AD7124_COMM_REG_RD    (1 << 6)
#define AD7124_COMM_REG_RA(x) ((x) & 0x3F)
 
#define AD7124_SPI_TIMEOUT 20
#define AD7124_DELAY_TIME  100
 
#define AD7124_CS_GPIO     GPIOB
#define AD7124_CS_PIN      GPIO_PIN_12
 
extern SPI_HandleTypeDef hspi2;
#define AD7124_SPI         hspi2
 
#define AD7124_CS_LOW      HAL_GPIO_WritePin(AD7124_CS_GPIO, AD7124_CS_PIN, GPIO_PIN_RESET);
#define AD7124_CS_HIGHT    HAL_GPIO_WritePin(AD7124_CS_GPIO, AD7124_CS_PIN, GPIO_PIN_SET);
 
void AD7124_Reset(void);
HAL_StatusTypeDef AD7124_Read_Register(uint8_t addr, uint8_t *data, uint16_t data_size);
HAL_StatusTypeDef AD7124_Write_Register(uint8_t addr, uint8_t *data, uint16_t data_size);
void Ad7124_Init(void);
uint32_t Ad7124_Read_Adc(void);
float AD7124_Conversion(uint32_t rData32);
uint8_t AD7124_GetCH(uint32_t rData32);
 
#endif /* __AD7124_H__ */
