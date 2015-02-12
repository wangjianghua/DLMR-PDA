#include "DIALOG.h"
#include "includes.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_FRAMEWIN_0  (GUI_ID_USER + 0x00)
#define ID_LISTBOX_0 (GUI_ID_USER + 0x02)
#define ID_BUTTON_0  (GUI_ID_USER + 0x03)
#define ID_BUTTON_1  (GUI_ID_USER + 0x04)
#define ID_TEXT_0    (GUI_ID_USER + 0x05)
#define ID_EDIT_0    (GUI_ID_USER + 0x06)


//电力载波
static const char WaveCarrier[]="\xe7\x94\xb5\xe5\x8a\x9b\xe8\xbd\xbd\xe6\xb3\xa2";

//红外
static const char Infrared[]="\xe7\xba\xa2\xe5\xa4\x96";

//读设备地址
static const char GetAddr[]="\xe8\xaf\xbb\xe8\xae\xbe\xe5\xa4\x87\xe5\x9c\xb0\xe5\x9d\x80-13H";

//读数据
static const char Readdata[]="\xe8\xaf\xbb\xe6\x95\xb0\xe6\x8d\xae-11H";

//写数据
static const char WriteData[]="\xe5\x86\x99\xe6\x95\xb0\xe6\x8d\xae-14H";

//当前正向有功电能
static const char Positive[]="\xe5\xbd\x93\xe5\x89\x8d\xe6\xad\xa3\xe5\x90\x91\xe6\x9c\x89\xe5\x8a\x9f\xe7\x94\xb5\xe8\x83\xbd";


//当前反向有功电能
static const char Negative[]="\xe5\xbd\x93\xe5\x89\x8d\xe5\x8f\x8d\xe5\x90\x91\xe6\x9c\x89\xe5\x8a\x9f\xe7\x94\xb5\xe8\x83\xbd";


//日冻结正向有功电能
static const char DayPositive[]="\xe6\x97\xa5\xe5\x86\xbb\xe7\xbb\x93\xe6\xad\xa3\xe5\x90\x91\xe6\x9c\x89\xe5\x8a\x9f\xe7\x94\xb5\xe8\x83\xbd";


//日冻结反向有功电能
static const char DayNegative[]="\xe6\x97\xa5\xe5\x86\xbb\xe7\xbb\x93\xe5\x8f\x8d\xe5\x90\x91\xe6\x9c\x89\xe5\x8a\x9f\xe7\x94\xb5\xe8\x83\xbd";


//退出
static const char Quit[]="\xe9\x80\x80\xe5\x87\xba";
//保存
static const char Save[]="\xe4\xbf\x9d\xe5\xad\x98";

//接收数据延时
static const char RevDataDelay[]="\xe6\x8e\xa5\xe6\x94\xb6\xe6\x95\xb0\xe6\x8d\xae\xe5\xbb\xb6\xe6\x97\xb6";
//执行间隔时间
static const char BetweenAct[]="\xe6\x89\xa7\xe8\xa1\x8c\xe9\x97\xb4\xe9\x9a\x94\xe6\x97\xb6\xe9\x97\xb4";

//规约
static const char Protocal[]="\xe8\xa7\x84  \xe7\xba\xa6:";
//通道
static const char Channel[]="\xe9\x80\x9a  \xe9\x81\x93:";
//波特率
static const char BaudRate[]="\xe6\xb3\xa2\xe7\x89\xb9\xe7\x8e\x87:";
//前导符
static const char Preamble[]="\xe5\x89\x8d\xe5\xaf\xbc\xe7\xac\xa6:";
//停止位
static const char StopBit[]="\xe5\x81\x9c\xe6\xad\xa2\xe4\xbd\x8d:";
//个数
static const char Number[]="\xe4\xb8\xaa  \xe6\x95\xb0:";
//密码
static const char PassWord[]="\xe5\xaf\x86  \xe7\xa0\x81:";

//屏幕超时
static const char ScrTimeout[]="\xe5\xb1\x8f\xe5\xb9\x95\xe8\xb6\x85\xe6\x97\xb6(\xe7\xa7\x92)";

//1-目标地址
static const char TarAddr[]="\xe7\x9b\xae\xe6\xa0\x87\xe5\x9c\xb0\xe5\x9d\x80:";

