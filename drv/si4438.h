
#ifndef RADIO_CONFIG_H_A
#define RADIO_CONFIG_H_A

// USER DEFINED PARAMETERS
// Define your own parameters here

// INPUT DATA
/*
// Crys_freq(Hz): 30000000    Crys_tol(ppm): 20    IF_mode: 2    High_perf_Ch_Fil: 1    OSRtune: 0    Ch_Fil_Bw_AFC: 0    ANT_DIV: 0    PM_pattern: 0    
// MOD_type: 3    Rsymb(sps): 10000    Fdev(Hz): 20000    RXBW(Hz): 150000    Manchester: 0    AFC_en: 0    Rsymb_error: 0.0    Chip-Version: 2    
// RF Freq.(MHz): 490    API_TC: 29    fhst: 250000    inputBW: 0    BERT: 0    RAW_dout: 0    D_source: 0    Hi_pfm_div: 1    
// 
// # WB filter 3 (BW =  92.61 kHz);  NB-filter 3 (BW = 92.61 kHz) 

// 
// Modulation index: 4
*/
#define CAP_BANK_VALUE        0x48  // Capacitor bank value for adjusting the XTAL frequency

#define ACK_TIMEOUT         5000  // Waiting cycles until getting the acknowLEDge
 #define MAX_PAYLOAD_LENGTH                   250                   // Maximum length of the packet, it does not include the length bytes.
#define LENGTH_OF_DATA_PAYLOAD               MAX_PAYLOAD_LENGTH     // DATA payload length
#define LENGTH_OF_ACK_PAYLOAD                3                      // ACK payload length
#define MAX_FIFO_SIZE                        64                     // Size of TX FIFO
#define TX_FIFO_ALMOST_EMPTY_THRESHOLD       48                     // Tx FIFO almost empty threshold level
#define RX_FIFO_ALMOST_FULL_THRESHOLD        32                     // Rx almost empty threshold


 #define  TX_AE_THRESHOLD		TX_FIFO_ALMOST_EMPTY_THRESHOLD	//发送将空中断阀值
 #define  RX_AF_THRESHOLD		RX_FIFO_ALMOST_FULL_THRESHOLD	//接收将满中断阀值
 #define  TRX_Len_Max           	MAX_PAYLOAD_LENGTH
 #define  TX_Len_MAX                    MAX_FIFO_SIZE
 
#define MOD_CW    0
#define MOD_OOK   1
#define MOD_2FSK  2
#define MOD_2GFSK 3
#define MOD_4FSK  4
#define MOD_4GFSK 5

#define MAX_CTS_RETRY					2500
#define NIRQ_TIMEOUT					10000
// CONFIGURATION PARAMETERS
#define RADIO_CONFIGURATION_DATA_RADIO_XO_FREQ                     {30000000L}
#define RADIO_CONFIGURATION_DATA_CHANNEL_NUMBER                    {0x00}
//#define RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH               {0x07}
#define RADIO_CONFIGURATION_DATA_RADIO_STATE_AFTER_POWER_UP        {0x03}
#define RADIO_CONFIGURATION_DATA_RADIO_DELAY_CNT_AFTER_RESET       {0xF000}
//#define RADIO_CONFIGURATION_DATA_CUSTOM_PAYLOAD					   {0xC5, 0xC5, 0xC5, 0xC5, 0xC5, 0xC5, 0xC5}
//Boot commands
#define CMD_POWER_UP					0x02 	
#define CMD_PATCH_IMAGE					0x04 	

//Common commands
#define CMD_NOP							0x00
#define CMD_PART_INFO					0x01
#define CMD_FUNC_INFO					0x10
#define CMD_SET_PROPERTY				0x11
#define CMD_GET_PROPERTY				0x12
#define CMD_GPIO_PIN_CFG				0x13
#define CMD_FIFO_INFO					0x15
#define CMD_PACKET_INFO                 0x16
#define CMD_IRCAL						0x17
#define CMD_GET_INT_STATUS				0x20
#define CMD_REQUEST_DEVICE_STATE		0x33
#define CMD_CHANGE_STATE				0x34
#define CMD_GET_ADC_READING				0x14
#define CMD_GET_PACKET_INFO				0x16
#define CMD_PROTOCOL_CFG				0x18
#define CMD_GET_PH_STATUS				0x21
#define CMD_GET_MODEM_STATUS			0x22
#define CMD_GET_CHIP_STATUS				0x23
#define CMD_RX_HOP						0x36 	 

