#include "includes.h"


const INT8U rPLC_TO_mPLC[] = {0x68, 0xDD, 0xAB, 0xCF, 0xEA, 0xBC, 0xDA, 0x68, 0x04, 0x07, 0x33, 0x34, 0x36, 0x33, 0x33, 0x33, 0x34, 0x1C, 0x16};
const INT8U mPLC_TO_rPLC[] = {0x68, 0xDD, 0xAB, 0xCF, 0xEA, 0xBC, 0xDA, 0x68, 0x04, 0x07, 0x33, 0x34, 0x36, 0x33, 0x33, 0x33, 0x33, 0x1B, 0x16};

const INT8U READ_PLC_NODE[] = {0x68, 0x1E, 0x00, 0x41, 0x04, 0x00, 0x5F, 0x64, 0x04, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x16, 0x01, 0x00, 0x02, 0x02, 0xB4, 0xD8, 0x16};

const INT8U Broad_Read_Addr[] = {0x68, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x68, 0x13, 0x00, 0xDF, 0x16};

const INT8U rPLC_270III[]  = {0x68, 0xDD, 0xAB, 0xCF, 0xEA, 0xBC, 0xDA, 0x68, 0x04, 0x08, 0x34, 0x34, 0x36, 0x33, 0x33, 0x33, 0x33, 0x33, 0x50, 0x16};
const INT8U rPLC_270III5[] = {0x68, 0xDD, 0xAB, 0xCF, 0xEA, 0xBC, 0xDA, 0x68, 0x04, 0x08, 0x34, 0x34, 0x36, 0x33, 0x33, 0x33, 0x34, 0x33, 0x51, 0x16};
const INT8U rPLC_270II[]   = {0x68, 0xDD, 0xAB, 0xCF, 0xEA, 0xBC, 0xDA, 0x68, 0x04, 0x08, 0x34, 0x34, 0x36, 0x33, 0x33, 0x33, 0x35, 0x33, 0x52, 0x16};

const INT8U rPLC_421_50BPS[]   = {0x68, 0xDD, 0xAB, 0xCF, 0xEA, 0xBC, 0xDA, 0x68, 0x04, 0x08, 0x34, 0x34, 0x36, 0x33, 0x33, 0x33, 0x33, 0x34, 0x51, 0x16};
const INT8U rPLC_421_100BPS[]  = {0x68, 0xDD, 0xAB, 0xCF, 0xEA, 0xBC, 0xDA, 0x68, 0x04, 0x08, 0x34, 0x34, 0x36, 0x33, 0x33, 0x33, 0x34, 0x34, 0x52, 0x16};
const INT8U rPLC_421_600BPS[]  = {0x68, 0xDD, 0xAB, 0xCF, 0xEA, 0xBC, 0xDA, 0x68, 0x04, 0x08, 0x34, 0x34, 0x36, 0x33, 0x33, 0x33, 0x35, 0x34, 0x53, 0x16};
const INT8U rPLC_421_1200BPS[] = {0x68, 0xDD, 0xAB, 0xCF, 0xEA, 0xBC, 0xDA, 0x68, 0x04, 0x08, 0x34, 0x34, 0x36, 0x33, 0x33, 0x33, 0x36, 0x34, 0x54, 0x16};

OS_EVENT *g_sem_plc;
OS_EVENT *g_sem_rf;
OS_EVENT *g_sem_ir;
OS_EVENT *g_sem_pc;
OS_EVENT *g_sem_rs485;
OS_EVENT *g_sem_check;
OS_EVENT *g_sem_chk_plc;
OS_EVENT *g_sem_chk_rf;
OS_EVENT *g_sem_chk_ir;

PROTO_PARA g_proto_para;

DL645_FRAME pc_frame_send;
DL645_FRAME pc_frame_recv;
DL645_FRAME_STAT pc_frame_stat;

DL645_FRAME rs485_frame_send;
DL645_FRAME rs485_frame_recv;
DL645_FRAME_STAT rs485_frame_stat;

u16 pc_uart_send(u8 *buf, u16 len)
{
    P_MSG_INFO  pMsg = NULL;
    

    if((NULL == buf) || (0 == len))
    {
        return (FALSE);
    }

    if(!(pMsg = (P_MSG_INFO)alloc_send_buffer(MSG_SHORT)))
    {
        return (FALSE);
    }

    memcpy(pMsg->msg_buffer, buf, len);

    pMsg->msg_header.msg_len = len;

    pMsg->msg_header.end_id = PC_COM_PORT;

    pMsg->msg_header.need_buffer_free = TRUE;

    return (End_send(pMsg));
}

u16 rs485_uart_send(u8 *buf, u16 len)
{
    P_MSG_INFO  pMsg = NULL;


    if((NULL == buf) || (0 == len))
    {
        return (FALSE);
    }

    if(!(pMsg = (P_MSG_INFO)alloc_send_buffer(MSG_SHORT)))
    {
        return (FALSE);
    }

    memcpy(pMsg->msg_buffer, buf, len);

    pMsg->msg_header.msg_len = len;

    pMsg->msg_header.end_id = RS485_COM_PORT;

    pMsg->msg_header.need_buffer_free = TRUE;
    
    return (End_send(pMsg));
}

u16 plc_uart_send(u8 *buf, u16 len)
{
    P_MSG_INFO  pMsg = NULL;


    if((NULL == buf) || (0 == len))
    {
        return (FALSE);
    }

    if(!(pMsg = (P_MSG_INFO)alloc_send_buffer(MSG_SHORT)))
    {
        return (FALSE);
    }

    memcpy(pMsg->msg_buffer, buf, len);

    pMsg->msg_header.msg_len = len;

    pMsg->msg_header.end_id = PLC_COM_PORT;

    pMsg->msg_header.need_buffer_free = TRUE;
    
    return (End_send(pMsg));
}

u16 ir_uart_send(u8 *buf, u16 len)
{
    P_MSG_INFO  pMsg = NULL;


    if((NULL == buf) || (0 == len))
    {
        return (FALSE);
    }

    if(!(pMsg = (P_MSG_INFO)alloc_send_buffer(MSG_SHORT)))
    {
        return (FALSE);
    }
    
    memcpy(pMsg->msg_buffer, buf, len);
    
    pMsg->msg_header.msg_len = len;
    
    pMsg->msg_header.end_id = IR_COM_PORT;
    
    pMsg->msg_header.need_buffer_free = TRUE;
    
    return (End_send(pMsg));
}

