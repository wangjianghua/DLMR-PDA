#include "includes.h"
//#include "CG_serial.h"


PLC_Copy_Status_C Mode_Status;
USART_Rev_C User_Com;

u8 RF_App_SEQ=0;
u8 Network_SEQ=0;
u8 Pro_III5_SEQ=0;
u16	Pan_ID;
u8 Main_Addr[6];

#define GDW_MAC_SEQ 	Pro_III5_SEQ

/****************************************GDW*******************************************************************************/
u8	RF_Rev_App_Sub(u8 *buf,u8 leng,u8 *buff)
{
	GDW_RF_App_layer_C	*GDW_RF_App_layer_ptr;
	u8 i;
	
	GDW_RF_App_layer_ptr=(GDW_RF_App_layer_C		*)buf;
	for(i=0;i<8;i++)
	{
		if(GDW_RF_App_layer_ptr->Data[i]==0x68)break;
	}
	memcpy(buff,GDW_RF_App_layer_ptr->Data+i,leng-3-i);
	return(leng-3-i);
}

u8	RF_Rev_Network_Sub(u8 *buf,u8 leng,GDW3762_App_C *GDW3762_App_Ptr)
{
	GDW_RF_Network_layer_C	*GDW_RF_Network_layer_ptr;
	Rout_Inf_C	*Rout_Inf_ptr;
	u8  	length=0;
	
	GDW_RF_Network_layer_ptr=(GDW_RF_Network_layer_C	*)buf;
	if(GDW_RF_Network_layer_ptr->Rout_Fg)//有路由信息
		Rout_Inf_ptr=(Rout_Inf_C	*)GDW_RF_Network_layer_ptr->Pad;
	if(GDW_RF_Network_layer_ptr->Radius)GDW_RF_Network_layer_ptr->Radius--;
	
	GDW3762_App_Ptr->AFN=Mode_Status.CMD;
	GDW3762_App_Ptr->DT[0]=1;
	GDW3762_App_Ptr->DT[1]=0;
	GDW3762_App_Ptr->Data[0]=Mode_Status.Protol;
			
	if(GDW_RF_Network_layer_ptr->Frame_type==0)//数据帧
	{
		if(!GDW_RF_Network_layer_ptr->Radius)
		{
			if(GDW_RF_Network_layer_ptr->SEQ==Network_SEQ)
				{
					Network_SEQ=GDW_RF_Network_layer_ptr->SEQ;
					//Ans_type=CompareAddressArea(0,GDW_RF_Network_layer_ptr->Dst_Addr);
					// if((Ans_type==COMPARE_FLAG_SINGLE_NO)||(Ans_type==COMPARE_FLAG_BROADCAST))
					 {
						if(GDW_RF_Network_layer_ptr->Rout_Fg)//有路由信息
						{
							length=RF_Rev_App_Sub(Rout_Inf_ptr->Pad+(Rout_Inf_ptr->cnt*6),leng-(14+3+Rout_Inf_ptr->cnt*6),GDW3762_App_Ptr->Data+2);
							GDW3762_App_Ptr->Data[1]=length;
						}
						else
						{
							length=RF_Rev_App_Sub(GDW_RF_Network_layer_ptr->Pad,leng-14,GDW3762_App_Ptr->Data+2);
							GDW3762_App_Ptr->Data[1]=length;
								
						}
						length+=5;
						
					 }
				}
		}
		
	}
	else
	if(GDW_RF_Network_layer_ptr->Frame_type==1)
	{

	}
	return(length);
}

