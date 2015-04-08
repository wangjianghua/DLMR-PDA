/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                             (c) Copyright 2013; Micrium, Inc.; Weston, FL
*
*                   All rights reserved.  Protected by international copyright laws.
*                   Knowledge of the source code may not be used to write a similar
*                   product.  This file may only be used in accordance with a license
*                   and should not be redistributed in any way.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                           MASTER INCLUDES
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                           STM3220G-EVAL
*                                         Evaluation Board
*
* Filename      : includes.h
* Version       : V1.00
* Programmer(s) : DC
*********************************************************************************************************
*/

#include <stm32f2xx_hal.h>

#ifndef  INCLUDES_PRESENT
#define  INCLUDES_PRESENT


typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef const int32_t sc32;  /*!< Read Only */
typedef const int16_t sc16;  /*!< Read Only */
typedef const int8_t sc8;   /*!< Read Only */

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef __I int32_t vsc32;  /*!< Read Only */
typedef __I int16_t vsc16;  /*!< Read Only */
typedef __I int8_t vsc8;   /*!< Read Only */

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef void *pvoid;

typedef const uint32_t uc32;  /*!< Read Only */
typedef const uint16_t uc16;  /*!< Read Only */
typedef const uint8_t uc8;   /*!< Read Only */

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef __I uint32_t vuc32;  /*!< Read Only */
typedef __I uint16_t vuc16;  /*!< Read Only */
typedef __I uint8_t vuc8;   /*!< Read Only */




/*
*********************************************************************************************************
*                                         STANDARD LIBRARIES
*********************************************************************************************************
*/

#include  <stdarg.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <math.h>


/*
*********************************************************************************************************
*                                              LIBRARIES
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <lib_def.h>
#include  <lib_ascii.h>
#include  <lib_math.h>
#include  <lib_mem.h>
#include  <lib_str.h>


/*
*********************************************************************************************************
*                                                 OS
*********************************************************************************************************
*/

#include  <ucos_ii.h>


/*
*********************************************************************************************************
*                                              APP / BSP
*********************************************************************************************************
*/

//#include <stm32f2xx.h>


#include  <bsp.h>

#include  <gui.h>

#include  <wm.h>

#include "app_device.h"
#include "key_drv.h"
#include "si4438.h"
#include "si4438_sub.h"
#include "DL645.h" //华兄
#include "GDW376_2.h" //华兄
#include "RF_App.h" //华兄
#include "APP_Protocol.h"
#include "queue.h" 
#include "link_manager.h"
#include "uart_link.h"
#include "sd.h"
#include "ff.h"
#include "rtc.h"
#include "GUI_public_fun.h"
#include "lcdconf.h"
#include "phrase.h"

#define WM_MSG_CLOSE                  (WM_USER + 0x80)

#define WM_MSG_DeleteDialog           (WM_USER + 0x81)

#define TEXT_LEN  10     //???????????????????10???
#define EWARM_OPTIMIZATION_EN          1u

//extern GUI_CONST_STORAGE GUI_FONT GUI_FontPDAFontSong;


extern WM_HWIN g_hWin_menu;
extern WM_HWIN g_hWin_para;
extern WM_HWIN g_hWin_std;

extern WM_HWIN g_hWin_DataSign;   //数据标识 
extern WM_HWIN g_hWin_relay;      //中继地址设置
extern WM_HWIN g_hWin_msg;       //消息日志 


extern WM_HWIN g_hWin_monitor;  //监控
extern WM_HWIN g_hWin_ReadMeter; //常用抄表
extern WM_HWIN g_hWin_WaveCarrior; // 载波功能设置


extern WM_HWIN g_hWin_task;
extern WM_HWIN g_hWin_about; //关于
extern WM_HWIN g_hWin_TimeSet; //时间设置
extern WM_HWIN g_hWin_mem;
extern WM_HWIN g_hWin_Err;

extern WM_HWIN g_hWin_ReadMeterMsg;

extern WM_HWIN g_hWin_TimeBar;
extern WM_HWIN g_hWin_Date;

extern WM_HWIN g_hWin_Input;
extern WM_HWIN g_hWin_speed;
extern WM_HWIN g_hWin_AdvanSet;

//extern int test_multiedit;

extern int DataSign_value;
extern char DataSign_Text[TEXT_LEN];//??????????,???????


//extern GUI_CONST_STORAGE GUI_BITMAP bm_Upload;


//extern GUI_CONST_STORAGE GUI_BITMAP bmbarrary;
/*
*********************************************************************************************************
*                                            INCLUDES END
*********************************************************************************************************
*/


#endif

