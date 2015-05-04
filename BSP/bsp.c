/*
*********************************************************************************************************
*                                     MICIRUM BOARD SUPPORT PACKAGE
*
*                             (c) Copyright 2012; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                        BOARD SUPPORT PACKAGE
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                           STM3220G-EVAL
*                                         Evaluation Board
*
* Filename      : bsp.c
* Version       : V1.00
* Programmer(s) : SL
*                 DC
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define   BSP_MODULE
#include "includes.h"


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart4, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}

/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/

#define LEDn                        4


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/




/*
*********************************************************************************************************
*                                             REGISTERS
*********************************************************************************************************
*/

#define  DWT_CR      *(CPU_REG32 *)0xE0001000
#define  DWT_CYCCNT  *(CPU_REG32 *)0xE0001004
#define  DEM_CR      *(CPU_REG32 *)0xE000EDFC


/*
*********************************************************************************************************
*                                            REGISTER BITS
*********************************************************************************************************
*/

#define  DEM_CR_TRCENA                   (1 << 24)

#define  DWT_CR_CYCCNTENA                (1 <<  0)


SPI_HandleTypeDef SpiHandle;

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/



/**
  * @brief  Sets the selected data port bits.
  * @note   This functions uses GPIOx_BSRR register to allow atomic read/modify 
  *         accesses. In this way, there is no risk of an IRQ occurring between
  *         the read and the modify access.
  * @param  GPIOx: where x can be (A..I) to select the GPIO peripheral.
  * @param  GPIO_Pin: specifies the port bits to be written.
  *          This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
  * @retval None
  */
void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  /* Check the parameters */
  GPIOx->BSRRL = GPIO_Pin;
}

/**
  * @brief  Clears the selected data port bits.
  * @note   This functions uses GPIOx_BSRR register to allow atomic read/modify 
  *         accesses. In this way, there is no risk of an IRQ occurring between
  *         the read and the modify access.
  * @param  GPIOx: where x can be (A..I) to select the GPIO peripheral.
  * @param  GPIO_Pin: specifies the port bits to be written.
  *          This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
  * @retval None
  */
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  GPIOx->BSRRH = GPIO_Pin;
}



/** @addtogroup STM32F2xx_HAL_Examples
  * @{
  */

/** @defgroup UART_HyperTerminal_DMA
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
  * @brief SPI MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  *           - DMA configuration for transmission request by peripheral 
  *           - NVIC configuration for DMA interrupt request enable
  * @param hspi: SPI handle pointer
  * @retval None
  */
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef  GPIO_InitStruct;
	
  if (hspi->Instance == SPI2)
  {
    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    /* Enable GPIO TX/RX clock */

    /* Enable SPI3 clock */
    SPIx_CLK_ENABLE();
    
    /* Enable DMA1 clock */
    //DMAx_CLK_ENABLE();

    /*##-2- Configure peripheral GPIO ##########################################*/
    /* SPI SCK GPIO pin configuration  */
    GPIO_InitStruct.Pin       = SPIx_SCK_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = SPIx_SCK_AF;

    HAL_GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);

    /* SPI MISO GPIO pin configuration  */
    GPIO_InitStruct.Pin = SPIx_MISO_PIN;
    GPIO_InitStruct.Alternate = SPIx_MISO_AF;

    HAL_GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStruct);

    /* SPI MOSI GPIO pin configuration  */
    GPIO_InitStruct.Pin = SPIx_MOSI_PIN;
    GPIO_InitStruct.Alternate = SPIx_MOSI_AF;

    HAL_GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin       = GPIO_PIN_12;
    GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;

    HAL_GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);

    //rf_sdn pin
    GPIO_InitStruct.Pin       = GPIO_PIN_15;
    GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;

    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    //rf_int pin
    GPIO_InitStruct.Pin       = GPIO_PIN_14;
    GPIO_InitStruct.Mode      = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    //power enable pin
    GPIO_InitStruct.Pin       = GPIO_PIN_10;
    GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;

    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 2);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

    /*##-3- Configure the NVIC for SPI #########################################*/
    /* NVIC for SPI */
    //HAL_NVIC_SetPriority(SPIx_IRQn, 0, 1);
    //HAL_NVIC_EnableIRQ(SPIx_IRQn);
  }
  else if(SPI1 == hspi->Instance)
  {
    /* USER CODE BEGIN SPI1_MspInit 0 */

    /* USER CODE END SPI1_MspInit 0 */
    /* Peripheral clock enable */
    __SPI1_CLK_ENABLE();

    /**SPI1 GPIO Configuration    
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USER CODE BEGIN SPI1_MspInit 1 */

    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USER CODE END SPI1_MspInit 1 */
  }  
}

