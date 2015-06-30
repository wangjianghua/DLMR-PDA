#include "includes.h"
#include "WIDGET.h"
#include "DIALOG.h"

u8 s_prbf[512];


const u8 c_645ctrlDef[2][PLC_CTRL_MAX_NUM] = 
{ 
    //97规约
    {0x05, 0x01, 0x04,  4,5,6,7,8,9,10,11,12,13,14,15,16},
    //07规约
    {0x13, 0X11, 0X14,  0x19,5,6,7,8,9,10,11,12,13,14,15,16}
};


const u32 c_645DidoDef[2][PLC_CTRL_MAX_NUM] = 
{ 
    //97规约
    {0x901f,    0x902f,     0x9410,     0x9420,5,6,7,8,9,10,11,12,13,14,15,16},
    //07规约
    {0x0001ff00,0X0002ff00, 0X0001ff01, 0X0002ff01,4,5,6,7,8,9,10,11,12,13,14,15,}
};

const u8 g_self_check_pwd[] = {'2', '2', '8', '8', '4', '4', '6', '6', '\0'}; 

void GUI_Fill_Zero(u8 *tempbuf)
{
    int len;
    int i;
    
    len = strlen(tempbuf);
    if(len < 12)
    {
        for(i = 0; i < len; i++)
        {
            tempbuf[GUI_645_ADDR_LENGTH - i-1] = tempbuf[len-i-1];
        }
        
        for(i = 0; i < (GUI_645_ADDR_LENGTH - len); i++)
        {
            tempbuf[i] = '0';
        }
        tempbuf[12] = '\0';
    }
    else if(len > 12)
    {
        tempbuf[12] = '\0';
    }
         
    return ;
}


u8 GUI_Hex2Char(u8 ch)
{
    ch = ch & 0x0f;
    if(ch < 10)
    {
        ch += '0';
    }
    else
    {
        ch = (ch - 10) + 'A';
    }
    
    return ch;
}


u8* GUI_hex2MeterAddrStr(u8 * srcBuf, u32 len)
{

    u8 *send_ptr;
    send_ptr = s_prbf;

    if(len > 256)
    {
        s_prbf[0] = 0;
        return s_prbf;
    }
    
    *send_ptr++ = GUI_Hex2Char( srcBuf[5]>>4);
    *send_ptr++ = GUI_Hex2Char( srcBuf[5]&0x0f);
    *send_ptr++ = GUI_Hex2Char( srcBuf[4]>>4);
    *send_ptr++ = GUI_Hex2Char( srcBuf[4]&0x0f);
    *send_ptr++ = GUI_Hex2Char( srcBuf[3]>>4);
    *send_ptr++ = GUI_Hex2Char( srcBuf[3]&0x0f);
    *send_ptr++ = GUI_Hex2Char( srcBuf[2]>>4);
    *send_ptr++ = GUI_Hex2Char( srcBuf[2]&0x0f);
    *send_ptr++ = GUI_Hex2Char( srcBuf[1]>>4);
    *send_ptr++ = GUI_Hex2Char( srcBuf[1]&0x0f);
    *send_ptr++ = GUI_Hex2Char( srcBuf[0]>>4);
    *send_ptr++ = GUI_Hex2Char( srcBuf[0]&0x0f);
    
  
    *send_ptr++ = 0;
    
    return s_prbf;

}

u32 GUI_GetMeterAddr(u8 * dbuf, u8 * gbuf)
{
    u8 rmd_ch;
    u8 i;
    
    for(i = 0; i < (GUI_645_ADDR_LENGTH/2); i++ )
    {
        if(((rmd_ch = GUI_char2hex(dbuf[i<<1])) == 0xff))
        {
            //error proc
            return DEV_ERROR;
        }
        gbuf[5-i] = rmd_ch<<4;
       
        
        if(((rmd_ch = GUI_char2hex(dbuf[(i<<1)+1])) == 0xff))
        {
            //error proc
            return DEV_ERROR;
        }
        gbuf[5-i] |= rmd_ch;
    }

    return DEV_OK;
}

