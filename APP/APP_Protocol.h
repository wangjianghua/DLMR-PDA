#ifndef __APP_PROTOCOL_H__
#define __APP_PROTOCOL_H__

#ifdef __cplusplus
 extern "C" {
#endif


//#define PLC_ADDR_SIZE           sizeof(g_gui_para.relayAddr)/sizeof(g_gui_para.relayAddr[0])

//#define PLC_ADDR_SIZE()         LISTVIEW_GetNumRows(RLY_GetListAddr())

#define mPLC_REPLY_ADDR_LEN    sizeof(mPLC_REPLY_ADDR)

#define FREQ_NUM               2

#define FREQ_270KHz            0
#define FREQ_421KHz            1

#define FREQ_INDEX             0
#define FREQ_LEN               1

#define DL645_INDEX           (FREQ_INDEX + FREQ_LEN)  

//启动中继条件下，控制字的附加值
#define DL645_RELAY_ADDED_VAL    0x20

#define RF_SEND_FIX_LEN         37
#define RF_RECV_FIX_LEN          RF_SEND_FIX_LEN + 4

#define RF_PRINT_FIX_LEN       8

#define RF_SEND_BUF         rf_send_buf
#define RF_SEND_LEN         rf_send_len
#define RF_RECV_BUF         g_rf_param.rx.buf
#define RF_RECV_LEN         g_rf_param.rx.rx_len

#define PLC_270_III              0
#define PLC_270_III_5            1
#define PLC_270_II               2
#define PLC_421_50BPS            3
#define PLC_421_100BPS           4
#define PLC_421_600BPS           5
#define PLC_421_1200BPS          6

#define PLC_BPS_SNAIL            15
#define PLC_BSP_100              10
#define PLC_BPS_SLOW             6
#define PLC_BPS_MIDDLE           5         
#define PLC_BPS_FAST             4
//#define PLC_BPS_HIGH             4




#define PLC_RES_SUCC           0
#define PLC_RES_FAIL           1
#define PLC_RES_TIMEOUT        2
#define PLC_RES_ERROR_FRAME    3
#define PLC_RES_NONE           4



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
#define GUI_PLC_MSG_FREQ        6    //速率设置


#define FILE_NAME_LEN           12
#define FILE_NUM_LEN             4
#define FILE_SIZE_LEN            4
#define MAX_FILE_NUM            10
#define SEQ_LEN                  1
#define VERSION_LEN              6

#define SHAKE_HANDS_CMD     0xF0000000
#define SCAN_FILE_CMD       0xF0000100
#define READ_FILE_CMD       0xF0010100
#define READ_TIME_CMD       0xF0100000
#define WRITE_TIME_CMD      0xF0110000
#define READ_VERSION_CMD    0xF0100001
#define RESET_CMD           0xF0100100

typedef struct _proto_para_
{
    u8 send_buf[256]; //DL645发送帧
    u16 send_len; //DL645发送帧长度

    u8 recv_buf[256]; //DL645接收帧
    u16 recv_len; //DL645接收帧长度

    u8 data_buf[30]; //提取数据
    u16 data_len; //提取数据的长度

    u8 fm_buf[512];

    u8 sendStatus;
    u8 result; //PLC通信结果: PLC_RES_SUCC、PLC_RES_FAIL、PLC_RES_TIMEOUT    
} PROTO_PARA, *P_PROTO_PARA;

extern OS_EVENT *g_sem_plc;
extern OS_EVENT *g_sem_rf;
extern OS_EVENT *g_sem_pc;
extern OS_EVENT *g_sem_rs485;
extern OS_EVENT *g_sem_check;
extern OS_EVENT *g_sem_chk_plc;
extern OS_EVENT *g_sem_chk_rf;
extern u8 g_cur_freq;
extern PROTO_PARA g_proto_para;
extern u8 rf_send_buf[256];
extern DL645_Frame_C dl645_frame_send;
extern DL645_Frame_C dl645_frame_recv;
extern DL645_Frame_Stat_C dl645_frame_stat;

u16 pc_uart_send(u8 *buf, u16 len);
u16 rs485_uart_send(u8 *buf, u16 len);
u16 plc_uart_send(u8 *buf, u16 len);
u16 ir_uart_send(u8 *buf, u16 len);
unsigned int PC_postProcess(pvoid h);
unsigned int RS485_postProcess(pvoid h);
unsigned int PLC_postProcess(pvoid h);
unsigned int IR_postProcess(pvoid h);
u16 cplc_read_addr(void);
void App_TaskProto(void *p_arg);
void App_TaskPC(void *p_arg);
void App_TaskRS485(void *p_arg);


#ifdef __cplusplus
}
#endif

#endif
