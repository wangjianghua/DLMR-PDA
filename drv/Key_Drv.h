#ifndef __KEY_DRV__H__
#define  __KEY_DRV__H__


#define KEYBOARD_ROW_PORT           GPIOC
#define KEYBOARD_COL_PORT           GPIOF

#define KEYBOARD_ROW_0              GPIO_PIN_0
#define KEYBOARD_ROW_1              GPIO_PIN_1
#define KEYBOARD_ROW_2              GPIO_PIN_2
#define KEYBOARD_ROW_3              GPIO_PIN_3
#define KEYBOARD_ROW_4              GPIO_PIN_4
#define KEYBOARD_ROW_5              GPIO_PIN_5

#define KEYBOARD_COL_0              GPIO_PIN_9
#define KEYBOARD_COL_1              GPIO_PIN_8
#define KEYBOARD_COL_2              GPIO_PIN_7
#define KEYBOARD_COL_3              GPIO_PIN_6

#define KEYBOARD_ROW_BASE           KEYBOARD_ROW_0
#define KEYBOARD_COL_BASE           KEYBOARD_COL_0

#define KEYBOARD_ROW               (KEYBOARD_ROW_0 | KEYBOARD_ROW_1 | KEYBOARD_ROW_2 | KEYBOARD_ROW_3 | KEYBOARD_ROW_4 | KEYBOARD_ROW_5)
#define KEYBOARD_COL               (KEYBOARD_COL_0 | KEYBOARD_COL_1 | KEYBOARD_COL_2 | KEYBOARD_COL_3)

#define KEYBOARD_ROW_NUM            6

#define KEYBOARD_COL0_CODE          0
#define KEYBOARD_COL1_CODE         (KEYBOARD_ROW_NUM * 1)//6
#define KEYBOARD_COL2_CODE         (KEYBOARD_ROW_NUM * 2)//12
#define KEYBOARD_COL3_CODE         (KEYBOARD_ROW_NUM * 3)//18

#define LED_KEY_ON()                LED_On(LED_RED)
#define LED_KEY_OFF()               LED_Off(LED_RED)

#define LED_KEY_DISP_TIME           2

#if 0
/*********************************************************
*        COL0---pin9 键值   0-5
*********************************************************/

#define KEY_MSG_OK                 (KEYBOARD_COL0_CODE + 0)
#define KEY_MSG_NEXT               (KEYBOARD_COL0_CODE + 1)
#define KEY_MSG_RIGHT              (KEYBOARD_COL0_CODE + 2)
#define KEY_MSG_UP                 (KEYBOARD_COL0_CODE + 3)
#define KEY_MSG_LEFT               (KEYBOARD_COL0_CODE + 4)
#define KEY_MSG_PAUSE              (KEYBOARD_COL0_CODE + 5)


/*********************************************************
*        COL1---pin8 键值   6-11
*********************************************************/
#define KEY_MSG_F2                 (KEYBOARD_COL1_CODE + 0)
#define KEY_MSG_SHARP              (KEYBOARD_COL1_CODE + 1)  /* #  */
#define KEY_MSG_9                  (KEYBOARD_COL1_CODE + 2)
#define KEY_MSG_6                  (KEYBOARD_COL1_CODE + 3)
#define KEY_MSG_3                  (KEYBOARD_COL1_CODE + 4)
#define KEY_MSG_C                  (KEYBOARD_COL1_CODE + 5)


/*********************************************************
*        COL2---pin7 键值   12-17
*********************************************************/

#define KEY_MSG_F1                 (KEYBOARD_COL2_CODE + 0)
#define KEY_MSG_0                  (KEYBOARD_COL2_CODE + 1)
#define KEY_MSG_8                  (KEYBOARD_COL2_CODE + 2)
#define KEY_MSG_5                  (KEYBOARD_COL2_CODE + 3)
#define KEY_MSG_2                  (KEYBOARD_COL2_CODE + 4)
#define KEY_MSG_DOWN               (KEYBOARD_COL2_CODE + 5)


/*********************************************************
*        COL3---pin6 键值   18-23
*********************************************************/

#define KEY_MSG_STAR               (KEYBOARD_COL3_CODE + 0)  /* * */
#define KEY_MSG_7                  (KEYBOARD_COL3_CODE + 1)
#define KEY_MSG_4                  (KEYBOARD_COL3_CODE + 2)
#define KEY_MSG_1                  (KEYBOARD_COL3_CODE + 3)
#define KEY_MSG_TAB                (KEYBOARD_COL3_CODE + 4)
#define KEY_MSG_ESC                (KEYBOARD_COL3_CODE + 5)

#endif 

typedef struct __key_con_block__
{
	uint32_t key_col;
	uint32_t key_row;
	OS_EVENT *key_sem;
}KEY_CON, *P_KEY_CON;

extern KEY_CON g_key_control;
//extern int g_key_press_count;//added on 2014.12.31


void KEY_Init(void);
void  App_TaskKey(void *p_arg);
void EXTI9_5_IRQHandler(void);


#endif 