unsigned int PC_postProcess(pvoid h)
{
    P_MSG_INFO  pMsg = (P_MSG_INFO)h;
    u8  *pBuf = (UCHAR *)(pMsg->msg_buffer);
    u16  mLen = pMsg->msg_header.msg_len;
    OS_CPU_SR  cpu_sr;   


    OS_ENTER_CRITICAL();
    memcpy(&pc_frame_recv, pBuf, mLen);
    OS_EXIT_CRITICAL();

    OSSemPost(g_sem_pc);

    return (TRUE);
}

unsigned int RS485_postProcess(pvoid h)
{
    P_MSG_INFO  pMsg = (P_MSG_INFO)h;
    u8  *pBuf = (UCHAR *)(pMsg->msg_buffer);
    u16  mLen = pMsg->msg_header.msg_len;
    OS_CPU_SR  cpu_sr;   


    OS_ENTER_CRITICAL();
    memcpy(&rs485_frame_recv, pBuf, mLen);
    OS_EXIT_CRITICAL();

    OSSemPost(g_sem_rs485);

    return (TRUE);
}

#if (EWARM_OPTIMIZATION_EN > 0u)
#pragma optimize = low
#endif
unsigned int PLC_postProcess(pvoid h)
{
    P_MSG_INFO  pMsg = (P_MSG_INFO)h;
    u8  *pBuf = (UCHAR *)(pMsg->msg_buffer);
    u16  mLen = pMsg->msg_header.msg_len;
    OS_CPU_SR  cpu_sr;


    OS_ENTER_CRITICAL();
    memcpy(&g_proto_para.dl645_frame_recv, pBuf, mLen);
    memcpy(g_proto_para.recv_buf, pBuf, mLen);
    g_proto_para.recv_len = mLen;
    OS_EXIT_CRITICAL();

    OSSemPost(g_sem_plc);
    OSSemPost(g_sem_chk_plc);

    return (TRUE);
}

#if (EWARM_OPTIMIZATION_EN > 0u)
#pragma optimize = low
#endif
unsigned int IR_postProcess(pvoid h)
{
    P_MSG_INFO  pMsg = (P_MSG_INFO)h;
    u8  *pBuf = (UCHAR *)(pMsg->msg_buffer);
    u16  mLen = pMsg->msg_header.msg_len;
    OS_CPU_SR  cpu_sr;


    if((0 == g_proto_para.ir_send_len) || (mLen <= g_proto_para.ir_send_len))
    {
        return (FALSE);
    }
        
    OS_ENTER_CRITICAL();
    memcpy(g_proto_para.ir_recv_buf, pBuf, mLen);
    g_proto_para.ir_recv_len = mLen;
    OS_EXIT_CRITICAL();

    OSSemPost(g_sem_ir);
    OSSemPost(g_sem_chk_ir);
    
    return (TRUE);
}

u32 Proto_Check_Frame(void)
{
    if(DL645_FRAME_ERROR == Analysis_DL645_Frame( g_gui_para.dstAddr, 
                                                 (u8 *)&g_proto_para.dl645_frame_recv,
                                                  &g_proto_para.dl645_frame_stat))
    {
        g_proto_para.recv_result = RECV_RES_INVALID;                                                 
    }
    else
    {                
        if(g_proto_para.dl645_frame_stat.Ctrl & DL645_REPLY_STAT_MASK)
        {
            g_proto_para.recv_result = RECV_RES_ABNORMAL_REPLY;
        }
        else
        {
            g_proto_para.recv_result = RECV_RES_SUCC;
        }
    }
    
    return (g_proto_para.recv_result);
}

void Proto_Data_Proc(void) 
{
    u16 len;
    

    if(RECV_RES_SUCC == Proto_Check_Frame())       
    {
        len = g_proto_para.dl645_frame_recv.Len;

        if((FRM_CTRW_97_READ_SLVS_DATA == (g_proto_para.dl645_frame_stat.Ctrl & CCTT_CONTROL_CODE_MASK)) ||
           (FRM_CTRW_97_WRITE_SLVS_DATA == (g_proto_para.dl645_frame_stat.Ctrl & CCTT_CONTROL_CODE_MASK)))
        {
            if(len >= DL645_97_DATA_ITEM_LEN)
            {
                g_proto_para.data_len = len - DL645_97_DATA_ITEM_LEN;
                memcpy(g_proto_para.data_buf, &g_proto_para.dl645_frame_recv.Data[DL645_97_DATA_ITEM_LEN], g_proto_para.data_len);
            }
        }
        else if((FRM_CTRW_07_READ_SLVS_DATA == (g_proto_para.dl645_frame_stat.Ctrl & CCTT_CONTROL_CODE_MASK)) ||
                (FRM_CTRW_07_WRITE_SLVS_DATA == (g_proto_para.dl645_frame_stat.Ctrl & CCTT_CONTROL_CODE_MASK)))

        {
            if(len >= DL645_07_DATA_ITEM_LEN)
            {
                g_proto_para.data_len = len - DL645_07_DATA_ITEM_LEN;
                memcpy(g_proto_para.data_buf, &g_proto_para.dl645_frame_recv.Data[DL645_07_DATA_ITEM_LEN], g_proto_para.data_len);
            }
        }      
        else if((FRM_CTRW_07_BROAD_READ_ADDR == (g_proto_para.dl645_frame_stat.Ctrl & CCTT_CONTROL_CODE_MASK)))
        {
            if(DL645_ADDR_LEN == len)
            {
                g_proto_para.data_len = DL645_ADDR_LEN;
                memcpy(g_proto_para.data_buf, &g_proto_para.dl645_frame_recv.Data[0], g_proto_para.data_len);    
            }
        }
    }
}

