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
* Filename      : bsp.h
* Version       : V1.00
* Programmer(s) : SL
*                 DC
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                 MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               BSP present pre-processor macro definition.
*
*           (2) This file and its dependencies requires IAR v6.20 or later to be compiled.
*
*********************************************************************************************************
*/

#ifndef  BSP_PRESENT
#define  BSP_PRESENT


/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/

#ifdef   BSP_MODULE
#define  BSP_EXT
#else
#define  BSP_EXT  extern
#endif


/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/

#include  <stdio.h>
#include  <stdarg.h>

#include  <cpu.h>
#include  <cpu_core.h>

#include  <lib_def.h>
#include  <lib_ascii.h>

//#include  <stm32f2xx_hal_gpio.h>
//#include  <stm32f2xx_hal_rcc.h>
//#include  <stm32f2xx_hal_fsmc.h>
//#include  <stm32f2xx_hal_usart.h>
//#include  <stm32f2xx_hal_dma.h>
//#include  <stm32f2xx_syscfg.h>

//#include  <bsp_ser.h>
#include "stm32f2xx_hal.h"
#include  <bsp_os.h>


/*
*********************************************************************************************************
*                                               CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               INT DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             PERIPH DEFINES
*********************************************************************************************************
*/
#define LED_RED                     0u
#define LED_GREEN                   1u

/*
*********************************************************************************************************
*                                               DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                                 MACRO'S
*********************************************************************************************************
*/


#define SPIx                             SPI2
#define SPIx_CLK_ENABLE()                __SPI2_CLK_ENABLE()

#define SPIx_FORCE_RESET()               __SPI2_FORCE_RESET()
#define SPIx_RELEASE_RESET()             __SPI2_RELEASE_RESET()

/* Definition for SPIx Pins */
#define SPIx_SCK_PIN                     GPIO_PIN_13
#define SPIx_SCK_GPIO_PORT               GPIOB
#define SPIx_SCK_AF                      GPIO_AF5_SPI2
#define SPIx_MISO_PIN                    GPIO_PIN_14
#define SPIx_MISO_GPIO_PORT              GPIOB
#define SPIx_MISO_AF                     GPIO_AF5_SPI2
#define SPIx_MOSI_PIN                    GPIO_PIN_15
#define SPIx_MOSI_GPIO_PORT              GPIOB
#define SPIx_MOSI_AF                     GPIO_AF5_SPI2

#define RF_PWR_ON()       HAL_GPIO_WritePin(GPIOF,GPIO_PIN_10,GPIO_PIN_RESET)
#define RF_PWR_OFF()       HAL_GPIO_WritePin(GPIOF,GPIO_PIN_10,GPIO_PIN_SET)

#define SYS_PWR_OFF()       HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET)
#define SYS_PWR_ON()       HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET)

#define KEY_LED_OFF()    GPIO_SetBits(GPIOG, GPIO_PIN_7)

/*
*********************************************************************************************************
*                                               INT DEFINES
*********************************************************************************************************
*/

