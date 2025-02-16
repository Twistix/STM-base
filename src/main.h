#ifndef MAIN_H
#define MAIN_H

#include <string.h>
#include "stm32l1xx_hal.h"
//#include "fatfs/ff.h"

#define SD_SPI_BUS          SPI1
#define SD_SPI_HANDLE       hspi1
#define SD_CS_GPIO_Port     GPIOA
#define SD_CS_Pin           GPIO_PIN_10


/* Externs */
extern SPI_HandleTypeDef SD_SPI_HANDLE;


/* Function prototypes */
void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_SPI1_Init(void);

#endif /* MAIN_H */