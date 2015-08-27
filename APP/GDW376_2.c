#include"includes.h"
//#include "HAL.h"
//static  u8 PL_Up_ch;  //上行通讯信道

//const u8 Bit_Def[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
//extern UCHAR g_rf_buf[PLC_BUF_MAX_LEN+6];
//=====================国网本地模块通讯协议帧头构造======================
//说明1：通讯方式固定为集中式载波
//说明2：通讯波特率固定为9600bps

//u8* DesAddrBuf,       目标节点地址，6字节为一个单元，至少有一个有效单元
//u8* RoutAddr,         路由地址
//u8 RoutLever,         路由级数， Len(DesAddrBuf)=6+6*RoutLever; 0是无中继
//u8 Protocol           转发数据单元协议类型：0(透传) 1(DLT645-1997) 2(DLT645-2007)
//u8 AFN,               功能代号 
//u8 DT1,               数据单元标识1 
//u8 DT2,               数据单元标识2
//u8 Protocol           AFN=02，DT1=1，DT2=0时，转发数据单元协议类型：0(透传) 1(DLT645-1997) 2(DLT645-2007)

//返回* pBuf,           构造好包的头部数据缓冲区的返回指针 
//返回说明：            在pBuf[pBuf[1]]位置开始填DLT数据的长度，紧接着开始填DLT数据
//返回：                pBuf[1]中，本头部数据长度,外部应用方法：（pBuf[1]+=1（DLT报文长度占位）+Len(DLTData)+GW_FIX_PKG_LAST_LEN）
//返回说明              在pBuf[2]位置开始计算CS
/******************************************************************************************************/
//-----------------------------本地通信协议帧头-------------------------------------------
/*
u16 Create_F3762_Title(u8 C,Info_C R,u8 AFN,u8 Fn,u8 *Frame_buff)
{
 Frame_3762_C *ptr;

 u16 L;

     ptr=(Frame_3762_C *)Frame_buff;
	 ptr->Start=0x68;
	 ptr->C=C;
	 ptr->R=R;
	 ptr->AFN=AFN;
	 ptr->DT[0]=Bit_Def[(Fn-1)%8];
	 ptr->DT[1]=0x00;
	 L=13;
   // }
 return(L);
}*/
/******************************************************************************
*                           建立下行模块通信帧
*得到数据单元的位置
*******************************************************************************
u16 Get_Data_Point(u8 C,Info_C *R,u8 *Rout,u8 *Addr,u8 AFN,u16 Fn,u8 *Frame)
{
 u8 *p,*s;
 u16 i,Offset;
 Frame[0]=0x68;
 Frame[3]=C;
 p=(u8 *)R;
 Offset=4;
 for(i=0;i<6;i++)Frame[Offset++]=p[i];
 
 if(R->Comm_Fg)    //有中继
 	{
 	 s=(u8 *)Tmer_Commun.Addr;
     for(i=0;i<4;i++)
     	{
         Frame[Offset++]=s[i];                                //把源地址默认为零
	    }
	 Frame[Offset++]=0; 
	 Frame[Offset++]=0; 
	 for(i=0;i<(6*R->Lever);i++)Frame[Offset++]=Rout[i];//写路由
	 for(i=0;i<6;i++)Frame[Offset++]=Addr[i];            //目的地址
    }
 Frame[Offset++]=AFN;                
 Frame[Offset++]=Bit_Def[(Fn-1)%8];
 Frame[Offset++]=(Fn-1)/8;
 return(Offset);
}*/
//--------------------------------组成完整的下方报文--------------------------------
//len:校验和之前的报文长度
void Create_GDW3762_Frame(u8 *Frame,u16 len)
{
 u8 i;
 u8 *p;
 u16 L;

 
 L=len+2;
 p=(u8 *)&L;
 
 Frame[1]=p[0];
 Frame[2]=p[1];
 Frame[len]=0;
 for(i=3;i<len;i++)Frame[len]+=Frame[i];
 Frame[len+1]=0x16;
}


#if 0
//----------------------检测报文合法性------------------------------------------
u8 Check_GDW3762(u8 *p)
{
 u8 i,cs;
 Frame_3762_C *ptr;

 ptr=(Frame_3762_C *)p;
 if(ptr->Start!=0x68)return(0);
 if(ptr->Length>254)return(0);
 if(p[ptr->Length-1]!=0x16)return(0);
 cs=0;
 for(i=3;i<(ptr->Length-2);i++)cs+=p[i];
 if(cs!=p[ptr->Length-2])return(0);
 return(1);
}