u8	RF_Rev_MAC_Sub(u8 *buf,Frame_3762_C *Frame_3762_Ptr,u8 leng)
{
	GDW_RF_MAC_layer_C	*GDW_RF_MAC_layer_ptr;
	GDW3762_App_C *GDW3762_App_Ptr;
	u8 length=0;
	GDW_RF_MAC_layer_ptr=(GDW_RF_MAC_layer_C	*)buf;
	if(GDW_RF_MAC_layer_ptr->Frame_type==0)//组网
	{

	}
	else
	if(GDW_RF_MAC_layer_ptr->Frame_type==1)//数据帧
	{
	        if(memcmp(Main_Addr,GDW_RF_MAC_layer_ptr->Dst_Addr,6))return(0);//主节点地址和目标地址不同就退出
		if(GDW_RF_MAC_layer_ptr->Pan_ID_zip)
		{
			if((GDW_RF_MAC_layer_ptr->PanID==0xFFFF)||(GDW_RF_MAC_layer_ptr->PanID==Pan_ID))//网络ID相同，或是广播
			{
				memcpy(Frame_3762_Ptr->Data+6,GDW_RF_MAC_layer_ptr->Scr_Addr,6);
				memcpy(Frame_3762_Ptr->Data+12,GDW_RF_MAC_layer_ptr->Dst_Addr,6);
				GDW3762_App_Ptr=(GDW3762_App_C *)(Frame_3762_Ptr->Data+18);
				
				length=RF_Rev_Network_Sub(GDW_RF_MAC_layer_ptr->Pad,leng-17,GDW3762_App_Ptr);
				if(length)
					{
						length+=12;
					}
				 
			}
		}
	}
	else
	if(GDW_RF_MAC_layer_ptr->Frame_type==2)//确认帧
	{
	
	}
	else
	if(GDW_RF_MAC_layer_ptr->Frame_type==3)//命令帧
	{

	}
	return(length);
}
//----------------------------------------------------------------------------------------
u8  RF_Rev_Phy_Sub(u8 *buf,u8 len)
{
	GDW_RF_PHY_layer_C	*GDW_RF_PHY_layer_ptr;
	u8 length=0,xor;
	Frame_3762_C *Frame_3762_Ptr;
    	
    	Info_up_C *R;
	
	GDW_RF_PHY_layer_ptr=(GDW_RF_PHY_layer_C  *)buf;
	xor=(len^GDW_RF_PHY_layer_ptr->chan)^GDW_RF_PHY_layer_ptr->ID;
	//if((GDW_RF_PHY_layer_ptr->chan==0)&&(GDW_RF_PHY_layer_ptr->ID==0x01))
	if((xor==GDW_RF_PHY_layer_ptr->Xor)&&(GDW_RF_PHY_layer_ptr->chan<66)&&(GDW_RF_PHY_layer_ptr->ID<99)&&(GDW_RF_PHY_layer_ptr->ID))
	{
	        Frame_3762_Ptr=(Frame_3762_C *)User_Com.buff;
	        Frame_3762_Ptr->Start=0x68;
	        Frame_3762_Ptr->C=0x81;
	        memset(Frame_3762_Ptr->Data,0,6);
	        R=(Info_up_C *)Frame_3762_Ptr->Data;
	        R->Rout_Fg=1;
	        R->Comm_Fg=1;
	        R->Lever=0;
		R->Event=0;
		length=RF_Rev_MAC_Sub(GDW_RF_PHY_layer_ptr->Pad,Frame_3762_Ptr, len-3);
		if(length)
			{
			        length+=6;
				Frame_3762_Ptr->Length=length+6;
       				Create_GDW3762_Frame(User_Com.buff,Frame_3762_Ptr->Length-2);

                    /* 华兄
                    UART1_SendData(User_Com.buff, Frame_3762_Ptr->Length);
    				RS485_Wait_Timeout(0,1);
    				*/
			}
	}
	_sys_plc_reset();
        return(1);
}
/********************************************************************************************************/
u8	Greate_GDW_App_Sub(u8 type,u8 *data,u8 len,u8 *send_buf)
{
	GDW_RF_App_layer_C		*GDW_RF_App_layer_ptr;
	
	GDW_RF_App_layer_ptr=(GDW_RF_App_layer_C		*)send_buf;
	GDW_RF_App_layer_ptr->FCD=2;

#if 1    
	RF_App_SEQ++;
	if(!RF_App_SEQ)RF_App_SEQ=1;
	GDW_RF_App_layer_ptr->SEQ=RF_App_SEQ;
#else //华兄
    GDW_RF_App_layer_ptr->SEQ = 0;
#endif

	GDW_RF_App_layer_ptr->DTI_bps=0;
	memcpy(GDW_RF_App_layer_ptr->Data,data,len);
	return(len+3);
}
//--------------------------------------------------------------------------------------------------------------------------
//u8 Network_SEQ=0;
u8	Greate_GDW_Network_Sub(u8 *Addr,u8 leve,u8 type,u8 *data,u8 len,u8 *send_buf)
{
	GDW_RF_Network_layer_C	*GDW_RF_Network_layer_ptr;
	Rout_Inf_C	*Rout_Inf_ptr;
	u8  	length=0;
	u8 i;

	GDW_RF_Network_layer_ptr=(GDW_RF_Network_layer_C	*)send_buf;
	GDW_RF_Network_layer_ptr->Frame_type=0;
	GDW_RF_Network_layer_ptr->Dst_Addr_mod=3;
	GDW_RF_Network_layer_ptr->Scr_Addr_mod=3;
	GDW_RF_Network_layer_ptr->Rev=0;
	if(leve)GDW_RF_Network_layer_ptr->Rout_Fg=1;
	else
		GDW_RF_Network_layer_ptr->Rout_Fg=0;
	memcpy(GDW_RF_Network_layer_ptr->Dst_Addr,Addr+((leve+1)*6),6);
	memcpy(GDW_RF_Network_layer_ptr->Scr_Addr,Addr,6);
	Network_SEQ++;
	if(Network_SEQ>0x0F)Network_SEQ=1;
	GDW_RF_Network_layer_ptr->SEQ=Network_SEQ;
	GDW_RF_Network_layer_ptr->Radius=leve+1;
	length=14;
	if(GDW_RF_Network_layer_ptr->Rout_Fg)
	{
		Rout_Inf_ptr=(Rout_Inf_C	*)GDW_RF_Network_layer_ptr->Pad;
		Rout_Inf_ptr->cnt=leve;
		Rout_Inf_ptr->Index=leve;
		Rout_Inf_ptr->mod=0;
		Rout_Inf_ptr->rev=0;
		length+=3;
		Rout_Inf_ptr->mod=0x3FFF;
		for(i=0;i<leve;i++)
		{
			memcpy(Rout_Inf_ptr->Pad+(i*6),Addr+((leve-i)*6),6);
			length+=6;
		}
		length+=Greate_GDW_App_Sub(type,data, len,Rout_Inf_ptr->Pad+(leve*6));
	}
	else
	{//无路由
		length+=Greate_GDW_App_Sub(type,data, len,GDW_RF_Network_layer_ptr->Pad);
	}
	return(length);
}

