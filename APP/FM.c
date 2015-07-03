/** 
 * @file     FM.c
 * @brief    文件管理
 * @details  应用层文件管理接口
 * @author   华兄
 * @email    591881218@qq.com
 * @date     2015
 * @version  vX.XX
 * @par Copyright (c):  
 *           深圳市合尔凯科技有限公司
 * @par History:          
 *   version: author, date, desc\n 
 */

#include "includes.h"


u8 fdisk_detect(void)
{
    u8 ret;
    FATFS fs;
    FIL fp;
    FRESULT res;


    res = f_mount(SD_DRV, &fs);

    if(FR_OK == res)
    {  
        res = f_open(&fp, "FDDETECT.TXT", FA_OPEN_EXISTING);

        if(FR_OK == res)
        {
            ret = TRUE;
        }
        else
        {
            f_close(&fp);
            
            res = f_open(&fp, "FDDETECT.TXT", FA_READ | FA_WRITE | FA_OPEN_ALWAYS);
            
            if(FR_OK == res)
            {
                f_puts("HRK Technology Inc.", &fp);
                
                ret = TRUE;
            }
            else
            {
                ret = FALSE;
            }
        }        

        f_close(&fp);
    }
    else
    {
        ret = FALSE;
    }

    f_mount(SD_DRV, NULL);

    return (ret);
}

u8 get_sd_info(void)
{
    FATFS fs, *p_fs;
    FRESULT res;
    u32 free_clusters, free_sectors, total_sectors;


    g_sys_ctrl.sd_total_capacity = 0; //总容量，单位KB
    g_sys_ctrl.sd_free_capacity = 0; //剩余容量，单位KB

    res = f_mount(SD_DRV, &fs);

    if(FR_OK != res)
    {
        return (FALSE);
    }

    p_fs = &fs;
    
    res = f_getfree("0:/", &free_clusters, &p_fs);

    if(FR_OK != res)
    {
        f_mount(SD_DRV, NULL);
        
        return (FALSE);
    }    

    total_sectors = (p_fs->n_fatent - 2) * p_fs->csize; //总扇区数
    free_sectors = free_clusters * p_fs->csize; //空闲扇区数

#if (512 != _MAX_SS)
    total_sectors *= p_fs->ssize / 512;
    free_sectors *= p_fs->ssize / 512;
#endif	    

    g_sys_ctrl.sd_total_capacity = total_sectors >> 1; //总容量，单位KB
    g_sys_ctrl.sd_free_capacity = free_sectors >> 1; //剩余容量，单位KB

    f_mount(SD_DRV, NULL);

    return (TRUE);
}

u32 get_file_num(void)
{
    u32 sd_file_num;
    FATFS fs;

    
    if(FR_OK == f_mount(SD_DRV, &fs))
    {
        scan_files("/");

        sd_file_num = 0;
        
        while(SD_FileName[sd_file_num][0])
        {
            sd_file_num++;
        }
    }

    
    f_mount(SD_DRV,NULL);
    
    return (sd_file_num);
}

void FM_Format_Disk(void)
{
    FRESULT res;
    FATFS fs;
    

    res = f_mount(SD_DRV, &fs);

    if(FR_OK == res)
    {
        res = f_mkfs(SD_DRV, SD_FORMAT, _MAX_SS);

        if(FR_OK == res)
        {
            DEBUG_PRINT(("SD format ok!\n"));
            
            GUI_Set_FD_Format_PROGBAR(100);
        }                        
    }
    
    f_mount(SD_DRV, NULL);
}

char SD_FileName[MAX_SD_FILE_NUM][MAX_SD_FILE_NAME_LEN] = {0};
unsigned long SD_FileSize[MAX_SD_FILE_NUM] = {0}; 
FIL_TIMESTAMP SD_FileTimestamp[MAX_SD_FILE_NUM] = {0};

#if (EWARM_OPTIMIZATION_EN > 0u)
#pragma optimize = low
#endif
void scan_files(char *path)
{	
	FILINFO finfo;
	DIR dirs;
	int i = 0;
	char *fname;
    
#if _USE_LFN
#define _DF1S	0x81
    static char lfn[_MAX_LFN * (_DF1S ? 2 : 1) + 1];
    finfo.lfname = lfn;
    finfo.lfsize = sizeof(lfn);
#endif


	if(FR_OK == f_opendir(&dirs, path)) 
	{
        memset(SD_FileName, 0, sizeof(SD_FileName));
        
		while(FR_OK == f_readdir(&dirs, &finfo))
		{
#if _USE_LFN
            fname = *finfo.lfname ? finfo.lfname : finfo.fname;
#else
            fname = finfo.fname;
#endif		

            if((!fname[0]) || (i >= (MAX_SD_FILE_NUM - 1)))
            {
            	break;
            }
            
            if(finfo.fattrib & AM_ARC)
            {
            	strcpy(SD_FileName[i], fname);
                
            	i++;
            }
		}
	}
}