/*
*********************************************************************************************************
*                                             App_TaskProto()
*
* Description : This task monitors the state of the push buttons and passes messages to AppTaskUserIF()
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskProto()' by 'OSTaskCreateExt()'.
*
* Return(s)  : none.
*
* Caller(s)  : This is a task.
*
* Note(s)    : none.
*********************************************************************************************************
*/
#if (EWARM_OPTIMIZATION_EN > 0u)
#pragma optimize = low
#endif
void  App_TaskProto (void *p_arg)
{
    const INT8U dl645_broad_addr[6] = {0x99, 0x99, 0x99, 0x99, 0x99, 0x99};
    INT8U rf_addr[] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88};
    INT8U dl645_read_addr[32];
    INT8U err, index;
    INT16U len;
    WM_HWIN hItem;
    
    
    (void)p_arg; 

    while (DEF_TRUE) {    
        OSMboxPend(g_sys_ctrl.up_mbox, 5, &err);

        if(OS_ERR_NONE == err)
        {
            hItem = GUI_Get_PROGBAR();
            
            if(WM_HWIN_NULL != hItem)
            {
                g_sys_ctrl.ProgBarVal = 0;
                
                PROGBAR_SetValue(hItem, g_sys_ctrl.ProgBarVal);         
            }
            
            switch(g_gui_para.cmd)
            {
            case GUI_CMD_BROAD_READ_ADDR: //广播读设备地址
                if(CHANNEL_PLC == g_rom_para.channel) //载波
                {
                    memcpy(&dl645_read_addr[DL645_INDEX], Broad_Read_Addr, sizeof(Broad_Read_Addr));

                    dl645_read_addr[PREAMBLE_INDEX] = g_rom_para.preamble;

                    g_proto_para.send_len = sizeof(Broad_Read_Addr) + PREAMBLE_LEN;
                    
                    memcpy(g_proto_para.send_buf, dl645_read_addr, g_proto_para.send_len);

                    while(OSSemAccept(g_sem_plc));
                      
                    plc_uart_send(g_proto_para.send_buf, g_proto_para.send_len);

                    g_proto_para.msg_state = MSG_STATE_SENDING;

                    OSMboxPost(g_sys_ctrl.down_mbox, (void *)&g_proto_para);
                    
                    OSSemPend(g_sem_plc, g_rom_para.bpsSpeed * OS_TICKS_PER_SEC, &err);

                    g_proto_para.data_len = 0;

                    if(OS_ERR_NONE == err)
                    {
                        Proto_Data_Proc();
                    }
                    else
                    {
                        g_proto_para.recv_result = RECV_RES_TIMEOUT;
                    }

                    g_proto_para.msg_state = MSG_STATE_RECEIVED;

                    OSMboxPost(g_sys_ctrl.down_mbox, (void *)&g_proto_para);
                }
                else if(CHANNEL_RF == g_rom_para.channel) //射频
                {
                    memcpy(g_proto_para.send_buf, Broad_Read_Addr, sizeof(Broad_Read_Addr));

                    g_proto_para.send_len = sizeof(Broad_Read_Addr);

                    while(OSSemAccept(g_sem_rf));
                    
                    RF_SEND_LEN = GDW_RF_Protocol_2013(rf_addr, 0x00, 0x00, 0x00, (u8 *)g_proto_para.send_buf, g_proto_para.send_len, RF_SEND_BUF);

                    g_proto_para.msg_state = MSG_STATE_SENDING;

                    OSMboxPost(g_sys_ctrl.down_mbox, (void *)&g_proto_para);

                    OSSemPend(g_sem_rf, 5 * OS_TICKS_PER_SEC, &err);

                    g_proto_para.data_len = 0;

                    if(OS_ERR_NONE == err)
                    {
                        memcpy(&g_proto_para.dl645_frame_recv, &RF_RECV_BUF[RF_RECV_FIX_LEN], RF_RECV_LEN - RF_RECV_FIX_LEN);                                       
                        memcpy(&g_proto_para.recv_buf,&RF_RECV_BUF[RF_RECV_FIX_LEN], RF_RECV_LEN - RF_RECV_FIX_LEN);
                        g_proto_para.recv_len = RF_RECV_LEN - RF_RECV_FIX_LEN - RF_PRINT_FIX_LEN;
                        Proto_Data_Proc();
                    }
                    else
                    {   
                        g_proto_para.recv_result = RECV_RES_TIMEOUT;                                        
                    }
                    
                    g_proto_para.msg_state = MSG_STATE_RECEIVED;
                    
                    OSMboxPost(g_sys_ctrl.down_mbox, (void *)&g_proto_para);
                }
                else if(CHANNEL_IR == g_rom_para.channel) //红外
                {
                    memcpy(&dl645_read_addr[DL645_INDEX], Broad_Read_Addr, sizeof(Broad_Read_Addr));

                    dl645_read_addr[PREAMBLE_INDEX] = g_rom_para.preamble;

                    g_proto_para.send_len = sizeof(Broad_Read_Addr) + PREAMBLE_LEN;

                    g_proto_para.ir_send_len = g_proto_para.send_len;
                    
                    memcpy(g_proto_para.send_buf, dl645_read_addr, g_proto_para.send_len);

                    while(OSSemAccept(g_sem_ir));
                      
                    ir_uart_send(g_proto_para.send_buf, g_proto_para.send_len);

                    g_proto_para.msg_state = MSG_STATE_SENDING;

                    OSMboxPost(g_sys_ctrl.down_mbox, (void *)&g_proto_para);
                    
                    OSSemPend(g_sem_ir, 5 * OS_TICKS_PER_SEC, &err);

                    g_proto_para.data_len = 0;

                    if(OS_ERR_NONE == err)
                    {
                        g_proto_para.recv_len = g_proto_para.ir_recv_len - g_proto_para.ir_send_len;
                        
                        memcpy(g_proto_para.recv_buf, &g_proto_para.ir_recv_buf[g_proto_para.ir_send_len], g_proto_para.recv_len);

                        for(index = 0; IR_PREAMBLE == g_proto_para.recv_buf[index]; index++);

                        memcpy(&g_proto_para.dl645_frame_recv, &g_proto_para.recv_buf[index], g_proto_para.recv_len - index);
                        
                        Proto_Data_Proc();
                    }
                    else
                    {
                        g_proto_para.recv_result = RECV_RES_TIMEOUT;
                    }

                    g_proto_para.ir_send_len = 0;

                    g_proto_para.msg_state = MSG_STATE_RECEIVED;

                    OSMboxPost(g_sys_ctrl.down_mbox, (void *)&g_proto_para);
                }                
                break;

            case GUI_CMD_BROAD_CAL_TIME:
                memcpy(g_gui_para.dstAddr, dl645_broad_addr, sizeof(dl645_broad_addr));
                
                g_gui_para.dataBuf[0] = g_rtc_time[SEC_POS];
                g_gui_para.dataBuf[1] = g_rtc_time[MIN_POS];
                g_gui_para.dataBuf[2] = g_rtc_time[HOUR_POS];
                g_gui_para.dataBuf[3] = g_rtc_time[DATE_POS];
                g_gui_para.dataBuf[4] = g_rtc_time[MONTH_POS];
                g_gui_para.dataBuf[5] = g_rtc_time[YEAR_POS];
                
                g_gui_para.dataLen = 6;
                
                memcpy(g_proto_para.dl645_frame_send.Data, g_gui_para.dataBuf, g_gui_para.dataLen);
                
                Create_DL645_Frame(g_gui_para.dstAddr, g_gui_para.ctrlCode, g_gui_para.dataLen, &g_proto_para.dl645_frame_send);
                
                g_proto_para.send_len = g_gui_para.dataLen + DL645_FIX_LEN;
                
                if(CHANNEL_PLC == g_rom_para.channel) //载波
                {                    
                    memcpy(&g_proto_para.send_buf[DL645_INDEX], &g_proto_para.dl645_frame_send, g_proto_para.send_len);
                    
                    g_proto_para.send_buf[PREAMBLE_INDEX] = g_rom_para.preamble;
                    g_proto_para.send_len += PREAMBLE_LEN;

                    while(OSSemAccept(g_sem_plc));
                      
                    plc_uart_send(g_proto_para.send_buf, g_proto_para.send_len);

                    g_proto_para.msg_state = MSG_STATE_SENDING;

                    OSMboxPost(g_sys_ctrl.down_mbox, (void *)&g_proto_para);
                    
                    g_proto_para.recv_len = 0;

                    g_proto_para.data_len = 0;

                    g_proto_para.recv_result = RECV_RES_SUCC;

                    g_proto_para.msg_state = MSG_STATE_RECEIVED;

                    OSMboxPost(g_sys_ctrl.down_mbox, (void *)&g_proto_para);                    
                }           
                else if(CHANNEL_RF == g_rom_para.channel) //射频
                {                    
                    memcpy(g_proto_para.send_buf, &g_proto_para.dl645_frame_send, g_proto_para.send_len);

                    while(OSSemAccept(g_sem_rf));
                    
                    RF_SEND_LEN = GDW_RF_Protocol_2013(rf_addr, 0x00, 0x00, 0x00, (u8 *)g_proto_para.send_buf, g_proto_para.send_len, RF_SEND_BUF);

                    g_proto_para.msg_state = MSG_STATE_SENDING;

                    OSMboxPost(g_sys_ctrl.down_mbox, (void *)&g_proto_para);
                    
                    g_proto_para.recv_len = 0;

                    g_proto_para.data_len = 0;

                    g_proto_para.recv_result = RECV_RES_SUCC;

                    g_proto_para.msg_state = MSG_STATE_RECEIVED;

                    OSMboxPost(g_sys_ctrl.down_mbox, (void *)&g_proto_para);                    
                }
                else if(CHANNEL_IR == g_rom_para.channel) //红外
                {
                    memcpy(&g_proto_para.send_buf[DL645_INDEX], &g_proto_para.dl645_frame_send, g_proto_para.send_len);
                    
                    g_proto_para.send_buf[PREAMBLE_INDEX] = g_rom_para.preamble;
                    g_proto_para.send_len += PREAMBLE_LEN;

                    g_proto_para.ir_send_len = g_proto_para.send_len;

                    while(OSSemAccept(g_sem_ir));
                      
                    ir_uart_send(g_proto_para.send_buf, g_proto_para.send_len);

                    g_proto_para.msg_state = MSG_STATE_SENDING;

                    OSMboxPost(g_sys_ctrl.down_mbox, (void *)&g_proto_para);
                    
                    g_proto_para.recv_len = 0;

                    g_proto_para.data_len = 0;

                    g_proto_para.recv_result = RECV_RES_SUCC;

                    g_proto_para.ir_send_len = 0;

                    g_proto_para.msg_state = MSG_STATE_RECEIVED;

                    OSMboxPost(g_sys_ctrl.down_mbox, (void *)&g_proto_para);                    
                }                
                break;
                
            case GUI_CMD_PLC_R2M: //监控态
                if(CHANNEL_PLC == g_rom_para.channel)
                {
                    while(OSSemAccept(g_sem_plc));
                    
                    plc_uart_send((u8 *)rPLC_TO_mPLC, sizeof(rPLC_TO_mPLC));

                    g_proto_para.msg_state = MSG_STATE_SENDING;
                    
                    OSSemPend(g_sem_plc, 5 * OS_TICKS_PER_SEC, &err);

                    g_proto_para.data_len = 0;

                    if(OS_ERR_NONE == err)
                    {
                        g_sys_ctrl.plc_state = PLC_STATE_MONITOR;
                        
                        Proto_Check_Frame();
                    }
                    else
                    {
                        g_proto_para.recv_result = RECV_RES_TIMEOUT;

                        g_proto_para.data_len = 0;
                    }

                    g_proto_para.msg_state = MSG_STATE_RECEIVED;

                    OSMboxPost(g_sys_ctrl.down_mbox, (void *)&g_proto_para);
                }
                break;

            case GUI_CMD_PLC_M2R: //抄控态
                if(CHANNEL_PLC == g_rom_para.channel)
                {
                    while(OSSemAccept(g_sem_plc));

                    plc_uart_send((u8 *)mPLC_TO_rPLC, sizeof(mPLC_TO_rPLC));

                    g_proto_para.msg_state = MSG_STATE_SENDING;
                    
                    OSSemPend(g_sem_plc, 5 * OS_TICKS_PER_SEC, &err);

                    g_proto_para.data_len = 0;

                    if(OS_ERR_NONE == err)
                    {
                        g_sys_ctrl.plc_state = PLC_STATE_METER_READ;
                        
                        Proto_Check_Frame();
                    }
                    else
                    {
                        g_proto_para.recv_result = RECV_RES_TIMEOUT;
                    }

                    g_proto_para.msg_state = MSG_STATE_RECEIVED;

                    OSMboxPost(g_sys_ctrl.down_mbox, (void *)&g_proto_para);
                }
                break;

            case GUI_CMD_PLC_READ_NODE: //读载波节点
                if(CHANNEL_PLC == g_rom_para.channel)
                {
                    while(OSSemAccept(g_sem_plc));
                    
                    plc_uart_send((u8 *)READ_PLC_NODE, sizeof(READ_PLC_NODE));

                    g_proto_para.msg_state = MSG_STATE_SENDING;
                    
                    OSSemPend(g_sem_plc, 2 * OS_TICKS_PER_SEC, &err);

                    g_proto_para.data_len = 0;

                    if(OS_ERR_NONE == err)
                    {
                        Proto_Check_Frame();
                    }
                    else
                    {
                        g_proto_para.recv_result = RECV_RES_TIMEOUT;
                    }

                    g_sys_ctrl.plc_state = PLC_STATE_READ_NODE;

                    g_proto_para.msg_state = MSG_STATE_RECEIVED;
                }
                break;

            case GUI_CMD_PLC_FREQ_SET: //频率设置
                if(CHANNEL_PLC == g_rom_para.channel)
                {
                    while(OSSemAccept(g_sem_plc));
                    
                    switch(g_rom_para.plc_freq)
                    {
                    case PLC_270_III:
                        plc_uart_send((u8 *)rPLC_270III, sizeof(rPLC_270III));
                        break;
                        
                    case PLC_270_III_5:
                        plc_uart_send((u8 *)rPLC_270III5, sizeof(rPLC_270III5));
                        break;
                        
                    case PLC_270_II:
                        plc_uart_send((u8 *)rPLC_270II, sizeof(rPLC_270II));
                        break;
                        
                    case PLC_421_50BPS:
                        plc_uart_send((u8 *)rPLC_421_50BPS, sizeof(rPLC_421_50BPS));
                        break;
                        
                    case PLC_421_100BPS:
                        plc_uart_send((u8 *)rPLC_421_100BPS, sizeof(rPLC_421_100BPS));
                        break;
                        
                    case PLC_421_600BPS:
                        plc_uart_send((u8 *)rPLC_421_600BPS, sizeof(rPLC_421_600BPS));
                        break;
                        
                    case PLC_421_1200BPS:
                        plc_uart_send((u8 *)rPLC_421_1200BPS, sizeof(rPLC_421_1200BPS));
                        break;
                        
                    default:
                        break;
                    }

                    g_proto_para.msg_state = MSG_STATE_SENDING;
                    
                    OSSemPend(g_sem_plc, g_rom_para.bpsSpeed * OS_TICKS_PER_SEC, &err);

                    g_proto_para.data_len = 0;

                    if(OS_ERR_NONE == err)
                    {
                        Proto_Check_Frame();
                    }
                    else
                    {
                        g_proto_para.recv_result = RECV_RES_TIMEOUT;
                    }

                    g_proto_para.msg_state = MSG_STATE_RECEIVED;

                    OSMboxPost(g_sys_ctrl.down_mbox, (void *)&g_proto_para);  
                }
                break;

            case GUI_CMD_MRW:
                if((FRM_CTRW_97_READ_SLVS_DATA == (g_gui_para.ctrlCode & CCTT_CONTROL_CODE_MASK)) ||
                   (FRM_CTRW_97_WRITE_SLVS_DATA == (g_gui_para.ctrlCode & CCTT_CONTROL_CODE_MASK)))
                {   /* 1997读写命令，2个字节数据标识 */
                    memcpy(g_proto_para.dl645_frame_send.Data, g_gui_para.dataItem, DL645_97_DATA_ITEM_LEN);
                    memcpy(&g_proto_para.dl645_frame_send.Data[DL645_97_DATA_ITEM_LEN], g_gui_para.dataBuf, g_gui_para.dataLen);
                    g_gui_para.dataLen += DL645_97_DATA_ITEM_LEN;
                }
                else if((FRM_CTRW_07_READ_SLVS_DATA == (g_gui_para.ctrlCode & CCTT_CONTROL_CODE_MASK)) || 
                        (FRM_CTRW_07_WRITE_SLVS_DATA == (g_gui_para.ctrlCode & CCTT_CONTROL_CODE_MASK)))
                
                {   /* 2007读写命令，4个字节数据标识 */
                    memcpy(g_proto_para.dl645_frame_send.Data, g_gui_para.dataItem, DL645_07_DATA_ITEM_LEN);
                    memcpy(&g_proto_para.dl645_frame_send.Data[DL645_07_DATA_ITEM_LEN], g_gui_para.dataBuf, g_gui_para.dataLen);
                    g_gui_para.dataLen += DL645_07_DATA_ITEM_LEN;
                }
                else
                {   /* 非读写命令，无数据标识 */
                    memcpy(g_proto_para.dl645_frame_send.Data, g_gui_para.dataBuf, g_gui_para.dataLen);
                } 
                
                Create_DL645_Frame(g_gui_para.dstAddr, g_gui_para.ctrlCode, g_gui_para.dataLen, &g_proto_para.dl645_frame_send);

                g_proto_para.send_len = g_gui_para.dataLen + DL645_FIX_LEN;

                if(CHANNEL_PLC == g_rom_para.channel) //载波
                {
                    memcpy(&g_proto_para.send_buf[DL645_INDEX], &g_proto_para.dl645_frame_send, g_proto_para.send_len);
                    
                    g_proto_para.send_buf[PREAMBLE_INDEX] = g_rom_para.preamble;
                    g_proto_para.send_len += PREAMBLE_LEN;
                    
                    while(OSSemAccept(g_sem_plc));
                    
                    plc_uart_send(g_proto_para.send_buf, g_proto_para.send_len);
                    
                    g_proto_para.msg_state = MSG_STATE_SENDING;
                    
                    OSMboxPost(g_sys_ctrl.down_mbox, (void *)&g_proto_para);
                    
                    OSSemPend(g_sem_plc, g_rom_para.bpsSpeed * OS_TICKS_PER_SEC, &err);

                    g_proto_para.data_len = 0;
                    
                    if(OS_ERR_NONE == err)
                    {
                        Proto_Data_Proc();
                    }
                    else
                    {   
                        g_proto_para.recv_result = RECV_RES_TIMEOUT;                                        
                    }
                    
                    g_proto_para.msg_state = MSG_STATE_RECEIVED;
                    
                    OSMboxPost(g_sys_ctrl.down_mbox, (void *)&g_proto_para);
                }                
                else if(CHANNEL_RF == g_rom_para.channel) //射频
                {                    
                    memcpy(&rf_addr[6], g_gui_para.dstAddr, DL645_ADDR_LEN);

                    memcpy(g_proto_para.send_buf, (u8 *)&g_proto_para.dl645_frame_send, g_proto_para.send_len);

                    while(OSSemAccept(g_sem_rf));
                    
                    RF_SEND_LEN = GDW_RF_Protocol_2013(rf_addr, 0x00, 0x00, 0x00, (u8 *)g_proto_para.send_buf, g_proto_para.send_len, RF_SEND_BUF);

                    g_proto_para.msg_state = MSG_STATE_SENDING;              

                    OSMboxPost(g_sys_ctrl.down_mbox, (void *)&g_proto_para);

                    OSSemPend(g_sem_rf, 5 * OS_TICKS_PER_SEC, &err);

                    g_proto_para.data_len = 0;

                    if(OS_ERR_NONE == err)
                    {
                        memcpy(&g_proto_para.dl645_frame_recv, &RF_RECV_BUF[RF_RECV_FIX_LEN], RF_RECV_LEN - RF_RECV_FIX_LEN);                                       
                        memcpy(&g_proto_para.recv_buf, &RF_RECV_BUF[RF_RECV_FIX_LEN], RF_RECV_LEN - RF_RECV_FIX_LEN);
                        g_proto_para.recv_len = RF_RECV_LEN - RF_RECV_FIX_LEN - RF_PRINT_FIX_LEN;
                        Proto_Data_Proc();
                    }
                    else
                    {   
                        g_proto_para.recv_result = RECV_RES_TIMEOUT;                                        
                    }
                    
                    g_proto_para.msg_state = MSG_STATE_RECEIVED;
                    
                    OSMboxPost(g_sys_ctrl.down_mbox, (void *)&g_proto_para);
                }
                else if(CHANNEL_IR == g_rom_para.channel) //红外
                {
                    memcpy(&g_proto_para.send_buf[DL645_INDEX], &g_proto_para.dl645_frame_send, g_proto_para.send_len);
                    
                    g_proto_para.send_buf[PREAMBLE_INDEX] = g_rom_para.preamble;
                    g_proto_para.send_len += PREAMBLE_LEN;

                    g_proto_para.ir_send_len = g_proto_para.send_len;
                    
                    while(OSSemAccept(g_sem_ir));
                    
                    ir_uart_send(g_proto_para.send_buf, g_proto_para.send_len);
                    
                    g_proto_para.msg_state = MSG_STATE_SENDING;
                    
                    OSMboxPost(g_sys_ctrl.down_mbox, (void *)&g_proto_para);
                    
                    OSSemPend(g_sem_ir, 5 * OS_TICKS_PER_SEC, &err);

                    g_proto_para.data_len = 0;
                    
                    if(OS_ERR_NONE == err)
                    {
                        g_proto_para.recv_len = g_proto_para.ir_recv_len - g_proto_para.ir_send_len;
                        
                        memcpy(g_proto_para.recv_buf, &g_proto_para.ir_recv_buf[g_proto_para.ir_send_len], g_proto_para.recv_len);

                        for(index = 0; IR_PREAMBLE == g_proto_para.recv_buf[index]; index++);

                        memcpy(&g_proto_para.dl645_frame_recv, &g_proto_para.recv_buf[index], g_proto_para.recv_len - index);
                        
                        Proto_Data_Proc();
                    }
                    else
                    {
                        g_proto_para.recv_result = RECV_RES_TIMEOUT;
                    }

                    g_proto_para.ir_send_len = 0;

                    g_proto_para.msg_state = MSG_STATE_RECEIVED;

                    OSMboxPost(g_sys_ctrl.down_mbox, (void *)&g_proto_para);
                }                
                break;

            case GUI_CMD_ROUTE: //路由
                if(CHANNEL_PLC == g_rom_para.channel) //载波
                {
                    memcpy(g_gui_para.relayAddr[g_sys_ctrl.sysAddrLevel], g_gui_para.dstAddr, DL645_ADDR_LEN);

                    g_gui_para.ctrlCode = c_645ctrlDef[g_rom_para.protocol][DL645_CTRL_READ_DATA];

                    len = (DL645_1997 == g_rom_para.protocol) ? (DL645_97_DATA_ITEM_LEN) : (DL645_07_DATA_ITEM_LEN);

                    g_proto_para.send_len = Create_DL645_Relay_Frame((u8 *)g_gui_para.relayAddr, g_sys_ctrl.sysAddrLevel, g_gui_para.dstAddr,
                                                                   g_gui_para.ctrlCode, len, g_gui_para.dataItem, (u8 *)&g_proto_para.dl645_frame_send);

                    memcpy(&g_proto_para.send_buf[DL645_INDEX], &g_proto_para.dl645_frame_send, g_proto_para.send_len);
                    
                    g_proto_para.send_buf[PREAMBLE_INDEX] = g_rom_para.preamble;
                    g_proto_para.send_len += PREAMBLE_LEN;
                    
                    while(OSSemAccept(g_sem_plc));
                    
                    plc_uart_send(g_proto_para.send_buf, g_proto_para.send_len);
                    
                    g_proto_para.msg_state = MSG_STATE_SENDING;
                    
                    OSMboxPost(g_sys_ctrl.down_mbox, (void *)&g_proto_para);
                    
                    OSSemPend(g_sem_plc, (g_sys_ctrl.sysAddrLevel + 1) * g_rom_para.bpsSpeed * OS_TICKS_PER_SEC, &err);

                    g_proto_para.data_len = 0;
                    
                    if(OS_ERR_NONE == err)
                    {
                        Proto_Data_Proc();
                    }
                    else
                    {   
                        g_proto_para.recv_result = RECV_RES_TIMEOUT;                                        
                    }
                    
                    g_proto_para.msg_state = MSG_STATE_RECEIVED;
                    
                    OSMboxPost(g_sys_ctrl.down_mbox, (void *)&g_proto_para);
                }
                break;

            default:
                break;
            }
        }
        else
        {
            if(CHANNEL_PLC == g_rom_para.channel) 
            {
                OSSemPend(g_sem_plc, 5, &err);

                if(OS_ERR_NONE == err)
                {
                    if(PLC_STATE_MONITOR == g_sys_ctrl.plc_state)
                    {
                        trm_msg_record(TRM_MSG_PLC_MONITOR);
                    }
                    
                    g_proto_para.recv_result = RECV_RES_SUCC;

                    g_proto_para.msg_state = MSG_STATE_RECEIVED;
                    
                    OSMboxPost(g_sys_ctrl.down_mbox, (void *)&g_proto_para);                
                }
            }
        }
    }
}

