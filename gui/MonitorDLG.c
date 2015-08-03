/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.22                          *
*        Compiled Jul  4 2013, 15:16:01                              *
*        (c) 2013 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/


#include "DIALOG.h"
#include "includes.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#if 0
#define ID_WINDOW_0     (GUI_ID_USER + 0x89)

#define ID_BUTTON_0     (GUI_ID_USER + 0x8B)
#define ID_BUTTON_1     (GUI_ID_USER + 0x8C)
#define ID_BUTTON_2     (GUI_ID_USER + 0x8D)
#define ID_BUTTON_3     (GUI_ID_USER + 0x8E)
#define ID_BUTTON_4     (GUI_ID_USER + 0x8F)
#define ID_BUTTON_5     (GUI_ID_USER + 0x90)

#define ID_TEXT_0       (GUI_ID_USER + 0x91)
#define ID_MULTIEDIT_0  (GUI_ID_USER + 0x92)

#define ID_EDIT_0       (GUI_ID_USER + 0x93)

#endif

/*********************************************************************
*
*       _aDialogCreate
*/

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, NULL,       ID_WINDOW_0,    0,   0,   240, 295, 0,  0x0, 0 },
    
  { BUTTON_CreateIndirect, ReadNope,       ID_BUTTON_0,    129, 43,  105, 25, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, ClearScr,       ID_BUTTON_1,    6,   43,  105, 25, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, SetMonitor,     ID_BUTTON_2,    6,   10,  105, 25, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, SetCopy,        ID_BUTTON_5,    129, 10,  105, 25, 0, 0x0, 0 },
    
  { BUTTON_CreateIndirect, Quit,           ID_BUTTON_3,    175,  262, 55, 25, 0, 0x0, 0 },
  //{ BUTTON_CreateIndirect, Save,           ID_BUTTON_4,    10,   263, 55, 25, 0, 0x0, 0 },
  { TEXT_CreateIndirect,   MND_countdown,  ID_TEXT_0,      10,  76,  100, 20, 0, 0x0, 0 },
  { MULTIEDIT_CreateIndirect, NULL,        ID_MULTIEDIT_0, 4,   103, 232, 153, 0, 0x0, 0 },
  { EDIT_CreateIndirect,     NULL,         ID_EDIT_0,      129,  73,  105, 25,  EDIT_CF_HCENTER, 0x64, 0 },  

  
};



/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

 
/**************************************************************
*
*                     ��ʼ���Ի���
*
***************************************************************/

WM_HWIN MNT_GetTime(void)
{
    return WM_GetDialogItem(g_hWin_monitor,ID_EDIT_0);
}

WM_HWIN MNT_GetReadNope(void)
{
    return WM_GetDialogItem(g_hWin_monitor,ID_BUTTON_0);
}

WM_HWIN MNT_Get_MultiEdit()
{
    return WM_GetDialogItem(g_hWin_monitor,ID_MULTIEDIT_0);
}


static void _init_dialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hItem;

#if 0    
    hItem=WM_GetDialogItem(pMsg->hWin,ID_BUTTON_4);
    BUTTON_SetBkColor(hItem,0,GUI_GREEN);
    WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);
#endif

    hItem=WM_GetDialogItem(pMsg->hWin,ID_BUTTON_3);
    BUTTON_SetBkColor(hItem,0,GUI_YELLOW);
    WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);

    hItem=WM_GetDialogItem(pMsg->hWin,ID_BUTTON_0);
    BUTTON_SetBkColor(hItem,0,GUI_CYAN);
    WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);

    hItem=WM_GetDialogItem(pMsg->hWin,ID_BUTTON_2);
    BUTTON_SetBkColor(hItem,0,GUI_CYAN);
    WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);

    hItem=WM_GetDialogItem(pMsg->hWin,ID_BUTTON_1);
    BUTTON_SetBkColor(hItem,0,GUI_CYAN);
    WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);
    
    hItem=WM_GetDialogItem(pMsg->hWin,ID_BUTTON_5);
    BUTTON_SetBkColor(hItem,0,GUI_CYAN);
    WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);

    hItem = WM_GetDialogItem(pMsg->hWin,ID_EDIT_0);
    EDIT_SetFont(hItem, &GUI_Fontdate20);
    EDIT_SetDecMode(hItem,999,0,999,0 ,0);
    EDIT_SetValue(hItem,COUNT_VALUE);
    WM_DisableWindow(hItem);
    WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);
    

    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIEDIT_0);
    MULTIEDIT_SetFont(hItem,&GUI_Font_Song_16);
    //WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);
    MULTIEDIT_SetAutoScrollV(hItem,1);
    //WM_DisableWindow(hItem);
    MULTIEDIT_SetReadOnly(hItem,1);
    MULTIEDIT_SetWrapWord(hItem);
}







