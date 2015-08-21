#include "includes.h"
#include "si4438.h"
#include "si4438_trx.h"



const u8 RF_POW_Start[]={CMD_POWER_UP, 0x01, 0x00, 0x01, 0xC9, 0xC3, 0x80};


const u8  Freq_Def[66][12]={
							0x11, 0x40, 0x08, 0x00, 0x3E, 0x0C, 0x0D, 0xA7, 0x44, 0x44, 0x20, 0xFE,//476.3
							0x11, 0x40, 0x08, 0x00, 0x3D, 0x0E, 0xEE, 0xEE, 0x36, 0x9D, 0x20, 0xFE,//471.5
							0x11, 0x40, 0x08, 0x00, 0x3D, 0x0F, 0x25, 0x8B, 0x36, 0x9D, 0x20, 0xFE,//471.7
							0x11, 0x40, 0x08, 0x00, 0x3D, 0x0F, 0x5C, 0x28, 0x36, 0x9D, 0x20, 0xFE,//471.9
							0x11, 0x40, 0x08, 0x00, 0x3D, 0x0F, 0x92, 0xC5, 0x36, 0x9D, 0x20, 0xFE,//472.1
							0x11, 0x40, 0x08, 0x00, 0x3D, 0x0F, 0xC9, 0x62, 0x36, 0x9D, 0x20, 0xFE,//472.3
							0x11, 0x40, 0x08, 0x00, 0x3E, 0x08, 0x00, 0x00, 0x36, 0x9D, 0x20, 0xFE,//472.5
							0x11, 0x40, 0x08, 0x00, 0x3E, 0x08, 0x36, 0x9D, 0x36, 0x9D, 0x20, 0xFE,//472.7
							0x11, 0x40, 0x08, 0x00, 0x3E, 0x08, 0x6D, 0x3A, 0x1B, 0x09, 0x20, 0xFE,//472.9
							0x11, 0x40, 0x08, 0x00, 0x3E, 0x08, 0xA3, 0xD7, 0x1B, 0x09, 0x20, 0xFE,//473.1
							0x11, 0x40, 0x08, 0x00, 0x3E, 0x08, 0xDA, 0x74, 0x1B, 0x09, 0x20, 0xFE,//473.3
							0x11, 0x40, 0x08, 0x00, 0x3E, 0x09, 0x11, 0x11, 0x1B, 0x09, 0x20, 0xFE,//473.5
							0x11, 0x40, 0x08, 0x00, 0x3E, 0x09, 0x47, 0xAE, 0x1B, 0x09, 0x20, 0xFE,//473.7
							0x11, 0x40, 0x08, 0x00, 0x3E, 0x09, 0x7E, 0x4B, 0x1B, 0x09, 0x20, 0xFE,//473.9
							0x11, 0x40, 0x08, 0x00, 0x3E, 0x09, 0xB4, 0xE8, 0x1B, 0x09, 0x20, 0xFE,//474.1
							0x11, 0x40, 0x08, 0x00, 0x3E, 0x09, 0xEB, 0x85, 0x1B, 0x09, 0x20, 0xFE,//474.3
							0x11, 0x40, 0x08, 0x00, 0x3E, 0x0A, 0x22, 0x22, 0x1B, 0x09, 0x20, 0xFE,//474.5
							0x11, 0x40, 0x08, 0x00, 0x3E, 0x0A, 0x58, 0xBF, 0x1B, 0x09, 0x20, 0xFE,//474.7
							0x11, 0x40, 0x08, 0x00, 0x3E, 0x0A, 0x8F, 0x5C, 0x1B, 0x09, 0x20, 0xFE,//474.9
							0x11, 0x40, 0x08, 0x00, 0x3E, 0x0A, 0xC5, 0xF9, 0x1B, 0x09, 0x20, 0xFE,//475.1
							0x11, 0x40, 0x08, 0x00, 0x3E, 0x0A, 0xFC, 0x96, 0x1B, 0x09, 0x20, 0xFE,//475.3
							0x11, 0x40, 0x08, 0x00, 0x3E, 0x0B, 0x33, 0x33, 0x1B, 0x09, 0x20, 0xFE,//475.5
							0x11, 0x40, 0x08, 0x00, 0x3E, 0x0B, 0x69, 0xD0, 0x1B, 0x09, 0x20, 0xFE,//475.7
							0x11, 0x40, 0x08, 0x00, 0x3E, 0x0B, 0xA0, 0x6D, 0x1B, 0x09, 0x20, 0xFE,//475.9
							0x11, 0x40, 0x08, 0x00, 0x3E, 0x0B, 0xD7, 0x0A, 0x1B, 0x09, 0x20, 0xFE,//476.1
							0x11, 0x40, 0x08, 0x00, 0x3E, 0x0C, 0x44, 0x44, 0x1B, 0x4F, 0x20, 0xFE,//476.5
							0x11, 0x40, 0x08, 0x00, 0x3E, 0x0C, 0x7A, 0xE1, 0x1B, 0x4F, 0x20, 0xFE,//476.7
							0x11, 0x40, 0x08, 0x00, 0x3E, 0x0C, 0xB1, 0x7E, 0x1B, 0x4F, 0x20, 0xFE,//476.9
							0x11, 0x40, 0x08, 0x00, 0x3E, 0x0C, 0xE8, 0x1B, 0x1B, 0x4F, 0x20, 0xFE,//477.1
							0x11, 0x40, 0x08, 0x00, 0x3E, 0x0D, 0x1E, 0xB8, 0x1B, 0x4F, 0x20, 0xFE,//477.3
							0x11, 0x40, 0x08, 0x00, 0x3E, 0x0D, 0x55, 0x55, 0x1B, 0x4F, 0x20, 0xFE,//477.5
							0x11, 0x40, 0x08, 0x00, 0x3E, 0x0D, 0x8B, 0xF2, 0x1B, 0x4F, 0x20, 0xFE,//477.7
							0x11, 0x40, 0x08, 0x00, 0x3E, 0x0D, 0xC2, 0x8F, 0x1B, 0x4F, 0x20, 0xFE,//477.9

							0x11, 0x40, 0x08, 0x00, 0x3F, 0x0D, 0x03, 0x69, 0x36, 0x9D, 0x20, 0xFE,//484.7
							0x11, 0x40, 0x08, 0x00, 0x3E, 0x0F, 0x40, 0xDA, 0x1B, 0x4F, 0x20, 0xFE,//479.3
							0x11, 0x40, 0x08, 0x00, 0x3E, 0x0F, 0x77, 0x77, 0x1B, 0x4F, 0x20, 0xFE,//479.5
							0x11, 0x40, 0x08, 0x00, 0x3E, 0x0F, 0xAE, 0x14, 0x1B, 0x4F, 0x20, 0xFE,//479.7
							0x11, 0x40, 0x08, 0x00, 0x3E, 0x0F, 0xE4, 0xB1, 0x1B, 0x4F, 0x20, 0xFE,//479.9
							0x11, 0x40, 0x08, 0x00, 0x3F, 0x08, 0x1B, 0x4E, 0x1B, 0x4F, 0x20, 0xFE,//480.1
							0x11, 0x40, 0x08, 0x00, 0x3F, 0x08, 0x51, 0xEB, 0x1B, 0x4F, 0x20, 0xFE,//480.3
							0x11, 0x40, 0x08, 0x00, 0x3F, 0x08, 0x88, 0x88, 0x1B, 0x4F, 0x20, 0xFE,//480.5
							0x11, 0x40, 0x08, 0x00, 0x3F, 0x08, 0xBF, 0x25, 0x1B, 0x4F, 0x20, 0xFE,//480.7
							0x11, 0x40, 0x08, 0x00, 0x3F, 0x08, 0xF5, 0xC2, 0x1B, 0x4F, 0x20, 0xFE,//480.9
							0x11, 0x40, 0x08, 0x00, 0x3F, 0x09, 0x2C, 0x5F, 0x1B, 0x4F, 0x20, 0xFE,//481.1
							0x11, 0x40, 0x08, 0x00, 0x3F, 0x09, 0x62, 0xFC, 0x1B, 0x4F, 0x20, 0xFE,//481.3
							0x11, 0x40, 0x08, 0x00, 0x3F, 0x09, 0x99, 0x99, 0x1B, 0x4F, 0x20, 0xFE,//481.5
							0x11, 0x40, 0x08, 0x00, 0x3F, 0x09, 0xD0, 0x36, 0x1B, 0x4F, 0x20, 0xFE,//481.7
							0x11, 0x40, 0x08, 0x00, 0x3F, 0x0A, 0x06, 0xD3, 0x1B, 0x4F, 0x20, 0xFE,//481.9
							0x11, 0x40, 0x08, 0x00, 0x3F, 0x0A, 0x3D, 0x70, 0x1B, 0x4F, 0x20, 0xFE,//482.1
							0x11, 0x40, 0x08, 0x00, 0x3F, 0x0A, 0x74, 0x0D, 0x1B, 0x4F, 0x20, 0xFE,//482.3
							0x11, 0x40, 0x08, 0x00, 0x3F, 0x0A, 0xAA, 0xAA, 0x1B, 0x4F, 0x20, 0xFE,//482.5
							0x11, 0x40, 0x08, 0x00, 0x3F, 0x0A, 0xE1, 0x47, 0x1B, 0x4F, 0x20, 0xFE,//482.7
							0x11, 0x40, 0x08, 0x00, 0x3F, 0x0B, 0x17, 0xE4, 0x1B, 0x4F, 0x20, 0xFE,//482.9
							0x11, 0x40, 0x08, 0x00, 0x3F, 0x0B, 0x4E, 0x81, 0x1B, 0x4F, 0x20, 0xFE,//483.1
							0x11, 0x40, 0x08, 0x00, 0x3F, 0x0B, 0x85, 0x1E, 0x1B, 0x4F, 0x20, 0xFE,//483.3
							0x11, 0x40, 0x08, 0x00, 0x3F, 0x0B, 0xBB, 0xBB, 0x1B, 0x4F, 0x20, 0xFE,//483.5
							0x11, 0x40, 0x08, 0x00, 0x3F, 0x0B, 0xF2, 0x58, 0x1B, 0x4F, 0x20, 0xFE,//483.7
							0x11, 0x40, 0x08, 0x00, 0x3F, 0x0C, 0x28, 0xF5, 0x1B, 0x4F, 0x20, 0xFE,//483.9
							0x11, 0x40, 0x08, 0x00, 0x3F, 0x0C, 0x5F, 0x92, 0x1B, 0x4F, 0x20, 0xFE,//484.1
							0x11, 0x40, 0x08, 0x00, 0x3F, 0x0C, 0x96, 0x2F, 0x1B, 0x4F, 0x20, 0xFE,//484.3
							0x11, 0x40, 0x08, 0x00, 0x3F, 0x0C, 0xCC, 0xCC, 0x1B, 0x4F, 0x20, 0xFE,//484.5
							0x11, 0x40, 0x08, 0x00, 0x3F, 0x0D, 0x3A, 0x06, 0x1B, 0x4F, 0x20, 0xFE,//484.9
							0x11, 0x40, 0x08, 0x00, 0x3F, 0x0D, 0x70, 0xA3, 0x1B, 0x4F, 0x20, 0xFE,//485.1
							0x11, 0x40, 0x08, 0x00, 0x3F, 0x0D, 0xA7, 0x40, 0x1B, 0x4F, 0x20, 0xFE,//485.3
							0x11, 0x40, 0x08, 0x00, 0x3F, 0x0D, 0xDD, 0xDD, 0x1B, 0x4F, 0x20, 0xFE,//485.5
							0x11, 0x40, 0x08, 0x00, 0x3F, 0x0E, 0x14, 0x7A, 0x1B, 0x4F, 0x20, 0xFE,//485.7
							};


