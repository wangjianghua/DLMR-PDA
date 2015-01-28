#ifndef __UART_LINK_H__
#define __UART_LINK_H__

#ifdef __cplusplus
 extern "C" {
#endif


#define LED_UART_EN               0u
#define LED_UART_ON()     BSP_LED_On(LED_UART)
#define LED_UART_OFF()    BSP_LED_Off(LED_UART)

#define PC_COM_PORT                0
#define RS485_COM_PORT             1
#define PLC_COM_PORT               2
#define MAX_COM_PORT               3
#define START_COM_PORT        PC_COM_PORT

#define END_OBJECT_NUM        MAX_COM_PORT

#define PC_HUART              huart4
#define RS485_HUART           huart1
#define PLC_HUART             huart2

#define END_TX_QUEUE_SIZE          5
#define END_RX_QUEUE_SIZE          5 

#define END_NON_MODULE_USED     0xff

#define END_IDLE       1            /* idle status */
#define END_READ       2            /* read mode */
#define END_WRITE      3            /* write mode */
#define END_BUSY       4            /* busy status */
#define END_FORWORD   (5)
#define END_USERD   0xfe

#define END_STATUS_IDLE         0
#define END_STATUS_SENDING      1
#define END_STATUS_RECEIVING    2

//#define END_STATUS_MUX            2  //双工
//#define END_STATUS_MIRROR       3  //镜像

#define END_STATUS_SPI_R        4  //当前为SPI口读传输状态
#define END_STATUS_SPI_W        5  //当前为SPI口写传输状态

#define END_STATUS_INVALID         6
#define END_STATUS_ERROR         7

//
#define END_STATUS_SEND_DONE         8
#define END_STATUS_SEND_TIMEOUT         9
#define END_STATUS_RECV_DONE         10
#define END_STATUS_RECV_TIMEOUT         11

#define END_DEBUG                 1

#define END_FAST_PROETCT_CYCLE    (unsigned char)1     /* 1 s */
#define END_SLOW_PROETCT_CYCLE    (unsigned char)5     /* 4 s */
#define END_SLOW_PROETCT_CYCLE2    (unsigned char)5     /* 5 s */

//#define RX_INTERNET_BUFFER_SIZE         4800

typedef U32 (* END_RECV_PTR)(UCHAR* txbuf, USHORT txnum);
typedef U32 (* END_SEND_PTR)(UCHAR* txbuf, USHORT txnum);
//typedef MD_STATUS (* END_WRITE_PTR)(UCHAR* wBuf, USHORT wNum,USHORT wAddr);
typedef U32 (* END_UPPER_CALL_BACK)();
typedef U32 (* END_TIMEOUT_CALL_BACK)(pvoid h);
typedef void (* END_START_PTR)(void);
typedef void (* END_STOP_PTR)(void);
typedef void (* END_SWITCH_STATUE_PTR)(unsigned char new_status);

typedef struct
{
    //U16 gUartRxCnt;
    //U16 gUartRxLen;
    U16 gUartTxCnt;
    U8 *gpUartTxAddress;
    U8 *gpUartRxAddress;
    U8 *gpUartRxStartAddress;
    U8 *gpUartRxEndAddress;
    U8 *gpUartRxReadAddress;
}UART_CCB, *P_UART_CCB;

typedef struct _end_object_static_
{
    unsigned long rxPacketCount;
    unsigned long txPacketCount;
} END_STAT, *P_END_STAT;

typedef struct _end_object_
{
    unsigned char end_id;
    unsigned char end_send_status;

    unsigned short receive_len;            //当前收包长度
    unsigned short last_receive_len;    //上次收包长度
    unsigned char  * end_recv_buffer;

    END_START_PTR Start;                //开始工作
    END_STOP_PTR Stop;                //end 复位
    END_SEND_PTR Send;

    END_RECV_PTR RecvData;                //end 复位
    //END_UPPER_CALL_BACK UpperCallBack;
    //END_SWITCH_STATUE_PTR SwitchStatus;  //对于半双工端口，切换端口工作状态
    END_STAT endStatistics;

    P_MSG_INFO pMsgInfo;   //当前正在处理的消息报文

    unsigned char recv_timeout;

} END_OBJ, *P_END_OBJ;

extern UART_CCB g_uart_ccb[MAX_COM_PORT];
extern END_OBJ g_EndObjectPool[MAX_COM_PORT];
extern queue *g_EndRxQueue[MAX_COM_PORT];
extern pvoid g_EndObject[END_OBJECT_NUM];
extern OS_EVENT *g_sem_end;

P_END_OBJ End_get_end_obj(UCHAR end_id);
void End_Init(void);
unsigned short End_send(P_MSG_INFO pMsgInfo);
unsigned char End_check_recv(P_END_OBJ pEndObj);
unsigned char End_check_send(UCHAR end_id);
unsigned char End_IsIdle(P_END_OBJ pEndObj);
unsigned char End_postProcess(unsigned char end_type,  pvoid h);
unsigned short End_tick_check(void);
U32 UART_ReceiveData(U8 end_id, UCHAR* rxbuf, USHORT rxnum);
void App_TaskEndTick(void *p_arg);
void App_TaskEndProc(void *p_arg);
void USART_IRQProc(UART_CCB *uccb, UART_HandleTypeDef *huart);
void UART4_IRQHandler(void);
void USART2_IRQHandler(void);   
void USART1_IRQHandler(void);


#ifdef __cplusplus
}
#endif

#endif