// ---------------------------------------------------

#define TEST_SECTOR_COUNT                      2

u8 g_test_buf[SD_SECTOR_SIZE * TEST_SECTOR_COUNT];

void FatFs_Test(void)
{
    u16 i;
    u32 sd_sector_size, sd_sector_count, sd_capacity;
    FATFS fs;
    FIL fp;
    UINT br;
    DIR dj;
    FRESULT res;
    

    g_rtc_time[YEAR_POS] = Hex2BcdChar(15);
    g_rtc_time[MONTH_POS] = Hex2BcdChar(1);
    g_rtc_time[DATE_POS] = Hex2BcdChar(19);
    g_rtc_time[DAY_POS] = Hex2BcdChar(1);
    g_rtc_time[HOUR_POS] = Hex2BcdChar(15);
    g_rtc_time[MIN_POS] = Hex2BcdChar(51);
    g_rtc_time[SEC_POS] = Hex2BcdChar(15);                
    RTC_WriteTime(g_rtc_time);

    if(FALSE == get_sd_info())
    {
        DEBUG_PRINT(("Get SD info error!\n"));
    }

    res = f_mount(SD_DRV, &fs);

    if(FR_OK == res)
    {
        //res = f_mkfs(SD_DRV, SD_FORMAT, _MAX_SS);
        
        if(FR_OK == res)
        {
            //DEBUG_PRINT(("SD format ok!\n"));
        }
        else
        {
            //DEBUG_PRINT(("SD format error!\n"));
        }
        
        //res = f_mkdir("SD卡测试");

        if(FR_OK == res)
        {
        }
        else
        {
        }
        
        //res = f_chdir("SD卡测试");

        if(FR_OK == res)
        {
        }
        else
        {
        }
        
        res = f_open(&fp, "SD.txt", FA_READ | FA_WRITE | FA_OPEN_ALWAYS);

        if(FR_OK == res)
        {
        }
        else
        {
        }

        res = f_read(&fp, g_test_buf, SD_SECTOR_SIZE, &br);

        if((FR_OK == res) && (0 != br))
        {
            
        }
        else
        {
            for(i = 0; i < (SD_SECTOR_SIZE * TEST_SECTOR_COUNT); i++)
            {
                g_test_buf[i] = '\0';
            }

            sd_sector_size = SD_SECTOR_SIZE;
            sd_sector_count = SD_GetSectorCount();
            sd_capacity = sd_sector_size * sd_sector_count;
            
            sprintf(g_test_buf, "SD卡信息(FatFs) \n\nSD Sector Size: %lu\nSD Sector Count: %lu\nSD Capacity: %lu\n",
                    sd_sector_size,
                    sd_sector_count,
                    sd_capacity);
            
            f_puts(g_test_buf, &fp); 
        }

        f_close(&fp);
    }

    f_mount(SD_DRV, NULL);  
}

// ---------------------------------------------------

u16 sprintf_trm_msg_record(u8 msg_type)
{
    u8 *ptr;
    u16 i;
    u16 len;
    u8 buf[32];
    

    len = 0;

    if(TRM_MSG_NONE != msg_type)
    {
        memset(g_proto_para.fm_buf, ' ', sizeof(g_proto_para.fm_buf));

        sprintf(buf, "20%02x-%02x-%02x %02x:%02x:%02x  ", g_rtc_time[YEAR_POS], 
                                                          g_rtc_time[MONTH_POS],
                                                          g_rtc_time[DATE_POS],
                                                          g_rtc_time[HOUR_POS],
                                                          g_rtc_time[MIN_POS],
                                                          g_rtc_time[SEC_POS]);

        ptr = g_proto_para.fm_buf;

        memcpy(ptr, buf, 21);
        ptr += 21;
        len += 21;

        if(TRM_MSG_PLC_MONITOR == msg_type)
        {
            *ptr++ = 'X';
            *ptr++ = ':';
            *ptr++ = ' ';
            len += 3;
            
            for(i = 0; i < g_proto_para.recv_len; i++)
            {
                *ptr++ = GUI_Hex2Char(g_proto_para.recv_buf[i] >> 4);
                *ptr++ = GUI_Hex2Char(g_proto_para.recv_buf[i] & 0x0f);
                *ptr++ = ' ';
                len += 3;
            }

            sprintf(buf, " LEN: %03u", g_proto_para.recv_len);
        }   
        else if(TRM_MSG_SEND == msg_type)      
        {
            *ptr++ = 'S';
            *ptr++ = ':';
            *ptr++ = ' ';
            len += 3;

            for(i = 0; i < g_proto_para.send_len; i++)
            {
                *ptr++ = GUI_Hex2Char(g_proto_para.send_buf[i] >> 4);
                *ptr++ = GUI_Hex2Char(g_proto_para.send_buf[i] & 0x0f);
                *ptr++ = ' ';
                len += 3;
            }
            
            sprintf(buf, " LEN: %03u", g_proto_para.send_len);
        }
        else if(TRM_MSG_RECV == msg_type)
        {   
            *ptr++ = 'R';
            *ptr++ = ':';
            *ptr++ = ' ';
            len += 3;
            
            for(i = 0; i < g_proto_para.recv_len; i++)
            {
                *ptr++ = GUI_Hex2Char(g_proto_para.recv_buf[i] >> 4);
                *ptr++ = GUI_Hex2Char(g_proto_para.recv_buf[i] & 0x0f);
                *ptr++ = ' ';
                len += 3;
            }
            
            sprintf(buf, " LEN: %03u", g_proto_para.recv_len);
        }
        else
        {
            *ptr++ = 'X';
            *ptr++ = ':';
            *ptr++ = ' ';
            len += 3;
            
            sprintf(buf, " LEN: %03u", 0);
        }

        memcpy(ptr, buf, 9);
        ptr += 9;
        len += 9;

        *ptr++ = '\r';
        *ptr++ = '\n';
        len += 2;
    }

    return (len);
}

