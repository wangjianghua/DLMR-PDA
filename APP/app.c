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
*                                            EXAMPLE CODE
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                           STM3220G-EVAL
*                                         Evaluation Board
*
* Filename      : app.c
* Version       : V1.00
* Programmer(s) : DC
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include  <app_cfg.h>
#include  <includes.h>
#include  "gui.h"
#include "DIALOG.h"
#include "lcdconf.h"

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/
#define VECT_TAB_OFFSET  0x8000 


                                                                /* ----------------- APPLICATION GLOBALS ------------------ */
static  OS_STK         App_TaskStartStk[APP_CFG_TASK_START_STK_SIZE];
static  OS_STK         App_TaskEndTickStk[APP_CFG_TASK_END_TICK_STK_SIZE];
static  OS_STK         App_TaskEndProcStk[APP_CFG_TASK_END_PROC_STK_SIZE];
static  OS_STK         App_TaskGUIStk[APP_CFG_TASK_GUI_STK_SIZE]; //added on  01.15
static  OS_STK         App_TaskKeyStk[APP_CFG_TASK_KEY_STK_SIZE];
static  OS_STK         App_TaskPLCStk[APP_CFG_TASK_PLC_STK_SIZE];
static  OS_STK         App_TaskGMPStk[APP_CFG_TASK_GMP_STK_SIZE];
static  OS_STK         App_TaskRFStk[APP_CFG_TASK_RF_STK_SIZE];


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void           App_TaskStart                (void *p_arg);
static  void           App_TaskGUI                  (void *p_arg); //GUI任务 added on 01.15
static  void           App_TaskGMP                  (void *p_arg);
static  void           App_TaskRF                   (void *p_arg);

static  void           App_MemAlloc                 (void);
static  void           App_TaskCreate               (void);
static  void           App_EventCreate              (void);


static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

    /* Configure the Vector Table location add offset address ------------------*/

  SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH */


  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 240;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 5;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);
}

/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/

int  main(void)
{
#if (OS_TASK_NAME_EN > 0)
    INT8U  err;
#endif   


    BSP_IntDisAll();                                            /* Disable all interrupts.                              */

    SystemClock_Config();
    
    CPU_Init();                                                 /* Initialize uC/CPU services.                          */
    
    OSInit();                                                   /* Initialize "uC/OS-II, The Real-Time Kernel"          */

    OSTaskCreateExt((void (*)(void *)) App_TaskStart,           /* Create the start task                                */
                    (void           *) 0,
                    (OS_STK         *)&App_TaskStartStk[APP_CFG_TASK_START_STK_SIZE - 1],
                    (INT8U           ) APP_CFG_TASK_START_PRIO,
                    (INT16U          ) APP_CFG_TASK_START_PRIO,
                    (OS_STK         *)&App_TaskStartStk[0],
                    (INT32U          ) APP_CFG_TASK_START_STK_SIZE,
                    (void           *) 0,
                    (INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(APP_CFG_TASK_START_PRIO, "Start", &err);
#endif

    OSStart();                                                  /* Start multitasking (i.e. give control to uC/OS-II)   */
}

/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'App_TaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

WM_HWIN g_hWin_menu;
WM_HWIN g_hWin_para;
WM_HWIN g_hWin_std; //  通信规约调试


WM_HWIN g_hWin_monitor;  //监控
WM_HWIN g_hWin_ReadMeter; //常用抄表
WM_HWIN g_hWin_WaveCarrior; // 载波功能设置
WM_HWIN g_hWin_DataSign; //数据标识
WM_HWIN g_hWin_relay;      //中继地址设置
WM_HWIN g_hWin_msg;       //消息日志 

WM_HWIN g_hWin_ReadMeterMsg;//抄表的消息记录

WM_HWIN g_hWin_task;//任务栏
WM_HWIN g_hWin_about; //关于

WM_HWIN g_hWin_TimeSet; //时间设置

WM_HWIN g_hWin_mem;
WM_HWIN g_hWin_Err;

WM_HWIN g_hWin_TimeBar;  //主页时间
WM_HWIN g_hWin_Date;     //显示日期

u8 rf_send_buf[256] = {0x23, 0x24, 0x78, 0x1a, 0x39, 0x23, 0x24, 0x78, 0x1a, 0x39, 0x12};
u8 rf_int_status[8];
u8 rf_part_info[8];
u8 rf_device_state[2];
extern u8 g_get_pro[4];

int G_II;

uc8 g_testString[] = "Hello \xe7\xa1\xae! \n";

uc16 g_16string [] = {0x53D6, 0x56DE, 0x62,'\r\n',0};

void APP_Shutdown()
{
#if 0    
    SYS_PWR_OFF();
    LCD_BL_OFF();
    LCD_PWR_OFF();
    LED_UART_OFF();
    LED_PLC_OFF();
#endif    
}

void APP_StartButtonTest()
{
    u32 keyCnt = 0;;
    while(1)
    {
        OSTimeDly(20);
        if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_11))
        {
            keyCnt++;
            if(keyCnt > 80)
            {
                SYS_PWR_ON();
                OSTimeDly(20);
                return;                
            }
        }
        else
        {
            APP_Shutdown();
            keyCnt = 0;
            return;
            //return;
        }
    }
}