/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Dropdown'
    //
    GUI_UC_SetEncodeUTF8();
    _init_dialog(pMsg);
    break;
 
  case WM_KEY:
    if((((WM_KEY_INFO*)(pMsg->Data.p))->PressedCnt) == 1)//�����ͷš�
    {
        switch(((WM_KEY_INFO *)(pMsg->Data.p))->Key)
        {
            case GUI_KEY_YELLOW:
                WM_DeleteWindow(g_hWin_monitor);
                g_hWin_monitor=HBWIN_NULL;
                WM_ShowWindow(g_hWin_TimeBar);
                WM_ShowWindow(g_hWin_Date);
                WM_SetFocus(g_hWin_menu);
                g_gui_para.state = GUI_STATE_IDLE;

                if(CHANNEL_PLC == g_rom_para.channel)
                {
                    g_gui_para.cmd = GUI_CMD_PLC_M2R;
                    g_gui_para.state = GUI_STATE_IDLE;
                    g_sys_ctrl.sysCtdVal = COUNT_VALUE;
                    g_sys_ctrl.sysCtdFlag = COUNTDOWN_OFF;
                    OSMboxPost(g_sys_ctrl.up_mbox, (void*)&g_gui_para);
                }
                break;
                
            case '#': //���ز��ڵ�
                //ButtonBlink(pMsg,ID_BUTTON_0);
                
                if(g_rom_para.channel != CHANNEL_PLC)
                {
                    ERR_NOTE(g_hWin_monitor,GUI_MSBOX_FUN_DISALE_ERROR);
                }
                else
                {
                    g_gui_para.cmd = GUI_CMD_PLC_READ_NODE;
                    g_gui_para.state = GUI_STATE_PLC_MONITOR;
                    g_sys_ctrl.sysCtdVal = COUNT_VALUE;
                    g_sys_ctrl.sysCtdFlag = COUNTDOWN_ON;
                    OSMboxPost(g_sys_ctrl.up_mbox, (void*)&g_gui_para);
                }
                break;
                
            case '*':
                //ButtonBlink(pMsg,ID_BUTTON_1);
                hItem=WM_GetDialogItem(pMsg->hWin,ID_MULTIEDIT_0);
                MULTIEDIT_SetText(hItem,"\0");
                break;
            case GUI_KEY_F1: /*���̬*/
                g_sys_ctrl.sysCtdFlag = COUNTDOWN_OFF; //����Ǵӳ���̬�л�������Ҫ�رճ���̬
                hItem = MNT_GetTime();
                EDIT_SetValue(hItem,240);
                hItem = MNT_GetReadNope();
                BUTTON_SetText(hItem, ReadNope);
                
                if(g_rom_para.channel != CHANNEL_PLC)
                {
                    ERR_NOTE(g_hWin_monitor,GUI_MSBOX_FUN_DISALE_ERROR);
                }
                else
                {
                    g_gui_para.cmd = GUI_CMD_PLC_R2M;
                    g_gui_para.state = GUI_STATE_PLC_MONITOR;
                    OSMboxPost(g_sys_ctrl.up_mbox, (void*)&g_gui_para);
                }
                break;
                
            case GUI_KEY_F2:/* ����̬ */
                g_sys_ctrl.sysCtdFlag = COUNTDOWN_OFF; //����Ǵӳ���̬�л�������Ҫ�رճ���̬
                hItem = MNT_GetTime();
                EDIT_SetValue(hItem,240);
                hItem = MNT_GetReadNope();
                BUTTON_SetText(hItem, ReadNope);
                //ButtonBlink(pMsg,ID_BUTTON_5);
                //TSK_Close_Monitor();
                if(g_rom_para.channel != CHANNEL_PLC)
                {
                    ERR_NOTE(g_hWin_monitor,GUI_MSBOX_FUN_DISALE_ERROR);
                }
                else
                {
                    g_gui_para.cmd = GUI_CMD_PLC_M2R;
                    g_gui_para.state = GUI_STATE_PLC_MONITOR;
                    OSMboxPost(g_sys_ctrl.up_mbox, (void*)&g_gui_para);
                }
                break;
        }
    }
    break;
  // USER START (Optionally insert additional message handling)
  // USER END
  case WM_MSG_CLOSE:
    WM_DeleteWindow(g_hWin_monitor);
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
**********************************************************************
*/
/*********************************************************************
*
*       CreateMonitor
*/
WM_HWIN CreateMonitor(void);
WM_HWIN CreateMonitor(void) {
  WM_HWIN hWin;
  //WM_HWIN hItem;
  //GUI_COLOR color=0x000000;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, g_hWin_menu, 0, 0);
  return hWin;
}