//Tx commands
#define CMD_START_TX 					0x31
#define CMD_TX_FIFO_WRITE				0x66

//Rx commands
#define CMD_START_RX 					0x32
#define CMD_RX_FIFO_READ				0x77

//CTS command
#define CMD_CTS_READ					0x44

//Fast response register commands
#define CMD_FAST_RESPONSE_REG_A			0x50
#define CMD_FAST_RESPONSE_REG_B			0x51
#define CMD_FAST_RESPONSE_REG_C			0x53
#define CMD_FAST_RESPONSE_REG_D			0x57


//+++++++++++++++++++++
//Property constants
//+++++++++++++++++++++

#define PROP_INT_CTL_GROUP						0x01
#define PROP_INT_CTL_ENABLE						0x00
#define PROP_INT_CTL_PH_ENABLE					0x01
#define PROP_INT_CTL_MODEM_ENABLE				0x02
#define PROP_INT_CTL_CHIP_ENABLE				0x03

#define PROP_FRR_CTL_GROUP						0x02
#define PROP_FRR_CTL_A_MODE						0x00
#define PROP_FRR_CTL_B_MODE						0x01
#define PROP_FRR_CTL_C_MODE						0x02
#define PROP_FRR_CTL_D_MODE						0x03

#define PROP_SYNC_BITS_GROUP					0x11
#define PROP_SYNC_BITS_31_24					0x01
#define PROP_SYNC_BITS_23_16					0x02

#define PROP_PA_GROUP							0x22
#define PROP_PA_MODE							0x00
#define PROP_PA_PWR_LVL							0x01
#define PROP_PA_BIAS_CLKDUTY					0x02
#define PROP_PA_TC								0x03

#define PROP_EZCONFIG_GROUP						0x24
#define PROP_EZCONFIG_MODULATION				0x00
#define PROP_EZCONFIG_CONFIG_SELECT				0x01

#define PROP_FREQ_CONTROL_GROUP					0x40
#define PROP_FREQ_CONTROL_INTE					0x00
#define PROP_FREQ_CONTROL_FRAC_2				0x01
#define PROP_FREQ_CONTROL_FRAC_1				0x02
#define PROP_FREQ_CONTROL_FRAC_0				0x03
#define PROP_FREQ_CONTROL_CHANNEL_STEP_SIZE_1	0x04
#define PROP_FREQ_CONTROL_CHANNEL_STEP_SIZE_0	0x05

#define PROP_GLOBAL_GROUP						0x00
#define PROP_GLOBAL_XO_TUNE						0x00
#define PROP_GLOBAL_CLK_CFG						0x01
#define PROP_GLOBAL_LOW_BATT_THRESH				0x02
#define PROP_GLOBAL_CONFIG						0x03
#define PROP_GLOBAL_WUT_CONFIG					0x04
#define PROP_GLOBAL_WUT_M_15_8					0x05
#define PROP_GLOBAL_WUT_M_7_0					0x06
#define PROP_GLOBAL_WUT_R						0x07
#define PROP_GLOBAL_WUT_LDC						0x08

#define PROP_PREAMBLE_GROUP						0x10
#define PROP_PREAMBLE_TX_LENGTH					0x00
#define PROP_PREAMBLE_CONFIG_STD_1				0x01
#define PROP_PREAMBLE_CONFIG_NSTD				0x02
#define PROP_PREAMBLE_CONFIG_STD_2				0x03
#define PROP_PREAMBLE_CONFIG					0x04
#define PROP_PREAMBLE_PATTERN_31_24				0x05
#define PROP_PREAMBLE_PATTERN_23_16				0x06
#define PROP_PREAMBLE_PATTERN_15_8				0x07
#define PROP_PREAMBLE_PATTERN_7_0				0x08

#define PROP_SYNC_GROUP							0x11
#define PROP_SYNC_CONFIG						0x00
#define PROP_SYNC_BITS_15_8						0x03
#define PROP_SYNC_BITS_7_0						0x04