#if (EWARM_OPTIMIZATION_EN > 0u)
#pragma optimize = low
#endif
u8 trm_msg_record(u8 msg_type)
{
    FATFS fs;
    FIL fp;
    UINT br;
    DIR dj;
    FRESULT res;
    u8 fname[16], oldest_file, i;
    u16 bytes;
    u32 sd_file_num;
    FILINFO fil_info;


    if((TRM_MSG_RECV == msg_type) &&
       (CHANNEL_PLC == g_rom_para.channel) &&
       (GUI_CMD_PLC_READ_NODE == g_sys_ctrl.plc_state))
    {
        return (FALSE);
    }

    res = f_mount(SD_DRV, &fs); //挂载SD卡

    if(FR_OK != res)
    {
        DEBUG_PRINT(("SD mount error!\n"));

        f_mount(SD_DRV, NULL); //移除SD卡
        
        return (FALSE);
    }
    
    scan_files("/");
    
    sd_file_num = 0;
    while(SD_FileName[sd_file_num][0])
    {
        if(FR_OK == f_stat(SD_FileName[sd_file_num], &fil_info))
        {
            SD_FileTimestamp[sd_file_num].date = fil_info.fdate;
            SD_FileTimestamp[sd_file_num].time = fil_info.ftime;
        }
        else
        {
            SD_FileTimestamp[sd_file_num].date = 0;
            SD_FileTimestamp[sd_file_num].time = 0;
        }
        
        sd_file_num++;
    }    
    
    sprintf(fname, "20%02x%02x%02x.txt", g_rtc_time[YEAR_POS], g_rtc_time[MONTH_POS], g_rtc_time[DATE_POS]);
    fname[FILE_NAME_LEN] = '\0';
    
    if(FR_OK != f_open(&fp, fname, FA_OPEN_EXISTING))
    {
        if(MAX_FILE_NUM == sd_file_num) //文件数已满，删除最旧的文件
        {
            oldest_file = 0;
            i = 1;
            
            while(1)
            {
                if(SD_FileTimestamp[oldest_file].date > SD_FileTimestamp[i].date)
                {
                    oldest_file = i;
                }
                
                i++;
                
                if(i >= sd_file_num)
                {
                    break;
                }                
            }
            
            f_unlink(SD_FileName[oldest_file]);
        }
        else if(sd_file_num > MAX_FILE_NUM)//SD卡文件数不符合要求，全部删除
        {
            i = 0;
            
            while(1)
            {
                f_unlink(SD_FileName[i]);
                
                i++;
                
                if(i >= sd_file_num)
                {
                    break;
                }
            }
        }
        
        f_open(&fp, fname, FA_OPEN_ALWAYS); //创建新文件
    }
    
    f_close(&fp); //关闭文件
    
    res = f_open(&fp, fname, FA_READ | FA_WRITE | FA_OPEN_EXISTING); //以读写方式打开已存在文件

    if(FR_OK != res)
    {
        DEBUG_PRINT(("Open file error!\n"));

        f_close(&fp); //关闭文件
        f_mount(SD_DRV, NULL);
        
        return (FALSE);
    }

    res = f_lseek(&fp, fp.fsize); //移动到文件末尾

    if(FR_OK != res)
    {
        DEBUG_PRINT(("Lseek file error!\n"));

        f_close(&fp);
        f_mount(SD_DRV, NULL);
        
        return (FALSE);
    }  

    bytes = sprintf_trm_msg_record(msg_type);
        
    res = f_write(&fp, g_proto_para.fm_buf, bytes, &br); //追加文件

    if((FR_OK != res) || (!br))
    {
        DEBUG_PRINT(("Append file error!\n"));

        f_close(&fp);
        f_mount(SD_DRV, NULL);
        
        return (FALSE);
    }    

    f_close(&fp);
    f_mount(SD_DRV, NULL);

    return (TRUE);
}