u8 g_get_pro[4] = {CMD_GET_PROPERTY,PROP_INT_CTL_GROUP,4,PROP_INT_CTL_ENABLE};
        
const u8 Config_LIB[]=RADIO_CONFIGURATION_DATA_ARRAY;

#if 1
const u8  Parameters_def[]={\
			0x08,CMD_SET_PROPERTY, PROP_INT_CTL_GROUP,4,PROP_INT_CTL_ENABLE,0x01,0x3B,0x00,0x00,\
			0x08,CMD_SET_PROPERTY,PROP_FRR_CTL_GROUP,4,PROP_FRR_CTL_A_MODE,0x04,0x06,0x0A,0x00,\
			0x05, CMD_SET_PROPERTY,PROP_PREAMBLE_GROUP,1,PROP_PREAMBLE_CONFIG_STD_1,0x20,\
			0x05,CMD_SET_PROPERTY,PROP_PREAMBLE_GROUP,1,PROP_PREAMBLE_TX_LENGTH,0x50,\
			0x05,CMD_SET_PROPERTY,PROP_PREAMBLE_GROUP,1,PROP_PREAMBLE_CONFIG,0x31,\
			0x07,CMD_SET_PROPERTY,PROP_SYNC_GROUP,3,PROP_SYNC_CONFIG,0x01,0x98,0xF3,\
			0x05,CMD_SET_PROPERTY,PROP_PKT_GROUP,1,PROP_PKT_CONFIG1,0x81,\
			0x07,CMD_SET_PROPERTY,PROP_PKT_GROUP,3,PROP_PKT_LEN,0x02,0x01,0x02,\
			0x08,CMD_SET_PROPERTY,PROP_PKT_GROUP,4,PROP_PKT_FIELD_1_LENGTH_12_8,0x00,0x00,0x00,0x00,\
			0x08,CMD_SET_PROPERTY,PROP_PKT_GROUP,4,PROP_PKT_RX_FIELD_1_LENGTH_12_8,0x00,0x01,0x00,0x00,\
			0x08,CMD_SET_PROPERTY,PROP_PKT_GROUP,4,PROP_PKT_RX_FIELD_2_LENGTH_12_8,0x00,MAX_PAYLOAD_LENGTH + 2,0x00,0x00,\
			0x05,CMD_SET_PROPERTY,PROP_PKT_GROUP,1,PROP_PKT_CRC_CONFIG,0x85,\
			0x06,CMD_SET_PROPERTY,PROP_PKT_GROUP,2,PROP_PKT_TX_THRESHOLD,TX_FIFO_ALMOST_EMPTY_THRESHOLD,RX_FIFO_ALMOST_FULL_THRESHOLD,\
			0x05,CMD_SET_PROPERTY,PROP_MODEM_GROUP,1,PROP_MODEM_RSSI_CONTROL,0x12,\
			0x07,CMD_SET_PROPERTY,PROP_PKT_GROUP,3,PROP_PKT_WHT_SEED_15_8,0xFF,0xFF,0x80,\
			0x05,CMD_SET_PROPERTY,PROP_GLOBAL_GROUP,1,PROP_GLOBAL_XO_TUNE,CAP_BANK_VALUE,\
			0x00};
