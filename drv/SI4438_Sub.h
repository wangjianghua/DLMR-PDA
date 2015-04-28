/*****************************************************************************

 * ---------------------------------------------------------------------------
 * Modification Record:
 *   Version		Name			date 		Content
 *     1.0		lihongxu         2013-05-08	        Create this file.
 *
 * Note			: 中断方式收发头文件
 *
 * ---------------------------------------------------------------------------
 * Copyright (c) 2013 Mega Hunt Micro Technology Inc. All Rights Reserved.
 *****************************************************************************/
#include"Includes.h"

#ifndef RADIO_MH1803_INTERRUPT_H
#define RADIO_MH1803_INTERRUPT_H

#define MAX_RF_PKT_LEN	256 	//可缓存的最大包长

#define RF_DEBUG	0		//射频调试开关 1:开启打印调试 0:关闭打印调试

#define RF_COMMON_REG_CHANGED	1	//0:使用芯片默认配置(建议)	1:修改调制模式、前导长度、同步字、Header等


#define TX_LED_ON()     RF_LED_Timeout=1;//(P2 &= 0xFD)		//点亮发送LED
#define TX_LED_OFF()    RF_LED_Timeout=0;//(P2 |= 0x02)		//关闭发送LED
#define RX_LED_ON()     RS485_Wait_Timeout(0,40)//(P7 &= 0xF7)		//点亮接收LED
#define RX_LED_OFF()    (P3 |= 0x08)	//关闭接收LED	



#define ENABLE_RF_CHIP()	GPIO_ResetBits(GPIOG,GPIO_PIN_15);   //sdn 0//P2&=0xBF;//P8&=0xFD;//1803为低电平使能芯片
#define DISABLE_RF_CHIP()	GPIO_SetBits(GPIOG,GPIO_PIN_15);   //sdn 1//P2|=0x40;//P8|=0x02;//1803为高电平禁用芯片]

#define HAVE_INTERRUPT()	//P8&0x04
#define DISABLE_RF_INT()    HAL_NVIC_DisableIRQ(EXTI15_10_IRQn)     //INTP7_Disable()		           //禁用外部中断
#define ENABLE_RF_INT()	    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn)      //INTP7_Enable()
#define RF_INT_CLEAR()	    __HAL_GPIO_EXTI_CLEAR_FLAG(GPIO_PIN_14);  
//开启外部中断

#define  RF_buf g_plc_buf

#define _sys_plc_stop() 
#define _sys_plc_reset()

#define SI_TYPE     0x4438

enum
{
	RF_STATE_IDLE,	//射频空闲状态
	RF_STATE_LISTEN,	//射频侦听状态
	RF_STATE_RX,		//射频接收状态
	RF_STATE_TX		//射频发送状态
};

 
#pragma pack(1)
typedef struct
{
    u16 Val:15;
    u16 Sig:1;
}RF_Sig_C;
typedef struct
{
    RF_Sig_C RSSI;
    RF_Sig_C FREQ;
}RF_Signal_C;
typedef struct
{
	u8 chen_new;
	u8 chen_old;
}RF_chen_C;


typedef struct rf_param
{
	u8 rf_state;          			 //当前射频所处状态
	struct rx 
	{//接收数据结构	
		u8 *buf;		    //接收缓冲区最大255
		u8 rx_index;	    //统计接收到的长度的总和，每来一次中断就将pkt_len长度值累加起来
		u8 rx_len;			//通过阀值来确定报中断的长度，发送完成的长度或者阀值的长度
		u8 Timeout;
	}rx;
	
	struct tx 
	{//发送数据结构
		u8 *buf;  		//发送数据的缓冲区
		u8 Index;			//待发送数据的指针，指向下一个要发送的字节地址
		u8 tx_len;			//剩余待发送数据的长度
		u8 Timeout;
	}tx;
    u8 RSSI;
    short cfo;
}rf_param;

#pragma pack()
/////////////////////////// End 板级相关定义 ///////////////////////////

typedef enum {
	RF_STATUS_SUCCESS = 0,		//接收数据包成功、发送数据包成功或初始化成功
	RF_STATUS_FALSE,			//因接收到的长度为0或长度超出缓冲区而无法接收、或其他失败
	RF_STATUS_TIMEOUT,		//接收或发送超时
	RF_STATUS_RESET,			//芯片发生复位或因故障程序主动对芯片进行了复位操作
	RF_STATUS_RXWAIT,			//程序等待接收过程中
}rf_status_t;
#define RF_AIR_BAUD_RATE 2400
#define RF_STATISTICS
#define MH1803_frm_change(x,y) RF_Frame_chang(x,y)
extern rf_param g_rf_param;
/*****************************************************************************
 * Name			: SI_Init
 * Function		: 初始化MH1803射频芯片及本模块的全局变量
 * ---------------------------------------------------------------------------
 * Input Parameters	: 无	
 * Output Parameters	: 无
 * Return Value		: 无
 *
 * ---------------------------------------------------------------------------
 * Description		: 在进行操作SPI前需要关闭中断(至少保证射频中断关闭)，然后再
 *	 		  通过spi	配置芯片。芯片初始化完毕后为空闲状态，再将芯片转换到
 *	 		  侦听状态，最后开启中断。
 *	 	             在模块初始化uart/spi/timer后调用此初始化函数
 * 	
 *****************************************************************************/
rf_status_t SI_Init();


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
rf_status_t RF_Tx(u8 * buf, u8 len);


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
void RF_Reset();





/*****************************************************************************
 * Name				: read_reg_intr
 * Function			: 读射频芯片寄存器的内容，本函数供普通函数调用
 * ---------------------------------------------------------------------------
 * Input Parameters	: addr:需要读取寄存器的地址
 * Output Parameters	: 无
 * Return Value		: 读取的寄存器的值。
 *			
 * ---------------------------------------------------------------------------
 * Description			: 本函数不允许被射频的中断打断
 *			
 *****************************************************************************/
u8 read_reg(u8 addr);


/*****************************************************************************
 * Name			: write_reg
 * Function		: 写射频芯片的寄存器，本函数供普通函数调用
 * ---------------------------------------------------------------------------
 * Input Parameters	: addr :寄存器的地址
 *			  val   :待写入的寄存器的值
 * Output Parameters	: 无
 * Return Value		: 无
 *
 * ---------------------------------------------------------------------------
 * Description		:
 *****************************************************************************/
void write_reg(u8 addr, u8 val);
void RF_Event_Handle(void);
void Rf_recv_frm(void);
void RF_Timeout_Sub(void);
u8 RF_Read_Sig(void);
u8 Read_RF_Dat_EN(u8 dir);
void RF_check_RevFin(void);
 void RF_Set_Channel(u8 chen);
 void RF_Channel_Update(u8 chen);

void RF_Init(void);
void RF_Listen();
 
#ifdef RF_STATISTICS
void rf_rssi_monitor(void);
//void rf_fdev_monitor(u8 STA2_Val);
u16 get_current_afc();
u16 get_current_rssi(void);
void carrier_output(void);
u8 RF_Config_Power_INC(void);



#endif
#endif