//2-控制字
static const char CtlCode[]="\xe6\x8e\xa7 \xe5\x88\xb6 \xe5\xad\x97:";

//3-长度
static const char Length[]="\xe9\x95\xbf    \xe5\xba\xa6:";

//4-数据标识
static const char DataSign[]="\xe6\x95\xb0\xe6\x8d\xae\xe6\xa0\x87\xe8\xaf\x86:";

//5-数据域
static const char DataField[]="\xe6\x95\xb0    \xe6\x8d\xae:";

//电表表号
static const char MeterNum[]="\xe7\x94\xb5\xe8\xa1\xa8\xe8\xa1\xa8\xe5\x8f\xb7";


//抄表项目
static const char ReadMeterSel[]="\xe6\x8a\x84\xe8\xa1\xa8\xe9\xa1\xb9\xe7\x9b\xae";

static const char ConfirmUpdata[]="\xe8\xaf\xb7\xe8\xbe\x93\xe5\x85\xa5\xe5\xaf\x86\xe7\xa0\x81";
//速率
static const char Speed[]="\xe9\x80\x9f    \xe7\x8e\x87";


static const char Year[]  ="\xe5\xb9\xb4:";
static const char Month[] ="\xe6\x9c\x88:";
static const char Day[]   ="\xe6\x97\xa5:";
static const char Hour[]  ="\xe6\x97\xb6:";
static const char Minite[]="\xe5\x88\x86:";
static const char Second[]="\xe7\xa7\x92:";

const char *pTextSpeed[]={

    "\xe8\xae\xbe\xe4\xb8\xba 270 III \xe4\xbb\xa3",
    "\xe8\xae\xbe\xe4\xb8\xba 270 3.5 \xe4\xbb\xa3",
    "\xe8\xae\xbe\xe4\xb8\xba 270 II \xe4\xbb\xa3",
    "\xe8\xae\xbe\xe4\xb8\xba 421 50BPS",
    "\xe8\xae\xbe\xe4\xb8\xba 421 50BPS",
    "\xe8\xae\xbe\xe4\xb8\xba 421 100BPS",
    "\xe8\xae\xbe\xe4\xb8\xba 421 600BPS",
    "\xe8\xae\xbe\xe4\xb8\xba 421 1200BPS",
    
};

static const GUI_WIDGET_CREATE_INFO _aListBoxCreate[] = {
  { FRAMEWIN_CreateIndirect,  "ListBox",  ID_FRAMEWIN_0,  20,  40,  200, 200,  0, 0x0, 0 },
  { LISTBOX_CreateIndirect,   "Listbox",  ID_LISTBOX_0,   5,   7,   182, 120,  0, 0x0, 0 },
  { BUTTON_CreateIndirect,    Save,       ID_BUTTON_0,    5,   140, 55,  25,   0, 0x0, 0 },
  { BUTTON_CreateIndirect,    Quit,       ID_BUTTON_1,    138, 140, 55,  25,   0, 0x0, 0 },
};

static const GUI_WIDGET_CREATE_INFO _aEditCreate[] = {
  { FRAMEWIN_CreateIndirect, "Edit",  ID_FRAMEWIN_0, 20,  60, 200, 160, 0, 0x0,  0 },
  { BUTTON_CreateIndirect,   Save,    ID_BUTTON_0,   5,   82, 55,  25,  0, 0x0,  0 },
  { BUTTON_CreateIndirect,   Quit,    ID_BUTTON_1,   138, 82, 55,  25,  0, 0x0,  0 },
  { EDIT_CreateIndirect,     "Edit",  ID_EDIT_0,     14,  38, 165, 25,  0, 0x64, 0 },
  //{ TEXT_CreateIndirect,   "Text",    ID_TEXT_0,   14,  16, 165, 20,  0, 0x0,  0 },

};

const char *Listbox_FrameTitle[]={
    Protocal,
    Channel,
    BaudRate,
    Preamble,
    StopBit,
    CtlCode,
    ReadMeterSel,
    Speed,
};

const char *Edit_FrameTitle[]={
    ScrTimeout,
    PassWord,
    RevDataDelay,
    BetweenAct,
    TarAddr,
    DataSign,
    Length,
    DataField,
    MeterNum,
    Year,
    Month,
    Day,
    Hour,
    Minite,
    Second,
    ConfirmUpdata
};

