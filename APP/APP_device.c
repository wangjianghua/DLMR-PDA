#include "includes.h"




ROM_PARA g_sys_register_para = 
{
    TAG_WORD_TABLE_MAP,
    0xffffbb00,         //unsigned int bootFlag;      //启动标志，不能改动
    0,              //0:  checksum
    0x20141101,      //version

    
    0xffffffff,         //unsigned int meterPassword;//密码
    4000,               //unsigned int recvDelayTime;//接受数据延时
    1000,               //unsigned int execInterval;//执行时间
    DL_T_07,            //unsigned int plcProtocol; //规约
    0,                  //unsigned int channel; //通道
    9600,               //unsigned int baudrate; //波特率
    0xFA,               //unsigned int preamble; //前导符
    1,                  //unsigned int stopbit;
    0,                  //unsigned int number;
    
    //PARAMETER SECTOR
    {
        0,               //reserved
    }
};

SYS_CONTROL g_sys_control;


/**
  * @brief  STM32 soft reset.
  * @param  None
  * @retval None
  */
void DEV_SoftReset(void)
{
  __set_FAULTMASK(SET);
    
  NVIC_SystemReset();
}



unsigned int calccrc(unsigned char crcbuf,unsigned int crc)
{
    unsigned char i, chk;
    
    crc=crc ^ crcbuf;
    
    for(i=0;i<8;i++)
    {        
        chk=crc&1;
        crc=crc>>1;
        crc=crc&0x7fff;
        if (chk==1)
            crc=crc^0xa001;
        crc=crc&0xffff;
    }
    
    return crc;
}


void  APP_memcpy( unsigned char         *pdest,
                        const  unsigned char  *psrc,
                        unsigned int csize)
{
    if(csize > (RAM_COPY_MAX_SIZE_K * 1024))
    {
        return;
    }

    while(csize--)
    {
        *pdest++ = *psrc++;
    }
}

//buf需要计算的数组首地址，len是需要计算的长度

unsigned int Get_checksum(unsigned char *buf, unsigned short len)
{
    unsigned char hi,lo; 
    unsigned int i; 
    unsigned int crc; 
    crc=0xFFFF; 
    for (i=0;i<len;i++) 
    { 
        crc=calccrc(*buf,crc); 
        buf++; 
    } 
    hi=crc%256; 
    lo=crc/256; 
    crc=(hi<<8)|lo; 
    return crc; 
}

void DEV_Power_Off()
{
    SYS_PWR_OFF();
    LCD_BL_OFF();
    LCD_PWR_OFF();
    LED_UART_OFF();
    LED_PLC_OFF();
}

//确定存储空间是空的
unsigned int DEV_verify_blank(unsigned int * buf_4byte, unsigned int len)
{
    unsigned int res = DEV_FALSE;

    if(len == 0)
        return res;
        
    while(len--)
    {
        if((*buf_4byte++) != 0XFFFFFFFF)
            return DEV_FALSE;
    }
    
    return DEV_TRUE;
}





//读数据
void DEV_Parameters_Read(void)
{

    unsigned int * pPara;
    int n = ROM_PARA_WRITE_TIMES;
    unsigned int crcr;
    //数组指针指向最后一帧数据
    //然后开始判断是不是存有数据

    while(n)
    {
        n--;
        pPara = (unsigned int *)((unsigned int)ROM_ADDR_SYS_PARA + (n*ROM_ADDR_PARA_SIZE));
        if(pPara[0] == TAG_WORD_TABLE_MAP)
        {
            crcr = Get_checksum((unsigned char *)&pPara[3], (ROM_ADDR_PARA_SIZE - 12));
            //校验
            if(crcr == pPara[2])
            {
                //版本检查
                if(pPara[3] >= g_sys_register_para.versionDate)
                {
                    APP_memcpy((unsigned char *)&g_sys_register_para, (unsigned char *)pPara, ROM_ADDR_PARA_SIZE);
                    g_sys_control.paraAddr= (unsigned int)pPara;
     
                }
                else
                {
                    //recover the original data from old database;

                }
                return;
            }
        }
    }

    
    DEV_Parameters_Write();
    
    
}



unsigned int DEV_Parameters_Write(void)
{
    unsigned int * pPara;
    int n = 0;
    unsigned int  i, addr, *s_addr;

    HAL_FLASH_Unlock();

    g_sys_register_para.crc = Get_checksum((unsigned char *)&g_sys_register_para.versionDate, ROM_ADDR_PARA_SIZE - 12);
    
    while(n < (ROM_PARA_WRITE_TIMES))
    {        
        pPara = (unsigned int *)((unsigned int)ROM_ADDR_SYS_PARA + n * ROM_ADDR_PARA_SIZE);
        if(DEV_verify_blank(pPara, ROM_ADDR_PARA_SIZE/4) == DEV_TRUE)
        {
            g_sys_control.paraAddr= (unsigned int)pPara;  //added on 2014.12.30
            addr = (unsigned int)pPara;
            s_addr = (unsigned int *)&g_sys_register_para;
            for(i = 0; i < ROM_ADDR_PARA_SIZE/4; i++)
            {
                //Flash002_WritePage(addr, s_addr);
                HAL_FLASH_Program(TYPEPROGRAM_WORD,addr, *s_addr);
                addr += 4;
                s_addr ++;
            }

            HAL_FLASH_Lock();
            return DEV_OK;
        }
        n++;
    }
    
    FLASH_Erase_Sector(ROM_PARA_ERASE_SECTOR, VOLTAGE_RANGE_3);

    pPara = (unsigned int *)(ROM_ADDR_SYS_PARA);
    
    addr = (unsigned int)pPara;
    s_addr = (unsigned int *)&g_sys_register_para;
   
    for(i = 0; i < ROM_ADDR_PARA_SIZE/4; i++)
    {            
        HAL_FLASH_Program(TYPEPROGRAM_WORD, addr, *s_addr);
        addr += 4;
        s_addr ++;
    }

    HAL_FLASH_Lock();
    
    return DEV_OK;    
}

const u8 c_test_addr[6] = {0x0, 0x36, 0x19, 0x0, 0x0, 0x0};
const u8 c_default_dataflag[4] = {0x00, 0x01, 0x0, 0x0};
void DEV_Init(void)
{
    //unsigned char err;   
    DEV_Parameters_Read();  

    memcpy(g_sys_control.recentMeterAddr, c_test_addr, 6);
    memcpy(g_sys_control.defaultDataFlag, c_default_dataflag, 4);
    
}



