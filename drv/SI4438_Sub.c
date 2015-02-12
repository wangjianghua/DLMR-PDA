/******************************************************************************

 *
 * ---------------------------------------------------------------------------
 * Copyright (c) 2013 Mega Hunt Micro Technology Inc. All Rights Reserved.
 *****************************************************************************/
#include "includes.h"
#include "si4438_sub.h"
#include "si4438.h"

#define RF_FREQ_470MHz





#ifdef RF_FREQ_470MHz

rf_param  g_rf_param;
u8 RF_LED_Timeout;

u8 g_plc_buf[100]={0};



	
/*****************************************************************************
 * Name			: SI_Init
 * Function		: 初始化MH1803 以及相关的寄存器
 * ---------------------------------------------------------------------------
 * Input Parameters	: 无
 * Output Parameters	: 无
 * Return Value		: 
 	返回类型:rf_status_t
 	RF_STATUS_FALSE:初始化失败
 	RF_STATUS_SUCCESS:初始化成功
 	RF_STATUS_RESET:复位异常 	
 * ---------------------------------------------------------------------------
 * Description		:
 *****************************************************************************/
 RF_chen_C	RF_chen={0,0};
 void RF_Set_Channel(u8 chen)
 {
	RF_chen.chen_new = chen;
 }

/*****************************************************************************
 * Name			: RF_Tx
 * Function		: 发送数据，由外部模块调用
 * ---------------------------------------------------------------------------
 * Input Parameters	: buf :发送数据缓冲区的首地址
 *			  len :待发送数据的长度，单位字节
 * Output Parameters	: 无
 * Return Value		: 
 		RF_STATUS_SUCCESS: 发送成功
 *		RF_STATUS_RESET:发送中复位
 		RF_STATUS_TIMEOUT:发送失败
 
 * ---------------------------------------------------------------------------
 * Description		:在调用本函数前需要查询is_rf_tx_blocked()是否为假(即射频空闲)，
 只有射频空闲才能调用 本rf_tx函数，否则可能会导致程序阻塞在本函数内	 	
 *****************************************************************************/
rf_status_t RF_Tx(u8 * buf, u8 len)
{
    u16 crc;

    
    DISABLE_RF_INT();
    memmove(buf + 1, buf, len); //数组全体往后挪一个位置
    buf[0] = len;
    crc = cal_crc_ITU(buf, len + 1);
    
    buf[len + 1] = (u8)crc;
    buf[len + 2] = (u8)(crc >> 8);
    g_rf_param.tx.buf = buf;
    g_rf_param.tx.tx_len = len + 3;
    
    g_rf_param.tx.Index = SI_Send_Packet(len + 3, buf);

    ENABLE_RF_INT();
    g_rf_param.rf_state = RF_STATE_TX; 
    g_rf_param.tx.Timeout = 1;
    
    
    
    
    
    return (RF_STATUS_SUCCESS);
}




	
/*****************************************************************************
 * Name			: rf_interrupt
 * Function		: 中断处理函数
 * ---------------------------------------------------------------------------
 * Input Parameters	: 无
 * Output Parameters	: 无
 * Return Value		: 无
 *
 * ---------------------------------------------------------------------------
 * Description		:本函数处理MH1803的中断,发送完成中断、接收
 * 完成中断、接收 将满中断、发送将空中断、发送将满中断 	
 * 	
 ******************************************************************************/
//------------------------------射频事件处理----------------------------------------

