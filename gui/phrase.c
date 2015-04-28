#include "phrase.h"
#include "includes.h"

/***************************************
*
*    参数设置
*
***************************************/
   
//通信参数设置
const char CommParaSet[]="\xe9\x80\x9a\xe4\xbf\xa1\xe5\x8f\x82\xe6\x95\xb0\xe8\xae\xbe\xe7\xbd\xae";
//接收数据延时
const char RevDataDelay[]="\xe6\x8e\xa5\xe6\x94\xb6\xe6\x95\xb0\xe6\x8d\xae\xe5\xbb\xb6\xe6\x97\xb6";
//执行间隔时间
const char BetweenAct[]="\xe6\x89\xa7\xe8\xa1\x8c\xe9\x97\xb4\xe9\x9a\x94\xe6\x97\xb6\xe9\x97\xb4";
//电力载波
const char WaveCarrier[]="\xe7\x94\xb5\xe5\x8a\x9b\xe7\xba\xbf\xe8\xbd\xbd\xe6\xb3\xa2";
//无线模式
const char ChannelWireless[]="\xe6\x97\xa0\xe7\xba\xbf\xe6\xa8\xa1\xe5\xbc\x8f";

//红外
const char Infrared[]="\xe7\xba\xa2\xe5\xa4\x96";
//规约
const char Protocal[]="\xe8\xa7\x84  \xe7\xba\xa6:";
//通道
const char Channel[]="\xe9\x80\x9a  \xe9\x81\x93:";
//波特率
const char BaudRate[]="\xe6\xb3\xa2\xe7\x89\xb9\xe7\x8e\x87:";
//前导符
const char Preamble[]="\xe5\x89\x8d\xe5\xaf\xbc\xe7\xac\xa6:";
//停止位
const char StopBit[]="\xe5\x81\x9c\xe6\xad\xa2\xe4\xbd\x8d:";
//个数
const char Number[]="\xe4\xb8\xaa  \xe6\x95\xb0:";
//密码
const char PassWord[]="\xe5\xaf\x86  \xe7\xa0\x81:";


//保存
const char Save[]="\xe4\xbf\x9d\xe5\xad\x98";
//取消
const char Cancel[]="\xe5\x8f\x96\xe6\xb6\x88";
//退出
const char Quit[]="\xe9\x80\x80\xe5\x87\xba";
//确认
const char Confirm[]="\xe7\xa1\xae\xe5\xae\x9a";
//操作码
const char OperateCode[] = "\xe6\x93\x8d\xe4\xbd\x9c\xe7\xa0\x81";


//时间设置
const char TimeSet[]="F1\xe6\x97\xb6\xe9\x97\xb4\xe8\xae\xbe\xe7\xbd\xae";

//屏幕超时
const char ScrTimeout[]="\xe5\xb1\x8f\xe5\xb9\x95\xe8\xb6\x85\xe6\x97\xb6(\xe7\xa7\x92)";



//高级设置
const char AdvanceSet[] = "F2\xe9\xab\x98\xe7\xba\xa7\xe8\xae\xbe\xe7\xbd\xae";


const GUI_ConstString pProtocolText[] = {
  "DL-T-97", "DL-T-07",NULL
};

const GUI_ConstString pChannelText[]={
    "\xe7\x94\xb5\xe5\x8a\x9b\xe8\xbd\xbd\xe6\xb3\xa2",
    "485",
    "\xe7\xba\xa2\xe5\xa4\x96",
};

/***************************************
*
*    时间设置
*
***************************************/


const char Year[]  ="\xe5\xb9\xb4:";
const char Month[] ="\xe6\x9c\x88:";
const char Day[]   ="\xe6\x97\xa5:";
const char Hour[]  ="\xe6\x97\xb6:";
const char Minite[]="\xe5\x88\x86:";
const char Second[]="\xe7\xa7\x92:";

