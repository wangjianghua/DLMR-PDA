#include "includes.h"


#define RTC_IIC_ADDR 0xD0

#if 0
#define   RTC_SDA_PORT   GPIOA
#define   RTC_SDA_PIN   GPIO_Pin_12
#else //华兄

#define   RTC_SDA_PORT   GPIOB
#define   RTC_SDA_PIN    GPIO_PIN_7

#endif

u8 g_rtc_time[MAX_RTC_ITEM];

#if (EWARM_OPTIMIZATION_EN > 0u)
#pragma optimize = low
#endif
void RTC_SomeNOP(void)
{

    unsigned char kk=50;

    while(kk--);

}//小于100KHz

#if (EWARM_OPTIMIZATION_EN > 0u)
#pragma optimize = low
#endif 
void RTC_SomeNOP_Short(void)
{

    unsigned char kk=20;
    while(kk--);

}//小于100KHz


void RTC_SCLHighToLow(void)
{    

    RTC_SomeNOP_Short();
    RTC_SCL_SET();RTC_SomeNOP();
    RTC_SCL_RST();RTC_SomeNOP_Short();;
}



void RTC_Start(void)
{

    RTC_SDA_SET();  

    RTC_SomeNOP();

    RTC_SCL_SET();        

    RTC_SomeNOP();        

    RTC_SDA_RST();    

    RTC_SomeNOP();        

    RTC_SCL_RST();  

    RTC_SomeNOP();

}



 void RTC_Stop(void)

{

    RTC_SDA_RST(); 

    RTC_SomeNOP();

    RTC_SCL_SET();

    RTC_SomeNOP();    

    RTC_SDA_SET();     

    RTC_SomeNOP();        

    RTC_SCL_RST();

    RTC_SomeNOP();

}



void RTC_Ack(void)

{

    RTC_SDA_RST(); 

    RTC_SCLHighToLow();    

}

void RTC_NoAck(void)
{

    RTC_SDA_SET();     

    RTC_SCLHighToLow();

}



unsigned char RTC_Read8Bit(void)

{

    unsigned char temp=0, BitCounter=8;

    GPIO_InitTypeDef GPIO_InitStructure;


    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Pin = RTC_SDA_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;

    HAL_GPIO_Init(RTC_SDA_PORT, &GPIO_InitStructure);

    RTC_SomeNOP();


    do{

        temp<<=1;

        RTC_SCL_SET();

        RTC_SomeNOP();

        if(HAL_GPIO_ReadPin(RTC_SDA_PORT, RTC_SDA_PIN)) temp++;        

        RTC_SCL_RST();

        RTC_SomeNOP();

    }while(--BitCounter);

    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;

    HAL_GPIO_Init(RTC_SDA_PORT, &GPIO_InitStructure);


    RTC_SomeNOP_Short();
    
    return(temp);

}



unsigned char RTC_Write8Bit(unsigned char data)
{

    unsigned int wait = 0, ErrIndication;
    unsigned char BitCounter = 8;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    //E2P_SDA_DIR = 0;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Pin = RTC_SDA_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;  

    do{    

        if(data&0x80)        
            RTC_SDA_SET();         
        else            
            RTC_SDA_RST();

        RTC_SCLHighToLow();
        data<<=1;
    
    }while(--BitCounter);

    
    //E2P_SDA_DIR = 1;    
    HAL_GPIO_Init(RTC_SDA_PORT, &GPIO_InitStructure);
        
    RTC_SCL_SET();    

    RTC_SomeNOP_Short();

    while(HAL_GPIO_ReadPin(RTC_SDA_PORT, RTC_SDA_PIN))
    {
        if(wait++ > 5000)
        {
            ErrIndication=1;
            break;
        }
    }

    RTC_SCL_RST();


    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;

    HAL_GPIO_Init(RTC_SDA_PORT, &GPIO_InitStructure);
    RTC_SDA_SET();

    RTC_SomeNOP_Short();
    

    return ErrIndication;

}


unsigned char RTC_ReadByte(unsigned short addr)
{

    unsigned char i;
    unsigned short j = 0;

    do
    {
        //CLR_WatchDog();

        if(j++ > 1000)
        {
            return ERROR;

        }

        RTC_Start();

    }while(RTC_Write8Bit(RTC_IIC_ADDR));  
  

    RTC_Write8Bit((unsigned char)(addr));

    RTC_Start();

    RTC_Write8Bit(RTC_IIC_ADDR|0x01);

  
    i = RTC_Read8Bit(); 

    RTC_NoAck();

    RTC_Stop();    
        

    RTC_SCL_SET();
    RTC_SDA_SET();

    return i;

}

void RTC_WriteByte(unsigned short addr,unsigned char nContent)
{
    unsigned char i;
    unsigned short j=0;

    do
    {
        if(j++ > 1000)
        {
            return;
        }

        RTC_Start();

     }while(RTC_Write8Bit(RTC_IIC_ADDR));    


    RTC_Write8Bit((unsigned char)(addr));
    RTC_Write8Bit(nContent);

    RTC_Stop();

    RTC_SDA_SET();
    RTC_SCL_SET();

}