//--------------------------------解析应答报文--------------------------------------------
//返回:AFN的位置，如果是0则是非法报文
u8 Check_Modem_down_Frame(u8 *buff)
{
 u8 i,cs,Offset=0;
 u16 len;
 u8 *p;
 Info_up_C *R;

 p=(u8 *)&len;
 if(buff[0]!=0x68)return(0);
 p[0]=buff[1];
 p[1]=buff[2];
 if(len>250)return(0);
 if(buff[len-1]!=0x16)return(0);
 len-=2;
 cs=0;
 for(i=3;i<len;i++)cs+=buff[i];
 if(cs==buff[len])
 	{
     R=(Info_up_C *)(buff+4);
	 Offset=10;
     if(R->Comm_Fg)Offset+=(12+R->Lever*6);
	 if(buff[Offset]!=0x02)Offset=0;
	 return(Offset);
    }
 return(0);
}
//---------------------------------解析报文------------------
u8 Parse_Modem_Down_Frame(u8 *buff)
{
 u8 i;
 u8 *p;
 u16 Fn;
 
 p=buff;

 for(i=0;i<8;i++)
 	{
     if(buff[1]&Bit_Def[i])break;
    }
 Fn=buff[2]*3+i+1;
 switch(buff[0])
 	{
     case 0:
	 	    if(Fn==1)return(0);              //确认
			else return(1);                  //否认
	 	   // break;
	 case 2:
	 	    break;
    }
 return(p[0]);                               //返回AFN
}
//static GDW_3762Bak_C GDW_3762Bak;
void GDW_3762_init(void)
{
    //GDW_3762Bak.Status=0;
}
/*********************************************************************************************/
extern sreg UCHAR ACZ_Flag;
__no_init sreg u8 PLC_Main_Addr[6];
#ifdef RF_FREQ_470MHz
	const u8 GDW_Ver[]={'X','L','3','7',0x28,0x06,0x13,0x52,0x03};
#else
	#ifdef PLC_RF_PW_en
		const u8 GDW_Ver[]={'X','L','3','6',0x28,0x06,0x13,0x52,0x03};
	#else
		const u8 GDW_Ver[]={'X','L','3','5',0x28,0x06,0x13,0x52,0x03};
	#endif
#endif
const u8 AFN3F5_DEF[]={0x65,0x03,0x32,0x00,0x64,0x00,0x4A,0x01,0x58,0x02,0xB0,0x04};
const u8 AFN3F10_DEF[]={0x41,0x30,0x03,0x05,0x00,0x00,60,10,0,255,0,0,0,3,
                        0x01,0x00,0x00,0x00,0x00,0x00,
                        0x00,0x00,0x00,0x00,
                        0x13,0x09,0x12,0x13,0x09,0x12,
                        'X','L','3','6',0x05,0x01,0x13,0x03,0x25,
                        0x4A,0x01,50,0x00,100,0x00,0x58,0x02,0xB0,0x04};
