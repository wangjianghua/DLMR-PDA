#include "DIALOG.h"
#include "includes.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

static const GUI_WIDGET_CREATE_INFO _aListBoxCreate[] = {
  { FRAMEWIN_CreateIndirect,  "ListBox",  ID_FRAMEWIN_0,  20,  40,  200, 200,  0, 0x0, 0 },
  { LISTBOX_CreateIndirect,   "Listbox",  ID_LISTBOX_0,   6,   7,   182, 120,  0, 0x0, 0 },
  { BUTTON_CreateIndirect,    Confirm,    ID_BUTTON_0,    7,   138, 55,  25,   0, 0x0, 0 },
  { BUTTON_CreateIndirect,    Cancel,       ID_BUTTON_1,    132, 138, 55,  25,   0, 0x0, 0 },
};

static const GUI_WIDGET_CREATE_INFO _aEditCreate[] = {
  { FRAMEWIN_CreateIndirect, "Edit",  ID_FRAMEWIN_0, 20,  60, 200, 160, 0, 0x0,  0 },
  { BUTTON_CreateIndirect,   Confirm, ID_BUTTON_0,   7,   82, 55,  25,  0, 0x0,  0 },
  { BUTTON_CreateIndirect,   Cancel,    ID_BUTTON_1,   132, 82, 55,  25,  0, 0x0,  0 },
  { EDIT_CreateIndirect,     "Edit",  ID_EDIT_0,     14,  38, 165, 25,  0, 0x64, 0 },
  //{ TEXT_CreateIndirect,   "Text",    ID_TEXT_0,   14,  16, 165, 20,  0, 0x0,  0 },

};

const u32 c_ValBaudRate[5] = {BAUD_RATE_1200, BAUD_RATE_1500, BAUD_RATE_2400, BAUD_RATE_4800, BAUD_RATE_9600};

const u8 c_TextBaudRate[5][6] = {"1200","1500","2400","4800","9600"};

void Select_Focus(void)
{
    
    //参数设置页面中的全部页面
    if((g_hWin_para > 0)&&(g_hWin_TimeSet <= 0)&&(g_hWin_AdvanSet <= 0))
    {
        WM_SetFocus(g_hWin_para);
        CPS_SetFocus();
    }

    if(g_hWin_TimeSet > 0)
    {
        WM_SetFocus(g_hWin_TimeSet);
        TMS_SetFocus();
    }
    
    if(g_hWin_AdvanSet > 0)
    {
        WM_SetFocus(g_hWin_AdvanSet);
        ADS_SetFocus();
    }
    
    //规约调试
    if((g_hWin_ProtoDbg > 0)&&(g_hWin_relay <= 0) && (g_hWin_freq <= 0)) 
    {
        WM_SetFocus(g_hWin_ProtoDbg);
        CPT_SetFocus();
    }

    if(g_hWin_relay > 0)
    {
        WM_SetFocus(g_hWin_relay);
    }

    if(g_hWin_freq > 0)
    {
        WM_SetFocus(g_hWin_freq);
    }
    
    //智能抄表
    if((g_hWin_ReadMeter > 0) && (g_hWin_freq <= 0)&&(g_hWin_MeterTime <= 0))
    {
        WM_SetFocus(g_hWin_ReadMeter);
        RMD_SetFocus();
    }

   if(g_hWin_MeterTime > 0)
    {
        WM_SetFocus(g_hWin_MeterTime);
    }

    //系统信息
    if((g_hWin_SysInfo > 0)&&(g_hWin_SDInfo <= 0))
    {
        WM_SetFocus(g_hWin_SysInfo);
    }

    if(g_hWin_SDInfo > 0)
    {
        WM_SetFocus(g_hWin_SDInfo);
    }

}

