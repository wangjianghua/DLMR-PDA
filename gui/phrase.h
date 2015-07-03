#ifndef __PHRASE_H__

#define __PHRASE_H__

#include "GUI_Type.h"
//参数设置
extern const char CommParaSet[];
extern const char RevDataDelay[];
extern const char BetweenAct[];
extern const char ChannelPLC[];
extern const char ChannelIR[];
extern const char Protocal[];
extern const char Channel[];
extern const char BaudRate[];
extern const char Preamble[];
extern const char StopBit[];
extern const char Number[];
extern const char PassWord[];
extern const char Save[];
extern const char Cancel[];
extern const char Quit[];
extern const char TimeSet[];
extern const char SleepTime[];
extern const char AdvanceSet[];
extern const GUI_ConstString pProtocolText[]; 
extern const GUI_ConstString pChannelText[]; 
extern const char OperateCode[];
extern const char ChannelRF[];
extern const char ShutDownTime[];
extern const char FE_Speed[];
extern const char BeepSound[];


//时间设置


extern const char Year[];
extern const char Month[];
extern const char Day[];
extern const char Hour[];
extern const char Minite[];
extern const char Second[];
extern const char TimeSet_24[];
extern const char TypeDirection[];
extern const char Confirm[];

//抄表
extern const char GetMeterNum[];
extern const char Msg[];
extern const char MeterNum[];
extern const char ReadMeterSel[];
extern const char Positive[];
extern const char Negative[];
extern const char DayPositive[];
extern const char DayNegative[];
extern const char TotalVal[];
extern const char ReadMeter[];
extern const char Speed[];
extern const char FRQ_Flag[];

extern const char *gc_messageBoxText[]; 


//规约调试
extern const char CommStdTest[];
extern const char TarAddr[];
extern const char CtlCode[];
extern const char Length[];
extern const char DataSign[];
extern const char DataField[];
extern const char RoutTab[];
extern const char DataSignBtn[];
extern const char GetAddr[];
extern const char Readdata[];
extern const char WriteData[];
extern const char ReadChip[];
extern const char Send[];
extern const char ClrDemond[];

extern const char *Listbox_FrameTitle[];

extern const char *Edit_FrameTitle[];

extern const char *pCtlCode[];

extern const char *pReadSel[];


//数据标识
extern const char _ReadMeterProj[][64];

//路由表
extern const char _RelayAddr[];
extern const char RelayNum[];
extern const char Add[];
extern const char Delete[];
extern const char Modify[];
extern const char RelayStart[];
extern const char StartRoute[];
extern const char AlreadyStart[];
extern const char MND_countdown[];
extern const char MND_reading[];
extern const char AddRlyAddr[];
extern const char ModifyRlyAddr[];
extern const char AlreadyClosed[];


//状态监控
extern const char CmdSel[];
extern const char Data[];
extern const char SetMonitor[];
extern const char SetCopy[];
extern const char ReadModule[];
extern const char ReadNope[];
extern const char Sel[];
extern const char Stop[];
extern const char ClearScr[];
extern const char Start[];

//内存管理
extern const char MemSize[];
extern const char MemUsage[];
extern const char FileNum[];
extern const char FactorySet[];
extern const char MemFormat[];
extern const char MemReturn[];
//extern const char MemConfirm[];
extern const char VersionNum[];
extern const char MM_Vtg[];


//说明帮助
extern const char Update[];
extern const char *HelpMsg[15];


//ERR_BOX
extern const char Worning[];
extern const char WrnText[];
extern const char ReadOutTime[];


//input_widget
extern const char *pTextSpeed[];
extern const char *pTextPreamble[];


//主界面
extern const char CommParaSetText[];
extern const char CommStdTestText[];
extern const char ChannelPLCText[];
extern const char TFcardText[];
extern const char ReadMeterText[];
extern const char MonitorText[];
extern const char About[];
extern const char UploadIcon[];
extern const char DownloadIcon[];
extern const char Battery_100[];
extern const char Battery_80[];
extern const char Battery_60[];
extern const char Battery_40[];
extern const char Battery_20[];
extern const char Battery_00[];
//extern const char Battery_Charge[];
extern const char Protocol_07[];
extern const char Protocol_97[];

extern const char SysInfo[];

extern const char TSK_PLC[];
extern const char TSK_RF[];
extern const char TSK_IR[];
//beep
extern const char BeepOff[];
extern const char BeepOn[];

extern const char SD_Mount[];
extern const char SD_Unmount[];
extern const char Clear[];
extern const char Back[];
extern const char Log[];

//速率设置
extern const char FreqSet[];


extern const char LCDModule[];
extern const char PLCModule[];
extern const char SiModule[];
extern const char HardVersion[];
extern const char SotfVersion[];
extern const char SDInfo[];
extern const char HardCheck[];
extern const char HardwareOK[];
extern const char HardwareErr[];

extern const char CmpName[] ;

#endif /*__PHRASE_H__*/