void GUI_GetHexDataFlag(u8 * strbuf, u8* dataflag, u8 len)
{
    
    u8 i;
    
    if(len > 4)
        return;

    for(i = 0; i < len; i++)
    {
        *strbuf++ = GUI_Hex2Char( dataflag[len-i-1]>>4);
        *strbuf++ = GUI_Hex2Char( dataflag[len-i-1]&0x0f);
    }

    *strbuf = '\0';
}
u32 GUI_GetStrDataFlag(u8 * dbuf, u32 pro_ver)
{
    u8 rmd_ch;
    u8 i;
    u8 len;

    if(pro_ver == DL645_07)
        len = DL645_07_DATA_ITEM_LEN;
    else
        len = DL645_97_DATA_ITEM_LEN;
    
    for(i = 0; i < len; i++ )
    {
        if(((rmd_ch = GUI_char2hex(dbuf[i<<1])) == 0xff))
        {
            //error proc
            return DEV_ERROR;
        }
        g_gui_para.dataFlag[len-1-i] = rmd_ch<<4;
        g_sys_ctrl.defaultDataFlag[len-1-i] = rmd_ch<<4;
        
        if(((rmd_ch = GUI_char2hex(dbuf[(i<<1)+1])) == 0xff))
        {
            //error proc
            return DEV_ERROR;
        }
        
        g_gui_para.dataFlag[len-1-i] |= rmd_ch;
        g_sys_ctrl.defaultDataFlag[len-1-i] |= rmd_ch;
    }

    return DEV_OK;
}
//电量化为尖峰平谷小数
u8* GUI_hex2PowerDataStr(u8 * srcBuf, u32 len)
{

    u8 *send_ptr;
    send_ptr = s_prbf;

    if(len > 256)
    {
        s_prbf[0] = 0;
        return s_prbf;
    }

    *send_ptr++ = GUI_Hex2Char( srcBuf[3]>>4);
    *send_ptr++ = GUI_Hex2Char( srcBuf[3]&0x0f);
    *send_ptr++ = GUI_Hex2Char( srcBuf[2]>>4);
    *send_ptr++ = GUI_Hex2Char( srcBuf[2]&0x0f);
    *send_ptr++ = GUI_Hex2Char( srcBuf[1]>>4);
    *send_ptr++ = GUI_Hex2Char( srcBuf[1]&0x0f);
    *send_ptr++ = '.';
    *send_ptr++ = GUI_Hex2Char( srcBuf[0]>>4);
    *send_ptr++ = GUI_Hex2Char( srcBuf[0]&0x0f);
    
  
    *send_ptr++ = 0;
    
    return s_prbf;

}

void GUI_print_recv_buf()
{
    MULTIEDIT_HANDLE hObj;
    u16 i;
    u8 *send_ptr;
    //int n = 0;
    if(g_proto_para.recv_len && (g_proto_para.recv_len < 80))
    {
        send_ptr = s_prbf;
        *send_ptr++ = 'R';
        *send_ptr++ = ':';
        for(i = 0; i < g_proto_para.recv_len; i++)
        {
            *send_ptr++ = GUI_Hex2Char( g_proto_para.recv_buf[i]>>4);
            *send_ptr++ = GUI_Hex2Char( g_proto_para.recv_buf[i]&0x0f);
            *send_ptr++ = ' ';
        }
        *send_ptr++ = '\n';
        *send_ptr++ = 0;

        if((g_sys_ctrl.guiState == GUI_PLC_MSG_TEST)
            ||(g_sys_ctrl.guiState == GUI_PLC_MSG_READ)
            ||(g_sys_ctrl.guiState == GUI_PLC_MSG_FREQ))
        {
            hObj = MSG_Get_MultiEdit();
            MULTIEDIT_AddText(hObj, s_prbf); 
            if(g_sys_ctrl.guiState == GUI_PLC_MSG_READ)
            {
                RMD_proc_resp_data();
            }

            if(g_sys_ctrl.guiState == GUI_PLC_MSG_TEST)
            {
                STM_proc_resp_data();
            }
        }
        else if(g_sys_ctrl.guiState == GUI_PLC_MSG_LISTING)
        {
            hObj = MNT_Get_MultiEdit();
            if(g_sys_ctrl.numMultiedit > 25)
            {
                g_sys_ctrl.numMultiedit = 0;
                MULTIEDIT_SetText(hObj, "\0"); 
            }
            MULTIEDIT_AddText(hObj, s_prbf); 
        }
    }
}

