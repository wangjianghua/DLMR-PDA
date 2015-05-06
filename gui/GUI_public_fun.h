#ifndef __GUI_PUBLIC_FUN__
#define __GUI_PUBLIC_FUN__



#define SOFTWARE_VERSION         21
#define HARDWARE_VERSION         21


#define GUI_645_ADDR_LENGTH 12

/*dropdown中下拉列表中的量，备用*/
#define DL_T_97    0
#define DL_T_07    1


#define PLC_CMD_TYPE_L2R          0  //read to listening
#define PLC_CMD_TYPE_R2L          1  //listening to read
#define PLC_CMD_TYPE_COMMON       2  //
#define PLC_CMD_TYPE_NODE         3
#define PLC_CMD_BROAD_READ        4  //广播读表号
#define PLC_CMD_FREQ_SET          5  //设置速率
#define PLC_CMD_TYPE_ROUTE        6  //启动路由表


#define PLC_ROUTE_OFF             0 
#define PLC_ROUTE_ON              1


#define ICON_FLOW_FLASH_TIMEOUT   60


#define CHANNEL_PLC       0
#define CHANNEL_WIRELESS   1
#define CHANNEL_485        2

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



#define GUI_MSBOX_ADDR_ERROR       0
#define GUI_MSBOX_DIDO_ERROR       1
#define GUI_MSBOX_PROC_ERROR       2
#define GUI_MSBOX_MONTH_ERROR      3
#define GUI_MSBOX_DAY_ERROR        4
#define GUI_MSBOX_HOUR_ERROR       5
#define GUI_MSBOX_MIN_ERROR        6
#define GUI_MSBOX_FORMAT_ERROR     7

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


#define EDIT_SCR_OUTTIME           0
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

//读节点倒计时
#define   COUNTDOWN_OFF            0
#define   COUNTDOWN_ON             1
#define   COUNT_VALUE              240

//#define   PLC_MONITOR_OFF          0
//#define   PLC_MONITOR_ON           1









//#define ERROR_BOX(error_no)     MESSAGEBOX_Create(&gc_messageBoxText[error_no][0],"Error",0)


typedef struct _sys_parameter_pkg_
{
    //U16 g_protocal;      //规约
    U8  cmdType;      
    U8  ctlCode;   //控制字
    U8  srcAddr[6]; //电表表号  reserved
    U8  dstAddr[6]; //电表表号  reserved
    U8  relayAddr[8][6];  //中继地址,实际是7级中继，在调用组中继帧接口的时候需要把目标地址也写进去,8个
    U16 dataLen;        //长度
    U8  dataFlag[4];      //数据标识
    U8  dataBuf[256];     //数据
    //U8  meterPWD[4] = {0x01,0x31,0x41,0x51};      //密码 
    //U16 g_crc;           //校验和
    
}SEND_PARA_PKG,*P_SEND_PARA_PKG;

extern SEND_PARA_PKG g_send_para_pkg;      //参数包

extern const char *gc_messageBoxText[];
extern const u8 c_645ctrlDef[2][PLC_CTRL_MAX_NUM] ;

extern const u32 c_645DidoDef[2][PLC_CTRL_MAX_NUM] ;

extern const u8 g_self_check_pwd[];

//启用小工具
void Enable_Widget(WM_MESSAGE * pMsg,int Widget_Id);

//禁用小工具
void Disable_Widget(WM_MESSAGE * pMsg,int Widget_Id);

//聚焦小工具
void Focus_Widget(WM_MESSAGE * pMsg,int Widget_Id);

//小工具不能被选中
void Not_Focus(WM_MESSAGE *pMsg,int Widget_Id);

/*button对按键的反应，闪烁一下*/
void ButtonBlink(WM_MESSAGE * pMsg,int Id);
void PUB_InitFreq(WM_MESSAGE *pMsg,int widgetID); //初始化速率设置eidt


//存储参数

//把数值转化为字符串
char *int_to_char(int src,char *pBuff,int radix) ;

void Data_Upload_Green(u32 color);
void Data_Download_Yellow(u32 color);

void GUI_Msg_Proc();


WM_HWIN MSG_Get_MultiEdit();
WM_HWIN STM_Get_PROGBAR();


WM_HWIN TSK_Get_Upload_Text();

WM_HWIN TSK_Get_Download_Text();

void Data_State_Judge();

u8* GUI_hex2PowerDataStr(u8 * srcBuf, u32 len);

WM_HWIN MNT_Get_MultiEdit();

u32 GUI_GetMeterAddr(u8 * dbuf, u8 * gbuf);

WM_HWIN GUI_Get_PROGBAR();
WM_HWIN MMD_Get_PROGBAR();


WM_HWIN RMD_Get_PROGBAR();
WM_HWIN RMD_Get_MeterNum(void);
WM_HWIN RMD_Get_ReadSel(void);
void RMD_proc_resp_data();


/**********
*
*先实现，看能不能重构成一个函数
*
**************/
void CPS_SetFocus(void);
void CPS_Color_Change(void);

void TMS_SetFocus(void);
void TMS_Color_Change(void);

void ADS_SetFocus(void);
void ADS_Color_Change(void);

void CPT_SetFocus(void);
void CPT_Color_Change(void);

void RMD_SetFocus(void);
void RMD_Color_Change(void);

void Select_Focus(void);



u32 GUI_GetStrDataFlag(u8 * dbuf, u32 pro_ver);
u8* GUI_hex2MeterAddrStr(u8 * srcBuf, u32 len);
void GUI_Fill_Zero(u8 *tempbuf); //自动补全零

void CST_Set_DataFlag(u8 * tst);
void GUI_GetHexDataFlag(u8 * strbuf, u8* dataflag, u8 len);
void MMD_Format_Disk(void);
void MMD_Set_FD_PROGBAR(u32 newVal);

#endif /*__GUI_PUBLIC_FUN__*/



