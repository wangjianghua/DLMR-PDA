/*********************************************************************
*          Portions COPYRIGHT 2013 STMicroelectronics                *
*          Portions SEGGER Microcontroller GmbH & Co. KG             *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2013  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.22 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The  software has  been licensed  to STMicroelectronics International
N.V. a Dutch company with a Swiss branch and its headquarters in Plan-
les-Ouates, Geneva, 39 Chemin du Champ des Filles, Switzerland for the
purposes of creating libraries for ARM Cortex-M-based 32-bit microcon_
troller products commercialized by Licensee only, sublicensed and dis_
tributed under the terms and conditions of the End User License Agree_
ment supplied by STMicroelectronics International N.V.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : LCDConf_FlexColor_Template.c
Purpose     : Display controller configuration (single layer)
---------------------------END-OF-HEADER------------------------------
*/

/**
  ******************************************************************************
  * @attention
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
#include "includes.h"
#include "GUI.h"
#include "GUIDRV_FlexColor.h"
#include "LCDConf.h"

/*********************************************************************
*
*       Layer configuration (to be modified)
*
**********************************************************************
*/

//
// Physical display size
//
#define XSIZE_PHYS  240 // To be adapted to x-screen size
#define YSIZE_PHYS  320 // To be adapted to y-screen size

/*********************************************************************
*
*       Configuration checking
*
**********************************************************************
*/
#ifndef   VXSIZE_PHYS
  #define VXSIZE_PHYS XSIZE_PHYS
#endif
#ifndef   VYSIZE_PHYS
  #define VYSIZE_PHYS YSIZE_PHYS
#endif
#ifndef   XSIZE_PHYS
  #error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
  #error Physical Y size of display is not defined!
#endif
#ifndef   GUICC_565
  #error Color conversion not defined!
#endif
#ifndef   GUIDRV_FLEXCOLOR
  #error No display driver defined!
#endif

//#define LCD_TYPE_9328              0x9328

//#define VER_ILI9341V

static uint32_t Lcd_Type;

void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue)
{
  /* Write 16-bit Index, then Write Reg */
  LCD_REF->LCD_REG = LCD_Reg;
  /* Write 16-bit Reg */
  LCD_REF->LCD_RAM = LCD_RegValue;
}



uint16_t LCD_ReadReg(uint8_t LCD_Reg)
{
  /* Write 16-bit Index (then Read Reg) */
  LCD_REF->LCD_REG = LCD_Reg;
  /* Read 16-bit Reg */
  return (LCD_REF->LCD_RAM);
}



void LCD_WriteCmd(uint8_t LCD_Reg)
{
  /* Write 16-bit Index, then Write Reg */
  LCD_REF->LCD_REG = LCD_Reg;
}

void LCD_WriteData(uint8_t LCD_Data)
{
    LCD_REF->LCD_RAM=LCD_Data;

}


uint32_t LCD_ReadData(void)
{
  
  return (LCD_REF->LCD_RAM);
}


//2?òaó??ˉμ?
//#pragma optimize=none

static void lcd_delay(U32 nCount)
{
  U32 index = 0; 
  for(index = (100000 * nCount); index != 0; index--)
  {
  }
}





/*********************************************************************
*
*       Local functions
*
**********************************************************************
*/
/********************************************************************
*
*       LcdWriteReg
*
* Function description:
*   Sets display register
*/
static void LcdWriteReg(U16 Data) {
  // ... TBD by user
   LCD_REF->LCD_REG =Data;
}

/********************************************************************
*
*       LcdWriteData
*
* Function description:
*   Writes a value to a display register
*/
static void LcdWriteData(U16 Data) {
  // ... TBD by user
  	LCD_REF->LCD_RAM = Data;
}

/********************************************************************
*
*       LcdWriteDataMultiple
*
* Function description:
*   Writes multiple values to a display register.
*/
static void LcdWriteDataMultiple(U16 * pData, int NumItems) {
  while (NumItems--) {
    // ... TBD by user
    LCD_REF->LCD_RAM=*pData++;
  }
}

/********************************************************************
*
*       LcdReadDataMultiple
*
* Function description:
*   Reads multiple values from a display register.
*/
static void LcdReadDataMultiple(U16 * pData, int NumItems) {
  while (NumItems--) {
    // ... TBD by user
    *pData++=LCD_REF->LCD_RAM;
  }
}