const u8 c_645ctrlDef[2][PLC_CTRL_MAX_NUM] = 
{ 
    //97规约
    {0x05, 0x01, 0x04,  4,5,6,7,8,9,10,11,12,13,14,15,16},
    //07规约
    {0x13, 0X11, 0X14,  4,5,6,7,8,9,10,11,12,13,14,15,16}
};


const u32 c_645DidoDef[2][PLC_CTRL_MAX_NUM] = 
{ 
    //97规约
    {0x901f,    0x902f,     0x9410,     0x9420,5,6,7,8,9,10,11,12,13,14,15,16},
    //07规约
    {0x0001ff00,0X0002ff00, 0X0001ff01, 0X0002ff01,4,5,6,7,8,9,10,11,12,13,14,15,}
};



static void Select_Focus(void)
{
    
    if(g_hWin_para>0)
    {
        WM_SetFocus(g_hWin_para);
    }
    
    if(g_hWin_std>0)
    {
        WM_SetFocus(g_hWin_std);
    }
    
    if(g_hWin_ReadMeter>0)
    {
        WM_SetFocus(g_hWin_ReadMeter);
    }

    if(g_hWin_TimeSet>0)
    {
        WM_SetFocus(g_hWin_TimeSet);
    }
    if(g_hWin_about>0)
    {
        WM_SetFocus(g_hWin_about);
    }

}


#if 1

