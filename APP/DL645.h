#ifndef __DL645_H__
#define __DL645_H__

#ifdef __cplusplus
 extern "C" {
#endif


#define DL645_1997	                            0
#define DL645_2007	                            1
#define	DL645_NONE	                            2

#define CCTT_CONTROL_CODE_MASK               0x1F

#define FRM_CTRW_97_READ_SLVS_DATA           0x01 //主机发出命令读数据
#define FRM_CTRW_97_WRITE_SLVS_DATA          0x04 //主机发出命令写数据

#define FRM_CTRW_07_READ_SLVS_DATA           0x11 //主机发出命令读数据
#define FRM_CTRW_07_EXT_READ_SLVS_DATA       0x12 //华兄
#define FRM_CTRW_07_WRITE_SLVS_DATA          0x14 //主机发出命令写数据

#define FRM_CTRW_07_BROAD_READ_ADDR          0x13 //广播读地址

#define DL645_97_DATA_ITEM_LEN                  2
#define DL645_07_DATA_ITEM_LEN                  4

#define DL645_ADDR_LEN				            6

#define DL645_REPLY_STAT_MASK                0x40

#define DL645_FIX_LEN                          12

#define DL645_MAX_DATA_LEN                    230 //华兄

#define DL645_FRAME_OK                          0
#define DL645_FRAME_ERROR                     128

typedef struct
{
	u8 Status; //帧状态: 0非法、1合法、2地址符合、3地址不符合
	u8 protocol; //协议
	u8 C; //645控制码
	u8 ID_Leng; //表示码长度
} DL645_Frame_Stat_C;

#pragma pack(1)
typedef struct
{
    u8 Start;
    u8 Addr[6];
    u8 Start1;
    u8 C;
    u8 L;
    u8 Data[DL645_MAX_DATA_LEN];
} DL645_Frame_C;
#pragma pack()

void Create_DL645_Frame(u8 *Addr, u8 C, u8 len, DL645_Frame_C *DL645_Frame);
u8 Create_DL645_LeveFrame(u8 *leve_Addr, u8 leve, u8 *Addr, u8 C, u8 len, u8 *data, u8 *Send_buf);
u32 Analysis_DL645_Frame(u8 *Addr, u8 *buff, DL645_Frame_Stat_C *pFreame_st);


#ifdef __cplusplus
}
#endif

#endif