// ------------------------------------- 华兄 ------------------------------------- //

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  
  if(huart->Instance==UART4)
  {
  /* USER CODE BEGIN UART4_MspInit 0 */

  /* USER CODE END UART4_MspInit 0 */
    /* Peripheral clock enable */
    __UART4_CLK_ENABLE();
  
    /**UART4 GPIO Configuration    
    PA0-WKUP     ------> UART4_TX
    PA1     ------> UART4_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* System interrupt init*/
    HAL_NVIC_SetPriority(UART4_IRQn, 1, 2);
    HAL_NVIC_EnableIRQ(UART4_IRQn);
  /* USER CODE BEGIN UART4_MspInit 1 */

  /* USER CODE END UART4_MspInit 1 */
  }
  else if(huart->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* Peripheral clock enable */
    __USART1_CLK_ENABLE();
  
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* System interrupt init*/
    HAL_NVIC_SetPriority(USART1_IRQn, 1, 2);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
  else if(huart->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* Peripheral clock enable */
    __USART2_CLK_ENABLE();
  
    /**USART2 GPIO Configuration    
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* System interrupt init*/
    HAL_NVIC_SetPriority(USART2_IRQn, 1, 2);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
}

/* UART4 init function */
void UART4_Init(void)
{
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 19200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  
  HAL_UART_Init(&huart4);
}

/* USART1 init function */
void USART1_UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  
  HAL_UART_Init(&huart1);
}

/* USART2 init function */
void USART2_UART_Init(void)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_9B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_EVEN;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  
  HAL_UART_Init(&huart2);
}

void BSP_UART_Init(void)
{
    UART4_Init();
    USART1_UART_Init();
    USART2_UART_Init();

	BSP_IntVectSet(BSP_INT_ID_USART4, UART4_IRQHandler);
	BSP_IntEn(BSP_INT_ID_USART4);    

	BSP_IntVectSet(BSP_INT_ID_USART1, USART1_IRQHandler);
	BSP_IntEn(BSP_INT_ID_USART1);    

	BSP_IntVectSet(BSP_INT_ID_USART2, USART2_IRQHandler);
	BSP_IntEn(BSP_INT_ID_USART2);    
}

// ----------------------------------------------------------------------------- //

/*
*********************************************************************************************************
*                                               BSP_Init()
*
* Description : Initialize the Board Support Package (BSP).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) This function SHOULD be called before any other BSP function is called.
*
*               (2) CPU instruction / data tracing requires the use of the following pins :
*                   (a) (1) Aysynchronous     :  PB[3]
*                       (2) Synchronous 1-bit :  PE[3:2]
*                       (3) Synchronous 2-bit :  PE[4:2]
*                       (4) Synchronous 4-bit :  PE[6:2]
*
*                   (c) The application may wish to adjust the trace bus width depending on I/O
*                       requirements.
*********************************************************************************************************
*/

void  BSP_Init (void)
{
    /* At this stage the microcontroller clock setting is already configured,
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f2xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f2xx.c file
     */


    BSP_NVIC_Configuration();

	BSP_RCC_Configuration();
	
    BSP_GPIO_Configuration();

    LED_Off(LED_RED);
    LED_Off(LED_GREEN);
	
	BSP_LCD_init();

	BSP_Keyboard_Init();

    BSP_SPI_Init();

    BSP_ADC_Init();

    BSP_UART_Init();

    //BSP_IWDG_Init();  //4.1

    PLC_PWR_ON();

    KEY_LED_OFF();
}

void BSP_RCC_Configuration(void)
{
    //SetSysClockTo72();

    /* Enable the GPIO Clock */
    //RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG, ENABLE);  
    __GPIOA_CLK_ENABLE();
    __GPIOB_CLK_ENABLE();
    __GPIOC_CLK_ENABLE();
    __GPIOD_CLK_ENABLE();
    __GPIOE_CLK_ENABLE();
    __GPIOF_CLK_ENABLE();
    __GPIOG_CLK_ENABLE();

    __FSMC_CLK_ENABLE();
    __CRC_CLK_ENABLE();    

    /* DMA controller clock enable */
    __DMA1_CLK_ENABLE();
    __DMA2_CLK_ENABLE();    

    /* Enable the FSMC Clock */
    //RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE); 

    //	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);

    /* Enable SYSCFG clock */
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);    
}