#else
const u8  Parameters_def[]={\
			0x08,CMD_SET_PROPERTY, PROP_INT_CTL_GROUP,4,PROP_INT_CTL_ENABLE,0x01,0x33,0x00,0x00,\
			0x08,CMD_SET_PROPERTY,PROP_FRR_CTL_GROUP,4,PROP_FRR_CTL_A_MODE,0x04,0x06,0x0A,0x00,\
			0x05, CMD_SET_PROPERTY,PROP_PREAMBLE_GROUP,1,PROP_PREAMBLE_CONFIG_STD_1,0x20,\
			0x05,CMD_SET_PROPERTY,PROP_PREAMBLE_GROUP,1,PROP_PREAMBLE_TX_LENGTH,0x05,\
			0x05,CMD_SET_PROPERTY,PROP_PREAMBLE_GROUP,1,PROP_PREAMBLE_CONFIG,0x31,\
			0x07,CMD_SET_PROPERTY,PROP_SYNC_GROUP,3,PROP_SYNC_CONFIG,0x01,0x98,0xF3,\
			0x05,CMD_SET_PROPERTY,PROP_PKT_GROUP,1,PROP_PKT_CONFIG1,0x81,\
			0x07,CMD_SET_PROPERTY,PROP_PKT_GROUP,3,PROP_PKT_LEN,0x02,0x01,0x02,\
			0x08,CMD_SET_PROPERTY,PROP_PKT_GROUP,4,PROP_PKT_FIELD_1_LENGTH_12_8,0x00,0x00,0x00,0x00,\
			0x08,CMD_SET_PROPERTY,PROP_PKT_GROUP,4,PROP_PKT_RX_FIELD_1_LENGTH_12_8,0x00,0x01,0x00,0x00,\
			0x08,CMD_SET_PROPERTY,PROP_PKT_GROUP,4,PROP_PKT_RX_FIELD_2_LENGTH_12_8,0x00,MAX_PAYLOAD_LENGTH + 2,0x00,0x00,\
			0x05,CMD_SET_PROPERTY,PROP_PKT_GROUP,1,PROP_PKT_CRC_CONFIG,0x83,\
			0x06,CMD_SET_PROPERTY,PROP_PKT_GROUP,2,PROP_PKT_TX_THRESHOLD,TX_FIFO_ALMOST_EMPTY_THRESHOLD,RX_FIFO_ALMOST_FULL_THRESHOLD,\
			0x05,CMD_SET_PROPERTY,PROP_MODEM_GROUP,1,PROP_MODEM_RSSI_CONTROL,0x12,\
			0x07,CMD_SET_PROPERTY,PROP_PKT_GROUP,3,PROP_PKT_WHT_SEED_15_8,0xFF,0xFF,0x80,\
			//0x05,CMD_GPIO_PIN_CFG,0x21,0x14,0x20,0x10,
			0x05,CMD_SET_PROPERTY,PROP_GLOBAL_GROUP,1,PROP_GLOBAL_XO_TUNE,CAP_BANK_VALUE,\
			0x00};
