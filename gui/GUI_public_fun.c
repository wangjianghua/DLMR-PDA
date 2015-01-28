#include "includes.h"
#include "WIDGET.h"
#include "DIALOG.h"

u8 s_prbf[512];


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

u32 GUI_GetMeterAddr(u8 * dbuf)
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
        g_send_para_pkg.dstAddr[5-i] = rmd_ch<<4;
        
        if(((rmd_ch = GUI_char2hex(dbuf[(i<<1)+1])) == 0xff))
        {
            //error proc
            return DEV_ERROR;
        }
        
        g_send_para_pkg.dstAddr[5-i] |= rmd_ch;
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
        *strbuf++ = GUI_Hex2Char( dataflag[i]>>4);
        *strbuf++ = GUI_Hex2Char( dataflag[i]&0x0f);
    }

    *strbuf = '\0';
}
u32 GUI_GetStrDataFlag(u8 * dbuf, u32 pro_ver)
{
    u8 rmd_ch;
    u8 i;
    u8 len;

    if(pro_ver == DL_T_07)
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
        g_send_para_pkg.dataFlag[len-1-i] = rmd_ch<<4;
        
        if(((rmd_ch = GUI_char2hex(dbuf[(i<<1)+1])) == 0xff))
        {
            //error proc
            return DEV_ERROR;
        }
        
        g_send_para_pkg.dataFlag[len-1-i] |= rmd_ch;
    }

    return DEV_OK;
}

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
    if(g_plc_prm.recv_len && (g_plc_prm.recv_len < 80))
    {
        send_ptr = s_prbf;
        *send_ptr++ = 'R';
        *send_ptr++ = ':';
        for(i = 0; i < g_plc_prm.recv_len; i++)
        {
            *send_ptr++ = GUI_Hex2Char( g_plc_prm.recv_buf[i]>>4);
            *send_ptr++ = GUI_Hex2Char( g_plc_prm.recv_buf[i]&0x0f);
            *send_ptr++ = ' ';
        }
        *send_ptr++ = '\n';
        *send_ptr++ = 0;

        if((g_sys_control.guiState == GUI_PLC_MSG_TEST)
            ||(g_sys_control.guiState == GUI_PLC_MSG_READ) )
        {
            hObj = STD_Get_MultiEdit();
            MULTIEDIT_AddText(hObj, s_prbf); 
            if(g_sys_control.guiState == GUI_PLC_MSG_READ)
            {
                RMD_proc_resp_data();
            }

            if(g_sys_control.guiState == GUI_PLC_MSG_TEST)
            {
                STM_proc_resp_data();
            }
        }
        else if(g_sys_control.guiState == GUI_PLC_MSG_LISTING)
        {
            hObj = MND_Get_MultiEdit();
            MULTIEDIT_AddText(hObj, s_prbf); 
        }
    }
}

void GUI_print_send_buf()
{
    MULTIEDIT_HANDLE hObj;
    u16 i;
    u8 *send_ptr;
    
    if(g_plc_prm.send_len && (g_plc_prm.send_len < 80))
    {        
        send_ptr = s_prbf;
        *send_ptr++ = 'S';
        *send_ptr++ = ':';
        for(i = 0; i < g_plc_prm.send_len; i++)
        {
            *send_ptr++ = GUI_Hex2Char( g_plc_prm.send_buf[i]>>4);
            *send_ptr++ = GUI_Hex2Char( g_plc_prm.send_buf[i]&0x0f);
            *send_ptr++ = ' ';
        }
        *send_ptr++ = '\n';
        *send_ptr++ = 0;
        hObj = STD_Get_MultiEdit();
        MULTIEDIT_AddText(hObj, s_prbf);        
    }
}


WM_HWIN GUI_Get_PROGBAR()
{
    switch(g_sys_control.guiState )
    {
    case  GUI_PLC_MSG_TEST:
        return STM_Get_PROGBAR();
        break;
    case  GUI_PLC_MSG_READ:
        return RMD_Get_PROGBAR();
        break;
    case  GUI_PLC_MSG_MEMORY:
        return MMD_Get_PROGBAR();
        break;

    }

    return WM_HWIN_NULL;
    
}