void RF_Event_Handle(void)
{
    u8 sta1,L;

    //BSP_LED_Toggle(0);

    //GPIO_ResetBits(GPIOA, GPIO_PIN_0);
    
    sta1 = SI_Read_INT_Info(); 

    if(sta1 & RXPKT_DONE) 
    {//接收数据包完成中断的处理

        //BSP_LED_On(0);
		
        g_rf_param.RSSI = SI_Read_RSSI();
        if (RF_STATE_RX != g_rf_param.rf_state)
        {
            _sys_plc_stop();
            g_rf_param.rx.rx_index = 0;
            g_rf_param.rf_state = RF_STATE_RX;
            g_rf_param.rx.rx_len = SI_Read_Length();
            
            //Read_PayLoad(g_rf_param.rx.buf+g_rf_param.rx.rx_index);
        }
        
        SI_Read_Fifo(   g_rf_param.rx.rx_len - g_rf_param.rx.rx_index,
                        g_rf_param.rx.buf + g_rf_param.rx.rx_index );
        g_rf_param.rx.rx_index = g_rf_param.rx.rx_len;

        
        RF_Listen();

        //BSP_LED_Off(0);



        //g_rf_param.rx.rx_len=0; //华兄
        g_rf_param.rx.Timeout=0;
		//_sys_plc_reset();
        OSSemPost(g_sem_rf); //华兄
	}
	 else if (sta1 & RXFF_AF) 
	{   
		if (RF_STATE_RX != g_rf_param.rf_state)
		{              
			 _sys_plc_stop();
			g_rf_param.rx.rx_index = 0;
			g_rf_param.rf_state = RF_STATE_RX;
            g_rf_param.rx.rx_len =SI_Read_Length();//Read_PayLoad(g_rf_param.rx.buf+g_rf_param.rx.rx_index);
		}
		SI_Read_Fifo(RX_AF_THRESHOLD,g_rf_param.rx.buf+g_rf_param.rx.rx_index);
		g_rf_param.rx.rx_index+=RX_AF_THRESHOLD;
		g_rf_param.rx.Timeout=1;
		//_sys_plc_reset();

	}
	else if(g_rf_param.rf_state == RF_STATE_TX)
	{
		if (sta1 & TXFF_AE) 
		{
			if(g_rf_param.tx.tx_len>g_rf_param.tx.Index)
				{
					L = SI_Write_Fifo(g_rf_param.tx.tx_len - g_rf_param.tx.Index,
                                            g_rf_param.tx.buf + g_rf_param.tx.Index);
					g_rf_param.tx.Index += L;
					g_rf_param.tx.Timeout = 1;
					TX_LED_ON(); 
                    //RF_Cnt=0;
				}
                     
		}
                //else
		if (sta1 & TXPKT_DONE) 
		{
		    //发送数据包完成中断的处理
			TX_LED_ON();         
            //点亮发送LED
//#ifdef RF_Mon_TS
            if(RF_chen.chen_new^RF_chen.chen_old)
            {
                SI_Freq_Set(RF_chen.chen_new/33,RF_chen.chen_new%33);
                RF_chen.chen_old=RF_chen.chen_new;
            }
//#endif
            RF_Listen();
            g_rf_param.tx.Timeout=0;
            //BSP_LED_Off(0);
            _sys_plc_reset();
		}
	}
    else
    {
        RF_Listen();
    }

    //GPIO_SetBits(GPIOA, GPIO_PIN_0);

   
}   
void RF_Channel_Update(u8 chen)
{
	SI_Freq_Set(chen/2,chen%2);
}

u8 RF_Read_Sig(void)
{
   // memcpy(buf,(u8 *)&g_rf_param.RSSI,4);

    return(g_rf_param.RSSI);
}

void RF_Timeout_Sub(void)
{
   
    if(g_rf_param.rx.Timeout)
    {
        g_rf_param.rx.Timeout++;
        if(g_rf_param.rx.Timeout>30)//大于300ms超时
        {
            RF_Listen();
            g_rf_param.rx.Timeout=0;
        }
       
    }
}
/*****************************************************************************
 * Name			:  RF_Reset()
 * Function		: 复位射频芯片，并使其处于监听状态，本函数供普通函数调用
 * ---------------------------------------------------------------------------
 * Input Parameters	: 无	
 * Output Parameters	: 无
 * Return Value		: 无
 *
 * ---------------------------------------------------------------------------
 * Description		:
 * *****************************************************************************/	
void RF_Reset()
{
	SI_Init();
}

/*****************************************************************************
 * Name			:  RF_Listen()
 * Function		: 使射频转换到接收状态，本函数供普通函数调用
 * ---------------------------------------------------------------------------
 * Input Parameters	: 无	
 * Output Parameters	: 无
 * Return Value		: 无
 *
 * ---------------------------------------------------------------------------
 * Description              :
 * *****************************************************************************/
void RF_Listen()
{
	SI_Enter_Rx();
	g_rf_param.rf_state = RF_STATE_LISTEN;
}

void RF_Int_Proc()
{
    if(RESET != __HAL_GPIO_EXTI_GET_IT(GPIO_PIN_14)) 
    {   
        __HAL_GPIO_EXTI_CLEAR_FLAG(GPIO_PIN_14);  
        RF_Event_Handle();
    }  

}

void RF_Init(void)
{
    //u8 val;

    if((SpiHandle.Instance->CR1 &SPI_CR1_SPE) != SPI_CR1_SPE)
    {
      /* Enable SPI peripheral */
      __HAL_SPI_ENABLE(&SpiHandle);
    }

	//DISABLE_RF_INT();	//关闭外部中断
	RF_PWR_ON();
	
	OSTimeDly(2);    

    BSP_IntVectSet(BSP_INT_ID_EXTI15_10, RF_Int_Proc);
    DISABLE_RF_INT();

    DISABLE_RF_CHIP(); // 关闭射频芯片
    	
    OSTimeDly(2);
    
	ENABLE_RF_CHIP(); // 使能射频芯片

    OSTimeDly(300);   //在3个节拍中查看复位是否完成	


    

    g_rf_param.rx.buf=RF_buf;
    g_rf_param.rx.rx_index=0;
    g_rf_param.rx.rx_len=0;
    g_rf_param.rx.Timeout=0;
    g_rf_param.tx.buf=RF_buf;
    g_rf_param.tx.Index=0;
    g_rf_param.tx.tx_len=0;
    g_rf_param.tx.Timeout=0;



    SI_Init();
  
    

    __HAL_GPIO_EXTI_CLEAR_FLAG(GPIO_PIN_14);
    
    ENABLE_RF_INT();	           //开启外部中断
    
    
}


//#endif
#endif
