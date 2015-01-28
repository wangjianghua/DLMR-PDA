#include "includes.h"


const u8 P_MSG_HEAD[] = "\n\nHello, I'm uart data link layer! \nYour message is: ";

const INT8U cPLC_READ_ADDR[] = {0xFE, 0x68, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x68, 0x13, 0x00, 0xDF, 0x16};
const INT8U mPLC_REPLY_ADDR[] = {0x68, 0x62, 0x33, 0x19, 0x00, 0x00, 0x00, 0x68, 0x93, 0x06, 0x95, 0x66, 0x4C, 0x33, 0x33, 0x33, 0xF7, 0x16};

const INT8U rPLC_TO_lPLC[] = {0x68, 0xDD, 0xAB, 0xCF, 0xEA, 0xBC, 0xDA, 0x68, 0x04, 0x07, 0x33, 0x34, 0x36, 0x33, 0x33, 0x33, 0x34, 0x1C, 0x16};
const INT8U lPLC_TO_rPLC[] = {0x68, 0xDD, 0xAB, 0xCF, 0xEA, 0xBC, 0xDA, 0x68, 0x04, 0x07, 0x33, 0x34, 0x36, 0x33, 0x33, 0x33, 0x33, 0x1B, 0x16};

const INT8U lBroadcast_Read_Meter[] = {0x68, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x68, 0x13, 0x00, 0xDF, 0x16};

OS_EVENT *g_sem_plc;

u8 g_msg_buf[UART_RECEIVE_BUF_SIZE];
u16 g_msg_len;

u8 g_cplc_read_addr[13];

u8 g_cur_freq;

DL645_Frame_C dl645_frame_send;
DL645_Frame_C dl645_frame_recv;
DL645_Frame_Stat_C dl645_frame_stat;
PLC_PRM g_plc_prm;

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
    
    return End_send(pMsg);
}

u16 plc_uart_send(u8 *buf, u16 len)
{
    P_MSG_INFO  pMsg = NULL;

#if OS_CRITICAL_METHOD == 3u
    OS_CPU_SR  cpu_sr = 0u;
#endif  


    if(!(pMsg = (P_MSG_INFO)alloc_send_buffer(MSG_SHORT)))
    {
        return (FALSE);
    }

    memcpy(pMsg->msg_buffer, buf, len);

    pMsg->msg_header.msg_len = len;

    pMsg->msg_header.end_id = PLC_COM_PORT;

    pMsg->msg_header.need_buffer_free = TRUE;
    
    return End_send(pMsg);
}

u16 uart_link_reply(void)
{
    P_MSG_INFO  pMsg = NULL;
    

    if(!(pMsg = (P_MSG_INFO)alloc_send_buffer(MSG_SHORT)))
    {
        return (FALSE);
    }

    memcpy(g_msg_buf, P_MSG_HEAD, sizeof(P_MSG_HEAD));

    memcpy(pMsg->msg_buffer, g_msg_buf, sizeof(g_msg_buf));

    pMsg->msg_header.msg_len = sizeof(g_msg_buf);

    pMsg->msg_header.end_id = PC_COM_PORT;

    pMsg->msg_header.need_buffer_free = TRUE;
    
    return End_send(pMsg);
}

u16 cplc_read_addr(void)
{
    P_MSG_INFO  pMsg = NULL;

#if OS_CRITICAL_METHOD == 3u
    OS_CPU_SR  cpu_sr = 0u;
#endif  


    if(!(pMsg = (P_MSG_INFO)alloc_send_buffer(MSG_SHORT)))
    {
        return (FALSE);
    }

    OS_ENTER_CRITICAL();
    memcpy(g_cplc_read_addr, cPLC_READ_ADDR, sizeof(cPLC_READ_ADDR));

    if(FREQ_270KHz == g_cur_freq)
    {
        g_cplc_read_addr[0] = 0xFC;
    }
    else
    {
        g_cplc_read_addr[0] = 0xFA;
    }
    OS_EXIT_CRITICAL();    

    memcpy(pMsg->msg_buffer, g_cplc_read_addr, sizeof(g_cplc_read_addr));

    pMsg->msg_header.msg_len = sizeof(g_cplc_read_addr);

    pMsg->msg_header.end_id = PLC_COM_PORT;

    pMsg->msg_header.need_buffer_free = TRUE;
    
    return End_send(pMsg);
}