//------------------------------------------------------------------------------------------------------------------------

void  GDW_Greate_MAC_Inf(GDW_RF_MAC_layer_C	*GDW_RF_MAC_layer_ptr,u8 type,u16 PanID)
{
	GDW_RF_MAC_layer_ptr->Frame_type=type;
	GDW_RF_MAC_layer_ptr->Safety_EN=0;
	GDW_RF_MAC_layer_ptr->Pend=0;
	GDW_RF_MAC_layer_ptr->ASK=0; //华兄
	GDW_RF_MAC_layer_ptr->Pan_ID_zip=1;
	GDW_RF_MAC_layer_ptr->Rev=0;
	GDW_RF_MAC_layer_ptr->SEQ_zip=1;
	GDW_RF_MAC_layer_ptr->Expend=0;
	GDW_RF_MAC_layer_ptr->Addr_D_mod=3;
	GDW_RF_MAC_layer_ptr->Ver=0;
	GDW_RF_MAC_layer_ptr->Addr_S_mod=3;
	GDW_MAC_SEQ++;
	GDW_RF_MAC_layer_ptr->SEQ=GDW_MAC_SEQ;
	GDW_RF_MAC_layer_ptr->PanID=PanID;
}
u8  Greate_GDW_MAC_net(u8	BeaconMark,u16	PANSize,u8 RSSIThreshold,u16 CentrePANId,u8 *CentreAddr,u8 *send_buf)
{
	GDW_RF_MAC_layer_C	*GDW_RF_MAC_layer_ptr;
	MLME_BEACON_C	*ptr;
	//u8 length=0;

	GDW_RF_MAC_layer_ptr=(GDW_RF_MAC_layer_C	*)send_buf;
	GDW_Greate_MAC_Inf(GDW_RF_MAC_layer_ptr,0,0xFFFF);
	memset(GDW_RF_MAC_layer_ptr->Dst_Addr,0xFF,6);
	memset(GDW_RF_MAC_layer_ptr->Scr_Addr,0xAA,6);
	ptr=(MLME_BEACON_C	*)GDW_RF_MAC_layer_ptr->Pad;
	ptr->TXDelayed=0;
	ptr->RoundCount=0;
	ptr->TSNum=0;
	ptr->RanKCount=0;
	ptr->Rev=0;
	ptr->BeaconMark=BeaconMark;
	ptr->PANSize=PANSize;
	ptr->RSSIThreshold=RSSIThreshold;
	ptr->CentrePANId=CentrePANId;
	memcpy(ptr->CentreAddr,CentreAddr,6);
	return(17+sizeof(MLME_BEACON_C));
	
}