void BSP_NVIC_Configuration(void)
{
    /* Configure one bit for preemption priority */
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_1);
}



void BSP_GPIO_Configuration(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure; 
    
       
    GPIO_InitStructure.Pin = GPIO_PIN_0 | GPIO_PIN_1; 
    //GPIO_InitStructure.Mode =  GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Mode =  GPIO_MODE_INPUT;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Pull  = GPIO_PULLUP;  
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

    /*##-2- Configure peripheral GPIO ##########################################*/
    /* UART TX GPIO pin configuration  */
    GPIO_InitStructure.Pin       = GPIO_PIN_2;
    GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Pull      = GPIO_PULLUP;
    GPIO_InitStructure.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStructure.Alternate = GPIO_AF7_USART2;

    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* UART RX GPIO pin configuration  */
    GPIO_InitStructure.Pin = GPIO_PIN_3;
    GPIO_InitStructure.Alternate = GPIO_AF7_USART2;

    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

        /* RTC */
    GPIO_InitStructure.Pin = GPIO_PIN_5| GPIO_PIN_6 | GPIO_PIN_7; 
    GPIO_InitStructure.Mode =  GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Pull  = GPIO_PULLUP; 
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);   
    //HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);  //
    //SYS_PWR_ON();

     
	 //PB0模拟输入，读取电压 
    GPIO_InitStructure.Pin=GPIO_PIN_0;
    GPIO_InitStructure.Mode=GPIO_MODE_ANALOG;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB,&GPIO_InitStructure);


    /* UART TX GPIO pin configuration  */

    GPIO_InitStructure.Pin       = GPIO_PIN_10;
    GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Pull      = GPIO_PULLUP;
    GPIO_InitStructure.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStructure.Alternate = GPIO_AF8_UART4;

    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);


    /* UART RX GPIO pin configuration  */
    GPIO_InitStructure.Pin = GPIO_PIN_11;
    GPIO_InitStructure.Alternate = GPIO_AF8_UART4;

    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Configure the GPIO pin */

    GPIO_InitStructure.Pin = GPIO_PIN_13; 
    GPIO_InitStructure.Mode =  GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Pull  = GPIO_PULLUP; 
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);     
    
    /* NE2 Configuration */
    GPIO_InitStructure.Pin = GPIO_PIN_9; 
    GPIO_InitStructure.Mode =  GPIO_MODE_AF_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Alternate = GPIO_AF12_FSMC;
    GPIO_InitStructure.Pull  = GPIO_NOPULL; 
    HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);


 
	/* Configure the GPIO pin */
    GPIO_InitStructure.Pin = GPIO_PIN_8; 
    GPIO_InitStructure.Mode =  GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Pull  = GPIO_PULLUP; 
    HAL_GPIO_Init(GPIOG, &GPIO_InitStructure); 

    GPIO_InitStructure.Pin = GPIO_PIN_7; 
    GPIO_InitStructure.Mode =  GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    //GPIO_InitStructure.Pull  = GPIO_NOPULL; 
    HAL_GPIO_Init(GPIOG, &GPIO_InitStructure); 

	/* Configure the PLC_PWR pin */
    GPIO_InitStructure.Pin = GPIO_PIN_6|GPIO_PIN_5; 
    GPIO_InitStructure.Mode =  GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Pull  = GPIO_PULLUP; 
    HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);     


}



/*
*********************************************************************************************************
*                                               BSP_RNG_Read()
*
* Description : Read the value of the RNG.
*
* Argument(s) : none.
*
* Return(s)   : ????.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_INT32U  BSP_RNG_Read (void)
{
    CPU_INT32U  rng_val;


    //RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE);         /* Enable RNG clock.                                    */
    __RNG_CLK_ENABLE();

    RNG->CR |= RNG_CR_IE;                                   /* Enable RNG ISR.                                      */
    RNG->CR |= RNG_CR_RNGEN;                                /* Enable LFSR & err detector.                          */

    while ((RNG->SR & RNG_SR_DRDY) == 0) {
            ;
    }
    rng_val = (CPU_INT32U)RNG->DR;                          /* Save RNG data reg.                                   */

    RNG->CR &= ~RNG_CR_IE;                                  /* Disable RNG ISR.                                     */
    RNG->CR &= ~RNG_CR_RNGEN;                               /* Disable LFSR & err detector.                         */
    //RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, DISABLE);        /* Disable RNG clock.                                   */
    __RNG_CLK_DISABLE();

    return (rng_val);                                           /* Return rand value.                                   */
}