#define  BSP_INT_ID_WWDG                                   0    /* Window WatchDog Interrupt                            */
#define  BSP_INT_ID_PVD                                    1    /* PVD through EXTI Line detection Interrupt            */
#define  BSP_INT_ID_TAMPER                                 2    /* Tamper Interrupt                                     */
#define  BSP_INT_ID_RTC                                    3    /* RTC global Interrupt                                 */
#define  BSP_INT_ID_FLASH                                  4    /* FLASH global Interrupt                               */
#define  BSP_INT_ID_RCC                                    5    /* RCC global Interrupt                                 */
#define  BSP_INT_ID_EXTI0                                  6    /* EXTI Line0 Interrupt                                 */
#define  BSP_INT_ID_EXTI1                                  7    /* EXTI Line1 Interrupt                                 */
#define  BSP_INT_ID_EXTI2                                  8    /* EXTI Line2 Interrupt                                 */
#define  BSP_INT_ID_EXTI3                                  9    /* EXTI Line3 Interrupt                                 */
#define  BSP_INT_ID_EXTI4                                 10    /* EXTI Line4 Interrupt                                 */
#define  BSP_INT_ID_DMA1_CH1                              11    /* DMA1 Channel 1 global Interrupt                      */
#define  BSP_INT_ID_DMA1_CH2                              12    /* DMA1 Channel 2 global Interrupt                      */
#define  BSP_INT_ID_DMA1_CH3                              13    /* DMA1 Channel 3 global Interrupt                      */
#define  BSP_INT_ID_DMA1_CH4                              14    /* DMA1 Channel 4 global Interrupt                      */
#define  BSP_INT_ID_DMA1_CH5                              15    /* DMA1 Channel 5 global Interrupt                      */
#define  BSP_INT_ID_DMA1_CH6                              16    /* DMA1 Channel 6 global Interrupt                      */
#define  BSP_INT_ID_DMA1_CH7                              17    /* DMA1 Channel 7 global Interrupt                      */
#define  BSP_INT_ID_ADC1_2                                18    /* ADC1 et ADC2 global Interrupt                        */
#define  BSP_INT_ID_CAN1_TX                               19    /* CAN1 TX Interrupts                                   */
#define  BSP_INT_ID_CAN1_RX0                              20    /* CAN1 RX0 Interrupts                                  */
#define  BSP_INT_ID_CAN1_RX1                              21    /* CAN1 RX1 Interrupt                                   */
#define  BSP_INT_ID_CAN1_SCE                              22    /* CAN1 SCE Interrupt                                   */
#define  BSP_INT_ID_EXTI9_5                               23    /* External Line[9:5] Interrupts                        */
#define  BSP_INT_ID_TIM1_BRK                              24    /* TIM1 Break Interrupt                                 */
#define  BSP_INT_ID_TIM1_UP                               25    /* TIM1 Update Interrupt                                */
#define  BSP_INT_ID_TIM1_TRG_COM                          26    /* TIM1 Trigger and Commutation Interrupt               */
#define  BSP_INT_ID_TIM1_CC                               27    /* TIM1 Capture Compare Interrupt                       */
#define  BSP_INT_ID_TIM2                                  28    /* TIM2 global Interrupt                                */
#define  BSP_INT_ID_TIM3                                  29    /* TIM3 global Interrupt                                */
#define  BSP_INT_ID_TIM4                                  30    /* TIM4 global Interrupt                                */
#define  BSP_INT_ID_I2C1_EV                               31    /* I2C1 Event Interrupt                                 */
#define  BSP_INT_ID_I2C1_ER                               32    /* I2C1 Error Interrupt                                 */
#define  BSP_INT_ID_I2C2_EV                               33    /* I2C2 Event Interrupt                                 */
#define  BSP_INT_ID_I2C2_ER                               34    /* I2C2 Error Interrupt                                 */
#define  BSP_INT_ID_SPI1                                  35    /* SPI1 global Interrupt                                */
#define  BSP_INT_ID_SPI2                                  36    /* SPI2 global Interrupt                                */
#define  BSP_INT_ID_USART1                                37    /* USART1 global Interrupt                              */
#define  BSP_INT_ID_USART2                                38    /* USART2 global Interrupt                              */
#define  BSP_INT_ID_USART3                                39    /* USART3 global Interrupt                              */
#define  BSP_INT_ID_EXTI15_10                             40    /* External Line[15:10] Interrupts                      */
#define  BSP_INT_ID_RTC_ALARM                             41    /* RTC Alarm through EXTI Line Interrupt                */
#define  BSP_INT_ID_OTG_FS_WKUP                           42    /* USB WakeUp from suspend through EXTI Line Interrupt  */

