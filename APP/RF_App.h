#ifndef _RF_APP_H_
#define _RF_APP_H_


//#include "includes.h"

typedef struct
{
 	u8 CMD;
    u8 Router_ID[6];
	u8 meter_ID[6];
	u8 DC_ID[6];
	u8 Protol;
    u8 Obj[2];
}PLC_Copy_Status_C;

typedef struct
{
    u8 * buff; //[Usart_Buf_MAX+2];
    u8 i;
    u8 Timeout;
} USART_Rev_C;

//--------------------------------------------------------------------------------------------------------------------------------
typedef struct
{
	u8	FCD;
	u8	SEQ;
	u8	DTI_bps;
	u8	Data[100];
}GDW_RF_App_layer_C;
#pragma pack(1)
typedef struct
{
	u8	cnt:5;
	u8	Index:3;
	u16	rev:2;
	u16	mod:14;
	u8 	Pad[100];
}Rout_Inf_C;
#pragma pack()
typedef struct
{
	u8	Frame_type:2;
	u8	Dst_Addr_mod:2;
	u8	Scr_Addr_mod:2;
	u8	Rev:1;
	u8	Rout_Fg:1;
	u8	Dst_Addr[6];
	u8	Scr_Addr[6];
	u8	Radius:4;
	u8	SEQ:4;
	u8	Pad[100];
}GDW_RF_Network_layer_C;
//--------------------------------------------------------------------
#pragma pack(1)
typedef struct
{
	u8	TXDelayed;
	u8	RoundCount;
	u16	TSNum:10;
	u16	RanKCount:4;
	u16	Rev:2;
	u8	BeaconMark;
	u16	PANSize;
	u8	RSSIThreshold;
	u16	CentrePANId;
	u8	CentreAddr[6];
}MLME_BEACON_C;

typedef struct
{
	u8	Frame_type:3;
	u8	Safety_EN:1;
	u8 	Pend:1;
	u8	ASK:1;
	u8	Pan_ID_zip:1;
	u8	Rev:1;
	u8	SEQ_zip:1;
	u8	Expend:1;
	u8	Addr_D_mod:2;
	u8	Ver:2;
	u8 	Addr_S_mod:2;
	u8	SEQ;
	u16	PanID;
	u8	Dst_Addr[6];
	u8	Scr_Addr[6];
	u8	Pad[100];
}GDW_RF_MAC_layer_C;
#pragma pack()
typedef struct
{
	u8	chan;
	u8 	ID;
	u8 	Xor;
	u8 	Pad[100];
}GDW_RF_PHY_layer_C;
u8  RF_Rev_Phy_Sub(u8 *buf,u8 l);
u8	GDW_RF_Protocol_2013(u8 *Addr,u8 leve,u8 CMD,u8 type,u8 *data,u8 len,u8 *send_buf);
#endif
