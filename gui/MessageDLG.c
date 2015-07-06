#include "DIALOG.h"
#include "includes.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#if 0

#define ID_WINDOW_0     (GUI_ID_USER + 0xAD)
#define ID_MULTIEDIT_0  (GUI_ID_USER + 0xAE)

#define ID_BUTTON_0     (GUI_ID_USER + 0xB1)
#define ID_BUTTON_1     (GUI_ID_USER + 0xB2)

#define ID_TEXT_0       (GUI_ID_USER + 0xB3)
#define ID_TEXT_1       (GUI_ID_USER + 0xB4)
#endif 


static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect,    NULL,        ID_WINDOW_0,    0,   0,   240, 295, 0, 0x0, 0 },
  { MULTIEDIT_CreateIndirect, "Multiedit", ID_MULTIEDIT_0, 7,   26,  225, 229, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,    Clear,       ID_BUTTON_1,    10,   262, 55, 25, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,    Back,        ID_BUTTON_0,    175,  262, 55, 25, 0, 0x0, 0 },
  { TEXT_CreateIndirect,      Log,         ID_TEXT_1,      13,  6,   119, 20, 0, 0x0, 0 },
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
    
WM_HWIN MSG_Get_MultiEdit()
{
    return WM_GetDialogItem(g_hWin_msg,ID_MULTIEDIT_0);
}

static int msg_key_cnt = 0;

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  int     tmpBufSize;

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Multiedit'
    //
    GUI_UC_SetEncodeUTF8();
    hItem=WM_GetDialogItem(pMsg->hWin,ID_BUTTON_1);
    BUTTON_SetBkColor(hItem,0,GUI_GREEN);
    WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);
    
    hItem=WM_GetDialogItem(pMsg->hWin,ID_BUTTON_0);
    BUTTON_SetBkColor(hItem,0,GUI_YELLOW);
    WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);
    

    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIEDIT_0);
    MULTIEDIT_SetFont(hItem,&GUI_Font_Song_16);
    MULTIEDIT_SetAutoScrollV(hItem,1);
    //WM_DisableWindow(hItem);
    MULTIEDIT_SetReadOnly(hItem,1);
    MULTIEDIT_SetWrapWord(hItem);
    break;
    
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIEDIT_0);
    if(MULTIEDIT_GetTextSize(hItem) > 2500)
    {
        MULTIEDIT_SetText(hItem, "\0");    
    }
    break;
  case WM_KEY:
    if((((WM_KEY_INFO*)(pMsg->Data.p))->PressedCnt) == 1)//°´¼üÊÍ·Å¡£added on 2015.1.4
    {
        switch(((WM_KEY_INFO *)(pMsg->Data.p))->Key)
        {
            case GUI_KEY_YELLOW:
                //WM_DeleteWindow(g_hWin_msg);
                //g_hWin_msg=HBWIN_NULL;
               // WM_HideWindow(g_hWin_msg);
                WM_BringToBottom(g_hWin_msg);
                //WM_SetFocus(g_hWin_ProtoDbg);
                if(g_hWin_ProtoDbg>0)
                {
                    WM_SetFocus(g_hWin_ProtoDbg);
                }
                else if(g_hWin_ReadMeter>0)
                {
                    WM_SetFocus(g_hWin_ReadMeter);
                }
                msg_key_cnt = 0;
                break;
            case GUI_KEY_GREEN:
                hItem=WM_GetDialogItem(pMsg->hWin,ID_MULTIEDIT_0);
                MULTIEDIT_SetText(hItem,"\0");
                msg_key_cnt = 0;
                break;
            case GUI_KEY_UP:
                if(msg_key_cnt < 10)
                {
                    hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIEDIT_0);
                    tmpBufSize = MULTIEDIT_GetTextSize(hItem);
                    MULTIEDIT_SetCursorOffset(hItem, (tmpBufSize-(msg_key_cnt * tmpBufSize/10)));
                    msg_key_cnt++;
                }
                else
                {
                    msg_key_cnt = 0;
                }            
                break;
            case GUI_KEY_DOWN:
                if(msg_key_cnt < 10)
                {
                    hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIEDIT_0);
                    tmpBufSize = MULTIEDIT_GetTextSize(hItem);
                    MULTIEDIT_SetCursorOffset(hItem, (msg_key_cnt * tmpBufSize/10));
                    msg_key_cnt++;
                }
                else
                {
                    msg_key_cnt = 0;
                }            
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
*       CreateMessage
*/
WM_HWIN CreateMessage(void);
WM_HWIN CreateMessage(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, g_hWin_menu, 0, 0);
  return hWin;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