#define  BSP_INT_ID_TIM8_BRK_TIM12                        43    /* TIM8 Break Interrupt and TIM12 global Interrupt      */
#define  BSP_INT_ID_TIM8_UP_TIM13                         44    /* TIM8 Update Interrupt and TIM13 global Interrupt     */
#define  BSP_INT_ID_TIM8_TRG_COM_TIM14                    45    /* TIM8 Trigger/Commutation and TIM14 global Interrupt  */
#define  BSP_INT_ID_TIM8_CC                               46    /* TIM8 Capture Compare Interrupt                       */
#define  BSP_INT_ID_DMA1_STREAM7                          47    /* DMA1 Stream7 Interrupt                               */
#define  BSP_INT_ID_FSMC                                  48    /* FSMC global Interrupt                                */
#define  BSP_INT_ID_SDIO                                  49    /* SDIO global Interrupt                                */

#define  BSP_INT_ID_TIM5                                  50    /* TIM5 global Interrupt                                */
#define  BSP_INT_ID_SPI3                                  51    /* SPI3 global Interrupt                                */
#define  BSP_INT_ID_USART4                                52    /* USART4 global Interrupt                              */
#define  BSP_INT_ID_USART5                                53    /* USART5 global Interrupt                              */
#define  BSP_INT_ID_TIM6                                  54    /* TIM6 global Interrupt                                */
#define  BSP_INT_ID_TIM7                                  55    /* TIM7 global Interrupt                                */
#define  BSP_INT_ID_DMA2_CH1                              56    /* DMA2 Channel 1 global Interrupt                      */
#define  BSP_INT_ID_DMA2_CH2                              57    /* DMA2 Channel 2 global Interrupt                      */
#define  BSP_INT_ID_DMA2_CH3                              58    /* DMA2 Channel 3 global Interrupt                      */
#define  BSP_INT_ID_DMA2_CH4                              59    /* DMA2 Channel 4 global Interrupt                      */
#define  BSP_INT_ID_DMA2_CH5                              60    /* DMA2 Channel 5 global Interrupt                      */

#define  BSP_INT_ID_ETH                                   61    /* ETH  global Interrupt                                */
#define  BSP_INT_ID_ETH_WKUP                              62    /* ETH  WakeUp from EXTI line interrupt                 */
#define  BSP_INT_ID_CAN2_TX                               63    /* CAN2 TX Interrupts                                   */
#define  BSP_INT_ID_CAN2_RX0                              64    /* CAN2 RX0 Interrupts                                  */
#define  BSP_INT_ID_CAN2_RX1                              65    /* CAN2 RX1 Interrupt                                   */
#define  BSP_INT_ID_CAN2_SCE                              66    /* CAN2 SCE Interrupt                                   */
#define  BSP_INT_ID_OTG_FS                                67    /* OTG global Interrupt                                 */

#define  BSP_INT_ID_DMA2_CH6                              68    /* DMA2 Channel 6 global Interrupt                      */
#define  BSP_INT_ID_DMA2_CH7                              69    /* DMA2 Channel 7 global Interrupt                      */
#define  BSP_INT_ID_DMA2_CH8                              70    /* DMA2 Channel 8 global Interrupt                      */
#define  BSP_INT_ID_USART6                                71    /* USART5 global Interrupt                              */
#define  BSP_INT_ID_I2C3_EV                               72    /* I2C3 Event  Interrupt                                */
#define  BSP_INT_ID_I2C3_ER                               73    /* I2C3 Error  Interrupt                                */
#define  BSP_INT_ID_OTG_HS_EP1_OUT                        74    /* OTG HS EP1 Output Interrupt                          */
#define  BSP_INT_ID_OTG_HS_EP1_IN                         75    /* OTG HS EP1 Input Interrupt                           */
#define  BSP_INT_ID_OTG_HS_WKUP                           76    /* OTG HS Wakeup Interrupt                              */
#define  BSP_INT_ID_OTG_HS                                77    /* OTG HS Interrupt                                     */
#define  BSP_INT_ID_DCMI                                  78    /* DCMI Interrupt                                       */
#define  BSP_INT_ID_CRYP                                  79    /* CRYPT Interrupt                                      */
#define  BSP_INT_ID_HASH_RNG                              80    /* HASH RNG Interrupt                                   */