#define PROP_PKT_GROUP							0x12
#define PROP_PKT_CRC_CONFIG						0x00
#define	PROP_PKT_WHT_POLY_15_8					0x01	
#define	PROP_PKT_WHT_POLY_7_0					0x02
#define	PROP_PKT_WHT_SEED_15_8					0x03
#define	PROP_PKT_WHT_SEED_7_0 					0x04
#define	PROP_PKT_WHT_BIT_NUM 					0x05
#define PROP_PKT_CONFIG1						0x06
#define PROP_PKT_CHIP_MAP						0x07 	
#define PROP_PKT_LEN							0x08
#define PROP_PKT_LEN_FIELD_SOURCE				0x09
#define PROP_PKT_LEN_ADJUST						0x0A
#define PROP_PKT_TX_THRESHOLD					0x0B
#define PROP_PKT_RX_THRESHOLD					0x0C
#define PROP_PKT_FIELD_1_LENGTH_12_8			0x0D
#define PROP_PKT_FIELD_1_LENGTH_7_0				0x0E
#define PROP_PKT_FIELD_1_CONFIG					0x0F
#define PROP_PKT_FIELD_1_CRC_CONFIG				0x10
#define PROP_PKT_FIELD_2_LENGTH_12_8			0x11
#define PROP_PKT_FIELD_2_LENGTH_7_0				0x12
#define PROP_PKT_FIELD_2_CONFIG					0x13
#define PROP_PKT_FIELD_2_CRC_CONFIG				0x14
#define PROP_PKT_FIELD_3_LENGTH_12_8			0x15
#define PROP_PKT_FIELD_3_LENGTH_7_0				0x16
#define PROP_PKT_FIELD_3_CONFIG					0x17
#define PROP_PKT_FIELD_3_CRC_CONFIG				0x18
#define PROP_PKT_FIELD_4_LENGTH_12_8			0x19
#define PROP_PKT_FIELD_4_LENGTH_7_0				0x1A
#define PROP_PKT_FIELD_4_CONFIG					0x1B
#define PROP_PKT_FIELD_4_CRC_CONFIG				0x1C
#define PROP_PKT_FIELD_5_LENGTH_12_8			0x1D
#define PROP_PKT_FIELD_5_LENGTH_7_0				0x1E
#define PROP_PKT_FIELD_5_CONFIG					0x1F
#define PROP_PKT_FIELD_5_CRC_CONFIG				0x20
#define PROP_PKT_RX_FIELD_1_LENGTH_12_8			0x21
#define PROP_PKT_RX_FIELD_1_LENGTH_7_0			0x22
#define PROP_PKT_RX_FIELD_1_CONFIG				0x23
#define PROP_PKT_RX_FIELD_1_CRC_CONFIG			0x24
#define PROP_PKT_RX_FIELD_2_LENGTH_12_8			0x25
#define PROP_PKT_RX_FIELD_2_LENGTH_7_0			0x26
#define PROP_PKT_RX_FIELD_2_CONFIG				0x27
#define PROP_PKT_RX_FIELD_2_CRC_CONFIG			0x28
#define PROP_PKT_RX_FIELD_3_LENGTH_12_8			0x29
#define PROP_PKT_RX_FIELD_3_LENGTH_7_0			0x2A
#define PROP_PKT_RX_FIELD_3_CONFIG				0x2B
#define PROP_PKT_RX_FIELD_3_CRC_CONFIG			0x2C
#define PROP_PKT_RX_FIELD_4_LENGTH_12_8			0x2D
#define PROP_PKT_RX_FIELD_4_LENGTH_7_0			0x2E
#define PROP_PKT_RX_FIELD_4_CONFIG				0x2F
#define PROP_PKT_RX_FIELD_4_CRC_CONFIG			0x30
#define PROP_PKT_RX_FIELD_5_LENGTH_12_8			0x31
#define PROP_PKT_RX_FIELD_5_LENGTH_7_0			0x32
#define PROP_PKT_RX_FIELD_5_CONFIG				0x33
#define PROP_PKT_RX_FIELD_5_CRC_CONFIG			0x34