const char TimeSet_24[]="\xe6\x97\xb6\xe9\x97\xb4\xe8\xae\xbe\xe7\xbd\xae(24\xe6\x97\xb6\xe5\x88\xb6)";
const char TypeDirection[]="24\xe5\xb0\x8f\xe6\x97\xb6\xe6\xa0\xbc\xe5\xbc\x8f";

/***************************************
*
*    抄表
*
***************************************/

//自动获取电表表号以及波特率
//获取参数
const char GetMeterNum[]="F1\xe8\x8e\xb7\xe5\x8f\x96\xe8\xa1\xa8\xe5\x8f\xb7";


//消息内容
const char Msg[]="#\xe6\xb6\x88\xe6\x81\xaf\xe5\x86\x85\xe5\xae\xb9";

//电表表号
const char MeterNum[]="\xe7\x94\xb5\xe8\xa1\xa8\xe8\xa1\xa8\xe5\x8f\xb7";


//抄表项目
const char ReadMeterSel[]="\xe6\x8a\x84\xe8\xa1\xa8\xe9\xa1\xb9\xe7\x9b\xae";


//当前正向有功电能
const char Positive[]="\xe5\xbd\x93\xe5\x89\x8d\xe6\xad\xa3\xe5\x90\x91\xe6\x9c\x89\xe5\x8a\x9f\xe7\x94\xb5\xe8\x83\xbd";


//当前反向有功电能
const char Negative[]="\xe5\xbd\x93\xe5\x89\x8d\xe5\x8f\x8d\xe5\x90\x91\xe6\x9c\x89\xe5\x8a\x9f\xe7\x94\xb5\xe8\x83\xbd";


//日冻结正向有功电能
const char DayPositive[]="\xe6\x97\xa5\xe5\x86\xbb\xe7\xbb\x93\xe6\xad\xa3\xe5\x90\x91\xe6\x9c\x89\xe5\x8a\x9f\xe7\x94\xb5\xe8\x83\xbd";


//日冻结反向有功电能
const char DayNegative[]="\xe6\x97\xa5\xe5\x86\xbb\xe7\xbb\x93\xe5\x8f\x8d\xe5\x90\x91\xe6\x9c\x89\xe5\x8a\x9f\xe7\x94\xb5\xe8\x83\xbd";

//总电量    ----
const char TotalVal[]="\xe6\x80\xbb\xe7\x94\xb5\xe9\x87\x8f";

//抄表
const char ReadMeter[]="F2\xe6\x8a\x84\xe8\xa1\xa8";

//速率
const char Speed[]="\xe9\x80\x9f    \xe7\x8e\x87:";

//前导符为FE时
const char FE_Speed[]="\xe6\xa0\x87\xe8\xaf\x86\xe7\xac\xa6\xe4\xb8\xba FE \xe6\x97\xb6";

//速率标识符
const char FRQ_Flag[]="\xe9\x80\x9f\xe7\x8e\x87\xe6\xa0\x87\xe8\xaf\x86\xe7\xac\xa6";





/*************************************************************
*
*               数据标识
**************************************************************/

const char _ReadMeterProj[][64] = 
{
    //当前正
    "\xe5\xbd\x93\xe5\x89\x8d\xe6\xad\xa3\xe5\x90\x91\xe6\x9c\x89\xe5\x8a\x9f\xe7\x94\xb5\xe8\x83\xbd",
    //当前反
    "\xe5\xbd\x93\xe5\x89\x8d\xe5\x8f\x8d\xe5\x90\x91\xe6\x9c\x89\xe5\x8a\x9f\xe7\x94\xb5\xe8\x83\xbd",
    //日冻结正
    "\xe6\x97\xa5\xe5\x86\xbb\xe7\xbb\x93\xe6\xad\xa3\xe5\x90\x91\xe6\x9c\x89\xe5\x8a\x9f\xe7\x94\xb5\xe8\x83\xbd",
    //日冻结反
    "\xe6\x97\xa5\xe5\x86\xbb\xe7\xbb\x93\xe5\x8f\x8d\xe5\x90\x91\xe6\x9c\x89\xe5\x8a\x9f\xe7\x94\xb5\xe8\x83\xbd",
    
    //"\xe6\xad\xa3\xe5\x90\x91\xe6\x9c\x89\xe5\x8a\x9f\xe7\x94\xb5\xe8\x83\xbd\xe6\x95\xb0\xe6\x8d\xae\xe5\x9d\x97",
    "",
};