/*
*********************************************************************************************************
*                                             App_TaskPC()
*
* Description : This task monitors the state of the push buttons and passes messages to AppTaskUserIF()
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskPC()' by 'OSTaskCreateExt()'.
*
* Return(s)  : none.
*
* Caller(s)  : This is a task.
*
* Note(s)    : none.
*********************************************************************************************************
*/
#if (EWARM_OPTIMIZATION_EN > 0u)
#pragma optimize = low
#endif
void  App_TaskPC (void *p_arg)
{
    const INT8U pc_addr[6] = {0x99, 0x99, 0x99, 0x99, 0x99, 0x99};
    INT8U err, seq, send_file_num, fname[16], buf[128];
    INT16U send_len;
    INT32U i, j, pc_data_item, sd_file_num;
    FATFS fs;
    UINT br;
    static FIL fp;
    static INT32U fsize, offset;
    

    (void)p_arg;
    
    while (DEF_TRUE) {
        OSSemPend(g_sem_pc, 0, &err);

        if(OS_ERR_NONE == err)
        {
            if(DL645_FRAME_OK == Analysis_DL645_Frame((u8 *)pc_addr, (u8 *)&pc_frame_recv, &pc_frame_stat))
            {
                memcpy(&pc_frame_send, &pc_frame_recv, sizeof(DL645_FRAME));
                
                if((FRM_CTRW_07_READ_SLVS_DATA == (pc_frame_stat.Ctrl & CCTT_CONTROL_CODE_MASK)) ||
                   (FRM_CTRW_07_WRITE_SLVS_DATA == (pc_frame_stat.Ctrl & CCTT_CONTROL_CODE_MASK)) || 
                   (FRM_CTRW_07_READ_SLVS_FOLLOW_DATA == (pc_frame_stat.Ctrl & CCTT_CONTROL_CODE_MASK)))
                {
                    pc_data_item = ((INT32U)pc_frame_recv.Data[3] << 24) | ((INT32U)pc_frame_recv.Data[2] << 16) | ((INT32U)pc_frame_recv.Data[1] << 8) | ((INT32U)pc_frame_recv.Data[0] << 0);
                
                    switch(pc_data_item)
                    {
                    case SHAKE_HANDS_CMD:
                        pc_frame_send.Len = DL645_07_DATA_ITEM_LEN;
                        pc_frame_send.Ctrl = 0x91;

                        Create_DL645_Frame((u8 *)pc_addr, pc_frame_send.Ctrl, pc_frame_send.Len, &pc_frame_send);

                        send_len = pc_frame_send.Len + DL645_FIX_LEN;

                        while(OSSemAccept(g_sem_pc));

                        pc_uart_send((u8 *)&pc_frame_send, send_len);
                        break;

                    case SCAN_FILE_CMD: //扫描根目录
                        if(FR_OK == f_mount(SD_DRV, &fs))
                        {
                            scan_files("/");

                            sd_file_num = 0;
                            while(SD_FileName[sd_file_num][0])
                            {
                                if(FR_OK == f_open(&fp, SD_FileName[sd_file_num], FA_OPEN_EXISTING | FA_READ))
                                {
                                    SD_FileSize[sd_file_num] = f_size(&fp);
                                }

                                f_close(&fp);

                                sd_file_num++;
                            }
                            
                            memcpy(&pc_frame_send.Data[DL645_07_DATA_ITEM_LEN], &sd_file_num, FILE_NUM_LEN);

                            i = sd_file_num;
                            j = 0;
                            while(i && (j < MAX_FILE_NUM))
                            {
                                memcpy(&pc_frame_send.Data[DL645_07_DATA_ITEM_LEN + FILE_NUM_LEN + FILE_NAME_LEN * j + FILE_SIZE_LEN * j], SD_FileName[j], FILE_NAME_LEN);
                                memcpy(&pc_frame_send.Data[DL645_07_DATA_ITEM_LEN + FILE_NUM_LEN + FILE_NAME_LEN * j + FILE_SIZE_LEN * j + FILE_NAME_LEN], &SD_FileSize[j], FILE_SIZE_LEN);

                                i--;
                                j++;
                            }

                            send_file_num = j;

                            pc_frame_send.Len = DL645_07_DATA_ITEM_LEN + FILE_NUM_LEN + FILE_NAME_LEN * send_file_num + FILE_SIZE_LEN * send_file_num;

                            pc_frame_send.Ctrl = 0x91;

                            Create_DL645_Frame((u8 *)pc_addr, pc_frame_send.Ctrl, pc_frame_send.Len, &pc_frame_send);
    
                            send_len = pc_frame_send.Len + DL645_FIX_LEN;

                            while(OSSemAccept(g_sem_pc));
                        
                            pc_uart_send((u8 *)&pc_frame_send, send_len);                                    
                        } 

                        f_mount(SD_DRV, NULL);
                        break;

                    case READ_FILE_CMD: //读指定文件内容
                        switch(pc_frame_recv.Ctrl)
                        {
                        case FRM_CTRW_07_READ_SLVS_DATA:
                            fsize = 0;
                            offset = 0;
                            
                            memcpy(fname, &pc_frame_recv.Data[DL645_07_DATA_ITEM_LEN], FILE_NAME_LEN);
                            
                            fname[FILE_NAME_LEN] = '\0';
                            
                            if(FR_OK == f_mount(SD_DRV, &fs))
                            {
                                if(FR_OK == f_open(&fp, fname, FA_OPEN_EXISTING | FA_READ))
                                {
                                    if(FR_OK == f_read(&fp, buf, 128, &br))
                                    {
                                        fsize = f_size(&fp);
                                        offset += br;

                                        memcpy(&pc_frame_send.Data[DL645_07_DATA_ITEM_LEN], fname, FILE_NAME_LEN);
                                        
                                        memcpy(&pc_frame_send.Data[DL645_07_DATA_ITEM_LEN + FILE_NAME_LEN], buf, br);
                            
                                        pc_frame_send.Len = DL645_07_DATA_ITEM_LEN + FILE_NAME_LEN + br;

                                        if(offset >= fsize)
                                        {
                                            pc_frame_send.Ctrl = 0x91;
                                        }
                                        else
                                        {
                                            pc_frame_send.Ctrl = 0xB1;
                                        }
                            
                                        Create_DL645_Frame((u8 *)pc_addr, pc_frame_send.Ctrl, pc_frame_send.Len, &pc_frame_send);
                            
                                        send_len = pc_frame_send.Len + DL645_FIX_LEN;
                            
                                        while(OSSemAccept(g_sem_pc));
                                    
                                        pc_uart_send((u8 *)&pc_frame_send, send_len);                                            
                                    }    
                                }

                                f_close(&fp);
                            } 
                            
                            f_mount(SD_DRV, NULL);
                            break;

                        case FRM_CTRW_07_READ_SLVS_FOLLOW_DATA:
                            memcpy(fname, &pc_frame_recv.Data[DL645_07_DATA_ITEM_LEN], FILE_NAME_LEN);
                            
                            fname[FILE_NAME_LEN] = '\0';

                            memcpy(&seq, &pc_frame_recv.Data[DL645_07_DATA_ITEM_LEN + FILE_NAME_LEN], SEQ_LEN);
                            
                            if(FR_OK == f_mount(SD_DRV, &fs))
                            {
                                if(FR_OK == f_open(&fp, fname, FA_OPEN_EXISTING | FA_READ))
                                {
                                    f_lseek(&fp, offset);
                                    
                                    if(FR_OK == f_read(&fp, buf, 128, &br))
                                    {
                                        offset += br;

                                        memcpy(&pc_frame_send.Data[DL645_07_DATA_ITEM_LEN], fname, FILE_NAME_LEN);
                                        
                                        memcpy(&pc_frame_send.Data[DL645_07_DATA_ITEM_LEN + FILE_NAME_LEN], buf, br);

                                        memcpy(&pc_frame_send.Data[DL645_07_DATA_ITEM_LEN + FILE_NAME_LEN + br], &seq, SEQ_LEN);
                            
                                        pc_frame_send.Len = DL645_07_DATA_ITEM_LEN + FILE_NAME_LEN + br + SEQ_LEN;

                                        if(offset >= fsize)
                                        {
                                            pc_frame_send.Ctrl = 0x92;
                                        }
                                        else
                                        {
                                            pc_frame_send.Ctrl = 0xB2;
                                        }
                            
                                        Create_DL645_Frame((u8 *)pc_addr, pc_frame_send.Ctrl, pc_frame_send.Len, &pc_frame_send);
                            
                                        send_len = pc_frame_send.Len + DL645_FIX_LEN;
                            
                                        while(OSSemAccept(g_sem_pc));
                                    
                                        pc_uart_send((u8 *)&pc_frame_send, send_len);                                            
                                    }    
                                }

                                f_close(&fp);
                            } 
                            
                            f_mount(SD_DRV, NULL);
                            break;
                            
                        default:
                            break;
                        }
                        break;

                    case READ_TIME_CMD:
                        memcpy(&pc_frame_send.Data[DL645_07_DATA_ITEM_LEN], &g_rtc_time, MAX_RTC_ITEM);
                        
                        pc_frame_send.Len = DL645_07_DATA_ITEM_LEN + MAX_RTC_ITEM;
                        
                        pc_frame_send.Ctrl = 0x91;
                        
                        Create_DL645_Frame((u8 *)pc_addr, pc_frame_send.Ctrl, pc_frame_send.Len, &pc_frame_send);
                        
                        send_len = pc_frame_send.Len + DL645_FIX_LEN;
                        
                        while(OSSemAccept(g_sem_pc));
                        
                        pc_uart_send((u8 *)&pc_frame_send, send_len); 
                        break;

                    case WRITE_TIME_CMD:
                        u8 new_time[MAX_RTC_ITEM];

                        
                        new_time[SEC_POS] = Hex2BcdChar(pc_frame_recv.Data[12]);
                        new_time[MIN_POS] = Hex2BcdChar(pc_frame_recv.Data[13]);
                        new_time[HOUR_POS] = Hex2BcdChar(pc_frame_recv.Data[14]);
                        new_time[WEEK_POS] = Hex2BcdChar(pc_frame_recv.Data[15]) % 7;
                        new_time[DATE_POS] = Hex2BcdChar(pc_frame_recv.Data[16]);
                        new_time[MONTH_POS] = Hex2BcdChar(pc_frame_recv.Data[17]);
                        new_time[YEAR_POS] = Hex2BcdChar(pc_frame_recv.Data[18]);

                        RTC_WriteTime(new_time);

                        pc_frame_send.Len = 0;
                        
                        pc_frame_send.Ctrl = 0x94;
                        
                        Create_DL645_Frame((u8 *)pc_addr, pc_frame_send.Ctrl, pc_frame_send.Len, &pc_frame_send);
                        
                        send_len = pc_frame_send.Len + DL645_FIX_LEN;
                        
                        while(OSSemAccept(g_sem_pc));
                        
                        pc_uart_send((u8 *)&pc_frame_send, send_len); 
                        break;

                    case READ_VERSION_CMD:
                        u32 temp;

                        
                        buf[0] = HARDWARE_VERSION;
                        buf[1] = SOFTWARE_VERSION;
                        temp = VERSION_DATE;
                        memcpy(&buf[2], &temp, sizeof(temp));
                        
                        memcpy(&pc_frame_send.Data[DL645_07_DATA_ITEM_LEN], buf, VERSION_LEN);
                        
                        pc_frame_send.Len = DL645_07_DATA_ITEM_LEN + VERSION_LEN;
                        
                        pc_frame_send.Ctrl = 0x91;
                        
                        Create_DL645_Frame((u8 *)pc_addr, pc_frame_send.Ctrl, pc_frame_send.Len, &pc_frame_send);
                        
                        send_len = pc_frame_send.Len + DL645_FIX_LEN;
                        
                        while(OSSemAccept(g_sem_pc));
                        
                        pc_uart_send((u8 *)&pc_frame_send, send_len); 
                        break;

                    case RESTORE_CMD:
                        dev_para_restore();
                        
                        pc_frame_send.Len = 0;
                        
                        pc_frame_send.Ctrl = 0x94;
                        
                        Create_DL645_Frame((u8 *)pc_addr, pc_frame_send.Ctrl, pc_frame_send.Len, &pc_frame_send);
                        
                        send_len = pc_frame_send.Len + DL645_FIX_LEN;
                        
                        while(OSSemAccept(g_sem_pc));
                        
                        pc_uart_send((u8 *)&pc_frame_send, send_len);
                        break;
                        
                    default:
                        break;
                    }
                }
            }
        }
    }
}