void PUB_Fill_Zero(u8 *tempbuf)
{
    int len;
    int i;
    //u8  zerobuf[13];
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

#endif 



static void Select_Input_Edit(int  EditNum)
{
    WM_HWIN hItem;
    u8 tmpTextBuf[32];
    
    hItem=WM_GetDialogItem(g_hWin_Input,ID_EDIT_0);
    switch(EditNum)
    {
        case EDIT_SCR_OUTTIME:
            EDIT_GetText(hItem,tmpTextBuf,3);
            g_sys_register_para.scrTimeout=atoi(tmpTextBuf);
            hItem=CPS_Set_ScrOutTime();
            
            break;
            
        case EDIT_PASSWORD:
            EDIT_GetText(hItem,tmpTextBuf,7);
            hItem=CPS_Set_Pwd();
            break;
            
        case EDIT_RECV_DELAY:
            EDIT_GetText(hItem,tmpTextBuf,5);
            hItem=CPS_Set_RevDelay();
            break;

        case EDIT_BETWEEN_ACT:
            EDIT_GetText(hItem,tmpTextBuf,5);
            hItem=CPS_Set_BetweenAct();
            break;

        case EDIT_ADDR:
            EDIT_GetText(hItem,tmpTextBuf,13);
            PUB_Fill_Zero(tmpTextBuf);
            GUI_GetMeterAddr(tmpTextBuf, g_sys_control.recentMeterAddr);
            hItem=CPT_Set_Addr();
            break;

        case EDIT_DATA_FLAG:
            EDIT_GetText(hItem,tmpTextBuf,9);
            hItem=CPT_Set_DataFlag();
            GUI_GetStrDataFlag(tmpTextBuf, g_sys_register_para.plcProtocol);
            break;

        case EDIT_LENGTH:
            EDIT_GetText(hItem,tmpTextBuf,2);
            hItem=CPT_Set_Length();
            break;

        case EDIT_DATA:
            EDIT_GetText(hItem,tmpTextBuf,13);
            hItem=CPT_Set_Data();
            break;

        case EDIT_METER_NUM:
            EDIT_GetText(hItem,tmpTextBuf,13);
            PUB_Fill_Zero(tmpTextBuf);
            GUI_GetMeterAddr(tmpTextBuf, g_sys_control.recentMeterAddr);
            hItem=RMD_Get_MeterNum();
            break;
            
        case EDIT_YEAR:
            EDIT_GetText(hItem,tmpTextBuf,3);
            hItem=TMS_Get_Year();
            break;
        case EDIT_MONTH:
            EDIT_GetText(hItem,tmpTextBuf,3);
            hItem=TMS_Get_Month();
            break;
        case EDIT_DAY:
            EDIT_GetText(hItem,tmpTextBuf,3);
            hItem=TMS_Get_Day();
            break;
        case EDIT_HOUR:
            EDIT_GetText(hItem,tmpTextBuf,3);
            hItem=TMS_Get_Hour();
            break;
        case EDIT_MIN:
            EDIT_GetText(hItem,tmpTextBuf,3);
            hItem=TMS_Get_Min();
            break;
        case EDIT_SEC:
            EDIT_GetText(hItem,tmpTextBuf,3);
            hItem=TMS_Get_Sec();
            break;
        case EDIT_PWD_CFM:
            EDIT_GetText(hItem,tmpTextBuf,7);
            break;

            
        default:
            break;
    }
    EDIT_SetText(hItem,tmpTextBuf);
}


static void _init_edit(WM_MESSAGE *pMsg,int EditNum)
{

    WM_HWIN hItem;
    u8 tmpTextBuf[32];
    u8 addr_len;
    u8 tmpZero[13];
    int i;
    switch(EditNum)
    {
        case EDIT_SCR_OUTTIME:
            hItem=CPS_Set_ScrOutTime();
            EDIT_GetText(hItem,tmpTextBuf,5);
            //hItem=CPS_Set_ScrOutTime();
            break;
            
        case EDIT_PASSWORD:
            hItem=CPS_Set_Pwd();
            EDIT_GetText(hItem,tmpTextBuf,7);
            break;
            
        case EDIT_RECV_DELAY:
            hItem=CPS_Set_RevDelay();
            EDIT_GetText(hItem,tmpTextBuf,5);
            break;

        case EDIT_BETWEEN_ACT:
            hItem=CPS_Set_BetweenAct();
            EDIT_GetText(hItem,tmpTextBuf,5);
            break;

        case EDIT_ADDR:
            hItem=CPT_Set_Addr();
            EDIT_GetText(hItem,tmpTextBuf,13);            
            break;

        case EDIT_DATA_FLAG:
            hItem=CPT_Set_DataFlag();
            EDIT_GetText(hItem,tmpTextBuf,9);
            break;

        case EDIT_LENGTH:
            hItem=CPT_Set_Length();
            EDIT_GetText(hItem,tmpTextBuf,2);
            break;

        case EDIT_DATA:
            hItem=CPT_Set_Data();
            EDIT_GetText(hItem,tmpTextBuf,13);
            break;

        case EDIT_METER_NUM:
            hItem=RMD_Get_MeterNum();
            EDIT_GetText(hItem,tmpTextBuf,13);
            break;

        case EDIT_YEAR:
            hItem=TMS_Get_Year();
            EDIT_GetText(hItem,tmpTextBuf,3);
            break;
            
        case EDIT_MONTH:
            hItem=TMS_Get_Month();
            EDIT_GetText(hItem,tmpTextBuf,3);
            break;
            
        case EDIT_DAY:
            hItem=TMS_Get_Day();
            EDIT_GetText(hItem,tmpTextBuf,3);
            break;
            
        case EDIT_HOUR:
            hItem=TMS_Get_Hour();
            EDIT_GetText(hItem,tmpTextBuf,3);
            break;
            
        case EDIT_MIN:
            hItem=TMS_Get_Min();
            EDIT_GetText(hItem,tmpTextBuf,3);
            break;
            
        case EDIT_SEC:
            hItem=TMS_Get_Sec();
            EDIT_GetText(hItem,tmpTextBuf,3);
            break;
            
        default:
            break;
     }
     FRAMEWIN_SetText(pMsg->hWin,Edit_FrameTitle[EditNum]);
     hItem=WM_GetDialogItem(pMsg->hWin,ID_EDIT_0);
     EDIT_SetText(hItem,tmpTextBuf);
}



/*********************************************************************
*
*       Static code
*
**********************************************************************
*/


void Button_Color_Set(WM_MESSAGE *pMsg)
{
    WM_HWIN hItem;
    hItem = WM_GetDialogItem(pMsg->hWin,ID_BUTTON_0);
    BUTTON_SetBkColor(hItem,0,GUI_GREEN);
    WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);
    
    hItem = WM_GetDialogItem(pMsg->hWin,ID_BUTTON_1);
    BUTTON_SetBkColor(hItem,0,GUI_YELLOW);
    WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);
}