unsigned int PC_postProcess(pvoid h)
{
    P_MSG_INFO  pMsg = (P_MSG_INFO)h;
    u8  *pBuf = (UCHAR *)(pMsg->msg_buffer);
    u16  mLen = pMsg->msg_header.msg_len;
    
#if OS_CRITICAL_METHOD == 3u
    OS_CPU_SR  cpu_sr = 0u;
#endif   


    OS_ENTER_CRITICAL();
    g_msg_len = 0;
    memset(g_msg_buf, '\0', sizeof(g_msg_buf));

    g_msg_len = mLen;
    memcpy(&g_msg_buf[sizeof(P_MSG_HEAD)], pBuf, mLen);
    OS_EXIT_CRITICAL();

    uart_link_reply();

    return (TRUE);
}

unsigned int RS485_postProcess(pvoid h)
{
    return (TRUE);
}

unsigned int PLC_postProcess(pvoid h)
{
    P_MSG_INFO  pMsg = (P_MSG_INFO)h;
    u8  *pBuf = (UCHAR *)(pMsg->msg_buffer);
    u16  mLen = pMsg->msg_header.msg_len;
    
#if OS_CRITICAL_METHOD == 3u
    OS_CPU_SR  cpu_sr = 0u;
#endif

    OS_ENTER_CRITICAL();
    memcpy(&dl645_frame_recv, pBuf, mLen);
    memcpy(g_plc_prm.recv_buf, pBuf, mLen);
    g_plc_prm.recv_len = mLen;

    LED_PLC_TOGGLE();

    pc_uart_send(pBuf, mLen);
    OS_EXIT_CRITICAL();

    OSSemPost(g_sem_plc);

    return (TRUE);
}

u16 sprintf_plc_listen_record(void)
{
    u8 *ptr;
    u16 i;
    u16 len;
    u8 buf[32];
    

    len = 0;
    
    if(g_plc_prm.recv_len)
    {
        memset(g_plc_prm.sd_buf, ' ', sizeof(g_plc_prm.sd_buf));
        
        sprintf(buf, "20%02x-%02x-%02x %02x:%02x:%02x  ", g_rtc_time[YEAR_POS], 
                                                           g_rtc_time[MONTH_POS],
                                                           g_rtc_time[DATE_POS],
                                                           g_rtc_time[HOUR_POS],
                                                           g_rtc_time[MIN_POS],
                                                           g_rtc_time[SEC_POS]);
        
        ptr = g_plc_prm.sd_buf;

        memcpy(ptr, buf, 21);
        ptr += 21;
        len += 21;

        *ptr++ = 'X';
        *ptr++ = ':';
        *ptr++ = ' ';
        len += 3;
        
        for(i = 0; i < g_plc_prm.recv_len; i++)
        {
            *ptr++ = GUI_Hex2Char(g_plc_prm.recv_buf[i] >> 4);
            *ptr++ = GUI_Hex2Char(g_plc_prm.recv_buf[i] & 0x0f);
            *ptr++ = ' ';
            len += 3;
        }

        sprintf(buf, " LEN: %03u", g_plc_prm.recv_len);
        
        memcpy(ptr, buf, 9);
        ptr += 9;
        len += 9;

        *ptr++ = '\r';
        *ptr++ = '\n';
        len += 2;
    }

    return (len);
}