/*********************************************************************
*
*       Public functions
*
**********************************************************************
*/
/*********************************************************************
*
*       LCD_X_Config
*
* Function description:
*   Called during the initialization process in order to set up the
*   display driver configuration.
*
*/
void LCD_X_Config(void) {
  GUI_DEVICE * pDevice;
  CONFIG_FLEXCOLOR Config = {0};
  GUI_PORT_API PortAPI = {0};
  //
  // Set display driver and color conversion
  //
  pDevice = GUI_DEVICE_CreateAndLink(GUIDRV_FLEXCOLOR, GUICC_565, 0, 0);
  //
  // Display driver configuration, required for Lin-driver
  //
  LCD_SetSizeEx (0, XSIZE_PHYS , YSIZE_PHYS);
  LCD_SetVSizeEx(0, VXSIZE_PHYS, VYSIZE_PHYS);
  //
  // Orientation
  //
  //Config.Orientation =  GUI_MIRROR_X;   //GUI_SWAP_XY
  GUIDRV_FlexColor_Config(pDevice, &Config);
  //
  // Set controller and operation mode
  //
  PortAPI.pfWrite16_A0  = LcdWriteReg;
  PortAPI.pfWrite16_A1  = LcdWriteData;
  PortAPI.pfWriteM16_A1 = LcdWriteDataMultiple;
  PortAPI.pfReadM16_A1  = LcdReadDataMultiple;
  //GUIDRV_FlexColor_SetFunc(pDevice, &PortAPI, GUIDRV_FLEXCOLOR_F66708, GUIDRV_FLEXCOLOR_M16C0B16);
  
  GUIDRV_FlexColor_SetFunc(pDevice, &PortAPI, GUIDRV_FLEXCOLOR_F66709, GUIDRV_FLEXCOLOR_M16C0B16);
}

/*********************************************************************
*
*       LCD_X_DisplayDriver
*
* Function description:
*   This function is called by the display driver for several purposes.
*   To support the according task the routine needs to be adapted to
*   the display controller. Please note that the commands marked with
*   'optional' are not cogently required and should only be adapted if
*   the display controller supports these features.
*
* Parameter:
*   LayerIndex - Index of layer to be configured
*   Cmd        - Please refer to the details in the switch statement below
*   pData      - Pointer to a LCD_X_DATA structure
*
* Return Value:
*   < -1 - Error
*     -1 - Command not handled
*      0 - Ok
*/


int LCD_GetID(void)
{
    Lcd_Type = LCD_ReadReg(0x00);
    if((Lcd_Type < 0xFF) || (0xFFFF == Lcd_Type) || (0x9300 == Lcd_Type))
    {       
       LCD_WriteCmd(0xD3); //3￠ê??áè?ID                  
       LCD_ReadData(); //Dummy
       LCD_ReadData(); //0x00
       Lcd_Type = LCD_ReadData(); //0x93                              
       Lcd_Type <<= 8;
       Lcd_Type |= LCD_ReadData(); //0x41 
       if(LCD_TYPE == Lcd_Type)
            return 1;
       else
            return 0;
    }
}


#define VER_ILI9341

