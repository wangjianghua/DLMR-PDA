#ifndef __GUI_PUBLIC_FNCT__
#define __GUI_PUBLIC_FNCT__


#include "GUI.h"

/********************************************
//
//   ID of User's Item
//
*********************************************/
#define ID_EDIT_0       ( GUI_ID_USER + 0x00 )
#define ID_EDIT_1       ( GUI_ID_USER + 0x01 )
#define ID_EDIT_2       ( GUI_ID_USER + 0x02 )
#define ID_EDIT_3       ( GUI_ID_USER + 0x03 )
#define ID_EDIT_4       ( GUI_ID_USER + 0x04 )
#define ID_EDIT_5       ( GUI_ID_USER + 0x05 )
#define ID_EDIT_6       ( GUI_ID_USER + 0x06 )
#define ID_EDIT_7       ( GUI_ID_USER + 0x07 )
#define ID_EDIT_8       ( GUI_ID_USER + 0x08 )
#define ID_EDIT_9       ( GUI_ID_USER + 0x09 )

#define ID_TEXT_0       ( GUI_ID_USER + 0x10 )
#define ID_TEXT_1       ( GUI_ID_USER + 0x11 )
#define ID_TEXT_2       ( GUI_ID_USER + 0x12 )
#define ID_TEXT_3       ( GUI_ID_USER + 0x13 )
#define ID_TEXT_4       ( GUI_ID_USER + 0x14 )
#define ID_TEXT_5       ( GUI_ID_USER + 0x15 )
#define ID_TEXT_6       ( GUI_ID_USER + 0x16 )
#define ID_TEXT_7       ( GUI_ID_USER + 0x17 )
#define ID_TEXT_8       ( GUI_ID_USER + 0x18 )
#define ID_TEXT_9       ( GUI_ID_USER + 0x19 )
#define ID_TEXT_10      ( GUI_ID_USER + 0x1A )
#define ID_TEXT_11      ( GUI_ID_USER + 0x1B )
#define ID_TEXT_12      ( GUI_ID_USER + 0x1C )


#define ID_BUTTON_0     ( GUI_ID_USER + 0x20 )
#define ID_BUTTON_1     ( GUI_ID_USER + 0x21 )
#define ID_BUTTON_2     ( GUI_ID_USER + 0x22 )
#define ID_BUTTON_3     ( GUI_ID_USER + 0x23 )
#define ID_BUTTON_4     ( GUI_ID_USER + 0x24 )
#define ID_BUTTON_5     ( GUI_ID_USER + 0x25 )
#define ID_BUTTON_6     ( GUI_ID_USER + 0x26 )
#define ID_BUTTON_7     ( GUI_ID_USER + 0x27 )

#define ID_LISTBOX_0    ( GUI_ID_USER + 0x30 )
#define ID_LISTBOX_1    ( GUI_ID_USER + 0x31 )
#define ID_LISTBOX_2    ( GUI_ID_USER + 0x32 )
#define ID_LISTBOX_3    ( GUI_ID_USER + 0x33 )

#define ID_PROGBAR_0    ( GUI_ID_USER + 0x40 )
#define ID_PROGBAR_1    ( GUI_ID_USER + 0x41 )

#define ID_LISTVIEW_0   ( GUI_ID_USER + 0x50 )
#define ID_LISTVIEW_1   ( GUI_ID_USER + 0x51 )

#define ID_MULTIEDIT_0  ( GUI_ID_USER + 0x60 )
#define ID_MULTIEDIT_1  ( GUI_ID_USER + 0x61 )

#define ID_WINDOW_0     ( GUI_ID_USER + 0x70 )

#define ID_FRAMEWIN_0   ( GUI_ID_USER + 0x75 )

/******************User Item ID End*******************/






#define GUI_645_ADDR_LENGTH      12

#define DL645_97                  0
#define DL645_07                  1

typedef enum
{
    GUI_STATE_NONE = 0,
    GUI_STATE_PARA_SET,
    GUI_STATE_AMR,
    GUI_STATE_PLC_FREQ_SET,  
    GUI_STATE_PROTO_DBG,
    GUI_STATE_PLC_MONITOR,
    GUI_STATE_MEM,
    MAX_GUI_STATE_TYPE
} GUI_STATE_TYPE; //华兄