#if (EWARM_OPTIMIZATION_EN > 0u)
#pragma optimize = low
#endif
u8 plc_listen_record(void)
{
    FATFS fs;
    FIL fp;
    UINT br;
    DIR dj;
    FRESULT res;
    u16 bytes;


    res = f_mount(SD_DRV, &fs); //挂载SD卡

    if(FR_OK != res)
    {
        DEBUG_PRINT(("SD mount error!\n"));

        f_mount(SD_DRV, NULL); //移除SD卡
        
        return (FALSE);
    }
    
    res = f_open(&fp, "plc_log.txt", FA_READ | FA_WRITE | FA_OPEN_ALWAYS); //以读写方式打开、创建文件

    if(FR_OK != res)
    {
        DEBUG_PRINT(("Open file error!\n"));

        f_close(&fp); //关闭文件
        f_mount(SD_DRV, NULL);
        
        return (FALSE);
    }

    res = f_lseek(&fp, fp.fsize); //移动到文件末尾

    if(FR_OK != res)
    {
        DEBUG_PRINT(("Lseek file error!\n"));

        f_close(&fp);
        f_mount(SD_DRV, NULL);
        
        return (FALSE);
    }  

    bytes = sprintf_plc_listen_record();
        
    res = f_write(&fp, g_plc_prm.sd_buf, bytes, &br); //追加文件

    if((FR_OK != res) || (!br))
    {
        DEBUG_PRINT(("Append file error!\n"));

        f_close(&fp);
        f_mount(SD_DRV, NULL);
        
        return (FALSE);
    }    

    f_close(&fp);
    f_mount(SD_DRV, NULL);

    return (TRUE);
}

void  PRO_Databuf_Proc() 
{
    u32 len;
    
    Analysis_DL645_Frame(g_send_para_pkg.dstAddr, 
                        (u8 *)&dl645_frame_recv,
                        &dl645_frame_stat);

    if((0 == dl645_frame_stat.Status) || (dl645_frame_stat.C & DL645_REPLY_STAT_MASK))
    {
        g_plc_prm.result = PLC_RES_FAIL;
        
        
    }
    else
    {
        g_plc_prm.result = PLC_RES_SUCC;                        

        len = dl645_frame_recv.L;

        if((FRM_CTRW_97_READ_SLVS_DATA == (dl645_frame_stat.C & CCTT_CONTROL_CODE_MASK)) ||
           (FRM_CTRW_97_WRITE_SLVS_DATA == (dl645_frame_stat.C & CCTT_CONTROL_CODE_MASK)))
        {   /* 1997读写命令，2个字节数据标识 */
            
            if(len >= DL645_97_DATA_ITEM_LEN)
            {
                g_plc_prm.data_len = len - DL645_97_DATA_ITEM_LEN;
                memcpy(g_plc_prm.data_buf, &dl645_frame_recv.Data[DL645_97_DATA_ITEM_LEN], len - DL645_97_DATA_ITEM_LEN);
            }

            
        }
        else if((FRM_CTRW_07_READ_SLVS_DATA == (dl645_frame_stat.C & CCTT_CONTROL_CODE_MASK)) ||
                (FRM_CTRW_07_WRITE_SLVS_DATA == (dl645_frame_stat.C & CCTT_CONTROL_CODE_MASK)))

        {
            if(len >= DL645_07_DATA_ITEM_LEN)
            {
                /* 2007读写命令，4个字节数据标识 */
                memcpy(g_plc_prm.data_buf, &dl645_frame_recv.Data[DL645_07_DATA_ITEM_LEN], len - DL645_07_DATA_ITEM_LEN);
    
                g_plc_prm.data_len = len - DL645_07_DATA_ITEM_LEN;
            }

        }      
        else if((FRM_CTRW_07_BROAD_READ_ADDR == (dl645_frame_stat.C & CCTT_CONTROL_CODE_MASK)))
        {
            if(len == 6)
            {
                /* 2007读写命令，4个字节数据标识 */
                memcpy(g_plc_prm.data_buf, &dl645_frame_recv.Data[0], 6);    
                g_plc_prm.data_len = 6;
            }
        }
            
    }
}