/*
*********************************************************************************************************
*                                             App_TaskStart()
*
* Description : This task monitors the state of the push buttons and passes messages to AppTaskUserIF()
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskStart()' by 'OSTaskCreateExt()'.
*
* Return(s)  : none.
*
* Caller(s)  : This is a task.
*
* Note(s)    : none.
*********************************************************************************************************
*/
static  void  App_TaskStart (void *p_arg)
{
   (void)p_arg; 
    u8 i = 0, n;

    DEV_Init();

    BSP_Init();                                                 /* Init BSP fncts.                                          */

    Mem_Init();                                                 /* Initialize Memory managment                              */

    BSP_CPU_TickInit();                                         /* Start Tick Initialization                                */

#if (OS_TASK_STAT_EN > 0)
    OSStatInit();                                               /* Determine CPU capacity                                   */
#endif

    APP_StartButtonTest();
    
    App_EventCreate();                                          /* Create Application Events                                */

    App_TaskCreate();                                           /* Create application Tasks                                 */ 
   
  	GUI_Init();                                                 /* Init the STemWin GUI Library */

	KEY_Init();

    End_Init();

    RF_Init();

    DEBUG_PRINT(("OS Tasks Run!\n"));

    //FatFs_Test();

    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.           */
        if(i++ >= 10)
        {
            i = 0;
            RTC_ReadTime(g_rtc_time); 
            g_sys_control.sleepTimeout++;
            if(g_sys_control.sleepTimeout > 60)
            {
                APP_Shutdown();
            }
                
        }

        for(n = 0; n < 32; n++)
        {
            if((0x01<<n) & g_sys_control.procTask)
            {
                switch(n)
                {
                case 0:
                    MMD_Format_Disk();
                    SYS_DEL_TASK(SYS_TASK_FORMAT_DISK);
                    break;
                }
            }
        }
               
        g_sys_control.pwrValue=BSP_ADC_ReadPwr();//读电压
        //TEXT_SetText(TSK_Get_Time_Text(),RTC2Text());
        OSTimeDlyHMSM(0, 0, 0, 100);
    }
}

/*
*********************************************************************************************************
*                                             App_TaskGUI()
*
* Description : This task monitors the state of the push buttons and passes messages to AppTaskUserIF()
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskGUI()' by 'OSTaskCreateExt()'.
*
* Return(s)  : none.
*
* Caller(s)  : This is a task.
*
* Note(s)    : none.
*********************************************************************************************************
*/
static  void  App_TaskGUI (void *p_arg)
{
    (void)p_arg;
    WM_HWIN hItem;
    unsigned char timebuf[10];
    unsigned char timebuf_nosec[6];
    unsigned char timebuf_date[11];
    
#if OS_CRITICAL_METHOD == 3u
        OS_CPU_SR  cpu_sr = 0u;
#endif  

    g_hWin_menu = CreatePDA_IconMenu();
    WM_ShowWindow(g_hWin_menu);
   
    g_hWin_msg=CreateMessage();
    WM_HideWindow(g_hWin_msg);
    WM_SetFocus(g_hWin_menu);

    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.           */
        GUI_Exec();
        
        OS_ENTER_CRITICAL();
        memcpy(timebuf,RTC2Text(),10);
        TEXT_SetText(g_hWin_TimeBar,timebuf);
        //memcpy(timebuf_nosec,RTC2Text_NoSec(),6);
        //TEXT_SetText(g_hWin_TimeBar,timebuf_nosec);
        memcpy(timebuf_date,RTC2Text_Date(),11);
        TEXT_SetText(g_hWin_Date,timebuf_date);
        OS_EXIT_CRITICAL();
        
        OSTimeDlyHMSM(0, 0, 0, 100);     
    }
}

