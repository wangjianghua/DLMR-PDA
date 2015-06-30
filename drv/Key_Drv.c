#include "includes.h"
#include "key_drv.h"


KEY_CTRL g_key_ctrl;
OS_EVENT *g_mbox_chk_key; //华兄

#define ID_WINDOW_0             (GUI_ID_USER + 0x00)
#define ID_BUTTON_0             (GUI_ID_USER + 0x01)
#define ID_BUTTON_1             (GUI_ID_USER + 0x02)
#define ID_EDIT_0             (GUI_ID_USER + 0x03)
#define ID_EDIT_1             (GUI_ID_USER + 0x04)
#define ID_TEXT_0             (GUI_ID_USER + 0x05)
#define ID_TEXT_1             (GUI_ID_USER + 0x06)



static const int s_key_msg_map[KEYBOARD_COL_NUM][KEYBOARD_ROW_NUM] = 
{
    {'*','7', '4', '1', GUI_KEY_TAB, GUI_KEY_ESCAPE},
    {GUI_KEY_F1,'0', '8', '5', '2', GUI_KEY_DOWN},
    {GUI_KEY_F2,'#', '9', '6', '3', GUI_KEY_BACKSPACE},
    {GUI_KEY_ENTER, GUI_KEY_YELLOW, GUI_KEY_RIGHT, GUI_KEY_UP, GUI_KEY_LEFT, GUI_KEY_GREEN},
};



void KEY_Init(void)
{
	BSP_IntVectSet(BSP_INT_ID_EXTI9_5, EXTI9_5_IRQHandler);
	BSP_IntEn(BSP_INT_ID_EXTI9_5);
}

/*
*********************************************************************************************************
*                                             App_TaskKey()
*
* Description : This task monitors the state of the push buttons and passes messages to AppTaskUserIF()
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskKey()' by 'OSTaskCreateExt()'.
*
* Return(s)  : none.
*
* Caller(s)  : This is a task.
*
* Note(s)    : none.
*********************************************************************************************************
*/
void  App_TaskKey (void *p_arg)
{
	INT8U err;
    int i, n, key_msg;
    WM_HWIN hItem;
    

    (void)p_arg;
    
    while (DEF_TRUE) {
		OSSemPend(g_key_ctrl.sem, 0 ,&err);

        if(g_key_ctrl.col == 0)
            goto key_proc_end;

        if(GPIO_PIN_RESET != HAL_GPIO_ReadPin(KEYBOARD_COL_PORT, g_key_ctrl.col ))
        {            
            goto key_proc_end;
        }

        OSTimeDly(30); //20ms

        if(GPIO_PIN_RESET != HAL_GPIO_ReadPin(KEYBOARD_COL_PORT, g_key_ctrl.col ))
        {            
            goto key_proc_end;
        }


		for(i = 0; i < KEYBOARD_ROW_NUM; i++)
		{
		    
            GPIO_SetBits(KEYBOARD_ROW_PORT, KEYBOARD_ROW);
			GPIO_ResetBits(KEYBOARD_ROW_PORT, KEYBOARD_ROW_BASE << i); 
            OSTimeDly(1);
            
			if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(KEYBOARD_COL_PORT, g_key_ctrl.col))
			{
                g_sys_ctrl.led_count = LED_KEY_DISP_TIME;

                LED_KEY_ON();

                LCD_BL_ON();
                
                LED_SLEEP_OFF();

                break;
			}		
        }

        switch(g_key_ctrl.col)
        {
        case GPIO_PIN_6:
            n = 0;
            break;
        case GPIO_PIN_7:            
            n = 1;
            break;
        case GPIO_PIN_8:
            n = 2;
            break;
        case GPIO_PIN_9:
            n = 3;
            break;
        default:
            goto key_proc_end;
        }
        
        if(i < KEYBOARD_ROW_NUM)
        {
            key_msg = s_key_msg_map[n][i];

            if(FALSE == g_sys_ctrl.self_check)
            {
                GUI_SendKeyMsg(key_msg , 1); //按下的时候发送
            }
            else
            {
                OSMboxPost(g_mbox_chk_key, &key_msg); //华兄
            }
        }
        while(1)
        {
            OSTimeDly(1);
            if(GPIO_PIN_RESET != HAL_GPIO_ReadPin(KEYBOARD_COL_PORT, g_key_ctrl.col))
                break;
        }

        g_sys_ctrl.shutdown_timeout = 0;
        g_sys_ctrl.sleep_timeout = 0;

        if(FALSE == g_sys_ctrl.self_check)
        {
            GUI_SendKeyMsg(key_msg , 0); //松开的时候发送
        }

        if(g_sys_ctrl.sysPowerState == SYS_POWER_IDLE)
            APP_Wakeup();

key_proc_end:
        GPIO_ResetBits(KEYBOARD_ROW_PORT, KEYBOARD_ROW);
        if(g_key_ctrl.col)
        {
            __HAL_GPIO_EXTI_CLEAR_FLAG(g_key_ctrl.col);            
        }

        g_key_ctrl.col = 0;
        
 
	}
}

void EXTI9_5_IRQHandler(void)
{
/*************************************************************************************
    if(EXTI_GetITStatus(EXTI_Line9) != RESET)  //首先判断是否是设置的IO口产生的中断
    {
     //添加中断处理程序

     EXTI_ClearFlag(EXTI_Line9);       //清除中断标志（必须）
    
     EXTI_ClearITPendingBit(EXTI_Line9);   //清除ENTI线上挂起位 
  
     }
  *************************************************************************************/
    if(RESET != __HAL_GPIO_EXTI_GET_IT(GPIO_PIN_6)) 
    {   
        if(g_key_ctrl.col==0)
        {
            g_key_ctrl.col = GPIO_PIN_6;		
            OSSemPost(g_key_ctrl.sem);
        }
        else
            __HAL_GPIO_EXTI_CLEAR_FLAG(GPIO_PIN_6);   
    }     

    if(RESET != __HAL_GPIO_EXTI_GET_IT(GPIO_PIN_7)) 
    { 
        if(g_key_ctrl.col==0)
        {
            g_key_ctrl.col = GPIO_PIN_7;		
            OSSemPost(g_key_ctrl.sem);
        }
        else
            __HAL_GPIO_EXTI_CLEAR_FLAG(GPIO_PIN_7);  
    }     

    if(RESET != __HAL_GPIO_EXTI_GET_IT(GPIO_PIN_8)) 
    { 
        if(g_key_ctrl.col==0)
        {
            g_key_ctrl.col = GPIO_PIN_8;		
            OSSemPost(g_key_ctrl.sem);  
        }
        else
            __HAL_GPIO_EXTI_CLEAR_FLAG(GPIO_PIN_8);        
    }      

    if(RESET != __HAL_GPIO_EXTI_GET_IT(GPIO_PIN_9)) 
    { 
        if(g_key_ctrl.col==0)
        {
            g_key_ctrl.col = GPIO_PIN_9;		
            OSSemPost(g_key_ctrl.sem);  
        }
        else
            __HAL_GPIO_EXTI_CLEAR_FLAG(GPIO_PIN_9);        
    }  
}

