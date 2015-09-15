#include "includes.h"


u16 Check_DL645_Frame(u8 *Buf)
{
    u8 len, i, CS, offset;
    
    for(offset=0; offset<12; offset++)
    {
        if(Buf[offset]==0x68)
            break;
    }
    
    if(offset>10)
        return (DL645_FRAME_ERROR);

    if(Buf[7+offset]!=0x68)
        return (DL645_FRAME_ERROR);
    
    len=Buf[offset+9];
    
    if(Buf[len+11+offset]!=0x16)
        return (DL645_FRAME_ERROR);
    
    CS=0;
    len+=10;
    
    for(i=0; i<len; i++)
        CS+=Buf[offset+i];
    
    if(CS!=Buf[len+offset])
        return (DL645_FRAME_ERROR);
    
    return (offset);
}

void Frame_Sub_33H(u8 *Buf, u8 Len)
{
    u8 i;

    for(i=0; i<Len; i++) Buf[i]-=0x33;
}

void Frame_Add_33H(u8 *Buf, u8 Len)
{
    u8 i;

    for(i=0; i<Len; i++) Buf[i]+=0x33;
}

void CS_Pack(u8 *Buf)
{
    u8 len, i, CS;
    
    len=Buf[9];
    CS=0;
    
    Frame_Add_33H(&Buf[10], len);
    
    len+=10;
    
    for(i=0; i<len; i++) CS+=Buf[i];
    
    Buf[len]=CS;
    Buf[len+1]=0x16;
}

u16 Create_DL645_Frame(u8 *Addr, u8 Ctrl, u8 Len, P_DL645_FRAME P_DL645_Frame)
{
    P_DL645_Frame->Start1=0x68;
    memcpy(P_DL645_Frame->Addr,Addr,6);
    P_DL645_Frame->Start2=0x68;
    P_DL645_Frame->Ctrl=Ctrl;
    P_DL645_Frame->Len=Len;
    CS_Pack(&P_DL645_Frame->Start1);

    return (Len+12);
}

u16 Create_DL645_Relay_Frame(u8 *Relay_Addr, u8 Level, u8 *Addr, u8 Ctrl, u8 Len, u8 *Data, P_DL645_FRAME P_DL645_Frame)
{
	u8 index, i;
	
	P_DL645_Frame->Ctrl=Ctrl;
	index=0;
    
	if(Level)
	{
		for(i=0;i<(Level);i++)
		{
			memcpy(P_DL645_Frame->Data+index,Relay_Addr+index+6,6);
			index+=6;
			P_DL645_Frame->Ctrl+=0x20;
		}

		memcpy(P_DL645_Frame->Addr,Relay_Addr,6);
	}
	else
	{
		memcpy(P_DL645_Frame->Addr,Addr,6);
	}
    
	memcpy(P_DL645_Frame->Data+index,Data,Len);
	index+=Len;
	P_DL645_Frame->Start1=0x68;
	P_DL645_Frame->Start2=0x68;
	P_DL645_Frame->Len=index;
    CS_Pack(&P_DL645_Frame->Start1);
    
	return (index+12);	
}

u16 Analysis_DL645_Frame(u8 *Addr, u8 *Buf, P_DL645_FRAME_STAT P_DL645_Frame_Stat)
{
	P_DL645_FRAME P_DL645_Frame;
	u16 i;
    
	P_DL645_Frame_Stat->Protocol=DL645_NONE;
	P_DL645_Frame_Stat->ID_Length=0;
	P_DL645_Frame_Stat->Status=0;
    
	i = Check_DL645_Frame(Buf);

    if(i == DL645_FRAME_ERROR)
        return (DL645_FRAME_ERROR);
    
	P_DL645_Frame_Stat->Status=1;
	
	P_DL645_Frame=(P_DL645_FRAME)(Buf+i);
    
	if(!memcmp(Addr,P_DL645_Frame->Addr,6))
	{
		P_DL645_Frame_Stat->Status=2;
	}
    else
    {
		P_DL645_Frame_Stat->Status=3;
    }
    
	Frame_Sub_33H(P_DL645_Frame->Data,P_DL645_Frame->Len);
    
	P_DL645_Frame_Stat->Ctrl = P_DL645_Frame->Ctrl;
    
	if((P_DL645_Frame->Ctrl&0x10)||(P_DL645_Frame->Ctrl==0x83))
	{
		P_DL645_Frame_Stat->Protocol=DL645_2007;
        
		if(P_DL645_Frame->Ctrl==0x91) P_DL645_Frame_Stat->ID_Length=4;
	}
	else
	{
		P_DL645_Frame_Stat->Protocol=DL645_1997;
        
		if(P_DL645_Frame->Ctrl==0x81) P_DL645_Frame_Stat->ID_Length=2;
	}     

    return (DL645_FRAME_OK);
}