static void _cbEditDlg(WM_MESSAGE *pMsg)
{
    WM_HWIN hItem;
    int     NCode;
    int     Id;
  
    switch (pMsg->MsgId) 
    {
      case WM_INIT_DIALOG:
        FRAMEWIN_SetTitleHeight(pMsg->hWin,25);
        Button_Color_Set(pMsg);
        _init_edit(pMsg,g_sys_control.selectWidget);
        break;
        
      case WM_KEY:
        if((((WM_KEY_INFO *)(pMsg->Data.p))->PressedCnt)==0)
        {
            switch(((WM_KEY_INFO *)(pMsg->Data.p))->Key)
            {
              case GUI_KEY_YELLOW:
                  WM_DeleteWindow(g_hWin_Input);
                  Select_Focus();
                  break;
              case GUI_KEY_GREEN:
                  Select_Input_Edit(g_sys_control.selectWidget);
                  WM_DeleteWindow(g_hWin_Input);
                  Select_Focus();
                  break;
            }
        }
        break;
      
      default:
        WM_DefaultProc(pMsg);
        break;
      }
}


static void _Init_ListBox(WM_MESSAGE *pMsg, int ListBoxNum)
{
    
    //u8 tmpTextBuf[32];
    u32 Sel_Num;
    WM_HWIN hItem;
    int i;
    
    hItem=WM_GetDialogItem(pMsg->hWin,ID_LISTBOX_0);
    
    switch(ListBoxNum)
    {
        case LISTBOX_PROTOCOL:
            LISTBOX_AddString(hItem, "DL-T-07");
            LISTBOX_AddString(hItem, "DL-T-97");
            switch(g_sys_register_para.plcProtocol)
            {
                case DL_T_07:
                    LISTBOX_SetSel(hItem,0);
                    break;
                case DL_T_97:
                    LISTBOX_SetSel(hItem,1);
                    break;
            }
            break;
            
        case LISTBOX_CHANNEL:
            //LISTBOX_AddString(hItem, "485");
            LISTBOX_AddString(hItem, WaveCarrier);
            //LISTBOX_AddString(hItem, Infrared);
            break;


        case LISTBOX_BAUDRATE:
            LISTBOX_AddString(hItem, "1200");
            LISTBOX_AddString(hItem, "1500");
            LISTBOX_AddString(hItem, "2400");
            LISTBOX_AddString(hItem, "4800");
            LISTBOX_AddString(hItem, "9600");
            switch(g_sys_register_para.baudrate)
            {
                case 1200:
                    LISTBOX_SetSel(hItem,0);
                    break;
                case 1500:
                    LISTBOX_SetSel(hItem,1);
                    break;
                case 2400:
                    LISTBOX_SetSel(hItem,2);
                    break;
                case 4800:
                    LISTBOX_SetSel(hItem,3);
                    break;
                case 9600:
                    LISTBOX_SetSel(hItem,4);
                    break;
            }
            break;

        case LISTBOX_PREAM:
#if 0
            LISTBOX_AddString(hItem, "FA");
            LISTBOX_AddString(hItem, "FB");
            LISTBOX_AddString(hItem, "FC");
            LISTBOX_AddString(hItem, "FD");
            LISTBOX_AddString(hItem, "FE");
            LISTBOX_AddString(hItem, "FF");
            switch(g_sys_register_para.preamble)
            {
                case 0xFA:
                    LISTBOX_SetSel(hItem,0);
                    break;
                case 0xFB:
                    LISTBOX_SetSel(hItem,1);
                    break;
                case 0xFC:
                    LISTBOX_SetSel(hItem,2);
                    break;
                case 0xFD:
                    LISTBOX_SetSel(hItem,3);
                    break;
                case 0xFE:
                    LISTBOX_SetSel(hItem,4);
                    break;
                case 0xFF:
                    LISTBOX_SetSel(hItem,5);
                    break;
            }
#endif
            break;

        case LISTBOX_STOPBIT:
            LISTBOX_AddString(hItem, "1");
            LISTBOX_AddString(hItem, "2");
            switch(g_sys_register_para.stopbit)
            {
                case ONE_STOPBIT:
                    LISTBOX_SetSel(hItem, 0);
                    break;
                case TWO_STOPBIT:
                    LISTBOX_SetSel(hItem, 1);
                    break;     
            }
            break;

        
        case LISTBOX_CTLCODE:  /*控制字*/
            LISTBOX_AddString(hItem, GetAddr);
            LISTBOX_AddString(hItem, Readdata);
            LISTBOX_AddString(hItem, WriteData);
            break;

        case LISTBOX_READ_SEL:
           LISTBOX_AddString(hItem, Positive);
           LISTBOX_AddString(hItem, Negative);
           LISTBOX_AddString(hItem, DayPositive);
           LISTBOX_AddString(hItem, DayNegative);
           break;

        case LISTBOX_SPEED:
            for(i = 0;i < 8;i++)
            {
                LISTBOX_AddString(hItem,pTextSpeed[i]);
            }
            break;
            
        default:
           break;
    }
    FRAMEWIN_SetText(pMsg->hWin,Listbox_FrameTitle[ListBoxNum]);

}