typedef enum
{
    GUI_CMD_BROADCAST_READ_ADDR = 0,
    GUI_CMD_PLC_R2L,
    GUI_CMD_PLC_L2R,    
    GUI_CMD_PLC_FREQ_SET,
    GUI_CMD_PLC_READ_NODE,
    GUI_CMD_MRW,
    GUI_CMD_ROUTE,
    MAX_GUI_CMD_TYPE
} GUI_CMD_TYPE; //华兄

#define PLC_ROUTE_OFF             0 
#define PLC_ROUTE_ON              1


#define ICON_FLOW_FLASH_TIMEOUT   60


#define CHANNEL_PLC       0
#define CHANNEL_RF        1
#define CHANNEL_IR        2

#define ONE_STOPBIT        0
#define TWO_STOPBIT        1

#define BAUD_RATE_1200      0
#define BAUD_RATE_1500      1   
#define BAUD_RATE_2400      2
#define BAUD_RATE_4800      3
#define BAUD_RATE_9600      4



#define PLC_READ_DEVADDR_0     0
#define PLC_READ_DATA_1        1
#define PLC_WRITE_DATA_2       2

#define PLC_CTRL_MAX_NUM    16

#define DATA_SIGN_LENGTH    4  //数据标识符长度


#define GUI_MSBOX_ADDR_ERROR         0
#define GUI_MSBOX_DIDO_ERROR         1
#define GUI_MSBOX_PROC_ERROR         2
#define GUI_MSBOX_MONTH_ERROR        3
#define GUI_MSBOX_DAY_ERROR          4
#define GUI_MSBOX_HOUR_ERROR         5
#define GUI_MSBOX_MIN_ERROR          6
#define GUI_MSBOX_FORMAT_ERROR       7
#define GUI_MSBOX_UPDATE_ERROR       8
#define GUI_MSBOX_RLY_OUTNUM_ERROR   9
#define GUI_MSBOX_OUTTIME_ERROR      10
#define GUI_MSBOX_LOWVTG_ERROR       11
#define GUI_MSBOX_FUN_DISALE_ERROR   12
#define GUI_MSBOX_RESET_ERROR        13

//#define KEY_PRESS_CNT_MIN          0  //按键次数
//#define KEY_PRESS_CNT_MAX          8  //最大按键次数


#define LISTBOX_PROTOCOL           0
#define LISTBOX_CHANNEL            1
#define LISTBOX_BAUDRATE           2
#define LISTBOX_PREAM              3
#define LISTBOX_STOPBIT            4
#define LISTBOX_CTLCODE            5
#define LISTBOX_READ_SEL           6
#define LISTBOX_SPEED              7


#define EDIT_SLEEP_TIME            0
#define EDIT_PASSWORD              1
#define EDIT_RECV_DELAY            2
#define EDIT_BETWEEN_ACT           3
#define EDIT_ADDR                  4
#define EDIT_DATA_FLAG             5
#define EDIT_LENGTH                6
#define EDIT_DATA                  7
#define EDIT_METER_NUM             8

#define EDIT_YEAR                  9
#define EDIT_MONTH                 10
#define EDIT_DAY                   11
#define EDIT_HOUR                  12
#define EDIT_MIN                   13
#define EDIT_SEC                   14

//#define EDIT_PWD_CFM               15 //升级确认
//#define EDIT_SPEED                 16
#define EDIT_OPCODE                15
#define EDIT_RELAY_ADDR            16
#define ADD_RELAY_ADDR             17
#define MODIFY_RELAY_ADDR          18
#define EDIT_SHUTDOWN_TIME            19

//读节点倒计时
#define   COUNTDOWN_OFF            0
#define   COUNTDOWN_ON             1
#define   COUNT_VALUE              240

//#define   PLC_MONITOR_OFF          0
//#define   PLC_MONITOR_ON           1





#define ON                         0
#define OFF                        1