/*
*********************************************************************************************************
*                                            BSP_CPU_ClkFreq()
*
* Description : Read CPU registers to determine the CPU clock frequency of the chip.
*
* Argument(s) : none.
*
* Return(s)   : The CPU clock frequency, in Hz.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_INT32U  BSP_CPU_ClkFreq (void)
{
    return HAL_RCC_GetHCLKFreq();
}


/*
*********************************************************************************************************
*                                       BSP_CPU_TickInit()
*
* Description : This function reads CPU registers to determine the CPU clock frequency of the chip in KHz.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_CPU_TickInit  (void)
{
    CPU_INT32U  cpu_clk_freq;
    CPU_INT32U  cnts;


    cpu_clk_freq = BSP_CPU_ClkFreq();                           /* Determine SysTick reference freq.                    */
#if (OS_VERSION >= 30000u)
    cnts         = (cpu_clk_freq / OSCfg_TickRate_Hz);          /* Determine nbr SysTick increments                     */
#else
    cnts         = (cpu_clk_freq / OS_TICKS_PER_SEC);
#endif
    OS_CPU_SysTickInit(cnts);                                   /* Initialize the SysTick.                            */
}



void BSP_Keyboard_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;


    
    /* Configure Button pin as input */
    GPIO_InitStructure.Pin= GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStructure.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Pull= GPIO_NOPULL;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);

    HAL_NVIC_SetPriority(EXTI9_5_IRQn, EXTI9_5_PREEM_PRIO, EXTI9_5_SUB_PRIO);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    GPIO_InitStructure.Pin= GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull= GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_ResetBits(GPIOC, 
        GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5);

    // -------------------------------------------------
}



IWDG_HandleTypeDef IwdgHandle;

/* IWDG init function */
void BSP_IWDG_Init(void)
{
  /* Enable the LSI oscillator */
  __HAL_RCC_LSI_ENABLE();
  
  /* Wait till LSI is ready */
  while (__HAL_RCC_GET_FLAG(RCC_FLAG_LSIRDY) == RESET)
  {
  }
  
  IwdgHandle.Instance = IWDG;
  IwdgHandle.Init.Prescaler = IWDG_PRESCALER_32;
  IwdgHandle.Init.Reload = 1000;
  HAL_IWDG_Init(&IwdgHandle);

  if(HAL_IWDG_Start(&IwdgHandle) != HAL_OK)
  {
  }
}


/*
*********************************************************************************************************
*                                          BSP_SRAM_Init()
*
* Description : Initialize SRAM.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/


void BSP_LCD_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    SRAM_HandleTypeDef hsram;
    FSMC_NORSRAM_TimingTypeDef SRAM_Timing, Extend_SRAM_Timing;   

    //HAL_SRAM_DeInit(&hsram);

    /* Set PD.00(D2), PD.01(D3), PD.04(NOE), PD.05(NWE), PD.08(D13), PD.09(D14),
       PD.10(D15), PD.14(D0), PD.15(D1) as alternate function push pull */
    GPIO_InitStructure.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5
                            |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_14
                            |GPIO_PIN_15;
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Pull= GPIO_NOPULL;
    GPIO_InitStructure.Alternate = GPIO_AF12_FSMC;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    /* Set PE.07(D4), PE.08(D5), PE.09(D6), PE.10(D7), PE.11(D8), PE.12(D9), PE.13(D10),
       PE.14(D11), PE.15(D12) as alternate function push pull */
    GPIO_InitStructure.Pin= GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10
                            |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14
                            |GPIO_PIN_15;
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Pull= GPIO_NOPULL;
    GPIO_InitStructure.Alternate = GPIO_AF12_FSMC;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);
    
    /* Set PF.00(A0 (RS)) as alternate function push pull */
    GPIO_InitStructure.Pin = GPIO_PIN_0;
    GPIO_InitStructure.Alternate = GPIO_AF12_FSMC;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);
    
    /* Set PG.9(NE2 (LCD/CS)) as alternate function push pull */
    GPIO_InitStructure.Pin = GPIO_PIN_9;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);

    /* Set PWR/BL GPIO */ /* Set RST GPIO */
    GPIO_InitStructure.Pin = GPIO_PIN_7 | GPIO_PIN_8 |GPIO_PIN_9;
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull= GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_SetBits(GPIOC, GPIO_PIN_7 | GPIO_PIN_8);
    
    GPIO_InitStructure.Pin = GPIO_PIN_9;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

                                                                            /*** Configure the SRAM Bank 3 ***/  
    /* Configure IPs */
    hsram.Instance  = FMC_NORSRAM_DEVICE;
    hsram.Extended  = FMC_NORSRAM_EXTENDED_DEVICE;
    

