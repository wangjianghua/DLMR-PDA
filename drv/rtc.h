#ifndef __RTC_H__
#define __RTC_H__

#ifdef __cplusplus
 extern "C" {
#endif


#define RTC_SCL_SET()       HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET)
#define RTC_SCL_RST()       HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET)

#define RTC_SDA_SET()       HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET)
#define RTC_SDA_RST()       HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET)

#define SEC_POS                           0
#define MIN_POS                           1
#define HOUR_POS                          2
#define DAY_POS                           3
#define DATE_POS                          4
#define MONTH_POS                         5
#define YEAR_POS                          6
#define MAX_RTC_ITEM                      7

#define TIME_CH_LEN                       3

typedef struct _sys_time
{
    char year;
    char mon;
    char date;    
    char day;    
    char hour;
    char min;
    char sec;
} SYS_TIME, *P_SYS_TIME;

extern u8 g_rtc_time[MAX_RTC_ITEM];
extern u8 g_rtc_check[MAX_RTC_ITEM];

void RTC_ReadTime(unsigned char *time);
void RTC_WriteTime(unsigned char *time);
unsigned char Bcd2HexChar(unsigned char bcd);
unsigned char Hex2BcdChar(unsigned char hex);
void RtcTimeToSysTime(SYS_TIME *Time);
unsigned char *RTC2Text(void);
unsigned char *RTC2Text_Date(void);

void RTC_CheckTime(unsigned char * time);

#ifdef __cplusplus
}
#endif

#endif
