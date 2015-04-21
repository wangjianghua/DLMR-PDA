#include "includes.h"


const u8 P_MSG_HEAD[] = "\n\nHello, I'm uart data link layer! \nYour message is: ";

const INT8U cPLC_READ_ADDR[] = {0xFE, 0x68, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x68, 0x13, 0x00, 0xDF, 0x16};
const INT8U mPLC_REPLY_ADDR[] = {0x68, 0x62, 0x33, 0x19, 0x00, 0x00, 0x00, 0x68, 0x93, 0x06, 0x95, 0x66, 0x4C, 0x33, 0x33, 0x33, 0xF7, 0x16};

const INT8U rPLC_TO_lPLC[] = {0x68, 0xDD, 0xAB, 0xCF, 0xEA, 0xBC, 0xDA, 0x68, 0x04, 0x07, 0x33, 0x34, 0x36, 0x33, 0x33, 0x33, 0x34, 0x1C, 0x16};
const INT8U lPLC_TO_rPLC[] = {0x68, 0xDD, 0xAB, 0xCF, 0xEA, 0xBC, 0xDA, 0x68, 0x04, 0x07, 0x33, 0x34, 0x36, 0x33, 0x33, 0x33, 0x33, 0x1B, 0x16};

const INT8U READ_PLC_NODE[] = {0x68, 0x1E, 0x00, 0x41, 0x04, 0x00, 0x5F, 0x64, 0x04, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x16, 0x01, 0x00, 0x02, 0x02, 0xB4, 0xD8, 0x16};

const INT8U lBroadcast_Read_Meter[12] = {0x68, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x68, 0x13, 0x00, 0xDF, 0x16};


const INT8U rPLC_270III[]  = {0x68, 0xDD, 0xAB, 0xCF, 0xEA, 0xBC, 0xDA, 0x68, 0x04, 0x08, 0x34, 0x34, 0x36, 0x33, 0x33, 0x33, 0x33, 0x33, 0x50, 0x16};
const INT8U rPLC_270III5[] = {0x68, 0xDD, 0xAB, 0xCF, 0xEA, 0xBC, 0xDA, 0x68, 0x04, 0x08, 0x34, 0x34, 0x36, 0x33, 0x33, 0x33, 0x34, 0x33, 0x51, 0x16};
const INT8U rPLC_270II[]   = {0x68, 0xDD, 0xAB, 0xCF, 0xEA, 0xBC, 0xDA, 0x68, 0x04, 0x08, 0x34, 0x34, 0x36, 0x33, 0x33, 0x33, 0x35, 0x33, 0x52, 0x16};

const INT8U rPLC_421_50BPS[]   = {0x68, 0xDD, 0xAB, 0xCF, 0xEA, 0xBC, 0xDA, 0x68, 0x04, 0x08, 0x34, 0x34, 0x36, 0x33, 0x33, 0x33, 0x33, 0x34, 0x51, 0x16};
const INT8U rPLC_421_100BPS[]  = {0x68, 0xDD, 0xAB, 0xCF, 0xEA, 0xBC, 0xDA, 0x68, 0x04, 0x08, 0x34, 0x34, 0x36, 0x33, 0x33, 0x33, 0x34, 0x34, 0x52, 0x16};
const INT8U rPLC_421_600BPS[]  = {0x68, 0xDD, 0xAB, 0xCF, 0xEA, 0xBC, 0xDA, 0x68, 0x04, 0x08, 0x34, 0x34, 0x36, 0x33, 0x33, 0x33, 0x35, 0x34, 0x53, 0x16};
const INT8U rPLC_421_1200BPS[] = {0x68, 0xDD, 0xAB, 0xCF, 0xEA, 0xBC, 0xDA, 0x68, 0x04, 0x08, 0x34, 0x34, 0x36, 0x33, 0x33, 0x33, 0x36, 0x34, 0x54, 0x16};

//密码
const INT8U wPLC_PWD[] = {0x34, 0x64, 0x74, 0x84};

//操作码 87 86 74 78
const INT8U wPLC_OP_CODE[] = {0x78, 0x74, 0x86, 0x87};