#define EXTI9_5_PREEM_PRIO            0
#define EXTI9_5_SUB_PRIO              3

/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void        BSP_Init                (void);
void        BSP_CPU_TickInit        (void);
CPU_INT32U  BSP_CPU_ClkFreq         (void);

void        BSP_IntEnAll            (void);
void        BSP_IntDisAll           (void);


void        BSP_IntEn      (CPU_DATA       int_id);
void        BSP_IntInit    (void);
void        BSP_IntDis     (CPU_DATA       int_id);
void        BSP_IntClr     (CPU_DATA       int_id);
void        BSP_IntVectSet (CPU_DATA       int_id,
                            CPU_FNCT_VOID  isr);

void        LED_On     (CPU_INT08U     led);
void        LED_Off    (CPU_INT08U     led);
void        LED_Toggle (CPU_INT08U     led);

CPU_INT32U  BSP_RNG_Read   (void);

void        BSP_IntPrioSet (CPU_DATA       int_id,
                            CPU_INT08U     prio);

void        BSP_PSRAM_Init (void);
void        BSP_SRAM_Init  (void);
void 		BSP_LCD_init	(void);
void 		BSP_Keyboard_Init(void);
void 		BSP_GPIO_Configuration(void);
void 		BSP_NVIC_Configuration(void);

void 		BSP_RCC_Configuration(void);

void        BSP_BEEP(void);
CPU_INT32U  BSP_ADC_ReadPwr(void);
void        BSP_ADC_Init(void);
CPU_INT32U  BSP_GetLSIFrequency(void);
void        BSP_IWDG_Init(void);


/*
*********************************************************************************************************
*                                           INTERRUPT SERVICES
*********************************************************************************************************
*/

