#ifndef __GUI_PUBLIC_FUN__
#define __GUI_PUBLIC_FUN__


#define GUI_645_ADDR_LENGTH 12

/*dropdown中下拉列表中的量，备用*/
#define DL_T_97    0
#define DL_T_07    1


#define PLC_CMD_TYPE_R2L          0  //read to listening
#define PLC_CMD_TYPE_L2R          1  //listening to read
#define PLC_CMD_TYPE_COMMON       2  //
#define PLC_CMD_BROAD_READ        3  //广播读标号


#define ICON_FLOW_FLASH_TIMEOUT   60


#define CHANNEL_WAVE       0
#define CHANNEL_485        1
#define CHANNEL_INFRD      2

#define ONE_STOPBIT        0
#define TWO_STOPBIT        1


#define PLC_READ_DEVADDR_0     0
#define PLC_READ_DATA_1        1
#define PLC_WRITE_DATA_2       2

#define PLC_CTRL_MAX_NUM    16

#define DATA_SIGN_LENGTH    4  //数据标识符长度



#define GUI_MSBOX_ADDR_ERROR       0
#define GUI_MSBOX_DIDO_ERROR       1
#define GUI_MSBOX_PROC_ERROR       2

//#define ERROR_BOX(error_no)     MESSAGEBOX_Create(&gc_messageBoxText[error_no][0],"Error",0)


typedef struct _sys_parameter_pkg_
{
    //U16 g_protocal;      //规约
    U8  cmdType;      
    U8  ctlCode;   //控制字
    U8  srcAddr[6]; //电表表号  reserved
    U8  dstAddr[6]; //电表表号  reserved
    U8  relayAddr[7][6];  //目标地址
    U16 dataLen;        //长度
    U8  dataFlag[4];      //数据标识
    U8   dataBuf[256];     //数据
    //U16 g_crc;           //校验和
    
}SEND_PARA_PKG,*P_SEND_PARA_PKG;

extern SEND_PARA_PKG g_send_para_pkg;      //参数包
//extern SYS_PARA_PKG sys_para_pkg; //参数包指针

extern const char *gc_messageBoxText[];

//SYS_PARA_PKG     g_para_pkg_ptr;



//extern U8 g_mail_box_info; //邮箱发送的消息结构体




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


//存储参数

/*获取edit中的参数*/
void para_store_edit(WM_MESSAGE * pMsg,int Widget_Id,char *text,int num);

/*获取dropdown中的参数*/
void para_store(WM_MESSAGE * pMsg);

//把数值转化为字符串
char *int_to_char(int src,char *pBuff,int radix) ;

void Data_Upload_Green(u32 color);
void Data_Download_Yellow(u32 color);

void GUI_Msg_Proc();


WM_HWIN STD_Get_MultiEdit();
WM_HWIN STM_Get_PROGBAR();


WM_HWIN TSK_Get_Upload_Text();

WM_HWIN TSK_Get_Download_Text();

void Data_State_Judge();


void RMD_proc_resp_data();
u8* GUI_hex2PowerDataStr(u8 * srcBuf, u32 len);

WM_HWIN MND_Get_MultiEdit();

u32 GUI_GetMeterAddr(u8 * dbuf);
WM_HWIN RMD_Get_PROGBAR();
WM_HWIN MMD_Get_PROGBAR();
WM_HWIN GUI_Get_PROGBAR();

u32 GUI_GetStrDataFlag(u8 * dbuf, u32 pro_ver);
u8* GUI_hex2MeterAddrStr(u8 * srcBuf, u32 len);
void CST_Set_DataFlag(u8 * tst);
void GUI_GetHexDataFlag(u8 * strbuf, u8* dataflag, u8 len);
void MMD_Format_Disk(void);
void MMD_Set_FD_PROGBAR(u32 newVal);

#endif /*__GUI_PUBLIC_FUN__*/



