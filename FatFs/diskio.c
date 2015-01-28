/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* Port @华兄 Email: 591881218@qq.com                                    */                       
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
//#include "usbdisk.h"	/* Example: USB drive control */
//#include "atadrive.h"	/* Example: ATA drive control */
//#include "sdcard.h"	/* Example: MMC/SDC contorl */
#include "sd.h"

/* Definitions of physical drive number for each drive */
//#define ATA		0	/* Example: Map ATA drive to drive number 0 */
//#define MMC		1	/* Example: Map MMC/SD card to drive number 1 */
//#define USB		2	/* Example: Map USB drive to drive number 2 */


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;


    pdrv = pdrv;
    
    stat &= ~(STA_NOINIT | STA_NODISK | STA_PROTECT);
    
	return (stat);
}


/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;


    pdrv = pdrv;

    result = SD_Init();

    if(0 == result)
    {
        stat &= ~(STA_NOINIT | STA_NODISK | STA_PROTECT);
    }
    else
    {
        stat = STA_NOINIT;
    }
    
	return (stat);
}


/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address in LBA */
	BYTE count		/* Number of sectors to read */
)
{
	DRESULT res;
	int result;


    pdrv = pdrv;

    if(!count)
    {
        res = RES_PARERR;

        return (res);
    }

    result = SD_ReadBlock(buff, sector, count);

    if(0 == result)
    {
        res = RES_OK;
    }
    else
    {
        res = RES_ERROR;
    }
    
	return (res);
}


/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if (0 == _READONLY)
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	BYTE count			/* Number of sectors to write */
)
{
	DRESULT res;
	int result;


    pdrv = pdrv;

    if(!count)
    {
        res = RES_PARERR;

        return (res);
    }

    result = SD_WriteBlock((BYTE *)buff, sector, count);

    if(0 == result)
    {
        res = RES_OK;
    }
    else
    {
        res = RES_ERROR;
    }
    
	return (res);
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	int result;


	switch (cmd) {
	case CTRL_SYNC:		
        SD_CS = 0;
        if(0 == SD_WaitReady())
        {
            res = RES_OK; 
        }
        else 
        {
            res = RES_ERROR;   
        }
        SD_CS = 1;
		break;

    case GET_SECTOR_SIZE:
        *(DWORD*)buff = SD_SECTOR_SIZE;
        res = RES_OK;
    break;        

	case GET_SECTOR_COUNT:
        *(DWORD*)buff = SD_GetSectorCount();
        res = RES_OK;
		break;

	case GET_BLOCK_SIZE:
        *(DWORD*)buff = 8;
        res = RES_OK;
		break;

	default:
		res = RES_PARERR;
	}

	return (res);
}
#endif


/*-----------------------------------------------------------------------*/
/* Get Fat Time                                                          */
/*-----------------------------------------------------------------------*/

DWORD get_fattime (void)
{
	DWORD time, temp;
    SYS_TIME sys_time;


    RtcTimeToSysTime(&sys_time);

    /**
     * bit31:25 年  （0~127）（从1980 开始）  
     * bit24:21 月  （1~12）  
     * bit20:16 日  （1~31）  
     * bit15:11 小时（0~23）  
     * bit10:5  分钟（0~59）  
     * bit4:0   秒  （0~29）  
     */
    temp = ((DWORD)sys_time.year + 2000) - 1980;
    time = temp << 25;

    temp = (DWORD)sys_time.mon;
    time |= temp << 21;

    temp = (DWORD)sys_time.date;
    time |= temp << 16;

    temp = (DWORD)sys_time.hour;
    time |= temp << 11;

    temp = (DWORD)sys_time.min;
    time |= temp << 5;

    temp = (DWORD)sys_time.sec;
    time |= temp << 2;
	
	return (time);
}