/***************************************************************
*
*路由表
***************************************************************/

//中继地址
//const char RelayAddr[]="1-\xe4\xb8\xad\xe7\xbb\xa7\xe5\x9c\xb0\xe5\x9d\x80";

//中继地址。。。
const char _RelayAddr[]="\xe4\xb8\xad\xe7\xbb\xa7\xe5\x9c\xb0\xe5\x9d\x80";

//中继序号
const char RelayNum[]="\xe4\xb8\xad\xe7\xbb\xa7\xe5\xba\x8f\xe5\x8f\xb7";

//按增加
const char Add[]="\xe6\x8c\x89 # \xe5\xa2\x9e\xe5\x8a\xa0";

//删除
const char Delete[]="\xe6\x8c\x89 F1 \xe5\x88\xa0\xe9\x99\xa4";

//修改
const char Modify[]="\xe6\x8c\x89 F2 \xe4\xbf\xae\xe6\x94\xb9";
//倒计时
const char MND_countdown[] = "\xe5\x80\x92\xe8\xae\xa1\xe6\x97\xb6 (\xe7\xa7\x92):";

//读取中
const char MND_reading[] = "\xe8\xaf\xbb\xe5\x8f\x96\xe4\xb8\xad...";

//开始
const char RelayStart[]="\xe6\x8c\x89 * \xe5\xbc\x80 \xe5\xa7\x8b";

//增加中继地址
const char AddRlyAddr[] = "\xe5\xa2\x9e\xe5\x8a\xa0\xe4\xb8\xad\xe7\xbb\xa7\xe5\x9c\xb0\xe5\x9d\x80";

//修改中继地址
const char ModifyRlyAddr[] = "\xe4\xbf\xae\xe6\x94\xb9\xe4\xb8\xad\xe7\xbb\xa7\xe5\x9c\xb0\xe5\x9d\x80";
//启动路由
const char StartRoute[]="*\xe5\x90\xaf\xe5\x8a\xa8\xe8\xb7\xaf\xe7\x94\xb1";

//路由已启动
const char AlreadyStart[]="*\xe8\xb7\xaf\xe7\x94\xb1\xe5\xb7\xb2\xe5\xbc\x80\xe5\x90\xaf";

//路由已关闭
const char AlreadyClosed[] ="*\xe8\xb7\xaf\xe7\x94\xb1\xe5\xb7\xb2\xe5\x85\xb3\xe9\x97\xad";


// 错误提示
const char AddrErr[]= "\xe5\x9c\xb0\xe5\x9d\x80\xe9\x94\x99\xe8\xaf\xaf!";
const char DataFlagErr[]="\xe6\x95\xb0\xe6\x8d\xae\xe6\xa0\x87\xe8\xaf\x86\xe9\x94\x99\xe8\xaf\xaf!";
const char ProtocolErr[]="\xe8\xa7\x84\xe7\xba\xa6\xe4\xb8\x8d\xe5\x8c\xb9\xe9\x85\x8d!";
const char MonthOutRange[]="\xe6\x9c\x88\xe4\xbb\xbd\xe8\xb6\x85\xe5\x87\xba\xe8\x8c\x83\xe5\x9b\xb4!";
const char DayOutRange[]  ="\xe6\x97\xa5\xe6\x9c\x9f\xe8\xb6\x85\xe5\x87\xba\xe8\x8c\x83\xe5\x9b\xb4!";
const char HourOutRange[] ="\xe6\x97\xb6\xe9\x97\xb4\xe8\xb6\x85\xe5\x87\xba\xe8\x8c\x83\xe5\x9b\xb4!";
const char MinOutRange[]  ="\xe5\x88\x86\xe9\x92\x9f\xe8\xb6\x85\xe5\x87\xba\xe8\x8c\x83\xe5\x9b\xb4!";