void GUI_Msg_Proc()
{
    WM_HWIN hItem;

    if(g_plc_prm.sendStatus == PLC_MSG_SENDING)    
    {        
        //g_plc_prm.send_buf
        //Data_Upload_Green();//数据发送图标变色
        //hItem=TSK_Get_Upload_Text();
        //TEXT_SetTextColor(hItem,GUI_GREEN);
        //GUI_Delay(15);
        Data_Upload_Green(1);
        GUI_print_send_buf();
       
    }

    if((g_plc_prm.sendStatus == PLC_MSG_RECEIVED) )
    {
        if( g_plc_prm.result == PLC_RES_SUCC )
        {
            //Data_Download_Yellow();//数据接收图标变色
            //hItem=TSK_Get_Download_Text();
            //TEXT_SetTextColor(hItem,GUI_YELLOW);
            //GUI_Delay(15);
            Data_Download_Yellow(1);
            GUI_print_recv_buf();

            hItem = GUI_Get_PROGBAR();
            if(hItem != WM_HWIN_NULL)
            {
                g_sys_control.testProgBarVal = 100;
                PROGBAR_SetValue(hItem, g_sys_control.testProgBarVal);              
            }
        }        
        //g_sys_control.guiState = GUI_PLC_MSG_IDLE;
        g_plc_prm.sendStatus = PLC_MSG_IDLE;
    }



   
    
}


SEND_PARA_PKG g_send_para_pkg;      //参数包


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
    
	if(pBuff[0]=='-') k=1; //负数
	else k=0; 
	char temp; 
	for(j=k;j<=(i-k-1)/2.0;j++) 
	{ 
		temp=pBuff[j]; 
		pBuff[j]=pBuff[i-j-1]; 
		pBuff[i-j-1]=temp; 
	} 
	return pBuff; 
} 



//启用小工具
void Enable_Widget(WM_MESSAGE * pMsg,int Widget_Id)
{

    WM_HWIN hItem;
    hItem=WM_GetDialogItem(pMsg->hWin,Widget_Id);
    WM_EnableWindow(hItem);
}

//禁用小工具
void Disable_Widget(WM_MESSAGE * pMsg,int Widget_Id)
{
    WM_HWIN hItem;
    hItem=WM_GetDialogItem(pMsg->hWin,Widget_Id);
    WM_DisableWindow(hItem);
}

//聚焦小工具
void Focus_Widget(WM_MESSAGE * pMsg,int Widget_Id) 
{
    WM_HWIN hItem;
    hItem=WM_GetDialogItem(pMsg->hWin,Widget_Id);
    WM_SetFocus(hItem);
}

//小工具不能被选中
void Not_Focus(WM_MESSAGE *pMsg,int Widget_Id)
{
    WM_HWIN hItem;
    hItem=WM_GetDialogItem(pMsg->hWin,Widget_Id); 
    WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);
}


void ButtonBlink(WM_MESSAGE * pMsg,int Id)
{
    
    WM_HWIN hItem;
    hItem=WM_GetDialogItem(pMsg->hWin,Id);
    BUTTON_SetBkColor(hItem,0,GUI_GREEN);
    GUI_Delay(12);
    BUTTON_SetBkColor(hItem,0,0xC0C0C0);
}


//存储参数

void para_store_edit(WM_MESSAGE * pMsg,int Widget_Id,char *text,int num)
{
    WM_HWIN hItem;
    hItem = WM_GetDialogItem(pMsg->hWin, Widget_Id);
    EDIT_GetText(hItem, text, 10);//sizeof不行，不会对形参分配空间
    //g_sys_register_para.para_data[num] = atoi(text);  
    switch(num)
    {
    case 0:
        g_sys_register_para.meterPassword = atoi(text);
        break;
    case 1:
        g_sys_register_para.recvDelayTime = atoi(text);
        break;
    case 2:
        g_sys_register_para.execInterval = atoi(text);
        break;
    }
}






















