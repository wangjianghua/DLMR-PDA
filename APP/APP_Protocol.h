#ifndef __APP_PROTOCOL_H__
#define __APP_PROTOCOL_H__

#ifdef __cplusplus
 extern "C" {
#endif


#define LED_PLC_TOGGLE()        BSP_LED_Toggle(LED_PLC)
#define LED_PLC_OFF()           BSP_LED_Off(LED_PLC)
#define LED_PLC_ON()            BSP_LED_On(LED_PLC)

#define mPLC_REPLY_ADDR_LEN    sizeof(mPLC_REPLY_ADDR)

#define FREQ_NUM               2

#define FREQ_270KHz            0
#define FREQ_421KHz            1

#define FREQ_INDEX             0
#define FREQ_LEN               1

#define DL645_INDEX           (FREQ_INDEX + FREQ_LEN)  

#define PLC_RES_SUCC           0
#define PLC_RES_FAIL           1
#define PLC_RES_TIMEOUT        2

#define PLC_MSG_NOINIT              0
#define PLC_MSG_IDLE                1
#define PLC_MSG_SENDING             2
#define PLC_MSG_RECEIVED            3


#define GUI_PLC_MSG_IDLE        0
#define GUI_PLC_MSG_TEST        1    //规约调试
#define GUI_PLC_MSG_READ        2    //抄表
#define GUI_PLC_MSG_SET         3    //载波设置
#define GUI_PLC_MSG_LISTING     4    //状态监控
#define GUI_PLC_MSG_MEMORY      5    //内存管理


typedef struct _plc_prm
{
    u8 send_buf[256]; //DL645发送帧
    u16 send_len; //DL645发送帧长度

    u8 recv_buf[256]; //DL645接收帧
    u16 recv_len; //DL645接收帧长度

    u8 data_buf[30]; //提取数据
    u16 data_len; //提取数据的长度

    u8 sd_buf[512];

    u8 sendStatus;
    u8 result; //PLC通信结果: PLC_RES_SUCC、PLC_RES_FAIL、PLC_RES_TIMEOUT    
} PLC_PRM, *P_PLC_PRM;

extern OS_EVENT *g_sem_plc;
extern u8 g_cur_freq;
extern PLC_PRM g_plc_prm;

u16 pc_uart_send(u8 *buf, u16 len);
u16 plc_uart_send(u8 *buf, u16 len);
u16 cplc_read_addr(void);
unsigned int PC_postProcess(pvoid h);
unsigned int RS485_postProcess(pvoid h);
unsigned int PLC_postProcess(pvoid h);
void App_TaskPLC(void *p_arg);


#ifdef __cplusplus
}
#endif

#endif
