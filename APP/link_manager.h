#ifndef __LINK_MANAGER_H__
#define __LINK_MANAGER_H__

#ifdef __cplusplus
 extern "C" {
#endif


#define UART_RECEIVE_BUF_SIZE              256u

#define MAX_MSG_SHORT                         9
#define MAX_MSG_LONG                          4
#define MAX_MSG_LARGE                         4

#define GET_MAX_MSG(type)             (MAX_MSG_CNT[type])

#define FREE                                  0
#define ALLOC                                 1
#define SENDING                               2
#define SENDED                                3

typedef enum
{
    MSG_SHORT = 0,
    MSG_LONG,
    MSG_LARGE,
    MAX_MSG_ITEM
} MSG_TTYPE;

//用来保存发送，接受消息的相关信息，内容待完善
typedef struct _msg_header_
{
    unsigned short msg_len;
    //unsigned short msg_wait_time;  //for plc layer
    unsigned short wait_resp_time; //单位0.1s，发送完毕后等待响应的时间，for plc layer
    unsigned long time_stamp;
    OS_EVENT * msg_event;
    unsigned char msg_type;  //
    unsigned char send_pid;
    //unsigned char msg_id;
    unsigned char end_id;
    unsigned char sub_id;
    unsigned char block_state;    /*free, allocated*/
    unsigned char need_buffer_free;    /* TRUE 标识end 发送完后负责释放；FALSE 标识end 不管释放由application  负责释放*/

} MSG_HEADER, *P_MSG_HEADER;

typedef struct _msg_info_
{
    MSG_HEADER msg_header;
    unsigned char  msg_buffer[1];
} MSG_INFO, *P_MSG_INFO;

//内存中消息队列的消息结构体
typedef struct _large_msg_info_
{
    MSG_HEADER msg_header;
    unsigned char  msg_buffer[UART_RECEIVE_BUF_SIZE];
} MSG_LARGE_INFO, *P_LARGE_MSG_INFO;

//内存中消息队列的消息结构体
typedef struct _long_msg_info_
{
    MSG_HEADER msg_header;
    unsigned char  msg_buffer[UART_RECEIVE_BUF_SIZE];
} MSG_LONG_INFO, *P_LONG_MSG_INFO;

//下行消息队列的消息结构体
typedef struct _short_msg_info_
{
    MSG_HEADER msg_header;
    unsigned char  msg_buffer[UART_RECEIVE_BUF_SIZE];
} MSG_SHORT_INFO, *P_SHORT_MSG_INFO;

extern MSG_SHORT_INFO gShortMsgPool[MAX_MSG_SHORT];
extern P_SHORT_MSG_INFO pShortMsgPool[MAX_MSG_SHORT];

void mem_msg_buffer_init(MSG_INFO * MsgPool, P_MSG_INFO * MsgArray, unsigned short MsgNum, U16 msg_size);
P_MSG_INFO alloc_send_buffer(unsigned char type);
unsigned char free_send_buffer(pvoid pmsg );


#ifdef __cplusplus
}
#endif

#endif