#if (EWARM_OPTIMIZATION_EN > 0u)
#pragma optimize = low
#endif
void RTC_ReadBuffer(unsigned short addr,unsigned char *data,unsigned char len)
{

    unsigned char i;
    unsigned short j = 0;

    if(len == 0)
        return;
    
    do
    {


        //CLR_WatchDog();

        if(j++ > 1000)
        {
            return;

        }

        RTC_Start();

    }while(RTC_Write8Bit(RTC_IIC_ADDR));    

    RTC_Write8Bit((unsigned char)(addr));

    RTC_Start();

    RTC_Write8Bit(RTC_IIC_ADDR|0x01);

    for(i = 0; i < len - 1; i++)

    {             

        data[i]=RTC_Read8Bit();         

        //data++;

        RTC_Ack();            

    }    


    data[i]=RTC_Read8Bit(); 
    RTC_NoAck();
    RTC_Stop();    

      
    RTC_SCL_SET();
    RTC_SDA_SET();

      

}

#if (EWARM_OPTIMIZATION_EN > 0u)
#pragma optimize = low
#endif
void RTC_WriteBuffer(unsigned short addr,unsigned char *data,unsigned char len)
{

    unsigned char i;
    unsigned short j=0;

    if(len == 0)
        return; 
    
   
    do
    {
        if(j++ > 1000)
        {
            //E2P_WP_SET();
            return;

        }
        RTC_Start();       

     }while(RTC_Write8Bit(RTC_IIC_ADDR));    
       
    RTC_Write8Bit((unsigned char)(addr));   

    for(i=0;i<len;i++)
    {
        RTC_Write8Bit(data[i]);
    }    

    RTC_Stop();


    RTC_SDA_SET();
    RTC_SCL_SET();        

}



void RTC_ReadTime(unsigned char *time)
{
    if(time == 0)
        return;

    RTC_ReadBuffer(0, time, 7);
    time[SEC_POS] &= 0x7f;
    time[MIN_POS] &= 0x7f;
    time[HOUR_POS] &= 0x3f;
    time[DAY_POS] &= 0x07;
    time[DATE_POS] &= 0x3f;
    time[MONTH_POS] &= 0x1f;
    
    time[YEAR_POS] &= 0x7f; //华兄

}




void RTC_WriteTime(unsigned char *time)
{
    if(time == 0)
        return;

    
    time[SEC_POS] &= 0x7f;
    time[MIN_POS] &= 0x7f;
    time[HOUR_POS] &= 0x3f;
    time[DAY_POS] &= 0x07;
    time[DATE_POS] &= 0x3f;
    time[MONTH_POS] &= 0x1f;

    time[YEAR_POS] &= 0x7f; //华兄

    RTC_WriteBuffer(0, time, 7);

}

/* 输入:  BCD CHAR    输出: 返回Hex UCHAR */
unsigned char Bcd2HexChar(unsigned char bcd)
{
    return ((bcd>>4)*10+(bcd&0x0F));
}

/* 输入:  Hex UCHAR   输出: 返回BCD CHAR */
unsigned char Hex2BcdChar(unsigned char hex)
{
    return ((hex%10)|(((unsigned char)(hex/10))<<4));
}

/* 华兄 */
void RtcTimeToSysTime(SYS_TIME *p_sys_time)
{
    u8 year[3], mon[3], day[3], date[3], hour[3], min[3], sec[3];

    
    sprintf(year, "%02x", g_rtc_time[YEAR_POS]);
    sprintf(mon, "%02x", g_rtc_time[MONTH_POS]);
    sprintf(date, "%02x", g_rtc_time[DATE_POS]);    
    sprintf(day, "%02x", g_rtc_time[DAY_POS]);            
    sprintf(hour, "%02x", g_rtc_time[HOUR_POS]);
    sprintf(min, "%02x", g_rtc_time[MIN_POS]);
    sprintf(sec, "%02x", g_rtc_time[SEC_POS]);
    
    year[2] = '\0';
    mon[2] = '\0';
    date[2] = '\0'; 
    day[2] = '\0';             
    hour[2] = '\0';
    min[2] = '\0';
    sec[2] = '\0';
   
    p_sys_time->year = atoi(year);
    p_sys_time->mon = atoi(mon);
    p_sys_time->date = atoi(date); 
    p_sys_time->day = atoi(day);
    p_sys_time->hour = atoi(hour);
    p_sys_time->min = atoi(min);
    p_sys_time->sec = atoi(sec);
}


unsigned char *RTC2Text(void)
{
    unsigned char timebuf[10];
    
    sprintf(timebuf,"%02x:%02x:%02x",g_rtc_time[HOUR_POS],g_rtc_time[MIN_POS],g_rtc_time[SEC_POS]);
    return timebuf;
}