/*
*********************************************************************************************************
*                                             App_TaskRF()
*
* Description : This task monitors the state of the push buttons and passes messages to AppTaskUserIF()
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskRF()' by 'OSTaskCreateExt()'.
*
* Return(s)  : none.
*
* Caller(s)  : This is a task.
*
* Note(s)    : none.
*********************************************************************************************************
*/
static  void  App_TaskRF (void *p_arg)
{
    INT8U err;
    u8 rf_read_addr[] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x00, 0x36, 0x19, 0x00, 0x00, 0x00};
    u8 rf_reply_addr[] = {0x00, 0x36, 0x19, 0x00, 0x00, 0x00, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};
    u8 rf_dl645_read[] = {0x68, 0x00, 0x36, 0x19, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x33, 0x32, 0x34, 0x33, 0x00, 0x16};
    u8 rf_dl645_reply[] = {0xFE, 0xFE, 0xFE, 0xFE, 0x68, 0x00, 0x36, 0x19, 0x00, 0x00, 0x00, 0x68, 0x91, 0x18, 0x33, 0x32, 0x34, 0x33, 0x4C, 0x6C, 0x33, 0x33, 0x89, 0x35, 0x33, 0x33, 0x53, 0x33, 0x33, 0x33, 0x6B, 0x33, 0x33, 0x33, 0x38, 0x69, 0x33, 0x33, 0xCD, 0x16};
    u8 rf_test_msg[] = "hello, world!";
    u8 rf_send_len;

#define RF_SEND_BUF         rf_send_buf
#define RF_SEND_LEN         rf_send_len

#define RF_RECV_BUF         g_rf_param.rx.buf
#define RF_RECV_LEN         g_rf_param.rx.rx_len

    
    (void)p_arg;

    while (DEF_TRUE) {
        OSSemPend(g_sem_rf, 3 * OS_TICKS_PER_SEC, &err);

        if(OS_ERR_NONE == err)
        {
            LED_PLC_TOGGLE();
            
            pc_uart_send(RF_RECV_BUF, RF_RECV_LEN);
        }
        else
        {            
#if 1
            LED_PLC_TOGGLE();

            rf_send_len = GDW_RF_Protocol_2013(rf_read_addr, 0x00, 0x00, 0x00, rf_dl645_read, sizeof(rf_dl645_read), rf_send_buf);

            pc_uart_send(RF_SEND_BUF, RF_SEND_LEN); 
#else
            RF_Tx(rf_test_msg, sizeof(rf_test_msg));
#endif
        }
    }
}

/*
*********************************************************************************************************
*                                             App_TaskGMP()
*
* Description : This task monitors the state of the push buttons and passes messages to AppTaskUserIF()
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskGMP()' by 'OSTaskCreateExt()'.
*
* Return(s)  : none.
*
* Caller(s)  : This is a task.
*
* Note(s)    : none.
*********************************************************************************************************
*/
static  void  App_TaskGMP (void *p_arg)
{
    INT8U err;

    u32   tm;

    WM_HWIN  wh;
    
    (void)p_arg;
    
    while (DEF_TRUE) {           
        /* Task body, always written as an infinite loop.           */
        if((g_sys_control.guiState == GUI_PLC_MSG_TEST)
            ||(g_sys_control.guiState == GUI_PLC_MSG_READ)
            ||(g_sys_control.guiState == GUI_PLC_MSG_LISTING))
        {
            OSMboxPend(g_sys_control.upMb, 10, &err);        
            if(OS_ERR_NONE == err)
            {
                GUI_Msg_Proc();
            }
            else if(OS_ERR_TIMEOUT == err)
            {
                wh = GUI_Get_PROGBAR();
                if(wh != WM_HWIN_NULL)
                {
                    PROGBAR_SetValue(wh, g_sys_control.testProgBarVal);
                    if(g_sys_control.testProgBarVal < 90)
                    {
                       g_sys_control.testProgBarVal += 1;
                    }
                }
            }
        }
        else
        {
            OSTimeDly(10);
        }
    }
}

/*
*********************************************************************************************************
*                                      App_EventCreate()
*
* Description : Create the application Events
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : App_TasStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/
static  void  App_EventCreate (void)
{    
    g_sem_end = OSSemCreate(0);
    g_sem_plc = OSSemCreate(0);
    g_sem_rf = OSSemCreate(0);
	g_key_control.key_sem = OSSemCreate(0);    
    
    g_sys_control.downMb = OSMboxCreate(NULL); /*创建消息邮箱用来发送调试参数的结构体*/
    g_sys_control.upMb = OSMboxCreate(NULL); /*创建消息邮箱用来发送调试参数的结构体*/    
}