/*
*********************************************************************************************************
*                                             App_TaskRS485()
*
* Description : This task monitors the state of the push buttons and passes messages to AppTaskUserIF()
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskRS485()' by 'OSTaskCreateExt()'.
*
* Return(s)  : none.
*
* Caller(s)  : This is a task.
*
* Note(s)    : none.
*********************************************************************************************************
*/
#if (EWARM_OPTIMIZATION_EN > 0u)
#pragma optimize = low
#endif
void  App_TaskRS485 (void *p_arg)
{
    const INT8U rs485_addr[6] = {0x99, 0x99, 0x99, 0x99, 0x99, 0x99};
    INT8U err, send_len;
    INT32U rs485_data_item;


    (void)p_arg;

    while (DEF_TRUE) {
        OSSemPend(g_sem_rs485, 0, &err);

        if(OS_ERR_NONE == err)
        {
            if(DL645_FRAME_OK == Analysis_DL645_Frame((u8 *)rs485_addr, (u8 *)&rs485_frame_recv, &rs485_frame_stat))
            {
                memcpy(&rs485_frame_send, &rs485_frame_recv, sizeof(DL645_FRAME));

                if((FRM_CTRW_07_READ_SLVS_DATA == (rs485_frame_stat.Ctrl & CCTT_CONTROL_CODE_MASK)) ||
                   (FRM_CTRW_07_READ_SLVS_FOLLOW_DATA == (rs485_frame_stat.Ctrl & CCTT_CONTROL_CODE_MASK)))
                {
                    rs485_data_item = ((INT32U)rs485_frame_recv.Data[3] << 24) | ((INT32U)rs485_frame_recv.Data[2] << 16) | ((INT32U)rs485_frame_recv.Data[1] << 8) | ((INT32U)rs485_frame_recv.Data[0] << 0);

                    switch(rs485_data_item)
                    {
                    case SHAKE_HANDS_CMD:
                        rs485_frame_send.Len = DL645_07_DATA_ITEM_LEN;
                        rs485_frame_send.Ctrl = 0x91;

                        Create_DL645_Frame((u8 *)rs485_addr, rs485_frame_send.Ctrl, rs485_frame_send.Len, &rs485_frame_send);

                        send_len = rs485_frame_send.Len + DL645_FIX_LEN;

                        while(OSSemAccept(g_sem_rs485));

                        rs485_uart_send((u8 *)&rs485_frame_send, send_len);
                        break;

                    default:
                        break;
                    }
                }
            }
        }
    }
}