void         BSP_IntHandlerWWDG               (void);
void         BSP_IntHandlerPVD                (void);
void         BSP_IntHandlerTAMPER             (void);
void         BSP_IntHandlerRTC                (void);
void         BSP_IntHandlerFLASH              (void);
void         BSP_IntHandlerRCC                (void);
void         BSP_IntHandlerEXTI0              (void);
void         BSP_IntHandlerEXTI1              (void);
void         BSP_IntHandlerEXTI2              (void);
void         BSP_IntHandlerEXTI3              (void);
void         BSP_IntHandlerEXTI4              (void);
void         BSP_IntHandlerDMA1_CH1           (void);
void         BSP_IntHandlerDMA1_CH2           (void);
void         BSP_IntHandlerDMA1_CH3           (void);
void         BSP_IntHandlerDMA1_CH4           (void);
void         BSP_IntHandlerDMA1_CH5           (void);
void         BSP_IntHandlerDMA1_CH6           (void);
void         BSP_IntHandlerDMA1_CH7           (void);
void         BSP_IntHandlerADC1_2             (void);
void         BSP_IntHandlerCAN1_TX            (void);
void         BSP_IntHandlerCAN1_RX0           (void);
void         BSP_IntHandlerCAN1_RX1           (void);
void         BSP_IntHandlerCAN1_SCE           (void);
void         BSP_IntHandlerEXTI9_5            (void);
void         BSP_IntHandlerTIM1_BRK           (void);
void         BSP_IntHandlerTIM1_UP            (void);
void         BSP_IntHandlerTIM1_TRG_COM       (void);
void         BSP_IntHandlerTIM1_CC            (void);
void         BSP_IntHandlerTIM2               (void);
void         BSP_IntHandlerTIM3               (void);
void         BSP_IntHandlerTIM4               (void);
void         BSP_IntHandlerI2C1_EV            (void);
void         BSP_IntHandlerI2C1_ER            (void);
void         BSP_IntHandlerI2C2_EV            (void);
void         BSP_IntHandlerI2C2_ER            (void);
void         BSP_IntHandlerSPI1               (void);
void         BSP_IntHandlerSPI2               (void);
void         BSP_IntHandlerUSART1             (void);
void         BSP_IntHandlerUSART2             (void);
void         BSP_IntHandlerUSART3             (void);
void         BSP_IntHandlerEXTI15_10          (void);
void         BSP_IntHandlerRTCAlarm           (void);
void         BSP_IntHandlerUSBWakeUp          (void);
void         BSP_IntHandlerTIM8_BRK_TIM12     (void);
void         BSP_IntHandlerTIM8_UP_TIM13      (void);
void         BSP_IntHandlerTIM8_TRG_COM_TIM14 (void);
void         BSP_IntHandlerTIM8_CC            (void);
void         BSP_IntHandlerDMA1_STREAM7       (void);
void         BSP_IntHandlerFSMC               (void);
void         BSP_IntHandlerSDIO               (void);
void         BSP_IntHandlerTIM5               (void);
void         BSP_IntHandlerSPI3               (void);
void         BSP_IntHandlerUSART4             (void);
void         BSP_IntHandlerUSART5             (void);
void         BSP_IntHandlerTIM6               (void);
void         BSP_IntHandlerTIM7               (void);
void         BSP_IntHandlerDMA2_CH1           (void);
void         BSP_IntHandlerDMA2_CH2           (void);
void         BSP_IntHandlerDMA2_CH3           (void);
void         BSP_IntHandlerDMA2_CH4           (void);
void         BSP_IntHandlerDMA2_CH5           (void);
void         BSP_IntHandlerETH                (void);
void         BSP_IntHandlerETHWakeup          (void);
void         BSP_IntHandlerCAN2_TX            (void);
void         BSP_IntHandlerCAN2_RX0           (void);
void         BSP_IntHandlerCAN2_RX1           (void);
void         BSP_IntHandlerCAN2_SCE           (void);
void         BSP_IntHandlerOTG                (void);
void         BSP_IntHandlerDMA2_CH6           (void);
void         BSP_IntHandlerDMA2_CH7           (void);
void         BSP_IntHandlerDMA2_CH8           (void);
void         BSP_IntHandlerUSART6             (void);
void         BSP_IntHandlerI2C3_EV            (void);
void         BSP_IntHandlerI2C3_ER            (void);
void         BSP_IntHandlerOTG_HS_EP1_OUT     (void);
void         BSP_IntHandlerOTG_HS_EP1_IN      (void);
void         BSP_IntHandlerOTG_HS_WKUP        (void);
void         BSP_IntHandlerOTG_HS             (void);
void         BSP_IntHandlerDCMI               (void);
void         BSP_IntHandlerCRYP               (void);
void         BSP_IntHandlerHASH_RNG           (void);


#define DEBUG_INFO_PRINT_EN            0u

#if (DEBUG_INFO_PRINT_EN > 0u)
#define DEBUG_PRINT(s)         printf s
#else
#define DEBUG_PRINT(s)             
#endif

#define clr_wdt()              HAL_IWDG_Refresh(&IwdgHandle)

#define LED_SLEEP_ON()         LED_On(LED_GREEN)
#define LED_SLEEP_OFF()        LED_Off(LED_GREEN)

extern SPI_HandleTypeDef SpiHandle;
extern SPI_HandleTypeDef hspi1;

extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

extern DMA_HandleTypeDef hdma_uart4_rx;
extern DMA_HandleTypeDef hdma_uart4_tx;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart2_tx;

extern IWDG_HandleTypeDef IwdgHandle;

void PLC_PWR_ON(void);
void PLC_PWR_OFF(void);

void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void BSP_SPI_Init(void);
void SPI1_Init(void);
void UART4_Init(void);


CPU_INT08U BSP_LCD_Check(void);
#define LCD_OK      1
#define LCD_ERR     0


#endif