#if (EWARM_OPTIMIZATION_EN > 0u)
#pragma optimize = low
#endif
//在按下绿色保存按钮的时候，调用此函数
static void Select_Input_Edit(int  EditNum)
{
    WM_HWIN hItem;
    u8 tmpTextBuf[32];
    u8 tmpListBuf[32];//中继地址序号
    int RowCount;
    int RowSel;
    
    hItem=WM_GetDialogItem(g_hWin_Input,ID_EDIT_0);
    switch(EditNum)
    {
        case EDIT_SLEEP_TIME:
            EDIT_GetText(hItem,tmpTextBuf,6);
            g_rom_para.auto_sleep_time = atoi(tmpTextBuf);
            if(g_rom_para.auto_sleep_time < 5)
            {
                g_rom_para.auto_sleep_time = 5;

                sprintf(tmpTextBuf, "%d", g_rom_para.auto_sleep_time);
            }
            hItem=ADS_GetSrcOutTime();
            break;
            
        case EDIT_PASSWORD:
            EDIT_GetText(hItem,tmpTextBuf,9);
            //g_rom_para.meterPassword = atoi(tmpTextBuf);
            if(DEV_ERROR == CPS_GetPwdPara(tmpTextBuf))
            {
                break;
            }
            hItem=CPS_GetPwd();
            break;
            
        case EDIT_RECV_DELAY:
            EDIT_GetText(hItem,tmpTextBuf,6);
            g_rom_para.recvDelayTime = atoi(tmpTextBuf);
            hItem=CPS_Set_RevDelay();
            break;

        case EDIT_BETWEEN_ACT:
            EDIT_GetText(hItem,tmpTextBuf,6);
            g_rom_para.execInterval = atoi(tmpTextBuf);
            hItem=CPS_Set_BetweenAct();
            break;

        case EDIT_ADDR:
            EDIT_GetText(hItem,tmpTextBuf,13);
            GUI_Fill_Zero(tmpTextBuf);
            GUI_GetMeterAddr(tmpTextBuf, g_sys_ctrl.recentMeterAddr);
            hItem=CPT_Set_Addr();
            break;

        case EDIT_DATA_FLAG:
            EDIT_GetText(hItem,tmpTextBuf,9);
            hItem=CPT_Set_DataFlag();
            GUI_GetStrDataFlag(tmpTextBuf, g_rom_para.protocol);
            break;

        case EDIT_LENGTH:
            EDIT_GetText(hItem,tmpTextBuf,6);
            hItem=CPT_Set_Length();
            break;

        case EDIT_DATA:
            EDIT_GetText(hItem,tmpTextBuf,13);
            hItem=CPT_Set_Data();
            break;

        case EDIT_METER_NUM:
            EDIT_GetText(hItem,tmpTextBuf,13);
            GUI_Fill_Zero(tmpTextBuf);
            GUI_GetMeterAddr(tmpTextBuf, g_sys_ctrl.recentMeterAddr);
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
        //case EDIT_PWD_CFM:
            //EDIT_GetText(hItem,tmpTextBuf,7);
            //hItem = 
          //  break;
        case EDIT_OPCODE:
            EDIT_GetText(hItem,tmpTextBuf,9);
            if(DEV_ERROR== CPS_GetOpcodePara(tmpTextBuf))
            {
                break;
            }
            hItem = CPS_GetOperator();
            break;

        case MODIFY_RELAY_ADDR: //修改中继地址
            EDIT_GetText(hItem,tmpTextBuf,13);
            GUI_Fill_Zero(tmpTextBuf);
            
            hItem = RLY_GetListAddr();
            RowSel = LISTVIEW_GetSel(hItem);
            LISTVIEW_SetItemText(hItem ,1, RowSel, tmpTextBuf);
            //LISTVIEW_Handle hObj,unsigned Column,unsigned Row,const char * s)
            hItem = RLY_GetAddr();
            break;

        case ADD_RELAY_ADDR:  //按#增加
            EDIT_GetText(hItem,tmpTextBuf,13);
            GUI_Fill_Zero(tmpTextBuf); 
            hItem = RLY_GetListAddr();
            
            LISTVIEW_AddRow(hItem, NULL);
            RowCount=LISTVIEW_GetNumRows(hItem);
            LISTVIEW_SetItemText(hItem, 1, (RowCount-1), tmpTextBuf);//第1列也就是，中继地址列
            int_to_char((RowCount-1), tmpListBuf, 10);
            LISTVIEW_SetItemText(hItem, 0, (RowCount-1), tmpListBuf);
            hItem = WM_GetDialogItem(g_hWin_Input,ID_EDIT_0);
            break;
            
        case EDIT_SHUTDOWN_TIME:
            EDIT_GetText(hItem,tmpTextBuf,6);
            g_rom_para.auto_shutdown_time = atoi(tmpTextBuf);
            if(g_rom_para.auto_shutdown_time < 60)
            {
                g_rom_para.auto_shutdown_time = 60;

                sprintf(tmpTextBuf, "%d", g_rom_para.auto_shutdown_time);
            }
            hItem=ADS_GetStDnTime();
            break;

        case EDIT_METER_ADDE:
            EDIT_GetText(hItem,tmpTextBuf,13);
            GUI_Fill_Zero(tmpTextBuf);
            GUI_GetMeterAddr(tmpTextBuf, g_sys_ctrl.recentMeterAddr);
            hItem=MTD_GetMeterAddr();
            
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
    int RowSel;
    int i;
    switch(EditNum)
    {
        case EDIT_SLEEP_TIME:
            hItem=ADS_GetSrcOutTime();
            EDIT_GetText(hItem,tmpTextBuf,5);
            //hItem=CPS_Set_ScrOutTime();
            break;
            
        case EDIT_PASSWORD:
            hItem = CPS_GetPwd();
            EDIT_GetText(hItem,tmpTextBuf,9);
            
            //memcpy();
            
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
        case EDIT_OPCODE: //长度需要注意,操作码
            hItem = CPS_GetOperator();
            EDIT_GetText(hItem,tmpTextBuf,9);
            break;

        case MODIFY_RELAY_ADDR:
            hItem = RLY_GetListAddr();
            RowSel = LISTVIEW_GetSel(hItem);
            LISTVIEW_GetItemText(hItem,1, RowSel, tmpTextBuf, 13);
            break;

        case ADD_RELAY_ADDR:
            memcpy(tmpTextBuf,"\0",1);
            break;

         case EDIT_SHUTDOWN_TIME:
            hItem = ADS_GetStDnTime();
            EDIT_GetText(hItem,tmpTextBuf,5);
            break;

         case EDIT_METER_ADDE:
            hItem =  MTD_GetMeterAddr();
            EDIT_GetText(hItem, tmpTextBuf, 13);
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
        _init_edit(pMsg,g_sys_ctrl.selectWidget);
        break;
        
      case WM_KEY:
        if((((WM_KEY_INFO *)(pMsg->Data.p))->PressedCnt) == 0)
        {
            switch(((WM_KEY_INFO *)(pMsg->Data.p))->Key)
            {
              case GUI_KEY_YELLOW:
                  WM_DeleteWindow(g_hWin_Input);
                  g_hWin_Input = HBWIN_NULL;
                  Select_Focus();
                  break;
              case GUI_KEY_GREEN:
                  Select_Input_Edit(g_sys_ctrl.selectWidget);
                  WM_DeleteWindow(g_hWin_Input);
                  Select_Focus();
                  g_hWin_Input = HBWIN_NULL;
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
            LISTBOX_AddString(hItem, "DL645-97");
            LISTBOX_AddString(hItem, "DL645-07");

#if 0            
            switch(g_rom_para.protocol)
            {
                case DL645_1997:
                    LISTBOX_SetSel(hItem,0);
                    break;
                case DL645_2007:
                    LISTBOX_SetSel(hItem,1);
                    break;                    
            }
#endif            
            break;
            
        case LISTBOX_CHANNEL:
            LISTBOX_AddString(hItem, ChannelPLC);
            LISTBOX_AddString(hItem, ChannelRF);
            LISTBOX_AddString(hItem, ChannelIR);
            break;


        case LISTBOX_BAUDRATE:
            LISTBOX_AddString(hItem, "1200");
            LISTBOX_AddString(hItem, "1500");
            LISTBOX_AddString(hItem, "2400");
            LISTBOX_AddString(hItem, "4800");
            LISTBOX_AddString(hItem, "9600");
            break;

        case LISTBOX_PREAM:
            break;

        case LISTBOX_STOPBIT:
            LISTBOX_AddString(hItem, "1");
            LISTBOX_AddString(hItem, "2");
            break;

        
        case LISTBOX_CTLCODE:  /*控制字*/
            if(g_rom_para.protocol == DL645_1997)
            {
                LISTBOX_AddString(hItem, Readdata_97);
                //LISTBOX_AddString(hItem, WriteData_97);
                LISTBOX_AddString(hItem, CalTime_Broad);
            }
            else if(g_rom_para.protocol == DL645_2007)
            {
                LISTBOX_AddString(hItem, Readdata_07);
                //LISTBOX_AddString(hItem, WriteData_07);
                LISTBOX_AddString(hItem, GetAddr_07);
                LISTBOX_AddString(hItem, CalTime_Broad);
            }
            //LISTBOX_AddString(hItem, WriteData);
            //LISTBOX_AddString(hItem, ClrDemond);
            break;

        case LISTBOX_READ_SEL:
           if(DL645_2007 == g_rom_para.protocol)
           {
               for( i = 0; i < 4; i++)
               {
                    LISTBOX_AddString(hItem,pReadSel_07[i]);
               }
           }
           if(DL645_1997 == g_rom_para.protocol)
           {
               for( i = 0; i < 4; i++)
               {
                    LISTBOX_AddString(hItem,pReadSel_97[i]);
               } 
           }
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
                    g_rom_para.protocol = DL645_1997;
                    hWin=CPS_Set_Proto();
                    EDIT_SetText(hWin,"DL645-97");
                    break;
                    
                case 1:
                    g_rom_para.protocol = DL645_2007;
                    hWin=CPS_Set_Proto();
                    EDIT_SetText(hWin,"DL645-07");
                    break;                    
            }
            break;

        case LISTBOX_CHANNEL:
            switch(SelNum)
            {
                case 2:
                    g_rom_para.channel=CHANNEL_IR;
                    hWin=CPS_Set_Channel();
                    EDIT_SetText(hWin,ChannelIR);
                    break;
                    
                case 0:
                    g_rom_para.channel=CHANNEL_PLC;
                    hWin=CPS_Set_Channel();
                    EDIT_SetText(hWin,ChannelPLC);
                    break;
                case 1:
                    g_rom_para.channel=CHANNEL_RF;
                    hWin=CPS_Set_Channel();
                    EDIT_SetText(hWin,ChannelRF);
                    break;
            }
            break;
            
        case LISTBOX_BAUDRATE:
            g_rom_para.baudrate = c_ValBaudRate[SelNum];
            hWin = CPS_Set_BaudRate();
            EDIT_SetText(hWin,c_TextBaudRate[SelNum]);
            break;
            
         case LISTBOX_STOPBIT:
            switch(SelNum)
            {
                case 0:
                    g_rom_para.stopbit=ONE_STOPBIT;
                    hWin=CPS_Set_StopBit();
                    EDIT_SetText(hWin,"1");
                    break;
                case 1:
                    g_rom_para.stopbit=TWO_STOPBIT;
                    hWin=CPS_Set_StopBit();
                    EDIT_SetText(hWin,"2");
                    break;
            }
            break;


         case LISTBOX_CTLCODE:
            hWin=CPT_Set_CtlCode();
            
            if(g_rom_para.protocol == DL645_1997)
            {
                g_gui_para.ctrlCode = g_ProtoDebugCtrl[g_rom_para.protocol][SelNum]; 
                EDIT_SetText(hWin,pCtlCode_97[SelNum]);
            }
            else if(g_rom_para.protocol == DL645_2007)
            {
                g_gui_para.ctrlCode = g_ProtoDebugCtrl[g_rom_para.protocol][SelNum]; 
                EDIT_SetText(hWin,pCtlCode_07[SelNum]);
            }
            

            break;
            
        case LISTBOX_READ_SEL:
            hWin=RMD_Get_ReadSel();
            
            if(DL645_2007 == g_rom_para.protocol)
            {        
                memcpy(g_gui_para.dataItem, &c_645dataItemDef[g_rom_para.protocol][SelNum], 4);
                
                EDIT_SetText(hWin,pReadSel_07[SelNum]);
            }
            else if(DL645_1997 == g_rom_para.protocol)
            {
                memcpy(g_gui_para.dataItem, &c_645dataItemDef[g_rom_para.protocol][SelNum], 2);
                
                EDIT_SetText(hWin,pReadSel_97[SelNum]);
            }

            g_sys_ctrl.data_item_index = SelNum;
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
          _Init_ListBox(pMsg,g_sys_ctrl.selectWidget);
          Button_Color_Set(pMsg);
          
          break;
          
        case WM_KEY:
          if((((WM_KEY_INFO *)(pMsg->Data.p))->PressedCnt) == 1)
          {
              switch(((WM_KEY_INFO *)(pMsg->Data.p))->Key)
              {
                case GUI_KEY_YELLOW:
                    WM_DeleteWindow(g_hWin_Input);
                    //WM_SetFocus(g_hWin_para);
                    Select_Focus();
                    break;
                case GUI_KEY_GREEN:
                    Select_ListBox_Row(g_sys_ctrl.selectWidget);
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