//------------------------------------------------------------------------------------------------------
#endif

const u8  RF_BPS2400_TUNE_1[]={ 0x11, 0x00, 0x01, 0x00, 0x52};
const u8 RF_BPS2400_TYPE_12[]={ 0x11, 0x20, 0x0C, 0x00, 0x03, 0x00, 0x07, 0x00, 0x5D, 0xC0, 0x01, 0xC9, 0xC3, 0x80, 0x00, 0x05};

//const u8  RF_GLOBAL_XO_TUNE_1[]={CMD_SET_PROPERTY, 0x00, 0x01, 0x00, 0x52};
const u8 STart_RX[]={CMD_START_RX,0x00,0x00,0x00,0x00,0x00,0x03,0x08};
const u8 Start_Tx_cmd[]={CMD_START_TX,0,0x30,0x00,0x00};
// Start Tx
     
const  u8  FIFO_RESET_cmd[]={CMD_FIFO_INFO,0x03};
const u8 CMD_PACKET_cmd[2]={CMD_PACKET_INFO,0};

const u8 Read_INT_Status_cmd[4]={CMD_GET_INT_STATUS,0,0,0};
const u8 SI_config_TxINT1[8]={CMD_SET_PROPERTY,PROP_INT_CTL_GROUP,4,PROP_INT_CTL_ENABLE,0x01,0x20,0x00,0x00};
const u8 SI_config_TxINT2[8]={CMD_SET_PROPERTY,PROP_INT_CTL_GROUP,4,PROP_INT_CTL_ENABLE,0x01,0x22,0x00,0x00};
const u8 Si_config_RxINT[8]={CMD_SET_PROPERTY,PROP_INT_CTL_GROUP,4,PROP_INT_CTL_ENABLE,0x01,0x19,0x00,0x00};