#define PROP_MODEM_GROUP						0x20
#define PROP_MODEM_MOD_TYPE						0x00
#define PROP_MODEM_MAP_CONTROL					0x01
#define PROP_MODEM_DSM_CTRL						0x02
#define PROP_MODEM_DATA_RATE_2					0x03
#define PROP_MODEM_DATA_RATE_1					0x04
#define PROP_MODEM_DATA_RATE_0					0x05
#define PROP_MODEM_FREQ_DEV_2					0x0A
#define PROP_MODEM_FREQ_DEV_1					0x0B
#define PROP_MODEM_FREQ_DEV_0					0x0C
#define PROP_MODEM_RESERVED_20_0D				0x0D
#define PROP_MODEM_RESERVED_20_0E				0x0E
#define PROP_MODEM_FSK4_MAP						0x3F
#define PROP_MODEM_ANT_DIV_CONTROL				0x49
#define PROP_MODEM_RSSI_THRESH					0x4A
#define PROP_MODEM_RSSI_JUMP_THRESH				0x4B
#define PROP_MODEM_RSSI_CONTROL					0x4C
#define PROP_MODEM_RSSI_CONTROL2				0x4D
#define PROP_MODEM_RSSI_COMP					0x4E
#define PROP_MODEM_RESERVED_20_50				0x50

#define PROP_MATCH_GROUP						0x30
#define PROP_MATCH_VALUE_1						0x00
#define PROP_MATCH_MASK_1						0x01
#define PROP_MATCH_CTRL_1						0x02
#define PROP_MATCH_VALUE_2						0x03
#define PROP_MATCH_MASK_2						0x04
#define PROP_MATCH_CTRL_2						0x05
#define PROP_MATCH_VALUE_3						0x06
#define PROP_MATCH_MASK_3						0x07
#define PROP_MATCH_CTRL_3						0x08
#define PROP_MATCH_VALUE_4						0x09
#define PROP_MATCH_MASK_4						0x0A
#define PROP_MATCH_CTRL_4						0x0B

#define PROP_FREQ_CONTROL_GROUP					0x40
#define PROP_FREQ_CONTROL_W_SIZE				0x06
#define PROP_FREQ_CONTROL_VCOCNT_RX_ADJ			0x07