const char ConfirmFormat[]="\xe7\xa1\xae\xe5\xae\x9a\xe6\xa0\xbc\xe5\xbc\x8f\xe5\x8c\x96\xe5\x86\x85\xe5\xad\x98\xe5\x90\x97\xef\xbc\x9f?";

const char UpdateConfirm[]="\xe7\xa1\xae\xe8\xae\xa4\xe5\x8d\x87\xe7\xba\xa7\xe5\x90\x97\xef\xbc\x9f?";
const char RlyOutRange[]="\xe8\xb6\x85\xe5\x87\xba\xe6\x9c\x80\xe5\xa4\xa7\xe4\xb8\xad\xe7\xbb\xa7\xe6\xb7\xb1\xe5\xba\xa6";
const char ReadOutTime[] = "\xe8\xaf\xb7\xe6\xb1\x82\xe8\xb6\x85\xe6\x97\xb6!!!";


const char *gc_messageBoxText[] = 
{
    AddrErr,
    DataFlagErr,
    ProtocolErr,
    MonthOutRange,
    DayOutRange,
    HourOutRange,
    MinOutRange,   
    ConfirmFormat,
    UpdateConfirm,
    RlyOutRange,
    ReadOutTime,
    
};

/************************************************
*
*       规约调试
*************************************************/

//通信规约调试
const char CommStdTest[]="\xe9\x80\x9a\xe4\xbf\xa1\xe8\xa7\x84\xe7\xba\xa6\xe8\xb0\x83\xe8\xaf\x95";

//1-目标地址
const char TarAddr[]="\xe7\x9b\xae\xe6\xa0\x87\xe5\x9c\xb0\xe5\x9d\x80";

//2-控制字
const char CtlCode[]="\xe6\x8e\xa7 \xe5\x88\xb6 \xe5\xad\x97";

//3-长度
const char Length[]="\xe9\x95\xbf    \xe5\xba\xa6";

//4-数据标识
const char DataSign[]="\xe6\x95\xb0\xe6\x8d\xae\xe6\xa0\x87\xe8\xaf\x86";

//5-数据域
const char DataField[]="\xe6\x95\xb0    \xe6\x8d\xae";

//F1路由表
const char RoutTab[]="F1\xe8\xb7\xaf\xe7\x94\xb1\xe8\xa1\xa8";

//F2数据标识
const char DataSignBtn[]="F2\xe6\x95\xb0\xe6\x8d\xae\xe6\xa0\x87\xe8\xaf\x86";

//读设备地址
const char GetAddr[]="\xe8\xaf\xbb\xe8\xae\xbe\xe5\xa4\x87\xe5\x9c\xb0\xe5\x9d\x80";

//读数据
const char Readdata[]="\xe8\xaf\xbb\xe6\x95\xb0\xe6\x8d\xae";

//写数据
const char WriteData[]="\xe5\x86\x99\xe6\x95\xb0\xe6\x8d\xae";

//读取产品芯片型号
const char ReadChip[]="\xe8\xaf\xbb\xe5\x8f\x96\xe4\xba\xa7\xe5\x93\x81\xe8\x8a\xaf\xe7\x89\x87\xe5\x9e\x8b\xe5\x8f\xb7";
//清需量
const char ClrDemond[] = "\xe6\xb8\x85\xe9\x9c\x80\xe9\x87\x8f";

//发送
 const char Send[]="\xe5\x8f\x91\xe9\x80\x81";


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
    OperateCode,
    _RelayAddr,  //中继地址
    AddRlyAddr,
    ModifyRlyAddr,
    
    //ConfirmUpdata
};

const char *pCtlCode[] = {
     GetAddr,
     Readdata,
     WriteData,
};

const char *pReadSel[] = {
    Positive,
    Negative,
    DayPositive,
    DayNegative,
};



/***********************************************
*
*           状态监控
***********************************************/
//设置命令选择
 const char CmdSel[]="\xe8\xae\xbe\xe7\xbd\xae\xe9\x80\x89\xe6\x8b\xa9";