void GUI_print_send_buf()
{
    MULTIEDIT_HANDLE hObj;
    u16 i;
    u8 *send_ptr;
    
    if(g_proto_para.send_len && (g_proto_para.send_len < 80))
    {        
        send_ptr = s_prbf;
        *send_ptr++ = 'S';
        *send_ptr++ = ':';
        for(i = 0; i < g_proto_para.send_len; i++)
        {
            *send_ptr++ = GUI_Hex2Char( g_proto_para.send_buf[i]>>4);
            *send_ptr++ = GUI_Hex2Char( g_proto_para.send_buf[i]&0x0f);
            *send_ptr++ = ' ';
        }
        *send_ptr++ = '\n';
        *send_ptr++ = 0;
        hObj = MSG_Get_MultiEdit();
        MULTIEDIT_AddText(hObj, s_prbf);        
    }
}


WM_HWIN GUI_Get_PROGBAR()
{
    switch(g_sys_ctrl.guiState )
    {
    case  GUI_PLC_MSG_TEST:
        return STM_Get_PROGBAR();
        break;
    case  GUI_PLC_MSG_READ:
        return RMD_Get_PROGBAR();
        break;
    case  GUI_PLC_MSG_MEMORY:
        return GUI_Get_FD_Item();
        break;
    }
    
    return WM_HWIN_NULL;
    
}

void GUI_FailRecvProc(void)
{
    if((g_sys_ctrl.guiState == GUI_PLC_MSG_READ)&&(g_gui_para.cmdType == PLC_CMD_TYPE_COMMON))
    {
        RMD_ReadErr();
    }

}


void GUI_ClearData(void)
{
    if((g_sys_ctrl.guiState == GUI_PLC_MSG_READ)&&(g_gui_para.cmdType == PLC_CMD_TYPE_COMMON))
    {
        RMD_ClearData();
    }
    if((g_sys_ctrl.guiState == GUI_PLC_MSG_TEST)&&(g_gui_para.cmdType == PLC_CMD_TYPE_COMMON))
    {
        CPT_ClearData();
    }
}

#if (EWARM_OPTIMIZATION_EN > 0u)
#pragma optimize = low
#endif
void GUI_Msg_Proc()
{
    WM_HWIN hItem;

    if(g_proto_para.sendStatus == PLC_MSG_SENDING)    
    {        
        GUI_Msg_Upload(ON);
        
        GUI_print_send_buf();
        
        return;
       
    }

    if((g_proto_para.result == PLC_RES_FAIL) || (g_proto_para.result == PLC_RES_TIMEOUT))
    {
        g_proto_para.result = PLC_RES_NONE;
        GUI_FailRecvProc();
        ERR_NOTE(g_hWin_menu, 10);
        hItem = GUI_Get_PROGBAR();
        PROGBAR_SetBarColor(hItem, 0, GUI_RED);
        return;
    }

    if(g_proto_para.result == PLC_RES_ERROR_FRAME)
    {
        GUI_Msg_Download(ON);
        
        GUI_print_recv_buf();

        hItem = GUI_Get_PROGBAR();
        if(hItem != WM_HWIN_NULL)
        {
            PROGBAR_SetBarColor(hItem, 0, GUI_RED);
            g_sys_ctrl.testProgBarVal = 100;
            PROGBAR_SetValue(hItem, g_sys_ctrl.testProgBarVal);              
        }
           
        //g_sys_ctrl.guiState = GUI_PLC_MSG_IDLE;
        g_proto_para.sendStatus = PLC_MSG_IDLE; 
        g_proto_para.result = PLC_RES_NONE;
        return;
    }

    if((g_proto_para.sendStatus == PLC_MSG_RECEIVED) )
    {
        if( g_proto_para.result == PLC_RES_SUCC )
        {
            GUI_Msg_Download(ON);
            
            GUI_ClearData();
            OSTimeDlyHMSM(0, 0, 0, 20);
            GUI_print_recv_buf();

            hItem = GUI_Get_PROGBAR();
            if(hItem != WM_HWIN_NULL)
            {
                g_sys_ctrl.testProgBarVal = 100;
                PROGBAR_SetValue(hItem, g_sys_ctrl.testProgBarVal);              
            }
        }        
        //g_sys_ctrl.guiState = GUI_PLC_MSG_IDLE;
        g_proto_para.sendStatus = PLC_MSG_IDLE;
        g_proto_para.result = PLC_RES_NONE;
    }
    
}