#if 1
    SRAM_Timing.AddressSetupTime      = 2;
    SRAM_Timing.AddressHoldTime       = 0;
    SRAM_Timing.DataSetupTime         = 9;
    SRAM_Timing.BusTurnAroundDuration = 0;
    SRAM_Timing.CLKDivision           = 0;
    SRAM_Timing.DataLatency           = 0;
    SRAM_Timing.AccessMode            = FSMC_ACCESS_MODE_A;
#endif


#if 0
    SRAM_Timing.AddressSetupTime = 0x01;
    SRAM_Timing.AddressHoldTime = 0x00;
    SRAM_Timing.DataSetupTime = 0x0f;
    SRAM_Timing.BusTurnAroundDuration = 0x00;
    SRAM_Timing.CLKDivision = 0x00;
    SRAM_Timing.DataLatency = 0x00;
    SRAM_Timing.AccessMode = FSMC_ACCESS_MODE_A;
  
	Extend_SRAM_Timing.AddressSetupTime = 0x00;
    Extend_SRAM_Timing.AddressHoldTime = 0x00;
    Extend_SRAM_Timing.DataSetupTime = 0x03;
    Extend_SRAM_Timing.BusTurnAroundDuration = 0x00;
    Extend_SRAM_Timing.CLKDivision = 0x00;
    Extend_SRAM_Timing.DataLatency = 0x00;
    Extend_SRAM_Timing.AccessMode = FSMC_ACCESS_MODE_A;
#endif
    


    hsram.Init.NSBank             = FSMC_NORSRAM_BANK2;
    hsram.Init.DataAddressMux     = FSMC_DATA_ADDRESS_MUX_DISABLE;
    hsram.Init.MemoryType         = FSMC_MEMORY_TYPE_SRAM;
    hsram.Init.MemoryDataWidth    = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
    hsram.Init.BurstAccessMode    = FSMC_BURST_ACCESS_MODE_DISABLE;
    hsram.Init.AsynchronousWait   = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
    hsram.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
    hsram.Init.WrapMode           = FSMC_WRAP_MODE_DISABLE;
    hsram.Init.WaitSignalActive   = FSMC_WAIT_TIMING_BEFORE_WS;
    hsram.Init.WriteOperation     = FSMC_WRITE_OPERATION_ENABLE;
    hsram.Init.WaitSignal         = FSMC_WAIT_SIGNAL_DISABLE;
    hsram.Init.ExtendedMode       = FSMC_EXTENDED_MODE_DISABLE;
    hsram.Init.WriteBurst         = FSMC_WRITE_BURST_DISABLE;

    HAL_SRAM_Init(&hsram, &SRAM_Timing, &Extend_SRAM_Timing);                                           

}

SPI_HandleTypeDef hspi1;

/* SPI1 init function */
void SPI1_Init(void)
{
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLED;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
  HAL_SPI_Init(&hspi1);
}

void BSP_SPI_Init()
{
    SpiHandle.Instance               = SPI2;
    SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
    SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
    SpiHandle.Init.CLKPhase          = SPI_PHASE_1EDGE;
    SpiHandle.Init.CLKPolarity       = SPI_POLARITY_LOW;
    SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLED;
    SpiHandle.Init.CRCPolynomial     = 7;
    SpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
    SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    SpiHandle.Init.NSS               = SPI_NSS_SOFT;
    SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLED;
    SpiHandle.Init.Mode              = SPI_MODE_MASTER;  
    
    if(HAL_SPI_Init(&SpiHandle) != HAL_OK)
    {
        /* Initialization Error */
        //Error_Handler();
    }
}





/********************************************************************************
*
*
*ADC初始化
*
*
*********************************************************************************/