//#define ERROR_BOX(error_no)     MESSAGEBOX_Create(&gc_messageBoxText[error_no][0],"Error",0)


typedef struct _gui_para_
{
    //U16 g_protocal;      //规约
    U8  state;
    U8  cmd;      
    U8  ctlCode;   //控制字
    U8  srcAddr[6]; //电表表号  reserved
    U8  dstAddr[6]; //电表表号  reserved
    U8  relayAddr[8][6];  //中继地址,实际是7级中继，在调用组中继帧接口的时候需要把目标地址也写进去,8个
    U16 dataLen;        //长度
    U8  dataFlag[4];      //数据标识
    U8  dataBuf[256];     //数据
    //U8  meterPWD[4] = {0x01,0x31,0x41,0x51};      //密码 
    //U16 g_crc;           //校验和
    
} GUI_PARA, *P_GUI_PARA;

extern GUI_PARA g_gui_para;      //参数包

extern const char *gc_messageBoxText[];
extern const u8 c_645ctrlDef[2][PLC_CTRL_MAX_NUM] ;

extern const u32 c_645DidoDef[2][PLC_CTRL_MAX_NUM] ;

extern const u8 g_self_check_pwd[];



/*button对按键的反应，闪烁一下*/
//void ButtonBlink(WM_MESSAGE * pMsg,int Id);
void PUB_InitFreq(WM_MESSAGE *pMsg,int widgetID); //初始化速率设置eidt


//存储参数

//把数值转化为字符串
char *int_to_char(int src,char *pBuff,int radix) ;

void GUI_Msg_Upload(u16 sw);
void GUI_Msg_Download(u16 sw);
void GUI_Msg_Proc(void);


WM_HWIN MSG_Get_MultiEdit();
WM_HWIN STM_Get_PROGBAR();


WM_HWIN TSK_Get_Upload_Text();

WM_HWIN TSK_Get_Download_Text();

void Data_State_Judge();

u8* GUI_hex2PowerDataStr(u8 * srcBuf, u32 len);

WM_HWIN MNT_Get_MultiEdit();

u32 GUI_GetMeterAddr(u8 * dbuf, u8 * gbuf);

WM_HWIN GUI_Get_PROGBAR();
WM_HWIN GUI_Get_FD_Usage_PROGBAR();


WM_HWIN RMD_Get_PROGBAR();
WM_HWIN RMD_Get_MeterNum(void);
WM_HWIN RMD_Get_ReadSel(void);
void RMD_ReadErr(void);
WM_HWIN RMD_BroadErr(void);
void RMD_ClearData(void);
void RMD_proc_resp_data();


/**********
*
*先实现，看能不能重构成一个函数
*
**************/
void CPS_SetFocus(void);
//void CPS_Color_Change(void);

void TMS_SetFocus(void);
//void TMS_Color_Change(void);

void ADS_SetFocus(void);
//void ADS_Color_Change(void);

void CPT_SetFocus(void);
//void CPT_Color_Change(void);

void RMD_SetFocus(void);
//void RMD_Color_Change(void);

void Select_Focus(void);

void CPT_ClearData(void);

void GUI_StartSys(void);

void TSK_Set_BeepOn(void);
void TSK_Set_BeepOff(void);

//向上选择
void GUI_SelEdt_Up(WM_HWIN hParaentWin, int firstID, int lastID, int editNum, int pressCnt);
//向下选择;
void GUI_SelEdt_Down(WM_HWIN hParaentWin, int firstID, int editNum, int pressCnt);
void GUI_Color_Change(WM_HWIN hParaentWin, int firstID, int editNum );

//extern int key_press_cnt;



u32 GUI_GetStrDataFlag(u8 * dbuf, u32 pro_ver);
u8* GUI_hex2MeterAddrStr(u8 * srcBuf, u32 len);
void GUI_Fill_Zero(u8 *tempbuf); //自动补全零

void CST_Set_DataFlag(u8 * tst);
void GUI_GetHexDataFlag(u8 * strbuf, u8* dataflag, u8 len);
void GUI_Set_FD_Format_PROGBAR(u32 newVal);


#endif /*__GUI_PUBLIC_FNCT__*/