/*
*********************************************************************************************************
*                                             App_TaskPLC()
*
* Description : This task monitors the state of the push buttons and passes messages to AppTaskUserIF()
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskPLC()' by 'OSTaskCreateExt()'.
*
* Return(s)  : none.
*
* Caller(s)  : This is a task.
*
* Note(s)    : none.
*********************************************************************************************************
*/
void  App_TaskPLC (void *p_arg)
{
    INT8U err;
    u8 len;
    WM_HWIN  wh;
    
    (void)p_arg; 

    while (DEF_TRUE) {    
        OSMboxPend(g_sys_control.downMb, 5, &err);

        if(OS_ERR_NONE == err)
        {
            wh = GUI_Get_PROGBAR();
            if(wh != WM_HWIN_NULL)
            {
                g_sys_control.testProgBarVal = 0;
                PROGBAR_SetValue(wh, g_sys_control.testProgBarVal);         
                //OSTimeDly(50);
            }
            
            switch(g_send_para_pkg.cmdType)
            {
            case PLC_CMD_BROAD_READ:
                OSSemAccept(g_sem_plc);
                
                plc_uart_send((u8 *)lBroadcast_Read_Meter, sizeof(lBroadcast_Read_Meter));

                g_plc_prm.sendStatus = PLC_MSG_SENDING;
                
                OSMboxPost(g_sys_control.upMb, (void *)&g_plc_prm);
                
                OSSemPend(g_sem_plc, 5 * OS_TICKS_PER_SEC, &err);

                g_plc_prm.data_len = 0;

                if(OS_ERR_NONE == err)
                {
                    PRO_Databuf_Proc();
                }
                else
                {
                    g_plc_prm.result = PLC_RES_TIMEOUT;

                }

                g_plc_prm.sendStatus = PLC_MSG_RECEIVED;

                OSMboxPost(g_sys_control.upMb, (void *)&g_plc_prm);
                break;
            case PLC_CMD_TYPE_R2L:
                OSSemAccept(g_sem_plc);
                
                plc_uart_send((u8 *)rPLC_TO_lPLC, sizeof(rPLC_TO_lPLC));

                g_plc_prm.sendStatus = PLC_MSG_SENDING;
                
                OSSemPend(g_sem_plc, 5 * OS_TICKS_PER_SEC, &err);

                if(OS_ERR_NONE == err)
                {
                    Analysis_DL645_Frame(g_send_para_pkg.dstAddr, 
                                        (u8 *)&dl645_frame_recv,
                                        &dl645_frame_stat);
                
                    if((0 == dl645_frame_stat.Status) || (dl645_frame_stat.C & DL645_REPLY_STAT_MASK))
                    {
                        g_plc_prm.result = PLC_RES_FAIL;
                        
                        g_plc_prm.data_len = 0;
                    }
                    else
                    {
                        g_plc_prm.result = PLC_RES_SUCC;

                        g_plc_prm.data_len = 0;
                    }
                }
                else
                {
                    g_plc_prm.result = PLC_RES_TIMEOUT;

                    g_plc_prm.data_len = 0;

                }

                g_plc_prm.sendStatus = PLC_MSG_RECEIVED;

                OSMboxPost(g_sys_control.upMb, (void *)&g_plc_prm);
                break;

            case PLC_CMD_TYPE_L2R:
                OSSemAccept(g_sem_plc);
                
                plc_uart_send((u8 *)lPLC_TO_rPLC, sizeof(lPLC_TO_rPLC));

                g_plc_prm.sendStatus = PLC_MSG_SENDING;
                
                OSSemPend(g_sem_plc, 5 * OS_TICKS_PER_SEC, &err);

                g_plc_prm.data_len = 0;

                if(OS_ERR_NONE == err)
                {
                    Analysis_DL645_Frame(g_send_para_pkg.dstAddr, 
                                         (u8 *)&dl645_frame_recv,
                                         &dl645_frame_stat);
                
                    if((0 == dl645_frame_stat.Status) || (dl645_frame_stat.C & DL645_REPLY_STAT_MASK))
                    {
                        g_plc_prm.result = PLC_RES_FAIL;
                                                
                    }
                    else
                    {
                        g_plc_prm.result = PLC_RES_SUCC;
                       
                    }
                }
                else
                {
                    g_plc_prm.result = PLC_RES_TIMEOUT;

                    

                }

                g_plc_prm.sendStatus = PLC_MSG_RECEIVED;

                OSMboxPost(g_sys_control.upMb, (void *)&g_plc_prm);
                break;

            case PLC_CMD_TYPE_COMMON:
                if((FRM_CTRW_97_READ_SLVS_DATA == (g_send_para_pkg.ctlCode & CCTT_CONTROL_CODE_MASK)) ||
                   (FRM_CTRW_97_WRITE_SLVS_DATA == (g_send_para_pkg.ctlCode & CCTT_CONTROL_CODE_MASK)))
                {   /* 1997读写命令，2个字节数据标识 */
                    memcpy(dl645_frame_send.Data, g_send_para_pkg.dataFlag, DL645_97_DATA_ITEM_LEN);
                    memcpy(&dl645_frame_send.Data[DL645_97_DATA_ITEM_LEN], g_send_para_pkg.dataBuf, g_send_para_pkg.dataLen);
                    g_send_para_pkg.dataLen += DL645_97_DATA_ITEM_LEN;
                }
                else if((FRM_CTRW_07_READ_SLVS_DATA == (g_send_para_pkg.ctlCode & CCTT_CONTROL_CODE_MASK)) ||
                        (FRM_CTRW_07_WRITE_SLVS_DATA == (g_send_para_pkg.ctlCode & CCTT_CONTROL_CODE_MASK)))
                
                {   /* 2007读写命令，4个字节数据标识 */
                    memcpy(dl645_frame_send.Data, g_send_para_pkg.dataFlag, DL645_07_DATA_ITEM_LEN);
                    memcpy(&dl645_frame_send.Data[DL645_07_DATA_ITEM_LEN], g_send_para_pkg.dataBuf, g_send_para_pkg.dataLen);
                    g_send_para_pkg.dataLen += DL645_07_DATA_ITEM_LEN;
                }
                else
                {   /* 非读写命令，无数据标识 */
                    memcpy(dl645_frame_send.Data, g_send_para_pkg.dataBuf, g_send_para_pkg.dataLen);
                } 
                
                Create_DL645_Frame(g_send_para_pkg.dstAddr, g_send_para_pkg.ctlCode, g_send_para_pkg.dataLen, &dl645_frame_send);
                
                g_plc_prm.send_len = g_send_para_pkg.dataLen + DL645_FIX_LEN;
                
                memcpy(&g_plc_prm.send_buf[DL645_INDEX], &dl645_frame_send, g_plc_prm.send_len);
                
                g_plc_prm.send_buf[FREQ_INDEX] = g_sys_register_para.preamble;
                g_plc_prm.send_len += FREQ_LEN;
                
                OSSemAccept(g_sem_plc);
                
                plc_uart_send(g_plc_prm.send_buf, g_plc_prm.send_len);
                
                g_plc_prm.sendStatus = PLC_MSG_SENDING;
                
                OSMboxPost(g_sys_control.upMb, (void *)&g_plc_prm);
                
                OSSemPend(g_sem_plc, 5 * OS_TICKS_PER_SEC, &err);

                g_plc_prm.data_len = 0;
                
                if(OS_ERR_NONE == err)
                {
                    PRO_Databuf_Proc();
                }
                else
                {   
                    g_plc_prm.result = PLC_RES_TIMEOUT;                                        
                }
                
                g_plc_prm.sendStatus = PLC_MSG_RECEIVED;
                
                OSMboxPost(g_sys_control.upMb, (void *)&g_plc_prm);
                break;

            default:
                break;
            }
        }
        else
        {
            OSSemPend(g_sem_plc, 5, &err);

            if(OS_ERR_NONE == err)
            {
                g_plc_prm.result = PLC_RES_SUCC;

                if(FALSE == plc_listen_record())
                {
                    DEBUG_PRINT(("PLC listening record error!\n"));
                }

                g_plc_prm.sendStatus = PLC_MSG_RECEIVED;
                
                OSMboxPost(g_sys_control.upMb, (void *)&g_plc_prm);                
            }
        }
    }
}