ADC_HandleTypeDef AdcHandle;
ADC_ChannelConfTypeDef sConfig;
__IO CPU_INT32U uhADCxConvertedValue = 0;//ADC读取的电压值


void BSP_ADC_Init(void)
{
 
    //GPIO_InitTypeDef GPIO_InitStructure;

    __ADC2_CLK_ENABLE();

    /*##-1- Configure the ADC peripheral #######################################*/
    AdcHandle.Instance          = ADC2;

    AdcHandle.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV2;//分频
    AdcHandle.Init.Resolution            = ADC_RESOLUTION12b;//分辨率
    AdcHandle.Init.ScanConvMode          = DISABLE;//扫描模式
    AdcHandle.Init.ContinuousConvMode    = DISABLE;//连续转换模式
    AdcHandle.Init.DiscontinuousConvMode = DISABLE;//间断模式
    AdcHandle.Init.NbrOfDiscConversion   = 0;
    AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
    AdcHandle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;//外部触发转换
    AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    AdcHandle.Init.NbrOfConversion       = 1;
    AdcHandle.Init.DMAContinuousRequests = DISABLE;
    AdcHandle.Init.EOCSelection          = DISABLE;

    HAL_ADC_Init(&AdcHandle);

    /*##-2- Configure ADC regular channel ######################################*/
    sConfig.Channel      = ADC_CHANNEL_8;
    sConfig.Rank         = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    sConfig.Offset       = 0;

    HAL_ADC_ConfigChannel(&AdcHandle, &sConfig);
}

CPU_INT32U BSP_ADC_ReadPwr(void)
{
    int i ;

    uhADCxConvertedValue = 0;
    //for(i=0 ; i< 10 ; i++)
    {
         HAL_ADC_Start(&AdcHandle);
         
      
        /*##-4- Wait for the end of conversion #####################################*/
        /*  Before starting a new conversion, you need to check the current state of
             the peripheral; if it's busy you need to wait for the end of current
             conversion before starting a new one.
             For simplicity reasons, this example is just waiting till the end of the
             conversion, but application may perform other tasks while conversion
             operation is ongoing. */
        HAL_ADC_PollForConversion(&AdcHandle, 10);
      
        /* Check if the continous conversion of regular channel is finished */
        
        
        if (HAL_ADC_GetState(&AdcHandle) == HAL_ADC_STATE_EOC_REG)
        {
          /*##-5- Get the converted value of regular channel  ########################*/
          uhADCxConvertedValue = HAL_ADC_GetValue(&AdcHandle);
        }   
    }

    //uhADCxConvertedValue = uhADCxConvertedValue/10;
    
    return  uhADCxConvertedValue ;//ADC读取的电压值


}



//蜂鸣器
void BSP_BEEP(void)
{
    int i = 50;
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_1;
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    while(i--)
    {
        GPIO_SetBits(GPIOB, GPIO_PIN_1);
        OSTimeDlyHMSM(0, 0, 0, 1);            
        GPIO_ResetBits(GPIOB, GPIO_PIN_1);
        OSTimeDlyHMSM(0, 0, 0, 1);            
        
    }
}

CPU_INT32U BSP_Charg(void)
{
    return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9);
}


static CPU_INT32U lcd_type;

CPU_INT08U BSP_LCD_Check(void)
{
    lcd_delay(50); //Delay 50ms

    LCD_PWR_ON();
    LCD_BL_ON();
    
    LCD_RST_LOW(); 
    GUI_Delay(150); //Delay 
    LCD_RST_HIGH();
	//lcd_delay(150); //Delay 
	GUI_Delay(150); 
    
    LCD_WriteReg(0x00, 0x0001);
	//lcd_delay(150); //Delay 
	GUI_Delay(150); 

    lcd_type = LCD_ReadReg(0x00);
       
       
       
   if((lcd_type < 0xFF) || (0xFFFF == lcd_type) || (0x9300 == lcd_type))
   {       
       LCD_WriteCmd(0xD3); //ID                  
       LCD_ReadData(); //Dummy
       LCD_ReadData(); //0x00
       lcd_type = LCD_ReadData(); //0x93                              
       lcd_type <<= 8;
       lcd_type |= LCD_ReadData(); //0x41              
   }

   if(0x9341 == lcd_type)
   {
        return LCD_OK;
   }
   else
        return LCD_ERR;

}


CPU_INT08U BSP_Si4438_Check(void)
{


}

CPU_INT08U BSP_PLC_Check(void)
{


}




