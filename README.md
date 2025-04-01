# ADC7124-8-SDK
透過SPI控制AD7124-8
Ad7124.h設定以STM32晶片為主
#define AD7124_CS_GPIO     GPIOB
#define AD7124_CS_PIN      GPIO_PIN_12
extern SPI_HandleTypeDef hspi2;
可透過修改上述繳未與SPI設定即可以用在其他晶片上。