const u8 Read_ID_cmd[8] = {CMD_PART_INFO,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

u8 Api_Write[16];			/*Ð´ÃüÁîÊý×é*/


extern RF_PARA g_rf_para;
extern u8 g_rf_buf[];
extern u8 rf_part_info[8];
extern u8 rf_device_id[8];
extern u8 rf_int_status[8];


u8 SI_FastResp(u8 Data_len,uc8 *CmdData, u8 Resp_Len, u8 *Resp_Data)    
{
  u16 i, temp;

  u8 Cts_Value=0;
  u16 ErrCnt=0, Cnt;
  
  NSEL_CLR;  
    
  for (i=0; i<Data_len; i++)    // Send input data array via SPI
  {
     
    while (!__HAL_SPI_GET_FLAG(&SpiHandle, SPI_FLAG_TXE)); //¼ì²éÖ¸¶¨µÄSPI±êÖ¾Î»ÉèÖÃÓë·ñ:·¢ËÍ»º´æ¿Õ±êÖ¾Î»
	SpiHandle.Instance->DR = CmdData[i];
			  
	while (!__HAL_SPI_GET_FLAG(&SpiHandle, SPI_FLAG_RXNE));
    temp = SpiHandle.Instance->DR;
    
    //return SpiHandle.Instance->DR;

  }

  for (i=0; i<Resp_Len; i++)    // Send input data array via SPI
  {
     
    while (!__HAL_SPI_GET_FLAG(&SpiHandle, SPI_FLAG_TXE)); //¼ì²éÖ¸¶¨µÄSPI±êÖ¾Î»ÉèÖÃÓë·ñ:·¢ËÍ»º´æ¿Õ±êÖ¾Î»
	SpiHandle.Instance->DR = 0xff;
			  
	while (!__HAL_SPI_GET_FLAG(&SpiHandle, SPI_FLAG_RXNE));
    Resp_Data[i] = SpiHandle.Instance->DR;    

  }
  
  NSEL_SET; 
 

  
  return 0;

}

/************************************************/
//º¯ÊýÃû£ºSendCommand 
//¹¦ÄÜ£ºÏò4463·¢ËÍÊý×éÊý¾Ý
//²ÎÊý£ºuint8_t *CmdData  Ðè·¢ËÍÊý¾Ý°üµÄÊ×µØÖ·£¬
//²ÎÊý£ºuint8_t Data_len   Ðè·¢ËÍÊý¾ÝµÄ³¤¶È
//·µ»ØÖµ£º
/************************************************/
u8 SI_SendCmd_Resp(u8 Data_len,uc8 *CmdData, u8 Resp_Len, u8 *Resp_Data)    
{
    u16 i, temp;

    u8 Cts_Value=0;
    u16 ErrCnt=0, Cnt;

    NSEL_CLR;  

    for (i=0; i<Data_len; i++)    // Send input data array via SPI
    {
     
        while (!__HAL_SPI_GET_FLAG(&SpiHandle, SPI_FLAG_TXE)); //¼ì²éÖ¸¶¨µÄSPI±êÖ¾Î»ÉèÖÃÓë·ñ:·¢ËÍ»º´æ¿Õ±êÖ¾Î»
        SpiHandle.Instance->DR = CmdData[i];
        		  
        while (!__HAL_SPI_GET_FLAG(&SpiHandle, SPI_FLAG_RXNE));
        temp = SpiHandle.Instance->DR;

        //return SpiHandle.Instance->DR;

    }

    NSEL_SET;    


    while (Cts_Value!=0xFF)                 
    {
        NSEL_CLR;

        while (!__HAL_SPI_GET_FLAG(&SpiHandle, SPI_FLAG_TXE)); //¼ì²éÖ¸¶¨µÄSPI±êÖ¾Î»ÉèÖÃÓë·ñ:·¢ËÍ»º´æ¿Õ±êÖ¾Î»
        SpiHandle.Instance->DR = CMD_CTS_READ;
        		  
        while (!__HAL_SPI_GET_FLAG(&SpiHandle, SPI_FLAG_RXNE));
        temp = SpiHandle.Instance->DR;

        while (!__HAL_SPI_GET_FLAG(&SpiHandle, SPI_FLAG_TXE)); //¼ì²éÖ¸¶¨µÄSPI±êÖ¾Î»ÉèÖÃÓë·ñ:·¢ËÍ»º´æ¿Õ±êÖ¾Î»
        SpiHandle.Instance->DR = 0xff;
        		  
        while (!__HAL_SPI_GET_FLAG(&SpiHandle, SPI_FLAG_RXNE));
        Cts_Value = SpiHandle.Instance->DR;
            	
        if(Cts_Value != 0xFF)
        {
            NSEL_SET;
        }
        else 
            break;

        if(++ErrCnt > MAX_CTS_RETRY)
           return 1;
    }

    if(Resp_Len)
    {
        //Get_Data_Group(Resp_Len, Resp_Data); 
        for (Cnt=0; Cnt<Resp_Len; Cnt++)
        {    
            while (!__HAL_SPI_GET_FLAG(&SpiHandle, SPI_FLAG_TXE)); //¼ì²éÖ¸¶¨µÄSPI±êÖ¾Î»ÉèÖÃÓë·ñ:·¢ËÍ»º´æ¿Õ±êÖ¾Î»
            SpiHandle.Instance->DR = 0xff;
            		  
            while (!__HAL_SPI_GET_FLAG(&SpiHandle, SPI_FLAG_RXNE));
            Resp_Data[Cnt] = SpiHandle.Instance->DR;
        }
    }

    NSEL_SET;    

    return 0;

}


/************************************************/
//º¯ÊýÃû£ºSendCommand 
//¹¦ÄÜ£ºÏò4463·¢ËÍÊý×éÊý¾Ý
//²ÎÊý£ºuint8_t *CmdData  Ðè·¢ËÍÊý¾Ý°üµÄÊ×µØÖ·£¬
//²ÎÊý£ºuint8_t Data_len   Ðè·¢ËÍÊý¾ÝµÄ³¤¶È
//·µ»ØÖµ£º
/************************************************/
u8 SI_SendCmd_Noresp(u8 Data_len,const u8 *CmdData)    
{
  u16 i,temp;

  u8 Cts_Value=0;
  u16 ErrCnt=0;
  
  NSEL_CLR;  
    
  for (i=0; i<Data_len; i++)    // Send input data array via SPI
  {
     //SPI2_ReadWriteByte(Data_sz[i]);
    while (!__HAL_SPI_GET_FLAG(&SpiHandle, SPI_FLAG_TXE)); //¼ì²éÖ¸¶¨µÄSPI±êÖ¾Î»ÉèÖÃÓë·ñ:·¢ËÍ»º´æ¿Õ±êÖ¾Î»
	SpiHandle.Instance->DR = CmdData[i];
			  
	while (!__HAL_SPI_GET_FLAG(&SpiHandle, SPI_FLAG_RXNE));
    temp = SpiHandle.Instance->DR;
    
    //return SpiHandle.Instance->DR;

  }
  
  NSEL_SET;    


  while (Cts_Value!=0xFF)                 
  {
    NSEL_CLR;
    
    while (!__HAL_SPI_GET_FLAG(&SpiHandle, SPI_FLAG_TXE)); //¼ì²éÖ¸¶¨µÄSPI±êÖ¾Î»ÉèÖÃÓë·ñ:·¢ËÍ»º´æ¿Õ±êÖ¾Î»
	SpiHandle.Instance->DR = CMD_CTS_READ;
			  
	while (!__HAL_SPI_GET_FLAG(&SpiHandle, SPI_FLAG_RXNE));
    temp = SpiHandle.Instance->DR;
    
    while (!__HAL_SPI_GET_FLAG(&SpiHandle, SPI_FLAG_TXE)); //¼ì²éÖ¸¶¨µÄSPI±êÖ¾Î»ÉèÖÃÓë·ñ:·¢ËÍ»º´æ¿Õ±êÖ¾Î»
	SpiHandle.Instance->DR = 0xff;
			  
	while (!__HAL_SPI_GET_FLAG(&SpiHandle, SPI_FLAG_RXNE));
    Cts_Value = SpiHandle.Instance->DR;
        	
    if(Cts_Value != 0xFF)
    {
        NSEL_SET;
    }
    else 
        break;
    
    if(++ErrCnt > MAX_CTS_RETRY)
       return 1;
  }
  
  NSEL_SET;    
  
  return 0;
  //return 0;
}


/***********************************************
//
//
//      »ñÈ¡4438µÄID
//
//
************************************************/

u32 SI_Read_ID(void)
{
    SI_SendCmd_Resp(4,Read_ID_cmd,8,rf_device_id);
    return((rf_device_id[1]<<8)|rf_device_id[2]);
}




/************************************************/
//º¯ÊýÃû£ºWrite_TxData 
//¹¦ÄÜ£ºÐ´ÈëFIFOÊý×éÊý¾Ý
//²ÎÊý£ºuint8_t *Tx_Data  Ðè·¢ËÍÊý¾Ý°üµÄÊ×µØÖ·£¬
//²ÎÊý£ºuint8_t Tx_Len   Ðè·¢ËÍÊý¾ÝµÄ³¤¶È
//·µ»ØÖµ£º
/************************************************/
u8 SI_Write_Fifo(u8 Tx_Len,const u8 *Tx_Data) 
{ 
    u8 Cts_Value=0;
    u16 ErrCnt=0;
    u16 i,temp;

    u8 len;
    
#if 0    
	if(Tx_Len > TX_AE_THRESHOLD)
        len = TX_AE_THRESHOLD;//·¢ËÍµôTX_AE_THRESHOLD£¬ÔÚÌîTX_AE_THRESHOLDµÄÊý¾        
	else 
        len = Tx_Len; 
#else //»ªÐÖ
    len = Tx_Len;
#endif

    NSEL_CLR;
   
    while (!__HAL_SPI_GET_FLAG(&SpiHandle, SPI_FLAG_TXE)); //¼ì²éÖ¸¶¨µÄSPI±êÖ¾Î»ÉèÖÃÓë·ñ:·¢ËÍ»º´æ¿Õ±êÖ¾Î»
    SpiHandle.Instance->DR = CMD_TX_FIFO_WRITE;
    		  
    while (!__HAL_SPI_GET_FLAG(&SpiHandle, SPI_FLAG_RXNE));
    Cts_Value = SpiHandle.Instance->DR;

#if 0
    while (!__HAL_SPI_GET_FLAG(&SpiHandle, SPI_FLAG_TXE)); //¼ì²éÖ¸¶¨µÄSPI±êÖ¾Î»ÉèÖÃÓë·ñ:·¢ËÍ»º´æ¿Õ±êÖ¾Î»
#if 0
    SpiHandle.Instance->DR = Tx_Len + 1;
#else //»ªÐÖ
    SpiHandle.Instance->DR = Tx_Len - 2; //Êý¾ÝÊµ¼Ê³¤¶È
#endif
    		  
    while (!__HAL_SPI_GET_FLAG(&SpiHandle, SPI_FLAG_RXNE));
    Cts_Value = SpiHandle.Instance->DR;
#endif
 
    for (i=0; i < len; i++)    // Send input data array via SPI
    {
       
        while (!__HAL_SPI_GET_FLAG(&SpiHandle, SPI_FLAG_TXE)); //¼ì²éÖ¸¶¨µÄSPI±êÖ¾Î»ÉèÖÃÓë·ñ:·¢ËÍ»º´æ¿Õ±êÖ¾Î»
        SpiHandle.Instance->DR = Tx_Data[i];
        	  
        while (!__HAL_SPI_GET_FLAG(&SpiHandle, SPI_FLAG_RXNE));

        temp = SpiHandle.Instance->DR;       
    }

    NSEL_SET;


    return 0;
  
}



/************************************************/
//º¯ÊýÃû£ºRead_FifoData 
//¹¦ÄÜ£º¶ÁÈ¡FIFOÊý×éÊý¾Ý
//²ÎÊý£ºuint8_t *Tx_Data  Ðè·¢ËÍÊý¾Ý°üµÄÊ×µØÖ·£¬
//²ÎÊý£ºuint8_t Tx_Len   Ðè·¢ËÍÊý¾ÝµÄ³¤¶È
//·µ»ØÖµ£º
/************************************************/
u8 SI_Read_Fifo(u8 Tx_Len,u8 *Tx_Data)  //(u8 leng,u8 *buf)
{ 
    u8 Cts_Value=0;
    u16 ErrCnt=0;
    u16 i,temp;

    NSEL_CLR;
    //SPI2_ReadWriteByte(CMD_TX_FIFO_WRITE); //·¢ËÍÐ´ÃüÁî
    while (!__HAL_SPI_GET_FLAG(&SpiHandle, SPI_FLAG_TXE)); //¼ì²éÖ¸¶¨µÄSPI±êÖ¾Î»ÉèÖÃÓë·ñ:·¢ËÍ»º´æ¿Õ±êÖ¾Î»
    SpiHandle.Instance->DR = CMD_RX_FIFO_READ;
    		  
    while (!__HAL_SPI_GET_FLAG(&SpiHandle, SPI_FLAG_RXNE));
    Cts_Value = SpiHandle.Instance->DR;

    //NSEL_SET;
    //end_Data_Group(Tx_Len,Tx_Data);

    

    //NSEL_CLR;
 
    for (i = 0; i < Tx_Len; i++)    // Send input data array via SPI
    {
        //SPI2_ReadWriteByte(Data_sz[i]);
        while (!__HAL_SPI_GET_FLAG(&SpiHandle, SPI_FLAG_TXE)); //¼ì²éÖ¸¶¨µÄSPI±êÖ¾Î»ÉèÖÃÓë·ñ:·¢ËÍ»º´æ¿Õ±êÖ¾Î»
        SpiHandle.Instance->DR = 0xff;
        	  
        while (!__HAL_SPI_GET_FLAG(&SpiHandle, SPI_FLAG_RXNE));
         Tx_Data[i] = SpiHandle.Instance->DR;       
    }

    NSEL_SET;

    return 0;
  
}



void SI_Freq_Set(u8 arry,u8 chen)
{
      SI_SendCmd_Noresp(12, Freq_Def[(arry*33)+chen]);       
}

u8 SI_Read_INT_Info(void)
{
      //u8 bItStatus[9];
      SI_SendCmd_Resp(4,Read_INT_Status_cmd,8,rf_int_status);
      //SI_SendCmd_Resp(4,Read_INT_Status_cmd,8,bItStatus);    	// Send command to the radio IC     
      return(rf_int_status[3]);
}




u8 SI_Read_RSSI(void)
{
    u8 bRssi;
    uc8  CmdData = CMD_FAST_RESPONSE_REG_C;
    //SI4438_Read(CMD_FAST_RESPONSE_REG_C,1,&bRssi);
    SI_FastResp(1,&CmdData, 1, &bRssi);
    return(bRssi);
}




u8 SI_Read_Length(void)
{
    u8 g_leng_abApi_buf[3];
    SI_SendCmd_Resp(2,CMD_PACKET_cmd,2,g_leng_abApi_buf);      				// Send command to the radio IC
    
    return(g_leng_abApi_buf[1]);
}



u8 SI_Send_Packet(u8 bLength, u8 *abPayload)
{
    u8 abApi_Write[9];
    u8 leng;
    u8 *s;
    abApi_Write[0] = CMD_CHANGE_STATE;        // Use change state command
    abApi_Write[1] = SI_STATE_READY;            // Go to Ready mode
    											//bApi_SendCommand(2,abApi_Write);        // Send command to the radio IC
    SI_SendCmd_Noresp(2,&abApi_Write[0] );
                 					// Wait for CTS

    abApi_Write[0] = CMD_FIFO_INFO;       		// Use FIFO info command to get the packet length information
    abApi_Write[1] =0x01;                 				//TX fifo reset
    SI_SendCmd_Noresp(2,abApi_Write);      		// Send command to the radio IC
    

    if(bLength > (TX_Len_MAX - 1))
  	{
  		leng = (TX_Len_MAX - 1);
		s = (u8 *)&SI_config_TxINT2[0];
  	}
    else 
    {
        leng = bLength;
        s = (u8 *)&SI_config_TxINT1[0];
    }
  
    SI_Write_Fifo(leng, abPayload);   	// Write data to Tx FIFO
    
    SI_SendCmd_Noresp(8,s);

    SI_SendCmd_Resp(4, g_get_pro, 4, rf_part_info);

    
    // Set TX packet length
    abApi_Write[0] = CMD_SET_PROPERTY;        // Use property command
    abApi_Write[1] = PROP_PKT_GROUP;        // Select property group
    abApi_Write[2] = 2;               // Number of properties to be written
    abApi_Write[3] = PROP_PKT_FIELD_1_LENGTH_7_0; // Specify first property
#if 0    
    abApi_Write[4] = bLength + 1;    // Field length (variable packet length info)
#else //»ªÐÖ
    abApi_Write[4] = bLength;
#endif
    //bApi_SendCommand(5,abApi_Write);        // Send command to the radio IC
    SI_SendCmd_Noresp(5,&abApi_Write[0] );
    

    // Read ITs, clear pending ones
    abApi_Write[0] = CMD_GET_INT_STATUS;  // Use interrupt status command
    abApi_Write[1] = 0;           // Clear PH_CLR_PEND
    abApi_Write[2] = 0;           // Clear MODEM_CLR_PEND
    abApi_Write[3] = 0;           // Clear CHIP_CLR_PEND
    // bApi_SendCommand(4,abApi_Write);    // Send command to the radio IC
    SI_SendCmd_Resp(4,&abApi_Write[0], 8,abApi_Write );
    
    // Start Tx
    SI_SendCmd_Noresp(5,Start_Tx_cmd);
    
    
    return(leng);
}

void SI_Enter_Rx(void)
{
       
       
       SI_SendCmd_Noresp(8,Si_config_RxINT);        // start rx       
     	
       SI_SendCmd_Noresp(2, FIFO_RESET_cmd);        // fifo reset
	 
       SI_SendCmd_Noresp(8,STart_RX);        // start rx
     
}

u16	cal_crc_ITU(u8 *ptr,u16 len)
{
	u16 crc=0xFFFF;
	u8 i;
	while(len--)
	{
		crc^=*ptr++;
		for(i=0;i<8;i++)
		{
			if(crc&0x0001)
			{
				crc>>=1;
				crc^=0x8408;
			}
			else
				crc>>=1;
		}
	}
	crc^=0xFFFF;
	return(crc);
}

rf_status_t SI_Init()
{
	
    u16 i;
    u8  *ptr;
    U8 L;
    
    ptr=(u8 *)Config_LIB;	

    {
        for(i=0; i<36; i++)
        {
            L=(*ptr);
            ptr++;
            if(!L)
                break;
            SI_SendCmd_Noresp(L,ptr);       // Send API command to the radio IC
            ptr+=L;
        }

        SI_Freq_Set(0,0);
        ptr=(u8 *)Parameters_def;
        for(i=0;i<36;i++)
        {
            L=(*ptr);
            ptr++;
            if(!L)break;
            SI_SendCmd_Noresp(L,ptr);       // Send API command to the radio IC
            ptr+=L;
        }

        //bApi_SendCommand(5, RF_BPS2400_TUNE_1);        // fifo reset
        //vApi_WaitforCTS();	

        // bApi_SendCommand(16, RF_BPS2400_TYPE_12);        // fifo reset
        // vApi_WaitforCTS();	

        SI_Read_INT_Info();
        SI_SendCmd_Noresp(2, FIFO_RESET_cmd);        // fifo reset
        	
        SI_SendCmd_Noresp(8,STart_RX);        // start rx
   
    }
    
    g_rf_para.rf_state = RF_STATE_LISTEN;
	return RF_STATUS_SUCCESS;
}