/*
*********************************************************************************************************
*                                             LED_On()
*
* Description : Turn ON any or all the LEDs on the board.
*
* Argument(s) : led     The ID of the LED to control:
*
*                       0    turns ON ALL  LEDs
*                       1    turns ON user LED1
*                       2    turns ON user LED2
*                       3    turns ON user LED3
*                       4    turns ON user LED4
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  LED_On (CPU_INT08U  led)
{
    switch(led)
    {
    case LED_RED:
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8, GPIO_PIN_RESET);
        break;

    case LED_GREEN:
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
        break;

    default:
        break;
    }
}

/*
*********************************************************************************************************
*                                              LED_Off()
*
* Description : Turn OFF any or all the LEDs on the board.
*
* Argument(s) : led     The ID of the LED to control:
*
*                       0    turns OFF ALL the LEDs
*                       1    turns OFF user LED1
*                       2    turns OFF user LED2
*                       3    turns OFF user LED3
*                       4    turns OFF user LED4
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  LED_Off (CPU_INT08U led)
{   
    switch(led)
    {
    case LED_RED:
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8, GPIO_PIN_SET);
        break;

    case LED_GREEN:
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
        break;

    default:
        break;
    }    
}

/*
*********************************************************************************************************
*                                            LED_Toggle()
*
* Description : TOGGLE any or all the LEDs on the board.
*
* Argument(s) : led     The ID of the LED to control:
*
*                       0    TOGGLE ALL the LEDs
*                       1    TOGGLE user LED1
*                       2    TOGGLE user LED2
*                       3    TOGGLE user LED3
*                       4    TOGGLE user LED4
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  LED_Toggle (CPU_INT08U  led)
{
    switch(led)
    {
    case LED_RED:
        HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_8);
        break;

    case LED_GREEN:
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        break;

    default:
        break;
    }        
}

#define NEW_POWER_VER_E6

void PLC_PWR_ON(void)
{
    //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);

#ifdef  NEW_POWER_VER_E6  
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET);  //PLC POWER_ON
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_SET);  //12v
#else    
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_RESET);
#endif

}

void PLC_PWR_OFF(void)
{
#ifdef NEW_POWER_VER_E6
    
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET);  //PLC POWER_ON
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_RESET);  //12v
#else
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_SET);
#endif
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                          CPU_TS_TmrInit()
*
* Description : Initialize & start CPU timestamp timer.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : CPU_TS_Init().
*
*               This function is an INTERNAL CPU module function & MUST be implemented by application/
*               BSP function(s) [see Note #1] but MUST NOT be called by application function(s).
*
* Note(s)     : (1) CPU_TS_TmrInit() is an application/BSP function that MUST be defined by the developer
*                   if either of the following CPU features is enabled :
*
*                   (a) CPU timestamps
*                   (b) CPU interrupts disabled time measurements
*
*                   See 'cpu_cfg.h  CPU TIMESTAMP CONFIGURATION  Note #1'
*                     & 'cpu_cfg.h  CPU INTERRUPTS DISABLED TIME MEASUREMENT CONFIGURATION  Note #1a'.
*
*               (2) (a) Timer count values MUST be returned via word-size-configurable 'CPU_TS_TMR'
*                       data type.
*
*                       (1) If timer has more bits, truncate timer values' higher-order bits greater
*                           than the configured 'CPU_TS_TMR' timestamp timer data type word size.
*
*                       (2) Since the timer MUST NOT have less bits than the configured 'CPU_TS_TMR'
*                           timestamp timer data type word size; 'CPU_CFG_TS_TMR_SIZE' MUST be
*                           configured so that ALL bits in 'CPU_TS_TMR' data type are significant.
*
*                           In other words, if timer size is not a binary-multiple of 8-bit octets
*                           (e.g. 20-bits or even 24-bits), then the next lower, binary-multiple
*                           octet word size SHOULD be configured (e.g. to 16-bits).  However, the
*                           minimum supported word size for CPU timestamp timers is 8-bits.
*
*                       See also 'cpu_cfg.h   CPU TIMESTAMP CONFIGURATION  Note #2'
*                              & 'cpu_core.h  CPU TIMESTAMP DATA TYPES     Note #1'.
*
*                   (b) Timer SHOULD be an 'up'  counter whose values increase with each time count.
*
*                   (c) When applicable, timer period SHOULD be less than the typical measured time
*                       but MUST be less than the maximum measured time; otherwise, timer resolution
*                       inadequate to measure desired times.
*
*                   See also 'CPU_TS_TmrRd()  Note #2'.
*********************************************************************************************************
*/

