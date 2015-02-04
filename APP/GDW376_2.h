#ifndef GDW376_2_H_
#define GDW376_2_H_


//#include"Def_type.h"

/******************************新添加**********************************************/
//信息域R
#pragma pack(1)
typedef struct
{
 u8 Rout_Fg:1;
 u8 Side_Fg:1;
 u8 Comm_Fg:1;//通信模块标示
 u8 Clash_Fg:1;
 u8 Lever:4;
 u8 Channel:4;
 u8 Ecc_Fg:4;
 u8 Resp_cnt;  //应答字节数
 u16 bps:15;
 u16 Kilo:1;
 u8 rev;
}Info_C;
typedef struct
{
 u8 Rout_Fg:1;
 u8 Side_Fg:1;
 u8 Comm_Fg:1;//通信模块标示
 u8 Rev1:1;
 u8 Lever:4;
 u8 Channel:4;
 u8 Rev2:4;
 u8 Phase:4;  //相线
 u8 Amm_ch:4;
 u8 Singal1:4;
 u8 Singal2:4;
 u8 Event:1;
 u8 Rev3:7;
 u8 SEQ;
 //u16 Rev3;
}Info_up_C;
typedef struct
{
 u8  AFN;
 u8  DT[2];
 u8  Data[200];
}GDW3762_App_C;
//---------------------------------------------------------------------------------
typedef struct
{
    u8 Buff[50];
    u8 Status;
    u8 cnt;
    u8 Phase;
}GDW_3762Bak_C;
//---------------------------------------------------------------------------------
typedef struct
{
 u8 Start;
 u16 Length;
 u8  C;
 u8  Data[250];
}Frame_3762_C;
#pragma pack()
void GDW_3762_init(void);
void Create_GDW3762_Frame(u8 *Frame,u16 len);
u8 Check_GDW3762(u8 *p);
u16 GDW3762_Comm_Sub(u8 *buff);
u8 GDW3762_Meter_Data_RESP(Info_up_C *R,u8 CMD,u8 type,u8 *Addr,u8 *buff,u8 L,u8 *Send_buff);
u8 GDW3762_Search_Node_RESP(Info_up_C *R,u8 CMD,u8 *Addr,u8 *buff,u8 L,u8 *Send_buff);
u8 GDW_3762_NoResp(u8 CMD,u8 *meter_ID,u8 *Send_buff,u8 *dat);
void GDW3762_Three_Send(u8 PLC_ST);
u8 GDW_Seach_Phase(void);
u8 Create_AFN00_F02(u8 *buff,u8 dat);
u8 Create_AFN00_F01(u8 *buff,u8 dat,u16 Time);
u8 GDW3762_Meter_AFN00_RESP(Info_up_C *R,u8 *Addr,u8 Fn,u8 dat,u16 Time,u8 *Send_buff);
#endif