int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData) {
  int r;
  (void) LayerIndex;
  (void) pData;


  
  switch (Cmd) {
  case LCD_X_INITCONTROLLER: {

	lcd_delay(50); //Delay 50ms

    LCD_PWR_ON();
    
    LCD_RST_LOW(); 
    GUI_Delay(150); //Delay 
    LCD_RST_HIGH();
	//lcd_delay(150); //Delay 
	GUI_Delay(150); 
    
    LCD_WriteReg(0x00, 0x0001);
	//lcd_delay(150); //Delay 
	GUI_Delay(150); 

    Lcd_Type = LCD_ReadReg(0x00);
       
       
       
   /* ?áIDòì3￡ */
   if((Lcd_Type < 0xFF) || (0xFFFF == Lcd_Type) || (0x9300 == Lcd_Type))
   {       
       LCD_WriteCmd(0xD3); //3￠ê??áè?ID                  
       LCD_ReadData(); //Dummy
       LCD_ReadData(); //0x00
       Lcd_Type = LCD_ReadData(); //0x93                              
       Lcd_Type <<= 8;
       Lcd_Type |= LCD_ReadData(); //0x41              
   }

   if(LCD_TYPE == Lcd_Type)
   {    
#ifdef VER_ILI9341
        LCD_WriteCmd(0xCF);  //power control B
        LCD_WriteData(0x00);
        LCD_WriteData(0x81);//C1
        LCD_WriteData(0x30);
        
        LCD_WriteCmd(0xED);  //Power on sequence control 
        LCD_WriteData(0x64);
        LCD_WriteData(0x03);
        LCD_WriteData(0x12);
        LCD_WriteData(0x81);
        
        LCD_WriteCmd(0xE8);  //Driver timing control A
        LCD_WriteData(0x85);
        LCD_WriteData(0x10);//00
        LCD_WriteData(0x78);
        
        LCD_WriteCmd(0xCB); //Power control A 
        LCD_WriteData(0x39);
        LCD_WriteData(0x2C);
        LCD_WriteData(0x00);
        LCD_WriteData(0x34);
        LCD_WriteData(0x02);
        
        LCD_WriteCmd(0xF7); //Pump ratio control  
        LCD_WriteData(0x20);
        
        LCD_WriteCmd(0xEA); //Driver timing control B
        LCD_WriteData(0x00);
        LCD_WriteData(0x00);
        
        LCD_WriteCmd(0xB1); //Frame Rate Control (In Normal Mode/Full Colors)
        LCD_WriteData(0x00);
        LCD_WriteData(0x1B);//1A
        
        LCD_WriteCmd(0xB6); //Display Function Control
        LCD_WriteData(0x0A);
        LCD_WriteData(0xA2);
        
        LCD_WriteCmd(0xC0);  //Power Control 1 
        LCD_WriteData(0x25);//1B
        
        LCD_WriteCmd(0xC1);  //Power Control 2
        LCD_WriteData(0x11);//12
        
        LCD_WriteCmd(0xC5);  //VCOM Control 1
        LCD_WriteData(0x45);//44 40
        LCD_WriteData(0x45);
        
        LCD_WriteCmd(0xC7);  //VCOM Control 2
        LCD_WriteData(0xA2); //B0
        
        LCD_WriteCmd(0xF2);   //Enable 3 gamma
        LCD_WriteData(0x00);
        
        LCD_WriteCmd(0x26);   // Gamma Set 
        LCD_WriteData(0x01);
        
        LCD_WriteCmd(0xE0);   //
        LCD_WriteData(0x0F);
        LCD_WriteData(0x26);
        LCD_WriteData(0x24);
        LCD_WriteData(0x0B);
        LCD_WriteData(0x0E);
        LCD_WriteData(0x09);
        LCD_WriteData(0x54);
        LCD_WriteData(0xA8);
        LCD_WriteData(0x46);
        LCD_WriteData(0x0C);
        LCD_WriteData(0x17);
        LCD_WriteData(0x09);
        LCD_WriteData(0x0F);
        LCD_WriteData(0x07);
        LCD_WriteData(0x00);
        
        LCD_WriteCmd(0xE1);
        LCD_WriteData(0x00);
        LCD_WriteData(0x19);
        LCD_WriteData(0x1B);
        LCD_WriteData(0x04);
        LCD_WriteData(0x10);
        LCD_WriteData(0x07);
        LCD_WriteData(0x2A);
        LCD_WriteData(0x47);
        LCD_WriteData(0x39);
        LCD_WriteData(0x03);
        LCD_WriteData(0x06);
        LCD_WriteData(0x06);
        LCD_WriteData(0x30);
        LCD_WriteData(0x38);
        LCD_WriteData(0x0F);
        
        LCD_WriteCmd(0x36);   //
        LCD_WriteData(0x08);
        
        LCD_WriteCmd(0x2A);
        LCD_WriteData(0x00);
        LCD_WriteData(0x00);
        LCD_WriteData(0x00);
        LCD_WriteData(0xEF);
        
        LCD_WriteCmd(0x2B);
        LCD_WriteData(0x00);
        LCD_WriteData(0x00);
        LCD_WriteData(0x01);
        LCD_WriteData(0x3F); 
        
        LCD_WriteCmd(0x3A);
        LCD_WriteData(0x55);
        LCD_WriteCmd(0x11);
        //lcd_delay(120);
        GUI_Delay(150); 
        LCD_WriteCmd(0x29);
        LCD_WriteCmd(0x2C);
#else
        LCD_WriteCmd(0xCF);  //P198  POWER CONTROL
        LCD_WriteData(0x00);    
        LCD_WriteData(0x81);
        LCD_WriteData(0X30);
        
        LCD_WriteCmd(0xED);
        LCD_WriteData(0x64);
        LCD_WriteData(0x03);
        LCD_WriteData(0X12);
        LCD_WriteData(0X81);
        
        
        LCD_WriteCmd(0xe8); 
        LCD_WriteData(0x85);
        LCD_WriteData(0x00);
        LCD_WriteData(0x78);
        
        //LCD_ILI9341_CMD(0xE9);        //P200
        //LCD_ILI9341_Parameter(0x85);
        //LCD_ILI9341_Parameter(0x10);
        //LCD_ILI9341_Parameter(0x78);
        
        LCD_WriteCmd(0xCB);
        LCD_WriteData(0x39);
        LCD_WriteData(0x2C);
        LCD_WriteData(0x00);
        LCD_WriteData(0x34);
        LCD_WriteData(0x02);
        
        
        
        LCD_WriteCmd(0x3A);  //PIXEL FORMAT SET 18BIT P136
        
        LCD_WriteData(0x55);//fpga
        
        LCD_WriteCmd(0xEA);
        LCD_WriteData(0x00);
        LCD_WriteData(0x00);
        
        //LCD_ILI9341_CMD(0xB0);    // frame rate control
        
        //LCD_ILI9341_Parameter(0xC6);
        
        LCD_WriteCmd(0xB1);  // frame rate control
        LCD_WriteData(0x00);
        LCD_WriteData(0x1A);  //1b:70hz  1f:61hz
        
        //LCD_ILI9341_CMD(0xB4);  //display inversion control
        //LCD_ILI9341_Parameter(0x00);
        
        LCD_WriteCmd(0xB6); // Display Function Control  P166
        LCD_WriteData(0x0a);//NORMALLY BLACK    isc:scan cycle
        LCD_WriteData(0xA2);
        
        
        LCD_WriteCmd(0xC0); //Power control  P180
        LCD_WriteData(0x1B); //VRH[5:0]
        
        LCD_WriteCmd(0xC1); //Power control  P181
        LCD_WriteData(0x11); //SAP[2:0];BT[3:0]  AVDD VGH VGL
        LCD_WriteCmd(0xC2);
        LCD_WriteData(0x11);
        //LCD_ILI9341_CMD(0xCF); //VCM control   P182
        //LCD_ILI9341_Parameter(0x00);
        //LCD_ILI9341_Parameter(0x89);
        //LCD_ILI9341_Parameter(0x30);
        
        LCD_WriteCmd(0xC5); //VCM control   P182
        LCD_WriteData(0x45);
        LCD_WriteData(0x45);
        
        LCD_WriteCmd(0xC7); //VCM control2  P184
        LCD_WriteData(0XA2);   //4:28  1:31
        
        LCD_WriteCmd(0x36); // Memory Access Control  P129
        
        LCD_WriteData(0x08);   //08:RGB
        
        LCD_WriteCmd(0xF2); // 3Gamma Function Disable  P206
        LCD_WriteData(0x00);
        LCD_WriteCmd(0xF7);
        LCD_WriteData(0x20);
        
        //LCD_ILI9341_CMD(0XF6);        //interface control  P194
        //LCD_ILI9341_Parameter(0x01);
        //LCD_ILI9341_Parameter(0x30);  //RGB565自动补位机?
        //LCD_ILI9341_Parameter(0x06);  //00:internal clock opration 01:RGB 10:VSYNC  RM:--> 0
        
        LCD_WriteCmd(0x26); //Gamma curve selected
        LCD_WriteData(0x01);

        LCD_WriteCmd(0xE0); //Set Gamma
        LCD_WriteData(0x0F);  //63
        LCD_WriteData(0x26);//62
        LCD_WriteData(0x28);//61
        LCD_WriteData(0x0B);//59
        LCD_WriteData(0x0D);//57
        LCD_WriteData(0x05);//50
        LCD_WriteData(0x4E);//43
        LCD_WriteData(0X85);//27/36
        LCD_WriteData(0x40);//20
        LCD_WriteData(0x0A);//13
        LCD_WriteData(0x17);//6
        LCD_WriteData(0x09);//4
        LCD_WriteData(0x10);//2
        LCD_WriteData(0x06);//1
        LCD_WriteData(0x00);   //0
        
        LCD_WriteCmd(0XE1); //Set Gamma
        LCD_WriteData(0x00);   //63
        LCD_WriteData(0x19);//62
        LCD_WriteData(0x13);//61
        LCD_WriteData(0x04);//59
        LCD_WriteData(0x0F);//57
        LCD_WriteData(0x05);//50
        LCD_WriteData(0x22);//43
        LCD_WriteData(0x22);//27//36
        LCD_WriteData(0x35);//20
        LCD_WriteData(0x01);//13
        LCD_WriteData(0x08);//6
        LCD_WriteData(0x06);//4
        LCD_WriteData(0x2A);//2
        LCD_WriteData(0x2E);//1
        LCD_WriteData(0x0F);   //0
        
        LCD_WriteCmd(0x11); //Exit Sleep
        GUI_Delay(120);
        LCD_WriteCmd(0x29); //Display on
#endif
    }   
    else
    {
        //DEBUG_PRINT(("wrong lcd type!\n"));
    }
 
    //
    // Called during the initialization process in order to set up the
    // display controller and put it into operation. If the display
    // controller is not initialized by any external routine this needs
    // to be adapted by the customer...
    //
    // ...
    return 0;
  }
  default:
    r = -1;
  }
  return r;
}

/*************************** End of file ****************************/