static void Select_ListBox_Row(int  WidgetNum)
{

    WM_HWIN hItem;
    WM_HWIN hWin;
    int  SelNum;
    hItem=WM_GetDialogItem(g_hWin_Input,ID_LISTBOX_0);
    SelNum=LISTBOX_GetSel(hItem);
    switch(WidgetNum) /*WidgetNum是为了把不同的页面的不同Edit工具句柄传回来*/
    {
        case LISTBOX_PROTOCOL:
            switch(SelNum) 
            {
                case 0:
                    g_sys_register_para.plcProtocol = DL_T_07;
                    hWin=CPS_Set_Proto();
                    EDIT_SetText(hWin,"DL-T-07");
                    break;
                case 1:
                    g_sys_register_para.plcProtocol = DL_T_97;
                    hWin=CPS_Set_Proto();
                    EDIT_SetText(hWin,"DL-T-97");
                    break;
            }
            break;

        case LISTBOX_CHANNEL:
            switch(SelNum)
            {
                case 1:
                   // g_sys_register_para.channel=CHANNEL_485;
                    //hWin=CPS_Set_Channel();
                    //EDIT_SetText(hWin,"485");
                    break;
                    
                case 0:
                    g_sys_register_para.channel=CHANNEL_WAVE;
                    hWin=CPS_Set_Channel();
                    EDIT_SetText(hWin,WaveCarrier);
                    break;
                case 2:
                    //g_sys_register_para.channel=CHANNEL_INFRD;
                    //hWin=CPS_Set_Channel();
                    //EDIT_SetText(hWin,Infrared);
                    break;
            }
            break;
            
        case LISTBOX_BAUDRATE:
            switch(SelNum)
            {
                case 0:
                    g_sys_register_para.baudrate=1200;
                    hWin=CPS_Set_BaudRate();
                    EDIT_SetText(hWin,"1200");
                    break;
                case 1:
                    g_sys_register_para.baudrate=1500;
                    hWin=CPS_Set_BaudRate();
                    EDIT_SetText(hWin,"1500");
                    break;
                case 2:
                    g_sys_register_para.baudrate=2400;
                    hWin=CPS_Set_BaudRate();
                    EDIT_SetText(hWin,"2400");
                    break;
                case 3:
                    g_sys_register_para.baudrate=4800;
                    hWin=CPS_Set_BaudRate();
                    EDIT_SetText(hWin,"4800");
                    break;
                case 4:
                    g_sys_register_para.baudrate=9600;
                    hWin=CPS_Set_BaudRate();
                    EDIT_SetText(hWin,"9600");
                    break;
            }
            break;
            
        case LISTBOX_PREAM:
#if 0
            switch(SelNum)
            {
                case 0:
                    g_sys_register_para.preamble=0xFA;
                    hWin=CPS_Set_Pream();
                    EDIT_SetText(hWin,"FA");
                    break;
                case 1:
                    g_sys_register_para.preamble=0XFB;
                    hWin=CPS_Set_Pream();
                    EDIT_SetText(hWin,"FB");
                    break;
                case 2:
                    g_sys_register_para.preamble=0XFC;
                    hWin=CPS_Set_Pream();
                    EDIT_SetText(hWin,"FC");
                    break;
                case 3:
                    g_sys_register_para.preamble=0XFD;
                    hWin=CPS_Set_Pream();
                    EDIT_SetText(hWin,"FD");
                    break;
                case 4:
                    g_sys_register_para.preamble=0XFE;
                    hWin=CPS_Set_Pream();
                    EDIT_SetText(hWin,"FE");
                    break;
                case 5:
                    g_sys_register_para.preamble=0XFF;
                    hWin=CPS_Set_Pream();
                    EDIT_SetText(hWin,"FF");
                    break;
            }
#endif
            break;

         case LISTBOX_STOPBIT:
            switch(SelNum)
            {
                case 0:
                    g_sys_register_para.stopbit=ONE_STOPBIT;
                    hWin=CPS_Set_StopBit();
                    EDIT_SetText(hWin,"1");
                    break;
                case 1:
                    g_sys_register_para.stopbit=TWO_STOPBIT;
                    hWin=CPS_Set_StopBit();
                    EDIT_SetText(hWin,"2");
                    break;
            }
            break;

//这里还可以再继续优化，先放着
         case LISTBOX_CTLCODE:
            switch(SelNum)
            {
                case 0:
                    
                    if(g_sys_register_para.plcProtocol == DL_T_97)
                    {
                        g_send_para_pkg.ctlCode = c_645ctrlDef[g_sys_register_para.plcProtocol][SelNum]; 
                    }
                    else if(g_sys_register_para.plcProtocol == DL_T_07)
                    {
                        g_send_para_pkg.ctlCode = c_645ctrlDef[g_sys_register_para.plcProtocol][SelNum]; 
                    }
                    hWin=CPT_Set_CtlCode();
                    EDIT_SetText(hWin,GetAddr);
                    break;
                    
                case 1:
                    
                    if(g_sys_register_para.plcProtocol == DL_T_97)
                    {
                        g_send_para_pkg.ctlCode = c_645ctrlDef[g_sys_register_para.plcProtocol][SelNum]; 
                    }
                    else if(g_sys_register_para.plcProtocol == DL_T_07)
                    {
                        g_send_para_pkg.ctlCode = c_645ctrlDef[g_sys_register_para.plcProtocol][SelNum]; 
                    }
                    hWin=CPT_Set_CtlCode();
                    EDIT_SetText(hWin,Readdata);
                    break;
                case 2:
                    
                    if(g_sys_register_para.plcProtocol == DL_T_97)
                    {
                        g_send_para_pkg.ctlCode = c_645ctrlDef[g_sys_register_para.plcProtocol][SelNum]; 
                    }
                    else if(g_sys_register_para.plcProtocol == DL_T_07)
                    {
                        g_send_para_pkg.ctlCode = c_645ctrlDef[g_sys_register_para.plcProtocol][SelNum]; 
                    }
                    hWin=CPT_Set_CtlCode();
                    EDIT_SetText(hWin,WriteData);
                    break;
            }
            break;



//先实现，可以优化
//把edit中的文本放入到一个字符串数组中，传入selnum
        case LISTBOX_READ_SEL:
            switch(SelNum)
            {
                case 0:
                    //g_sys_register_para.stopbit=ONE_STOPBIT;
                    if(g_sys_register_para.plcProtocol==DL_T_07)
                    {        
                        memcpy(g_send_para_pkg.dataFlag,
                            &c_645DidoDef[g_sys_register_para.plcProtocol][SelNum],
                            4);
                        g_send_para_pkg.ctlCode=0x11;
                    }
                    else if(g_sys_register_para.plcProtocol==DL_T_97)
                    {
                        memcpy(g_send_para_pkg.dataFlag,
                            &c_645DidoDef[g_sys_register_para.plcProtocol][SelNum],
                            2);
                        g_send_para_pkg.ctlCode=0x01; 
                
                    }
                    hWin=RMD_Get_ReadSel();
                    EDIT_SetText(hWin,Positive);
                    break;
                case 1:
                    //g_sys_register_para.stopbit=TWO_STOPBIT;
                    if(g_sys_register_para.plcProtocol==DL_T_07)
                    {        
                        memcpy(g_send_para_pkg.dataFlag,
                            &c_645DidoDef[g_sys_register_para.plcProtocol][SelNum],
                            4);
                        
                        g_send_para_pkg.ctlCode=0x11;
                    }
                    else if(g_sys_register_para.plcProtocol==DL_T_97)
                    {
                        memcpy(g_send_para_pkg.dataFlag,
                            &c_645DidoDef[g_sys_register_para.plcProtocol][SelNum],
                            2);
                        g_send_para_pkg.ctlCode=0x01; 
                
                    }
                    hWin=RMD_Get_ReadSel();
                    EDIT_SetText(hWin,Negative);
                    break;
                case 2:
                    //g_sys_register_para.stopbit=TWO_STOPBIT;
                    if(g_sys_register_para.plcProtocol==DL_T_07)
                    {        
                        memcpy(g_send_para_pkg.dataFlag,
                            &c_645DidoDef[g_sys_register_para.plcProtocol][SelNum],
                            4);
                        
                        g_send_para_pkg.ctlCode=0x11;
                    }
                    else if(g_sys_register_para.plcProtocol==DL_T_97)
                    {
                        memcpy(g_send_para_pkg.dataFlag,
                            &c_645DidoDef[g_sys_register_para.plcProtocol][SelNum],
                            2);
                        g_send_para_pkg.ctlCode=0x01; 
                
                    }
                    hWin=RMD_Get_ReadSel();
                    EDIT_SetText(hWin,DayPositive);
                    break;
                case 3:
                    //g_sys_register_para.stopbit=TWO_STOPBIT;
                    if(g_sys_register_para.plcProtocol==DL_T_07)
                    {        
                        memcpy(g_send_para_pkg.dataFlag,
                            &c_645DidoDef[g_sys_register_para.plcProtocol][SelNum],
                            4);
                        
                        g_send_para_pkg.ctlCode=0x11;
                    }
                    else if(g_sys_register_para.plcProtocol==DL_T_97)
                    {
                        memcpy(g_send_para_pkg.dataFlag,
                            &c_645DidoDef[g_sys_register_para.plcProtocol][SelNum],
                            2);
                        g_send_para_pkg.ctlCode=0x01; 
                
                    }
                    hWin=RMD_Get_ReadSel();
                    EDIT_SetText(hWin,DayNegative);
                    break;
            }
            break;

        default:
            break;

      }
}