//发送数据
// const char SendData[]="\xe5\x8f\x91\xe9\x80\x81\xe6\x95\xb0\xe6\x8d\xae";
 const char Data[]="\xe6\x95\xb0\xe6\x8d\xae";

//接收数据
//const char RevData[]="\xe6\x8e\xa5\xe6\x94\xb6\xe6\x95\xb0\xe6\x8d\xae";

//设为监控器状态 
const char SetMonitor[]="F1\xe8\xae\xbe\xe4\xb8\xba\xe7\x9b\x91\xe6\x8e\xa7\xe6\x80\x81";

//设为抄控器状态
const char SetCopy[]="F2\xe8\xae\xbe\xe4\xb8\xba\xe6\x8a\x84\xe6\x8e\xa7\xe6\x80\x81";

//读取模块状态
const char ReadModule[]="\xe8\xaf\xbb\xe5\x8f\x96\xe6\xa8\xa1\xe5\x9d\x97\xe7\x8a\xb6\xe6\x80\x81";

//读载波节点
const char ReadNope[]="#\xe8\xaf\xbb\xe8\xbd\xbd\xe6\xb3\xa2\xe8\x8a\x82\xe7\x82\xb9";

//设置
const char Sel[]="#\xe8\xae\xbe\xe7\xbd\xae";

//停止
const char Stop[]="F2 \xe5\x81\x9c \xe6\xad\xa2";

//清除
const char ClearScr[]="* \xe6\xb8\x85 \xe9\x99\xa4";

//开始
const char Start[]="F1 \xe5\xbc\x80 \xe5\xa7\x8b";

/****************************************************
*
*       内存管理 
*****************************************************/


const char MemSize[]="\xe5\xae\xb9\xe9\x87\x8f";
const char MemUsage[]="\xe4\xbd\xbf\xe7\x94\xa8\xe7\x8e\x87";
const char FileNum[]="\xe6\x96\x87\xe4\xbb\xb6\xe6\x95\xb0\xe7\x9b\xae";
const char FactorySet[]="\xe6\x81\xa2\xe5\xa4\x8d\xe5\x87\xba\xe5\x8e\x82\xe8\xae\xbe\xe7\xbd\xae";
const char MemFormat[]="\xe6\xa0\xbc\xe5\xbc\x8f\xe5\x8c\x96";
const char MemReturn[]="\xe8\xbf\x94\xe5\x9b\x9e";
//const char MemConfirm[]="F1\xe7\xa1\xae\xe5\xae\x9a";
const char VersionNum[]="\xe7\x89\x88\xe6\x9c\xac\xe5\x8f\xb7:2015.04.21";
const char MM_Vtg[] = "\xe7\x94\xb5\xe6\xb1\xa0\xe7\x94\xb5\xe5\x8e\x8b(V)";

/****************************************************
*
*       说明帮助
*****************************************************/

const char Update[]="\xe5\x8d\x87\xe7\xba\xa7";