u8  Greate_GDW_MAC_layer(u8 *Addr,u8 leve,u8 type,u8 *data,u8 len,u8 *send_buf)
{
	GDW_RF_MAC_layer_C	*GDW_RF_MAC_layer_ptr;
	u8 length=0;

	GDW_RF_MAC_layer_ptr=(GDW_RF_MAC_layer_C	*)send_buf;
	GDW_Greate_MAC_Inf(GDW_RF_MAC_layer_ptr,1,0xFFFF);
	memcpy(GDW_RF_MAC_layer_ptr->Dst_Addr,Addr+6,6);
	memcpy(GDW_RF_MAC_layer_ptr->Scr_Addr,Addr,6);
	memcpy(Main_Addr,Addr,6);
	length=17;
	length+=Greate_GDW_Network_Sub(Addr,leve,type,data,len,GDW_RF_MAC_layer_ptr->Pad);
	return(length);
}
//------------------------------------------------------------------------------------------------------------------------
u8	GDW_RF_Protocol_2013(u8 *Addr,u8 leve,u8 CMD,u8 type,u8 *data,u8 len,u8 *send_buf)
{
	GDW_RF_PHY_layer_C	*GDW_RF_PHY_layer_ptr;
	u8 length=0;

	Mode_Status.CMD=CMD;
	Mode_Status.Protol=type;
	  _sys_plc_stop();
	GDW_RF_PHY_layer_ptr=(GDW_RF_PHY_layer_C  *)send_buf;
	GDW_RF_PHY_layer_ptr->chan=0;
	GDW_RF_PHY_layer_ptr->ID=0x01;
	 length=Greate_GDW_MAC_layer(Addr,leve, type,data,len,GDW_RF_PHY_layer_ptr->Pad);
	 length+=3;
	 GDW_RF_PHY_layer_ptr->Xor=(length^GDW_RF_PHY_layer_ptr->chan)^GDW_RF_PHY_layer_ptr->ID;
	 RF_Tx(send_buf, length);
	 return(length);
}
//----------------------------------------------开始组网------------------------------------------
u8	GDW_Create_net(u8	BeaconMark,u16	PANSize,u8 RSSIThreshold,u16 CentrePANId,u8 *CentreAddr,u8 *send_buf)
{
	GDW_RF_PHY_layer_C	*GDW_RF_PHY_layer_ptr;
	u8 length=0;
	_sys_plc_stop();
	GDW_RF_PHY_layer_ptr=(GDW_RF_PHY_layer_C  *)send_buf;
	GDW_RF_PHY_layer_ptr->chan=0;
	GDW_RF_PHY_layer_ptr->ID=0x01;
	length=Greate_GDW_MAC_net(BeaconMark,PANSize,RSSIThreshold,CentrePANId,CentreAddr,GDW_RF_PHY_layer_ptr->Pad);
	 length+=3;
	 GDW_RF_PHY_layer_ptr->Xor=(length^GDW_RF_PHY_layer_ptr->chan)^GDW_RF_PHY_layer_ptr->ID;
	 RF_Tx(send_buf, length);
	 return(length);
}
//
