#include "main.h"


/* Globals */
SPI_HandleTypeDef SD_SPI_HANDLE;


/* Main function */
int main(void)
{
    // HAL initialization
    HAL_Init();

    // Configure the system clock
    SystemClock_Config();

    // Initialize GPIOs
    MX_GPIO_Init();

    // Initialize SPI1
    MX_SPI1_Init();

    /* ========================= TESTS ============================== */
//#if 0
    //some variables for FatFs
    FATFS FatFs; 	//Fatfs handle
    FIL fil; 		//File handle
    FRESULT fres; //Result after operations
    BYTE buffer[30];

    //Open the file system
    fres = f_mount(&FatFs, "", 1); //1=mount now

    //Now let's try and write a file "write.txt"
    fres = f_open(&fil, "write.txt", FA_WRITE | FA_OPEN_ALWAYS | FA_CREATE_ALWAYS);

    //Copy in a string
    strncpy((char*)buffer, "a new file is made!", 19);
    UINT bytesWrote;
    fres = f_write(&fil, buffer, 19, &bytesWrote);

    //Be a tidy kiwi - don't forget to close your file!
    f_close(&fil);

    f_mount(NULL, "", 0);
//#endif

    /* =============================================================== */

    // Main loop
    while (1) {}
}


void SystemClock_Config(void) {
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};

    /* Enable HSE Oscillator and Activate PLL with HSE as source */
    RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLMUL          = RCC_PLL_MUL6;
    RCC_OscInitStruct.PLL.PLLDIV          = RCC_PLL_DIV3;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);


    /* Set Voltage scale1 as MCU will run at 32MHz */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    
    /* Poll VOSF bit of in PWR_CSR. Wait until it is reset to 0 */
    while (__HAL_PWR_GET_FLAG(PWR_FLAG_VOS) != RESET) {};

    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
    clocks dividers */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);
}


void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Enable GPIO clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* Configure SD CS pin */
    GPIO_InitStruct.Pin = SD_CS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(SD_CS_GPIO_Port, &GPIO_InitStruct);
    HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_SET);
}


void MX_SPI1_Init(void) {
    SD_SPI_HANDLE.Instance = SD_SPI_BUS;
    SD_SPI_HANDLE.Init.Mode = SPI_MODE_MASTER;  // Master mode
    SD_SPI_HANDLE.Init.Direction = SPI_DIRECTION_2LINES;  // Full-duplex
    SD_SPI_HANDLE.Init.DataSize = SPI_DATASIZE_8BIT;  // 8-bit data
    SD_SPI_HANDLE.Init.CLKPolarity = SPI_POLARITY_LOW;  // CPOL = 0
    SD_SPI_HANDLE.Init.CLKPhase = SPI_PHASE_1EDGE;  // CPHA = 0
    SD_SPI_HANDLE.Init.NSS = SPI_NSS_SOFT;  // Software NSS (manual CS control)
    SD_SPI_HANDLE.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;  // ~400kHz (low-speed init)
    SD_SPI_HANDLE.Init.FirstBit = SPI_FIRSTBIT_MSB;  // MSB first
    SD_SPI_HANDLE.Init.TIMode = SPI_TIMODE_DISABLE;
    SD_SPI_HANDLE.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    HAL_SPI_Init(&SD_SPI_HANDLE);
}