static void _cbListBoxDlg(WM_MESSAGE *pMsg)
{
    WM_HWIN hItem;
   // int     Id;
    //int     SelNum;

    switch(pMsg->MsgId)
    {
        case WM_INIT_DIALOG:
          GUI_UC_SetEncodeUTF8();
          FRAMEWIN_SetTitleHeight(pMsg->hWin,25);
          //WINDOW_SetBkColor(pMsg->hWin,GUI_WHITE);
          //hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTBOX_0);
          _Init_ListBox(pMsg,g_sys_control.selectWidget);
          Button_Color_Set(pMsg);
          
          break;
          
        case WM_KEY:
          if((((WM_KEY_INFO *)(pMsg->Data.p))->PressedCnt)==0)
          {
              switch(((WM_KEY_INFO *)(pMsg->Data.p))->Key)
              {
                case GUI_KEY_YELLOW:
                    WM_DeleteWindow(g_hWin_Input);
                    //WM_SetFocus(g_hWin_para);
                    Select_Focus();
                    break;
                case GUI_KEY_GREEN:
                    Select_ListBox_Row(g_sys_control.selectWidget);
                    WM_DeleteWindow(g_hWin_Input);
                    Select_Focus();
                    break;

              }
          }
          break;
    
        default:
          WM_DefaultProc(pMsg);
          break;
    }
}


/*********************************************************************
*
*       Public code
*
**********************************************************************/


//listbox对话框

WM_HWIN Create_ListBox_Set(WM_HWIN parentWin);

WM_HWIN Create_ListBox_Set(WM_HWIN parentWin)
{
    WM_HWIN hWin;
    hWin=GUI_CreateDialogBox(_aListBoxCreate , GUI_COUNTOF(_aListBoxCreate),_cbListBoxDlg,parentWin,0,0);
    return hWin;
}



//edit对话框
WM_HWIN Create_Edit_Set(WM_HWIN parentWin);

WM_HWIN Create_Edit_Set(WM_HWIN parentWin)
{
    WM_HWIN hWin;
    hWin=GUI_CreateDialogBox(_aEditCreate , GUI_COUNTOF(_aEditCreate), _cbEditDlg ,parentWin,0,0);
    return hWin;
}