GUI_PARA g_gui_para;      //参数包


//MBOX_SEND_INFO g_mail_box_info; //邮箱发送的消息



/************************************************************
*
*                 把数值转化为字符串
*
************************************************************/
char *int_to_char(int src,char *pBuff,int radix) 
{  
	//索引
	char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"; 
	unsigned unum; //中间变量
	int i=0,j,k; 
	if(radix==10&&src<0) //负数  
	{ 
		unum=(unsigned)-src; 
		pBuff[i++]='-'; 
	} 
	else unum=(unsigned)src;
    
	do  
	{ 
		pBuff[i++]=index[unum%(unsigned)radix]; 
		unum/=radix; 
	}while(unum); 
	pBuff[i]='\0'; 
    
	if(pBuff[0]=='-') 
        k=1;
	else 
        k=0; 
	char temp; 
	for(j=k;j<=(i-k-1)/2.0;j++) 
	{ 
		temp=pBuff[j]; 
		pBuff[j]=pBuff[i-j-1]; 
		pBuff[i-j-1]=temp; 
	} 
	return pBuff; 
} 






//初始化速率设置的EDIT
void PUB_InitFreq(WM_MESSAGE *pMsg,int widgetID)
{
   WM_HWIN hItem; 
   hItem = WM_GetDialogItem(pMsg->hWin, widgetID);
   switch(g_rom_para.preamble)
   {
       case 0: /*第一次初始化的时候或者flash中没有内容,默认为270III代*/
           EDIT_SetText(hItem, pTextPreamble[0]);
           g_rom_para.freqSel = PLC_270_III;
           g_rom_para.bpsSpeed = PLC_BPS_MIDDLE;
           break;
       case 0xFC:
           EDIT_SetText(hItem, pTextPreamble[0]);
           break;
       case 0xFA:
           EDIT_SetText(hItem, pTextPreamble[1]);
           break;
       case 0xFB:
           EDIT_SetText(hItem, pTextPreamble[2]);
           break;
       case 0xFD:
           EDIT_SetText(hItem, pTextPreamble[3]);
           break;
       case 0xFF:
           EDIT_SetText(hItem, pTextPreamble[4]);
           break;
       case 0xFE:
           switch(g_rom_para.freqSel)
           {
               
               case PLC_270_III:
                   EDIT_SetText(hItem, pTextSpeed[PLC_270_III]);
                   break;
               case PLC_270_III_5:
                   EDIT_SetText(hItem, pTextSpeed[PLC_270_III_5]);
                   break;
               case PLC_270_II:
                   EDIT_SetText(hItem, pTextSpeed[PLC_270_II]);
                   break;
               case PLC_421_50BPS:
                   EDIT_SetText(hItem, pTextSpeed[PLC_421_50BPS]);
                   break;
               case PLC_421_100BPS:
                   EDIT_SetText(hItem, pTextSpeed[PLC_421_100BPS]);
                   break;
               case PLC_421_600BPS:
                   EDIT_SetText(hItem, pTextSpeed[PLC_421_600BPS]);
                   break;
               case PLC_421_1200BPS:
                   EDIT_SetText(hItem, pTextSpeed[PLC_421_1200BPS]);
                   break;
           }
           break;
       default:
           break;
   }
}

extern GUI_CONST_STORAGE GUI_BITMAP bmlogo_1;

void GUI_StartSys(void)
{
    //GUI_Init();
    GUI_SetBkColor(GUI_WHITE);
    GUI_SetFont(&GUI_Fontbold18);
    GUI_UC_SetEncodeUTF8();
    GUI_SetColor(GUI_DARKBLUE);
    GUI_Clear();
    WM_SetFocus(WM_HBKWIN);
    
    GUI_DrawBitmap(&bmlogo_1, 20, 100);
    GUI_DispStringAt(CmpName, 5, 228);
    //OSTimeDlyHMSM(0, 0, 0, 300);
    OSTimeDlyHMSM(0, 0, 3, 0);
}


void ButtonBlink(WM_MESSAGE * pMsg,int Id)
{
    
    WM_HWIN hItem;
    hItem=WM_GetDialogItem(pMsg->hWin,Id);
    BUTTON_SetBkColor(hItem,0,GUI_GREEN);
    GUI_Delay(12);
    BUTTON_SetBkColor(hItem,0,0xC0C0C0);
}