const char *HelpMsg[15]={
    "1.\xe7\xbb\xbf\xe8\x89\xb2\xe5\x92\x8c\xe9\xbb\x84\xe8\x89\xb2\xe6\x8c\x89\xe9\x94\xae",
        
    "\xe5\xaf\xb9\xe5\xba\x94\xe7\x9a\x84\xe5\xb0\xb1\xe6\x98\xaf\xe5\xb1\x8f\xe5\xb9\x95\xe4\xb8\xad",
    
    "\xe7\x9a\x84\xe4\xb8\xa4\xe4\xb8\xaa\xe7\x9b\xb8\xe5\x90\x8c\xe9\xa2\x9c\xe8\x89\xb2\xe7\x9a\x84",
    
    "\xe6\x8c\x89\xe9\x92\xae(\xef\xbc\x88\xe7\xa1\xae\xe5\xae\x9a\xe6\x88\x96\xe8\x80\x85\xe9\x80\x80\xe5\x87\xba\xef\xbc\x89)\n\n",

    "2.\xe7\x94\xa8\xe4\xb8\x8a\xe4\xb8\x8b\xe6\x96\xb9\xe5\x90\x91\xe9\x94\xae\xe5\x8f\xaf\xe4\xbb\xa5",

    "\xe5\xaf\xb9\xe7\x9b\xb8\xe5\xba\x94\xe9\x80\x89\xe9\xa1\xb9\xe8\xbf\x9b\xe8\xa1\x8c\xe9\x80\x89\xe6\x8b\xa9,",

    "\xe7\x94\xa8TAB\xe9\x94\xae\xe4\xb9\x9f\xe5\x8f\xaf\xe4\xbb\xa5\xe8\xbf\x9b\xe8\xa1\x8c\xe5\x88\x87\xe6\x8d\xa2\n\n",

    "3.\xe6\x8c\x89\xe9\x92\xae\xe4\xb8\x8a\xe6\xa0\x87\xe6\xb3\xa8\xe7\x9a\x84\xe5\xad\x97\xe7\xac\xa6",

    "\xe6\x88\x96\xe5\xad\x97\xe6\xaf\x8d\xe6\x98\xaf\xe7\x9b\xb8\xe5\xba\x94\xe9\x80\x89\xe9\xa1\xb9\xe7\x9a\x84",

    "\xe5\xbf\xab\xe6\x8d\xb7\xe6\x96\xb9\xe5\xbc\x8f\xef\xbc\x8c\xe7\x9b\xb4\xe6\x8e\xa5\xe7\x82\xb9\xe5\x87\xbb",

    "\xe5\x8d\xb3\xe5\x8f\xaf\xe8\xbf\x9b\xe8\xa1\x8c\xe7\x9b\xb8\xe5\x85\xb3\xe6\x93\x8d\xe4\xbd\x9c\n\n",

    "4.\xe9\x80\x89\xe4\xb8\xad\xe7\x9b\xb8\xe5\xba\x94\xe9\x80\x89\xe9\xa1\xb9\xe5\x90\x8e\xef\xbc\x8c",

    "\xe6\x8c\x89OK\xe9\x94\xae\xe8\xbf\x9b\xe5\x85\xa5\xe7\xbc\x96\xe8\xbe\x91",
};

//
//ERR_BOX

const char Worning[]="\xe8\xad\xa6\xe5\x91\x8a";
const char WrnText[]="\xe7\xa1\xae\xe5\xae\x9a\xe6\xa0\xbc\xe5\xbc\x8f\xe5\x8c\x96\xe5\x86\x85\xe5\xad\x98\xe5\x90\x97\xef\xbc\x9f?";


//input_widget
const char *pTextSpeed[]={

    "\xe8\xae\xbe\xe4\xb8\xba 270 III \xe4\xbb\xa3",
    "\xe8\xae\xbe\xe4\xb8\xba 270 3.5 \xe4\xbb\xa3",
    "\xe8\xae\xbe\xe4\xb8\xba 270 II \xe4\xbb\xa3",
    "\xe8\xae\xbe\xe4\xb8\xba 421 50BPS",
    "\xe8\xae\xbe\xe4\xb8\xba 421 100BPS",
    "\xe8\xae\xbe\xe4\xb8\xba 421 600BPS",
    "\xe8\xae\xbe\xe4\xb8\xba 421 1200BPS",
    
};

const char *pTextPreamble[]={

    "FC(270 III \xe4\xbb\xa3)",
    "FA(421 1200BPS)",
    "FB(421 600BPS)",
    "FD(421 100BPS)",
    "FF(421 50BPS)",
    "FE(\xe6\x89\xa9\xe5\xb1\x95\xe8\xae\xbe\xe7\xbd\xae)",
    
};

/*******************************************
*
*           主界面
********************************************/


//通信参数设置
const char CommParaSetText[]="\xe5\x8f\x82\xe6\x95\xb0\xe8\xae\xbe\xe7\xbd\xae";

//通信规约调试
const char CommStdTestText[]="\xe8\xa7\x84\xe7\xba\xa6\xe8\xb0\x83\xe8\xaf\x95";