/*
*********************************************************************************************************
*                                      App_TaskCreate()
*
* Description : Create the application tasks.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : App_TasStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/
static  void  App_TaskCreate (void)
{
    INT8U  err;

    
    OSTaskCreateExt((void (*)(void *)) App_TaskGUI,
                    (void           *) 0,
                    (OS_STK         *)&App_TaskGUIStk[APP_CFG_TASK_GUI_STK_SIZE - 1],
                    (INT8U           ) APP_CFG_TASK_GUI_PRIO,
                    (INT16U          ) APP_CFG_TASK_GUI_PRIO,
                    (OS_STK         *)&App_TaskGUIStk[0],
                    (INT32U          ) APP_CFG_TASK_GUI_STK_SIZE,
                    (void           *) 0,
                    (INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(APP_CFG_TASK_GUI_PRIO, "GUI", &err);
#endif

    OSTaskCreateExt((void (*)(void *)) App_TaskGMP,
                    (void           *) 0,
                    (OS_STK         *)&App_TaskGMPStk[APP_CFG_TASK_GMP_STK_SIZE - 1],
                    (INT8U           ) APP_CFG_TASK_GMP_PRIO,
                    (INT16U          ) APP_CFG_TASK_GMP_PRIO,
                    (OS_STK         *)&App_TaskGMPStk[0],
                    (INT32U          ) APP_CFG_TASK_GMP_STK_SIZE,
                    (void           *) 0,
                    (INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
    
#if (OS_TASK_NAME_EN > 0)
        OSTaskNameSet(APP_CFG_TASK_GUI_PRIO, "GMP", &err);
#endif

    OSTaskCreateExt((void (*)(void *)) App_TaskKey,              
                    (void           *) 0,
                    (OS_STK         *)&App_TaskKeyStk[APP_CFG_TASK_KEY_STK_SIZE - 1],
                    (INT8U           ) APP_CFG_TASK_KEY_PRIO,
                    (INT16U          ) APP_CFG_TASK_KEY_PRIO,
                    (OS_STK         *)&App_TaskKeyStk[0],
                    (INT32U          ) APP_CFG_TASK_KEY_STK_SIZE,
                    (void           *) 0,
                    (INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(APP_CFG_TASK_KEY_PRIO, "Key", &err);
#endif

    OSTaskCreateExt((void (*)(void *)) App_TaskEndTick,
                    (void           *) 0,
                    (OS_STK         *)&App_TaskEndTickStk[APP_CFG_TASK_END_TICK_STK_SIZE - 1],
                    (INT8U           ) APP_CFG_TASK_END_TICK_PRIO,
                    (INT16U          ) APP_CFG_TASK_END_TICK_PRIO,
                    (OS_STK         *)&App_TaskEndTickStk[0],
                    (INT32U          ) APP_CFG_TASK_END_TICK_STK_SIZE,
                    (void           *) 0,
                    (INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(APP_CFG_TASK_END_TICK_PRIO, "EndTick", &err);
#endif

    OSTaskCreateExt((void (*)(void *)) App_TaskEndProc,
                    (void           *) 0,
                    (OS_STK         *)&App_TaskEndProcStk[APP_CFG_TASK_END_PROC_STK_SIZE - 1],
                    (INT8U           ) APP_CFG_TASK_END_PROC_PRIO,
                    (INT16U          ) APP_CFG_TASK_END_PROC_PRIO,
                    (OS_STK         *)&App_TaskEndProcStk[0],
                    (INT32U          ) APP_CFG_TASK_END_PROC_STK_SIZE,
                    (void           *) 0,
                    (INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(APP_CFG_TASK_END_PROC_PRIO, "EndProc", &err);      
#endif

    OSTaskCreateExt((void (*)(void *)) App_TaskPLC,
                    (void           *) 0,
                    (OS_STK         *)&App_TaskPLCStk[APP_CFG_TASK_PLC_STK_SIZE - 1],
                    (INT8U           ) APP_CFG_TASK_PLC_PRIO,
                    (INT16U          ) APP_CFG_TASK_PLC_PRIO,
                    (OS_STK         *)&App_TaskPLCStk[0],
                    (INT32U          ) APP_CFG_TASK_PLC_STK_SIZE,
                    (void           *) 0,
                    (INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(APP_CFG_TASK_PLC_PRIO, "PLC", &err);    
#endif

    OSTaskCreateExt((void (*)(void *)) App_TaskRF,
                    (void           *) 0,
                    (OS_STK         *)&App_TaskRFStk[APP_CFG_TASK_RF_STK_SIZE - 1],
                    (INT8U           ) APP_CFG_TASK_RF_PRIO,
                    (INT16U          ) APP_CFG_TASK_RF_PRIO,
                    (OS_STK         *)&App_TaskRFStk[0],
                    (INT32U          ) APP_CFG_TASK_RF_STK_SIZE,
                    (void           *) 0,
                    (INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(APP_CFG_TASK_PLC_PRIO, "RF", &err);    
#endif
}