u8 rf_send_buf[256];

OS_EVENT *g_sem_plc;
OS_EVENT *g_sem_rf;
OS_EVENT *g_sem_pc;

u8 g_msg_buf[UART_RECEIVE_BUF_SIZE];
u16 g_msg_len;

u8 g_cplc_read_addr[13];

u8 g_cur_freq;

DL645_Frame_C dl645_frame_send;
DL645_Frame_C dl645_frame_recv;
DL645_Frame_Stat_C dl645_frame_stat;
PLC_PRM g_plc_prm;
DL645_Frame_C pc_frame_send;
DL645_Frame_C pc_frame_recv;
DL645_Frame_Stat_C pc_frame_stat;

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

    memcpy(&pc_frame_recv, pBuf, mLen);
    OS_EXIT_CRITICAL();

    OSSemPost(g_sem_pc);

    return (TRUE);
}

unsigned int RS485_postProcess(pvoid h)
{
    return (TRUE);
}

u32 PRO_DL645_Proc()
{
    if(DL645_FRAME_ERROR == Analysis_DL645_Frame(g_send_para_pkg.dstAddr, 
                                        (u8 *)&dl645_frame_recv,
                                        &dl645_frame_stat))
    {
         g_plc_prm.result = PLC_RES_FAIL;                                                 
    }
    else
    {                
        if((0 == dl645_frame_stat.Status)                            
            || (dl645_frame_stat.C == 0))
        {
            g_plc_prm.result = PLC_RES_FAIL;                            
        }
        else if ((dl645_frame_stat.C & DL645_REPLY_STAT_MASK))
        {
            g_plc_prm.result = PLC_RES_ERROR_FRAME;
        }
        else
        {
            g_plc_prm.result = PLC_RES_SUCC;
        }
    }
    return g_plc_prm.result;
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
    u8 fname[16], oldest_file, i;
    u16 bytes;
    u32 sd_file_num;
    FILINFO fil_info;


    res = f_mount(SD_DRV, &fs); //挂载SD卡

    if(FR_OK != res)
    {
        DEBUG_PRINT(("SD mount error!\n"));

        f_mount(SD_DRV, NULL); //移除SD卡
        
        return (FALSE);
    }
    
    scan_files("/");
    
    sd_file_num = 0;
    while(SD_FileName[sd_file_num][0])
    {
        if(FR_OK == f_stat(SD_FileName[sd_file_num], &fil_info))
        {
            SD_FileTimestamp[sd_file_num].date = fil_info.fdate;
            SD_FileTimestamp[sd_file_num].time = fil_info.ftime;
        }
        else
        {
            SD_FileTimestamp[sd_file_num].date = 0;
            SD_FileTimestamp[sd_file_num].time = 0;
        }
        
        sd_file_num++;
    }    
    
    sprintf(fname, "20%02x%02x%02x.txt", g_rtc_time[YEAR_POS], g_rtc_time[MONTH_POS], g_rtc_time[DATE_POS]);
    fname[FILE_NAME_LEN] = '\0';
    
    if(FR_OK != f_open(&fp, fname, FA_OPEN_EXISTING))
    {
        if(MAX_FILE_NUM == sd_file_num) //文件数已满，删除最旧的文件
        {
            oldest_file = 0;
            i = 1;
            
            while(1)
            {
                if(SD_FileTimestamp[oldest_file].date > SD_FileTimestamp[i].date)
                {
                    oldest_file = i;
                }
                
                i++;
                
                if(i >= sd_file_num)
                {
                    break;
                }                
            }
            
            f_unlink(SD_FileName[oldest_file]);
        }
        else if(sd_file_num > MAX_FILE_NUM)//SD卡文件数不符合要求，全部删除
        {
            i = 0;
            
            while(1)
            {
                f_unlink(SD_FileName[i]);
                
                i++;
                
                if(i >= sd_file_num)
                {
                    break;
                }
            }
        }
        
        f_open(&fp, fname, FA_OPEN_ALWAYS); //创建新文件
    }
    
    f_close(&fp); //关闭文件
    
    res = f_open(&fp, fname, FA_READ | FA_WRITE | FA_OPEN_EXISTING); //以读写方式打开已存在文件

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

    if(PLC_RES_SUCC == PRO_DL645_Proc())       
    {

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
#pragma optimize = low

void  App_TaskPLC (void *p_arg)
{
    INT8U err;
    u8 len;
    WM_HWIN wh;
    int i;
    u8 rf_addr[] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x00, 0x36, 0x19, 0x00, 0x00, 0x00};
    //u8 rf_addr[] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};
    u32 rf_send_len;
    u8 plc_read_addr[20];
    
    
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
            }
            
            switch(g_send_para_pkg.cmdType)
            {
            case PLC_CMD_BROAD_READ: //广播命令
                if(CHANNEL_PLC == g_sys_register_para.channel)
                {
                    OSSemAccept(g_sem_plc);
  
                    memcpy(&plc_read_addr[1], lBroadcast_Read_Meter, sizeof(lBroadcast_Read_Meter));

                    plc_read_addr[0] = g_sys_register_para.preamble;
                      
                    plc_uart_send((u8 *)plc_read_addr, sizeof(lBroadcast_Read_Meter) + 1);

                    g_plc_prm.sendStatus = PLC_MSG_SENDING;

                    g_plc_prm.send_len = 12 + 1;

                    memcpy(g_plc_prm.send_buf, plc_read_addr, g_plc_prm.send_len);                    

                    OSMboxPost(g_sys_control.upMb, (void *)&g_plc_prm);

                    OSTimeDly(200);

                    g_plc_prm.result = PLC_RES_NONE;
                    
                    OSSemPend(g_sem_plc, g_sys_register_para.bpsSpeed * OS_TICKS_PER_SEC, &err);

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
                }
                else if(CHANNEL_WIRELESS == g_sys_register_para.channel) //无线
                {
                    OSSemAccept(g_sem_rf);

                    memset(RF_SEND_BUF, 0, sizeof(RF_SEND_BUF));
                    
                    GDW_RF_Protocol_2013(rf_addr, 0x00, 0x00, 0x00, (u8 *)&lBroadcast_Read_Meter, 12, RF_SEND_BUF);

                    g_plc_prm.sendStatus = PLC_MSG_SENDING;

                    g_plc_prm.send_len = 12;

                    memcpy(g_plc_prm.send_buf, lBroadcast_Read_Meter, g_plc_prm.send_len);

                    OSMboxPost(g_sys_control.upMb, (void *)&g_plc_prm);

                    OSTimeDly(200);                    

                    OSSemPend(g_sem_rf, 5 * OS_TICKS_PER_SEC, &err);

                    g_plc_prm.data_len = 0;

                    if(OS_ERR_NONE == err)
                    {
                        memcpy(&dl645_frame_recv, &RF_RECV_BUF[RF_RECV_FIX_LEN], RF_RECV_LEN - RF_RECV_FIX_LEN);                                       
                        memcpy(&g_plc_prm.recv_buf,&RF_RECV_BUF[RF_RECV_FIX_LEN], RF_RECV_LEN - RF_RECV_FIX_LEN);
                        g_plc_prm.recv_len = RF_RECV_LEN - RF_RECV_FIX_LEN - RF_PRINT_FIX_LEN;
                        PRO_Databuf_Proc();
                    }
                    else
                    {   
                         g_plc_prm.result = PLC_RES_TIMEOUT;                                        
                    }
                    
                    g_plc_prm.sendStatus = PLC_MSG_RECEIVED;
                    
                    OSMboxPost(g_sys_control.upMb, (void *)&g_plc_prm);
                }
                break;
                
            case PLC_CMD_TYPE_R2L: //切换为监控态
                OSSemAccept(g_sem_plc);
                
                plc_uart_send((u8 *)rPLC_TO_lPLC, sizeof(rPLC_TO_lPLC));

                g_plc_prm.sendStatus = PLC_MSG_SENDING;
                
                OSSemPend(g_sem_plc, g_sys_register_para.bpsSpeed * OS_TICKS_PER_SEC, &err);

                g_plc_prm.data_len = 0;

                if(OS_ERR_NONE == err)
                {
                    PRO_DL645_Proc();
                }
                else
                {
                    g_plc_prm.result = PLC_RES_TIMEOUT;

                    g_plc_prm.data_len = 0;
                }

                g_plc_prm.sendStatus = PLC_MSG_RECEIVED;

                OSMboxPost(g_sys_control.upMb, (void *)&g_plc_prm);
                break;

            case PLC_CMD_TYPE_L2R:     //抄控态
                OSSemAccept(g_sem_plc);

                plc_uart_send((u8 *)lPLC_TO_rPLC, sizeof(lPLC_TO_rPLC));

                g_plc_prm.sendStatus = PLC_MSG_SENDING;
                
                OSSemPend(g_sem_plc, g_sys_register_para.bpsSpeed * OS_TICKS_PER_SEC, &err);

                g_plc_prm.data_len = 0;

                if(OS_ERR_NONE == err)
                {
                    PRO_DL645_Proc();
                }
                else
                {
                    g_plc_prm.result = PLC_RES_TIMEOUT;
                }

                g_plc_prm.sendStatus = PLC_MSG_RECEIVED;

                OSMboxPost(g_sys_control.upMb, (void *)&g_plc_prm);
                break;

            case PLC_CMD_TYPE_NODE:  //读载波节点
                OSSemAccept(g_sem_plc);
                
                plc_uart_send((u8 *)READ_PLC_NODE, sizeof(READ_PLC_NODE));

                g_plc_prm.sendStatus = PLC_MSG_SENDING;
                
                OSSemPend(g_sem_plc, g_sys_register_para.bpsSpeed * OS_TICKS_PER_SEC, &err);

                g_plc_prm.data_len = 0;

                if(OS_ERR_NONE == err)
                {
                    PRO_DL645_Proc();
                }
                else
                {
                    g_plc_prm.result = PLC_RES_TIMEOUT;
                }

                g_plc_prm.sendStatus = PLC_MSG_RECEIVED;

                OSMboxPost(g_sys_control.upMb, (void *)&g_plc_prm);
                break;

            case PLC_CMD_FREQ_SET:   //速率设置
                
                OSSemAccept(g_sem_plc);
                
                switch(g_sys_register_para.freqSel)
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

                g_plc_prm.sendStatus = PLC_MSG_SENDING;
                
                OSSemPend(g_sem_plc, g_sys_register_para.bpsSpeed * OS_TICKS_PER_SEC, &err);

                g_plc_prm.data_len = 0;

                if(OS_ERR_NONE == err)
                {
                    PRO_DL645_Proc();
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
                else if((FRM_CTRW_07_READ_SLVS_DATA == (g_send_para_pkg.ctlCode & CCTT_CONTROL_CODE_MASK))
                        ||(FRM_CTRW_07_WRITE_SLVS_DATA == (g_send_para_pkg.ctlCode & CCTT_CONTROL_CODE_MASK)))
                
                { 
                    if(FRM_CTRW_07_READ_SLVS_DATA == (g_send_para_pkg.ctlCode & CCTT_CONTROL_CODE_MASK))
                    {
                        /* 2007读命令，4个字节数据标识 */
                        memcpy(dl645_frame_send.Data, g_send_para_pkg.dataFlag, DL645_07_DATA_ITEM_LEN);
                        memcpy(&dl645_frame_send.Data[DL645_07_DATA_ITEM_LEN], g_send_para_pkg.dataBuf, g_send_para_pkg.dataLen);
                        g_send_para_pkg.dataLen += DL645_07_DATA_ITEM_LEN;
                    }
                }
                else
                {   /* 非读写命令，无数据标识 */
                    memcpy(dl645_frame_send.Data, g_send_para_pkg.dataBuf, g_send_para_pkg.dataLen);
                } 
                
                Create_DL645_Frame(g_send_para_pkg.dstAddr, g_send_para_pkg.ctlCode, g_send_para_pkg.dataLen, &dl645_frame_send);

                RF_SEND_LEN = g_send_para_pkg.dataLen + DL645_FIX_LEN;
                
                if(CHANNEL_WIRELESS == g_sys_register_para.channel)  //无线
                {
                    memcpy(&rf_addr[6], g_send_para_pkg.dstAddr, DL645_ADDR_LEN);

                    OSSemAccept(g_sem_rf);
                    
                    GDW_RF_Protocol_2013(rf_addr, 0x00, 0x00, 0x00, (u8 *)&dl645_frame_send, RF_SEND_LEN, RF_SEND_BUF);

                    g_plc_prm.sendStatus = PLC_MSG_SENDING;

                    g_plc_prm.send_len = RF_SEND_LEN;

                    memcpy(g_plc_prm.send_buf, (u8 *)&dl645_frame_send, g_plc_prm.send_len);                    

                    OSMboxPost(g_sys_control.upMb, (void *)&g_plc_prm);

                    OSTimeDly(200);                    

                    OSSemPend(g_sem_rf, 5 * OS_TICKS_PER_SEC, &err);

                    g_plc_prm.data_len = 0;

                    if(OS_ERR_NONE == err)
                    {
                        memcpy(&dl645_frame_recv, &RF_RECV_BUF[RF_RECV_FIX_LEN], RF_RECV_LEN - RF_RECV_FIX_LEN);                                       
                        memcpy(&g_plc_prm.recv_buf,&RF_RECV_BUF[RF_RECV_FIX_LEN], RF_RECV_LEN - RF_RECV_FIX_LEN);
                        g_plc_prm.recv_len = RF_RECV_LEN - RF_RECV_FIX_LEN - RF_PRINT_FIX_LEN;
                        PRO_Databuf_Proc();
                    }
                    else
                    {   
                        g_plc_prm.result = PLC_RES_TIMEOUT;                                        
                    }
                    
                    g_plc_prm.sendStatus = PLC_MSG_RECEIVED;
                    
                    OSMboxPost(g_sys_control.upMb, (void *)&g_plc_prm);
                }
                else if(CHANNEL_PLC == g_sys_register_para.channel)
                {
                    g_plc_prm.send_len = RF_SEND_LEN;
                    
                    memcpy(&g_plc_prm.send_buf[DL645_INDEX], &dl645_frame_send, g_plc_prm.send_len);
                    
                    g_plc_prm.send_buf[FREQ_INDEX] = g_sys_register_para.preamble;
                    g_plc_prm.send_len += FREQ_LEN;
                    
                    OSSemAccept(g_sem_plc);
                    
                    plc_uart_send(g_plc_prm.send_buf, g_plc_prm.send_len);
                    
                    g_plc_prm.sendStatus = PLC_MSG_SENDING;
                    
                    OSMboxPost(g_sys_control.upMb, (void *)&g_plc_prm);

                    OSTimeDly(200);
                    
                    OSSemPend(g_sem_plc, g_sys_register_para.bpsSpeed * OS_TICKS_PER_SEC, &err);

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
                }
                break;

            case PLC_CMD_TYPE_ROUTE: 
#if 0                
                //读命令
                if((FRM_CTRW_07_READ_SLVS_DATA == (g_send_para_pkg.ctlCode & CCTT_CONTROL_CODE_MASK)))
                {
                    //中继地址的第一个地址是在帧中的地址项里，而目标地址是在数据项中,所以i+1
                    for(i = 0; i < (g_sys_control.sysAddrLevel-1); i++)
                    {
                        memcpy(&dl645_frame_send.Data[DL645_ADDR_LEN * i], g_send_para_pkg.relayAddr[i+1], DL645_ADDR_LEN);
                    }
                    memcpy(&dl645_frame_send.Data[DL645_ADDR_LEN * (g_sys_control.sysAddrLevel-1)],g_send_para_pkg.dstAddr,DL645_ADDR_LEN);
                    
                    memcpy(&dl645_frame_send.Data[g_sys_control.sysAddrLevel * DL645_ADDR_LEN], g_send_para_pkg.dataFlag, DL645_07_DATA_ITEM_LEN);
                    //memcpy(&dl645_frame_send.Data[(g_sys_control.sysAddrLevel * DL645_ADDR_LEN) + DL645_07_DATA_ITEM_LEN], g_send_para_pkg.dataBuf, g_send_para_pkg.dataLen);
                    g_send_para_pkg.dataLen += (DL645_07_DATA_ITEM_LEN + g_sys_control.sysAddrLevel * DL645_ADDR_LEN);
                    g_send_para_pkg.ctlCode = c_645ctrlDef[g_sys_register_para.plcProtocol][1]; 
                    g_send_para_pkg.ctlCode += g_sys_control.sysAddrLevel * DL645_RELAY_ADDED_VAL;
                    //memcpy(g_send_para_pkg.relayAddr[g_sys_control.sysAddrLevel],g_send_para_pkg.dstAddr,DL645_ADDR_LEN);
                }

                Create_DL645_Frame(g_send_para_pkg.relayAddr[0], g_send_para_pkg.ctlCode, g_send_para_pkg.dataLen, &dl645_frame_send);
                //Create_DL645_LeveFrame(u8 * leve_Addr,u8 leve,u8 * Addr,u8 C,u8 len,u8 * data,u8 * Send_buf)();
                
                g_plc_prm.send_len = g_send_para_pkg.dataLen + DL645_FIX_LEN;
                
                memcpy(&g_plc_prm.send_buf[DL645_INDEX], &dl645_frame_send, g_plc_prm.send_len);
                
                g_plc_prm.send_buf[FREQ_INDEX] = g_sys_register_para.preamble;
                g_plc_prm.send_len += FREQ_LEN;
                
                OSSemAccept(g_sem_plc);
                
                plc_uart_send(g_plc_prm.send_buf, g_plc_prm.send_len);
                
                g_plc_prm.sendStatus = PLC_MSG_SENDING;
                
                OSMboxPost(g_sys_control.upMb, (void *)&g_plc_prm);

                OSTimeDly(200);
                
                OSSemPend(g_sem_plc, g_sys_register_para.bpsSpeed * OS_TICKS_PER_SEC, &err);

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
#endif

                memcpy(g_send_para_pkg.relayAddr[g_sys_control.sysAddrLevel], g_send_para_pkg.dstAddr, DL645_ADDR_LEN);
                g_send_para_pkg.ctlCode = c_645ctrlDef[g_sys_register_para.plcProtocol][1];
                g_plc_prm.send_len = Create_DL645_LeveFrame(g_send_para_pkg.relayAddr, g_sys_control.sysAddrLevel, g_send_para_pkg.dstAddr,
                                                            g_send_para_pkg.ctlCode, DL645_07_DATA_ITEM_LEN, g_send_para_pkg.dataFlag, &dl645_frame_send);

                //plc_uart_send(g_plc_prm.send_buf, g_plc_prm.send_len);

                memcpy(&g_plc_prm.send_buf[DL645_INDEX], &dl645_frame_send, g_plc_prm.send_len);
                
                g_plc_prm.send_buf[FREQ_INDEX] = g_sys_register_para.preamble;
                g_plc_prm.send_len += FREQ_LEN;
                
                OSSemAccept(g_sem_plc);
                
                plc_uart_send(g_plc_prm.send_buf, g_plc_prm.send_len);
                
                g_plc_prm.sendStatus = PLC_MSG_SENDING;
                
                OSMboxPost(g_sys_control.upMb, (void *)&g_plc_prm);

                OSTimeDly(200);
                
                OSSemPend(g_sem_plc, g_sys_register_para.bpsSpeed * OS_TICKS_PER_SEC, &err);

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
            if(CHANNEL_PLC == g_sys_register_para.channel) 
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
    INT8U err, send_file_num, fname[16], buf[128], pc_addr[6] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};
    INT16U send_len;
    INT32U i, j, pc_data_item, sd_file_num;
    FATFS fs;
    UINT br;
    static INT8U seq;
    static FIL fp;
    static INT32U fsize, offset;
    

    (void)p_arg;
    
    while (DEF_TRUE) {
        OSSemPend(g_sem_pc, 0, &err);

        if(OS_ERR_NONE == err)
        {
            if(DL645_FRAME_ERROR != Analysis_DL645_Frame(pc_addr, (u8 *)&pc_frame_recv, &pc_frame_stat))
            {
                if(0 != pc_frame_stat.Status)
                {
                    memcpy(&pc_frame_send, &pc_frame_recv, sizeof(DL645_Frame_C));
                    
                    if((FRM_CTRW_07_READ_SLVS_DATA == (pc_frame_stat.C & CCTT_CONTROL_CODE_MASK)) ||
                       (FRM_CTRW_07_EXT_READ_SLVS_DATA == (pc_frame_stat.C & CCTT_CONTROL_CODE_MASK)))
                    {
                        pc_data_item = ((INT32U)pc_frame_recv.Data[3] << 24) | ((INT32U)pc_frame_recv.Data[2] << 16) | ((INT32U)pc_frame_recv.Data[1] << 8) | ((INT32U)pc_frame_recv.Data[0] << 0);
                    
                        switch(pc_data_item)
                        {
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

                                pc_frame_send.L += FILE_NUM_LEN + FILE_NAME_LEN * send_file_num + FILE_SIZE_LEN * send_file_num;

                                pc_frame_send.C = 0x91;

                                Create_DL645_Frame(pc_addr, pc_frame_send.C, pc_frame_send.L, &pc_frame_send);
        
                                send_len = pc_frame_send.L + DL645_FIX_LEN;

                                OSSemAccept(g_sem_pc);
                            
                                pc_uart_send((u8 *)&pc_frame_send, send_len);                                    
                            } 


                            f_mount(SD_DRV, NULL);
                            break;

                        case READ_FILE_CMD: //读指定文件内容
                            switch(pc_frame_recv.C)
                            {
                            case FRM_CTRW_07_READ_SLVS_DATA:
                                fsize = 0;
                                offset = 0;
                                seq = 0;
                                
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
                                
                                            pc_frame_send.L = DL645_07_DATA_ITEM_LEN + FILE_NAME_LEN + br;

                                            if(offset >= fsize)
                                            {
                                                pc_frame_send.C = 0x91;
                                            }
                                            else
                                            {
                                                pc_frame_send.C = 0xB1;

                                                seq++;
                                            }
                                
                                            Create_DL645_Frame(pc_addr, pc_frame_send.C, pc_frame_send.L, &pc_frame_send);
                                
                                            send_len = pc_frame_send.L + DL645_FIX_LEN;
                                
                                            OSSemAccept(g_sem_pc);
                                        
                                            pc_uart_send((u8 *)&pc_frame_send, send_len);                                            
                                        }    
                                    }

                                    f_close(&fp);
                                } 

                                
                                f_mount(SD_DRV, NULL);
                                
                                break;

                            case FRM_CTRW_07_EXT_READ_SLVS_DATA:
                                memcpy(fname, &pc_frame_recv.Data[DL645_07_DATA_ITEM_LEN], FILE_NAME_LEN);
                                
                                fname[FILE_NAME_LEN] = '\0';
                                
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

                                            memcpy(&pc_frame_send.Data[DL645_07_DATA_ITEM_LEN + FILE_NAME_LEN + br], seq, SEQ_LEN);
                                
                                            pc_frame_send.L = DL645_07_DATA_ITEM_LEN + FILE_NAME_LEN + br + SEQ_LEN;

                                            if(offset >= fsize)
                                            {
                                                pc_frame_send.C = 0x92;
                                            }
                                            else
                                            {
                                                pc_frame_send.C = 0xB2;

                                                seq++;
                                            }
                                
                                            Create_DL645_Frame(pc_addr, pc_frame_send.C, pc_frame_send.L, &pc_frame_send);
                                
                                            send_len = pc_frame_send.L + DL645_FIX_LEN;
                                
                                            OSSemAccept(g_sem_pc);
                                        
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
                            
                        default:
                            break;
                        }
                    }
                }
            }
        }
        else
        {

        }
    }
}