#define PROP_RX_HOP_GROUP						0x50
#define PROP_RX_HOP_CONTROL						0x00
#define PROP_RX_HOP_TABLE_SIZE					0x01
#define PROP_RX_HOP_TABLE_ENTRY_0				0x02
#define PROP_RX_HOP_TABLE_ENTRY_1				0x03
#define PROP_RX_HOP_TABLE_ENTRY_2				0x04
#define PROP_RX_HOP_TABLE_ENTRY_3				0x05
#define PROP_RX_HOP_TABLE_ENTRY_4				0x06
#define PROP_RX_HOP_TABLE_ENTRY_5				0x07
#define PROP_RX_HOP_TABLE_ENTRY_6				0x08
#define PROP_RX_HOP_TABLE_ENTRY_7				0x09
#define PROP_RX_HOP_TABLE_ENTRY_8				0x0A
#define PROP_RX_HOP_TABLE_ENTRY_9				0x0B
#define PROP_RX_HOP_TABLE_ENTRY_10				0x0C
#define PROP_RX_HOP_TABLE_ENTRY_11				0x0D
#define PROP_RX_HOP_TABLE_ENTRY_12				0x0E
#define PROP_RX_HOP_TABLE_ENTRY_13				0x0F
#define PROP_RX_HOP_TABLE_ENTRY_14				0x10
#define PROP_RX_HOP_TABLE_ENTRY_15				0x11
#define PROP_RX_HOP_TABLE_ENTRY_16				0x12
#define PROP_RX_HOP_TABLE_ENTRY_17				0x13
#define PROP_RX_HOP_TABLE_ENTRY_18				0x14
#define PROP_RX_HOP_TABLE_ENTRY_19				0x15
#define PROP_RX_HOP_TABLE_ENTRY_20				0x16
#define PROP_RX_HOP_TABLE_ENTRY_21				0x17
#define PROP_RX_HOP_TABLE_ENTRY_22				0x18
#define PROP_RX_HOP_TABLE_ENTRY_23				0x19
#define PROP_RX_HOP_TABLE_ENTRY_24				0x1A
#define PROP_RX_HOP_TABLE_ENTRY_25				0x1B
#define PROP_RX_HOP_TABLE_ENTRY_26				0x1C
#define PROP_RX_HOP_TABLE_ENTRY_27				0x1D
#define PROP_RX_HOP_TABLE_ENTRY_28				0x1E
#define PROP_RX_HOP_TABLE_ENTRY_29				0x1F
#define PROP_RX_HOP_TABLE_ENTRY_30				0x20
#define PROP_RX_HOP_TABLE_ENTRY_31				0x21
#define PROP_RX_HOP_TABLE_ENTRY_32				0x22
#define PROP_RX_HOP_TABLE_ENTRY_33				0x23
#define PROP_RX_HOP_TABLE_ENTRY_34				0x24
#define PROP_RX_HOP_TABLE_ENTRY_35				0x25
#define PROP_RX_HOP_TABLE_ENTRY_36				0x26
#define PROP_RX_HOP_TABLE_ENTRY_37				0x27
#define PROP_RX_HOP_TABLE_ENTRY_38				0x28
#define PROP_RX_HOP_TABLE_ENTRY_39				0x29
#define PROP_RX_HOP_TABLE_ENTRY_40				0x2A
#define PROP_RX_HOP_TABLE_ENTRY_41				0x2B
#define PROP_RX_HOP_TABLE_ENTRY_42				0x2C
#define PROP_RX_HOP_TABLE_ENTRY_43				0x2D
#define PROP_RX_HOP_TABLE_ENTRY_44				0x2E
#define PROP_RX_HOP_TABLE_ENTRY_45				0x2F
#define PROP_RX_HOP_TABLE_ENTRY_46				0x30
#define PROP_RX_HOP_TABLE_ENTRY_47				0x31
#define PROP_RX_HOP_TABLE_ENTRY_48				0x32
#define PROP_RX_HOP_TABLE_ENTRY_49				0x33
#define PROP_RX_HOP_TABLE_ENTRY_50				0x34
#define PROP_RX_HOP_TABLE_ENTRY_51				0x35
#define PROP_RX_HOP_TABLE_ENTRY_52				0x36
#define PROP_RX_HOP_TABLE_ENTRY_53				0x37
#define PROP_RX_HOP_TABLE_ENTRY_54				0x38
#define PROP_RX_HOP_TABLE_ENTRY_55				0x39
#define PROP_RX_HOP_TABLE_ENTRY_56				0x3A
#define PROP_RX_HOP_TABLE_ENTRY_57				0x3B
#define PROP_RX_HOP_TABLE_ENTRY_58				0x3C
#define PROP_RX_HOP_TABLE_ENTRY_59				0x3D
#define PROP_RX_HOP_TABLE_ENTRY_60				0x3E
#define PROP_RX_HOP_TABLE_ENTRY_61				0x3F
#define PROP_RX_HOP_TABLE_ENTRY_62				0x40
#define PROP_RX_HOP_TABLE_ENTRY_63				0x41
//-----------------------------------------------------------------------------
#define  TXPKT_DONE			0x20	//发送数据包完成中断
#define  RXPKT_DONE			0x10
#define  TXFF_AE				0x02
#define  RXFF_AF				0x01


#define SI_STATE_NOCHANGE       0
#define SI_STATE_SLEEP          1
#define SI_STATE_SPIACTIVE      2
#define SI_STATE_READY          3
#define SI_STATE_TXTUNE         5
#define SI_STATE_RXTUNE         6
#define SI_STATE_TX             7
#define SI_STATE_RX             8


#define   NSEL_CLR   		 GPIO_ResetBits(GPIOB,GPIO_PIN_12);
#define   NSEL_SET   		 GPIO_SetBits(GPIOB,GPIO_PIN_12);


extern const u8 Read_INT_Status_cmd[4];

u8 Api_GetResponse(u8 Resp_Len, u8 *Resp_Data);
u16	cal_crc_ITU(u8 *ptr,u16 len);
void SI4438_Init(void);
u8 SI_Read_INT_Info(void);
void SI_Freq_Set(u8 arry,u8 chen);
u8 SI_Read_RSSI(void);
u8 Read_PayLoad(u8 *buf);
u8   SI_Read_Length(void);
u8 SI_Read_Fifo(u8 Tx_Len,u8 *Tx_Data);
u8  SI4438_FIFO_Send(u8 leng,u8 *buf);
u8 SI_Send_Packet(u8 bLength, u8 *abPayload);
void SI_Enter_Rx(void);
#endif /* RADIO_CONFIG_H_ */