//载波功能设置
const char WaveCarrierText[]="\xe8\xbd\xbd\xe6\xb3\xa2\xe8\xae\xbe\xe7\xbd\xae";

//内存管理
const char TFcardText[]="\xe5\x86\x85\xe5\xad\x98\xe7\xae\xa1\xe7\x90\x86";

//系统信息
const char SysInfo[] = "\xe7\xb3\xbb\xe7\xbb\x9f\xe4\xbf\xa1\xe6\x81\xaf";


//智能抄表
const char ReadMeterText[]="\xe6\x99\xba\xe8\x83\xbd\xe6\x8a\x84\xe8\xa1\xa8";

//状态监控
const char MonitorText[]="\xe7\x8a\xb6\xe6\x80\x81\xe7\x9b\x91\xe6\x8e\xa7";

//帮助说明
const char About[]="\xe8\xaf\xb4\xe6\x98\x8e\xe5\xb8\xae\xe5\x8a\xa9";

//上行标志

const char UploadIcon[]="\xe5\xa3\xb9";

//\xe8\xb4\xb0

//下行标志
const char DownloadIcon[]="\xe8\xb4\xb0";

//十玖捌柒陆伍
//\xe5\x8d\x81
//\xe7\x8e\x96
//\xe6\x8d\x8c
//\xe6\x9f\x92
//\xe9\x99\x86
//\xe4\xbc\x8d
//电池
const char Battery_100[]="\xe5\x8d\x81";
const char Battery_80[] ="\xe7\x8e\x96";
const char Battery_60[] ="\xe6\x8d\x8c";
const char Battery_40[] ="\xe6\x9f\x92";
const char Battery_20[] ="\xe9\x99\x86";
const char Battery_00[] ="\xe4\xbc\x8d";
//const char Battery_Charge[] = "\xe8\x82\x86";
const char TSK_Wireless[] = "\xe8\x82\x86";

//监控
// const char MonitorIcon[]="\xe8\x82\x86";
//关闭监控
// const char CloseMonitor[]="\xe5\x8f\x81";

//当前规约:DL-T-07
// const char Protocol_07[]="\xe8\xa7\x84\xe7\xba\xa6:DLT-07";
const char Protocol_07[]="DLT-07";


const char Protocol_97[]="DLT-97";


/***************************************************************
*
*消息记录
***************************************************/
//清除
const char Clear[]="\xe6\xb8\x85\xe9\x99\xa4";
//返回
const char Back[]="\xe8\xbf\x94\xe5\x9b\x9e";

//日志
const char Log[]="\xe6\x97\xa5\xe5\xbf\x97";

/****************************************************************
*
*           速率设置
*****************************************************************/
//设置

const char FreqSet[]="\xe8\xae\xbe\xe7\xbd\xae";


//系统信息
const char LCDModule[] = "\xe6\xb6\xb2\xe6\x99\xb6\xe6\xa8\xa1\xe5\x9d\x97";
const char PLCModule[] = "\xe8\xbd\xbd\xe6\xb3\xa2\xe6\xa8\xa1\xe5\x9d\x97";
const char SiModule[] = "\xe6\x97\xa0\xe7\xba\xbf\xe6\xa8\xa1\xe5\x9d\x97";
const char HardVersion[] = "\xe7\xa1\xac\xe4\xbb\xb6\xe7\x89\x88\xe6\x9c\xac";
const char SotfVersion[] = "\xe8\xbd\xaf\xe4\xbb\xb6\xe7\x89\x88\xe6\x9c\xac";
const char SDInfo[] = "SD\xe5\x8d\xa1\xe4\xbf\xa1\xe6\x81\xaf";
const char HardCheck[] = "\xe7\xa1\xac\xe4\xbb\xb6\xe6\xa3\x80\xe6\xb5\x8b";
const char HardwareErr[] = "\xe5\xbc\x82\xe5\xb8\xb8";
const char HardwareOK[] = "\xe6\xad\xa3\xe5\xb8\xb8";