const u8 Phase_Send_df[4]={0x01,0x02,0x04,0x08};
extern u16 PLC_421_Send_Timeout;
u16 GDW3762_Comm_Sub(u8 *buff)
{
    Frame_3762_C *Frame_3762_Ptr;
    Info_C *Info_ptr;
    GDW3762_App_C *GDW3762_App_Ptr;
    u16 Offset,L=0;
    u8 i,Fn;

    
    Frame_3762_Ptr=(Frame_3762_C *)buff;
    Info_ptr=(Info_C *)(buff+4);//Frame_3762_Ptr->Data;
    Offset=6;
    if(Info_ptr->Comm_Fg)Offset+=(12+Info_ptr->Lever*6);
    GDW3762_App_Ptr=(GDW3762_App_C *)(buff+4+Offset);
    
    for(i=0;i<8;i++)
 	{
     if(GDW3762_App_Ptr->DT[0]&Bit_Def[i])break;
    }
    Fn=(GDW3762_App_Ptr->DT[1]*8)+i+1;
 
    if(((PLC_Main_Addr[0]&0x0F)>9)||((PLC_Main_Addr[0]&0xF0)>0x90))
    {
      memset(PLC_Main_Addr,0,6);
      PLC_Main_Addr[0]=1;
    }
    //switch(GDW3762_App_Ptr->AFN)
    switch(buff[4+Offset])    
    {
        case 0x03:
            if(Fn==1)//读版本
            {
                memcpy(GDW3762_App_Ptr->Data,GDW_Ver,sizeof(GDW_Ver));
                memset(Frame_3762_Ptr->Data,0,6);
                Frame_3762_Ptr->C&=0x3F;
                Frame_3762_Ptr->C|=0x80;
                Frame_3762_Ptr->Length=6+6+3+sizeof(GDW_Ver);
                Create_GDW3762_Frame(buff,Frame_3762_Ptr->Length-2);
                L=Frame_3762_Ptr->Length;
            }
            else
            if(Fn==4)
            {
                memcpy(GDW3762_App_Ptr->Data,PLC_Main_Addr,6);
                memset(Frame_3762_Ptr->Data,0,6);
                Frame_3762_Ptr->C&=0x3F;
                Frame_3762_Ptr->C|=0x80;
                Frame_3762_Ptr->Length=6+6+3+6;
                Create_GDW3762_Frame(buff,Frame_3762_Ptr->Length-2);
                L=Frame_3762_Ptr->Length;  
            }
            else
             if(Fn==5)
            {
                memcpy(GDW3762_App_Ptr->Data,AFN3F5_DEF,sizeof(AFN3F5_DEF));
                memset(Frame_3762_Ptr->Data,0,6);
                Frame_3762_Ptr->C&=0x3F;
                Frame_3762_Ptr->C|=0x80;
                Frame_3762_Ptr->Length=6+6+3+sizeof(AFN3F5_DEF);
                Create_GDW3762_Frame(buff,Frame_3762_Ptr->Length-2);
                L=Frame_3762_Ptr->Length;//return(Frame_3762_Ptr->Length);
            }
            if(Fn==10)
             {
                memcpy(GDW3762_App_Ptr->Data,AFN3F10_DEF,sizeof(AFN3F10_DEF));
                i=ACZ_Flag&0x07;
                if(i&0x01)i|=0x08;
                i>>=1;
                i^=0x07;
                GDW3762_App_Ptr->Data[2]|=i<<5;
                memcpy(GDW3762_App_Ptr->Data+14,PLC_Main_Addr,6);
                memset(Frame_3762_Ptr->Data,0,6);
                Frame_3762_Ptr->C&=0x3F;
                Frame_3762_Ptr->C|=0x80;
                Frame_3762_Ptr->Length=6+6+3+sizeof(AFN3F10_DEF);
                Create_GDW3762_Frame(buff,Frame_3762_Ptr->Length-2);
                L=Frame_3762_Ptr->Length;//return(Frame_3762_Ptr->Length);
             }
            break;
        case 0x02:
            if(Fn==1)
            {
             _sys_plc_reset();
             if(Info_ptr->Resp_cnt<40)Info_ptr->Resp_cnt=40;
             if(Info_ptr->bps==330)
                GDW_DRplc_Frame_head(Frame_3762_Ptr->Data+6,Info_ptr->Lever,0x02,GDW3762_App_Ptr->Data[0],GDW3762_App_Ptr->Data+2);
             else
                if(Info_ptr->bps==331)Frame_GDW3762CovPLC270_3D5(Frame_3762_Ptr->Data+6,0x02,GDW3762_App_Ptr->Data[0],(DL645_FRAME *)(GDW3762_App_Ptr->Data+2));
             else
            PLC421_GDW3762_CopyMeter(Frame_3762_Ptr->Data+6,Info_ptr->Lever,0x02,GDW3762_App_Ptr->Data[0],GDW3762_App_Ptr->Data[1],GDW3762_App_Ptr->Data+2,Info_ptr->Channel,Info_ptr->bps,Info_ptr->Resp_cnt,0);
            }
            break;
        case 0x04:
            if(Fn==2)//从节点点名
            {
                if(!Info_ptr->Channel)
                {
                    Info_ptr->Channel=Boand();
                }
                PLC421_GDW3762_CopyMeter(Frame_3762_Ptr->Data+6,Info_ptr->Lever,0x04,GDW3762_App_Ptr->Data[0],GDW3762_App_Ptr->Data[2],GDW3762_App_Ptr->Data+3,Info_ptr->Channel,Info_ptr->bps,Info_ptr->Resp_cnt,Info_ptr->rev); 
            }
            break;
        case 0x05:
            if(Fn==1)//设主节点地址
            {
              memcpy(PLC_Main_Addr,GDW3762_App_Ptr->Data,6);
              memset(Frame_3762_Ptr->Data,0,6);
              GDW3762_App_Ptr->AFN=0x00;
              GDW3762_App_Ptr->DT[0]=1;
              GDW3762_App_Ptr->DT[1]=0;
              GDW3762_App_Ptr->Data[0]=0xFF;
              GDW3762_App_Ptr->Data[1]=0xFF;
              GDW3762_App_Ptr->Data[2]=0xFF;
              GDW3762_App_Ptr->Data[3]=0xFF;
              GDW3762_App_Ptr->Data[4]=0x00;
              GDW3762_App_Ptr->Data[5]=0x00;
              Frame_3762_Ptr->C&=0x3F;
              Frame_3762_Ptr->C|=0x80;
              Frame_3762_Ptr->Length=6+6+9;
              Create_GDW3762_Frame(buff,Frame_3762_Ptr->Length-2);
              L=Frame_3762_Ptr->Length;
            }
            else
            if(Fn==3)//启功广播
            {
              _sys_plc_reset();
              memcpy(buff+100,PLC_Main_Addr,6);
              memcpy(buff+106,GDW3762_App_Ptr->Data+3,6);
              if(Info_ptr->bps==330)
              Frame_GDW3762CovPLC270_3D5(PLC_Main_Addr,0x13,GDW3762_App_Ptr->Data[0],(DL645_FRAME *)(GDW3762_App_Ptr->Data+2));
              else
              PLC421_GDW3762_SYN(buff+100,0x13,GDW3762_App_Ptr->Data[0],GDW3762_App_Ptr->Data[1],GDW3762_App_Ptr->Data+2,Info_ptr->Channel,Info_ptr->bps);
              memset(Frame_3762_Ptr->Data,0,6);
              GDW3762_App_Ptr->AFN=0x00;
              GDW3762_App_Ptr->DT[0]=1;
              GDW3762_App_Ptr->DT[1]=0;
              GDW3762_App_Ptr->Data[0]=0xFF;
              GDW3762_App_Ptr->Data[1]=0xFF;
              GDW3762_App_Ptr->Data[2]=0xFF;
              GDW3762_App_Ptr->Data[3]=0xFF;
              GDW3762_App_Ptr->Data[4]=0x0F;
              GDW3762_App_Ptr->Data[5]=0x00;
              Frame_3762_Ptr->C&=0x3F;
              Frame_3762_Ptr->C|=0x80;
              Frame_3762_Ptr->Length=6+6+9;
              Create_GDW3762_Frame(buff,Frame_3762_Ptr->Length-2);
              L=Frame_3762_Ptr->Length;
            }
            break;
        case 0x13:   
            
             if(Info_ptr->bps==99)
		{
			 PLC_send_mode=0;
			 Info_ptr->bps=100;
             	}
	       else
	       	{
	       		PLC_421_Send_Timeout=0;
	       	 	PLC_send_mode=1;
	       	}
			 
             if(!Info_ptr->Lever)
                memcpy(PLC_Main_Addr,Frame_3762_Ptr->Data+6,6);
             //if(Info_ptr->Resp_cnt<40)Info_ptr->Resp_cnt=40;
             if(Info_ptr->bps==330)
                {
                 _sys_plc_reset();
                GDW_DRplc_Frame_head(Frame_3762_Ptr->Data+6,Info_ptr->Lever,0x13,GDW3762_App_Ptr->Data[0],GDW3762_App_Ptr->Data+3);
                }
#ifdef RF_FREQ_470MHz
             else
                if((Info_ptr->bps==2400)||(Info_ptr->bps==2500))
                {
                    _sys_plc_reset();
                    GDW_RF_Frame_head(Frame_3762_Ptr->Data+6,Info_ptr->Lever,0x13,GDW3762_App_Ptr->Data[0],GDW3762_App_Ptr->Data+3,Info_ptr->bps);
                }
		else
		if(Info_ptr->bps==10000)
		{
			GDW_RF_Protocol_2013(Frame_3762_Ptr->Data+6,Info_ptr->Lever,0x13,GDW3762_App_Ptr->Data[0],GDW3762_App_Ptr->Data+3,GDW3762_App_Ptr->Data[2],g_rf_buf);
		}
#endif
             else
                if(Info_ptr->bps==331)
                {
                    _sys_plc_reset();
                    Frame_GDW3762CovPLC270_3D5(Frame_3762_Ptr->Data+6,0x13,GDW3762_App_Ptr->Data[0],(DL645_FRAME *)(GDW3762_App_Ptr->Data+3));
                }
             else
                if(Info_ptr->bps==329)
                {
                    _sys_plc_reset();
                    GDW_DRplc_Frame_head_HLJ(Frame_3762_Ptr->Data+6,Info_ptr->Lever,0x13,GDW3762_App_Ptr->Data[0],GDW3762_App_Ptr->Data+3);//黑龙江协议
                }
             else
             	{
             		 if(!Info_ptr->Channel)
	                    {
	                        Info_ptr->Channel=Boand();
	                        i=PLC421_Send_Status();
	                        if(!(Phase_Send_df[Info_ptr->Channel]&i))
	                        {
	                            Info_ptr->Channel++;
	                            if(Info_ptr->Channel>3)Info_ptr->Channel=1;
	                            if(!(Phase_Send_df[Info_ptr->Channel]&i))
	                            {
	                                Info_ptr->Channel++;
	                                if(Info_ptr->Channel>3)Info_ptr->Channel=1;
	                            }
	                        }
	                        
	                    }
	                if(Info_ptr->bps&1)
	                {
	                    _sys_plc_reset();
	                    PLC421_GDW3762_SYN(Frame_3762_Ptr->Data+6,0x13,GDW3762_App_Ptr->Data[0],GDW3762_App_Ptr->Data[2],GDW3762_App_Ptr->Data+3,Info_ptr->Channel,Info_ptr->bps-1);
	                }
	             else
	                {
	                /*
	                    if(!Info_ptr->Channel)
	                    {
	                        Info_ptr->Channel=Boand();
	                        i=PLC421_Send_Status();
	                        if(!(Phase_Send_df[Info_ptr->Channel]&i))
	                        {
	                            Info_ptr->Channel++;
	                            if(Info_ptr->Channel>3)Info_ptr->Channel=1;
	                            if(!(Phase_Send_df[Info_ptr->Channel]&i))
	                            {
	                                Info_ptr->Channel++;
	                                if(Info_ptr->Channel>3)Info_ptr->Channel=1;
	                            }
	                        }
	                        
	                    }
	                 */
	                    i=PLC421_GDW3762_CopyMeter(Frame_3762_Ptr->Data+6,Info_ptr->Lever,0x13,GDW3762_App_Ptr->Data[0],GDW3762_App_Ptr->Data[2],GDW3762_App_Ptr->Data+3,Info_ptr->Channel,Info_ptr->bps,Info_ptr->Resp_cnt,Info_ptr->rev);
	                    //if(!i)
	                    //    L=Create_AFN00_F02(buff,9);
	                }
             	}
           // }
            break;
        case 0x16:
            if(!Info_ptr->Channel)Info_ptr->Channel=3;
            if(Fn==1)
            {
              PLC421_GDW3762_SearchNode(Frame_3762_Ptr->Data+6,Info_ptr->Lever,Search_Command,0x16,GDW3762_App_Ptr->Data[0],GDW3762_App_Ptr->Data+1,Info_ptr->Channel,Info_ptr->bps,Info_ptr->Resp_cnt);  
            }
            if(Fn==2)
            {
              PLC421_GDW3762_SearchNode(Frame_3762_Ptr->Data+6,Info_ptr->Lever,Search_Confirm,0x16,GDW3762_App_Ptr->Data[0],GDW3762_App_Ptr->Data+1,Info_ptr->Channel,Info_ptr->bps,Info_ptr->Resp_cnt);
            }
            break;
        default://其他命令应答否认帧
            GDW3762_App_Ptr=(GDW3762_App_C *)(buff+10);
            memset(Frame_3762_Ptr->Data,0,6);
            GDW3762_App_Ptr->AFN=0x00;
            GDW3762_App_Ptr->DT[0]=2;
            GDW3762_App_Ptr->DT[1]=0;
            GDW3762_App_Ptr->Data[0]=1;
            Frame_3762_Ptr->C&=0x3F;
            Frame_3762_Ptr->C|=0x80;
            Frame_3762_Ptr->Length=6+6+4;
            Create_GDW3762_Frame(buff,Frame_3762_Ptr->Length-2);
            L=Frame_3762_Ptr->Length;//return(Frame_3762_Ptr->Length);
            break;
    }
  return(L);
}
u8 Create_AFN00_F02(u8 *buff,u8 dat)
{
    Frame_3762_C *Frame_3762_Ptr;
    Info_up_C *Info_ptr;

    GDW3762_App_C *GDW3762_App_Ptr;

    Frame_3762_Ptr=(Frame_3762_C *)buff;
    Frame_3762_Ptr->Start=0x68;
    GDW3762_App_Ptr=(GDW3762_App_C *)(buff+10);
    Info_ptr=(Info_up_C *)Frame_3762_Ptr->Data;
    Info_ptr->Lever=0;
    Info_ptr->Amm_ch=0;
    Info_ptr->Phase=0;
    Info_ptr->Rev1=0;
    Info_ptr->Rev2=0;
    Info_ptr->Rev3=0;
    Info_ptr->Singal1=0;
    Info_ptr->Singal2=0;
    Info_ptr->Side_Fg=0;
    Info_ptr->Comm_Fg=0;
    //memset(Frame_3762_Ptr->Data,0,6);
    GDW3762_App_Ptr->AFN=0x00;
    GDW3762_App_Ptr->DT[0]=2;
    GDW3762_App_Ptr->DT[1]=0;
    GDW3762_App_Ptr->Data[0]=dat;
    Frame_3762_Ptr->C=0x81;
    Frame_3762_Ptr->Length=6+6+4;
    Create_GDW3762_Frame(buff,Frame_3762_Ptr->Length-2);
    return(Frame_3762_Ptr->Length);
}
u8 Create_AFN00_F01(u8 *buff,u8 dat,u16 Time)
{
    Frame_3762_C *Frame_3762_Ptr;
    Info_up_C *Info_ptr;

    GDW3762_App_C *GDW3762_App_Ptr;

    Frame_3762_Ptr=(Frame_3762_C *)buff;
    Frame_3762_Ptr->Start=0x68;
    GDW3762_App_Ptr=(GDW3762_App_C *)(buff+10);
    Info_ptr=(Info_up_C *)Frame_3762_Ptr->Data;
    Info_ptr->Lever=0;
    Info_ptr->Amm_ch=0;
    Info_ptr->Phase=0;
    Info_ptr->Rev1=0;
    Info_ptr->Rev2=0;
    Info_ptr->Rev3=0;
    Info_ptr->Singal1=0;
    Info_ptr->Singal2=0;
    Info_ptr->Side_Fg=0;
    Info_ptr->Comm_Fg=0;
    //memset(Frame_3762_Ptr->Data,0,6);
    GDW3762_App_Ptr->AFN=0x00;
    GDW3762_App_Ptr->DT[0]=1;
    GDW3762_App_Ptr->DT[1]=0;
    GDW3762_App_Ptr->Data[0]=dat;
    GDW3762_App_Ptr->Data[1]=0xFF;
    GDW3762_App_Ptr->Data[2]=(u8)Time;
    GDW3762_App_Ptr->Data[3]=(u8)(Time>>8);
    Frame_3762_Ptr->C=0xC1;
    Frame_3762_Ptr->Length=6+6+7;
    Create_GDW3762_Frame(buff,Frame_3762_Ptr->Length-2);
    return(Frame_3762_Ptr->Length);
}
/*
void GDW3762_AFN13_SUB(u8 phase)
{
    Frame_3762_C *Frame_3762_Ptr;
    Info_C *Info_ptr;
    GDW3762_App_C *GDW3762_App_Ptr;
    u16 Offset;
   // u8 i,Fn;

    
    Frame_3762_Ptr=(Frame_3762_C *)GDW_3762Bak.Buff;
    Info_ptr=(Info_C *)Frame_3762_Ptr->Data;
    Offset=6;
    if(Info_ptr->Comm_Fg)Offset+=(12+Info_ptr->Lever*6);
    GDW3762_App_Ptr=(GDW3762_App_C *)(Frame_3762_Ptr->Data+Offset);

    PLC421_GDW3762_CopyMeter(Frame_3762_Ptr->Data+6,Info_ptr->Lever,0x13,GDW3762_App_Ptr->Data[0],GDW3762_App_Ptr->Data[2],GDW3762_App_Ptr->Data+3,phase,Info_ptr->bps,Info_ptr->Resp_cnt,Info_ptr->rev);
}
u8 GDW_Seach_Phase(void)
{
    if((GDW_3762Bak.Status<3)&&GDW_3762Bak.Status)
    {
     GDW_3762Bak.Status++;
     GDW3762_AFN13_SUB(GDW_3762Bak.Status);
     return(0);
    }
    else
     return(1);
}
 extern u16 PLC_Resp_timeout;

void GDW3762_Three_Send(u8 PLC_ST)
{
 switch(GDW_3762Bak.Status)
 {
    case 1:
        if(GDW_3762Bak.Phase>2)GDW_3762Bak.Status=0;
        else
        {
            if(PLC_ST)GDW_3762Bak.cnt=0;
            else
            {
                GDW_3762Bak.cnt++;
                if(GDW_3762Bak.cnt>50)
                {
                    GDW_3762Bak.Status=2;
                    GDW_3762Bak.Phase++;
                    
                }
            }
        }
        
        break;
    case 2:
       
                GDW_3762Bak.cnt=0;
                DI();
                PLC_Resp_timeout=0;
                EI();
                GDW3762_AFN13_SUB(GDW_3762Bak.Phase);
                GDW_3762Bak.Status=1;
     
        break;
     default:
        
        break;
 }
}*/
u8 GDW3762_Meter_AFN00_RESP(Info_up_C *R,u8 *Addr,u8 Fn,u8 dat,u16 Time,u8 *Send_buff)
{
    Frame_3762_C *Frame_3762_Ptr;
    u8 Point,tm;
    GDW3762_App_C *GDW3762_App_Ptr;
    Frame_3762_Ptr=(Frame_3762_C *)Send_buff;
    Frame_3762_Ptr->Start=0x68;
    Frame_3762_Ptr->C=0x81;
    *(Info_up_C *)Frame_3762_Ptr->Data=*R;
    Point=6;
    memcpy(Frame_3762_Ptr->Data+Point,Addr+6,6);
    Point+=6;
    memcpy(Frame_3762_Ptr->Data+Point,Addr,6);
    Point+=6;
    GDW3762_App_Ptr=(GDW3762_App_C *)(Frame_3762_Ptr->Data+Point);
    GDW3762_App_Ptr->AFN=0;
    GDW3762_App_Ptr->DT[0]=Fn;
    GDW3762_App_Ptr->DT[1]=0;
    Point+=3;
    if(Fn==1)
    {
        GDW3762_App_Ptr->Data[0]=dat;
        GDW3762_App_Ptr->Data[1]=0xFF;
        tm=Time/100;
        if(Time%100)tm++;
        GDW3762_App_Ptr->Data[2]=tm;
        GDW3762_App_Ptr->Data[3]=(u8)(tm>>8);
        Point+=4;
    }
    else
    {
        GDW3762_App_Ptr->Data[0]=0;
        Point++;
    }
    Frame_3762_Ptr->Length=Point+6;
    Create_GDW3762_Frame(Send_buff,Frame_3762_Ptr->Length-2);
    return(Frame_3762_Ptr->Length);
}
//------------------------------------------------------------------------------------------------
u8 GDW3762_Meter_Data_RESP(Info_up_C *R,u8 CMD,u8 type,u8 *Addr,u8 *buff,u8 L,u8 *Send_buff)
{
    Frame_3762_C *Frame_3762_Ptr;
    u8 Point;
    //u8 Len;
    GDW3762_App_C *GDW3762_App_Ptr;

    Frame_3762_Ptr=(Frame_3762_C *)Send_buff;
    Frame_3762_Ptr->Start=0x68;
    Frame_3762_Ptr->C=0x81;
    *(Info_up_C *)Frame_3762_Ptr->Data=*R;
    Point=6;
    memcpy(Frame_3762_Ptr->Data+Point,Addr+6,6);
    Point+=6;
    memcpy(Frame_3762_Ptr->Data+Point,Addr,6);
    Point+=6;
    
    GDW3762_App_Ptr=(GDW3762_App_C *)(Frame_3762_Ptr->Data+Point);
    GDW3762_App_Ptr->AFN=CMD;
    GDW3762_App_Ptr->DT[0]=1;
    GDW3762_App_Ptr->DT[1]=0;
    GDW3762_App_Ptr->Data[0]=type;
    Point+=3;
    if(type)
    {
	    // if(buff[1]&0x10)GDW3762_App_Ptr->Data[0]=2;
	    // else GDW3762_App_Ptr->Data[0]=1;
	     if(buff[0]&0x80)
	        GDW3762_App_Ptr->Data[1]=PLC421_Copy_Meter(0,0,Addr+6,buff[0],L,buff+1,GDW3762_App_Ptr->Data+2);
	     else return(0);
    }
    else
    {
	     GDW3762_App_Ptr->Data[1]=L+1;
	     memcpy(GDW3762_App_Ptr->Data+2,buff,GDW3762_App_Ptr->Data[1]);
	     Point++;
    }
    Point+=(GDW3762_App_Ptr->Data[1]+2);
    Frame_3762_Ptr->Length=Point+6;
    Create_GDW3762_Frame(Send_buff,Frame_3762_Ptr->Length-2);
    return(Frame_3762_Ptr->Length);
}
u8 GDW3762_Search_Node_RESP(Info_up_C *R,u8 CMD,u8 *Addr,u8 *buff,u8 L,u8 *Send_buff)
{
    Frame_3762_C *Frame_3762_Ptr;
    u8 Point;
    GDW3762_App_C *GDW3762_App_Ptr;
    Frame_3762_Ptr=(Frame_3762_C *)Send_buff;
    Frame_3762_Ptr->Start=0x68;
    Frame_3762_Ptr->C=0x81;
    *(Info_up_C *)Frame_3762_Ptr->Data=*R;
    Point=6;
    memcpy(Frame_3762_Ptr->Data+Point,Addr+6,6);
    Point+=6;
    memcpy(Frame_3762_Ptr->Data+Point,Addr,6);
    Point+=6;
    GDW3762_App_Ptr=(GDW3762_App_C *)(Frame_3762_Ptr->Data+Point);
    GDW3762_App_Ptr->AFN=CMD;
    GDW3762_App_Ptr->DT[0]=1;
    GDW3762_App_Ptr->DT[1]=0;
    Point+=3;
    GDW3762_App_Ptr->Data[0]=L;
    Point++;
    memcpy(GDW3762_App_Ptr->Data+1,buff,L);
    Point+=L;
    Frame_3762_Ptr->Length=Point+6;
    Create_GDW3762_Frame(Send_buff,Frame_3762_Ptr->Length-2);
    return(Frame_3762_Ptr->Length);
}
u8 GDW_3762_NoResp(u8 CMD,u8 *meter_ID,u8 *Send_buff,u8 *Main_Addr)
{
    Frame_3762_C *Frame_3762_Ptr;
    Info_up_C *R;
    u8 Point;
    //u8 Len;
    GDW3762_App_C *GDW3762_App_Ptr;

    Frame_3762_Ptr=(Frame_3762_C *)Send_buff;
    Frame_3762_Ptr->Start=0x68;
    Frame_3762_Ptr->C=0x81;
    memset(Frame_3762_Ptr->Data,0,6);
    R=(Info_up_C *)Frame_3762_Ptr->Data;
    R->Comm_Fg=1;
    R->Rout_Fg=0;
    Point=6;
    memcpy(Frame_3762_Ptr->Data+Point,meter_ID,6);
    Point+=6;
    memcpy(Frame_3762_Ptr->Data+Point,Main_Addr,6);
    Point+=6;
    //memset(Main_Addr,0xFF,6);
    GDW3762_App_Ptr=(GDW3762_App_C *)(Frame_3762_Ptr->Data+Point);
    GDW3762_App_Ptr->AFN=CMD;
    if(CMD==0x04)
    {
       GDW3762_App_Ptr->AFN=0; 
       GDW3762_App_Ptr->DT[0]=2;
       GDW3762_App_Ptr->DT[1]=0;
       GDW3762_App_Ptr->Data[0]=0;
       Point+=4;
    }
    else
    {
    GDW3762_App_Ptr->DT[0]=1;
    GDW3762_App_Ptr->DT[1]=0;
    GDW3762_App_Ptr->Data[0]=0;
    GDW3762_App_Ptr->Data[1]=0;
    //GDW3762_App_Ptr->Data[2]=dat[0];//
    //GDW3762_App_Ptr->Data[3]=dat[1];//调试用信息
    Point+=5;
    }
    Frame_3762_Ptr->Length=Point+6;
    Create_GDW3762_Frame(Send_buff,Frame_3762_Ptr->Length-2);
    
    return(Frame_3762_Ptr->Length);
}

#endif