#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
void  CPU_TS_TmrInit (void)
{
    CPU_INT32U  cpu_clk_freq_hz;


    DEM_CR         |= (CPU_INT32U)DEM_CR_TRCENA;                /* Enable Cortex-M3's DWT CYCCNT reg.                   */
    DWT_CYCCNT      = (CPU_INT32U)0u;
    DWT_CR         |= (CPU_INT32U)DWT_CR_CYCCNTENA;

    cpu_clk_freq_hz = BSP_CPU_ClkFreq();
    CPU_TS_TmrFreqSet(cpu_clk_freq_hz);
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                           CPU_TS_TmrRd()
*
* Description : Get current CPU timestamp timer count value.
*
* Argument(s) : none.
*
* Return(s)   : Timestamp timer count (see Notes #2a & #2b).
*
* Caller(s)   : CPU_TS_Init(),
*               CPU_TS_Get32(),
*               CPU_TS_Get64(),
*               CPU_IntDisMeasStart(),
*               CPU_IntDisMeasStop().
*
*               This function is an INTERNAL CPU module function & MUST be implemented by application/
*               BSP function(s) [see Note #1] but SHOULD NOT be called by application function(s).
*
* Note(s)     : (1) CPU_TS_TmrRd() is an application/BSP function that MUST be defined by the developer
*                   if either of the following CPU features is enabled :
*
*                   (a) CPU timestamps
*                   (b) CPU interrupts disabled time measurements
*
*                   See 'cpu_cfg.h  CPU TIMESTAMP CONFIGURATION  Note #1'
*                     & 'cpu_cfg.h  CPU INTERRUPTS DISABLED TIME MEASUREMENT CONFIGURATION  Note #1a'.
*
*               (2) (a) Timer count values MUST be returned via word-size-configurable 'CPU_TS_TMR'
*                       data type.
*
*                       (1) If timer has more bits, truncate timer values' higher-order bits greater
*                           than the configured 'CPU_TS_TMR' timestamp timer data type word size.
*
*                       (2) Since the timer MUST NOT have less bits than the configured 'CPU_TS_TMR'
*                           timestamp timer data type word size; 'CPU_CFG_TS_TMR_SIZE' MUST be
*                           configured so that ALL bits in 'CPU_TS_TMR' data type are significant.
*
*                           In other words, if timer size is not a binary-multiple of 8-bit octets
*                           (e.g. 20-bits or even 24-bits), then the next lower, binary-multiple
*                           octet word size SHOULD be configured (e.g. to 16-bits).  However, the
*                           minimum supported word size for CPU timestamp timers is 8-bits.
*
*                       See also 'cpu_cfg.h   CPU TIMESTAMP CONFIGURATION  Note #2'
*                              & 'cpu_core.h  CPU TIMESTAMP DATA TYPES     Note #1'.
*
*                   (b) Timer SHOULD be an 'up'  counter whose values increase with each time count.
*
*                       (1) If timer is a 'down' counter whose values decrease with each time count,
*                           then the returned timer value MUST be ones-complemented.
*
*                   (c) (1) When applicable, the amount of time measured by CPU timestamps is
*                           calculated by either of the following equations :
*
*                           (A) Time measured  =  Number timer counts  *  Timer period
*
*                                   where
*
*                                       Number timer counts     Number of timer counts measured
*                                       Timer period            Timer's period in some units of
*                                                                   (fractional) seconds
*                                       Time measured           Amount of time measured, in same
*                                                                   units of (fractional) seconds
*                                                                   as the Timer period
*
*                                                  Number timer counts
*                           (B) Time measured  =  ---------------------
*                                                    Timer frequency
*
*                                   where
*
*                                       Number timer counts     Number of timer counts measured
*                                       Timer frequency         Timer's frequency in some units
*                                                                   of counts per second
*                                       Time measured           Amount of time measured, in seconds
*
*                       (2) Timer period SHOULD be less than the typical measured time but MUST be less
*                           than the maximum measured time; otherwise, timer resolution inadequate to
*                           measure desired times.
*********************************************************************************************************
*/

#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
CPU_TS_TMR  CPU_TS_TmrRd (void)
{
    return ((CPU_TS_TMR)DWT_CYCCNT);
}
#endif

