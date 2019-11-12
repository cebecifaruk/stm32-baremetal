/**
  ******************************************************************************
  * @file    usb_endp.c
  * @author  MCD Application Team
  * @version V3.4.0
  * @date    29-June-2012
  * @brief   Endpoint routines
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
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


/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_istr.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t Receive_Buffer[65];
uint8_t Send_Buffer[65];
extern __IO uint8_t PrevXferComplete;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : EP1_OUT_Callback.
* Description    : EP1 OUT Callback Routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
extern uint8_t IOState;
extern uint16_t getIO(void);
extern uint16_t setIO(uint16_t data);
void EP1_OUT_Callback(void)
{
  /* Read received data bytes) */  
  USB_SIL_Read(EP1_OUT, Receive_Buffer);
  switch (Receive_Buffer[0])
	{
		case 0x80: // Set Backlight
			break;
		case 0x81: // Set Touch Calibration 
			NVIC_SystemReset();
			break;
		case 0x38: // Set IO Status
			memset(Send_Buffer,0xFF,64);
	
			/* prepare buffer to send */
			Send_Buffer[0] = 0x37;			
  
			/* Reset the control token to inform upper layer that a transfer is ongoing */
			PrevXferComplete = 0;
  
			/* Copy mouse position info in ENDP1 Tx Packet Memory Area*/
			USB_SIL_Write(EP1_IN, Send_Buffer, 64);
			SetEPTxValid(ENDP1);		
			break;
		
		case 0x37: // Request IO Status

			memset(Send_Buffer,0xFF,64);
	
			/* prepare buffer to send */
			Send_Buffer[0] = 0x37;
  
			/* Reset the control token to inform upper layer that a transfer is ongoing */
			PrevXferComplete = 0;
  
			/* Copy mouse position info in ENDP1 Tx Packet Memory Area*/
			USB_SIL_Write(EP1_IN, Send_Buffer, 64);
			SetEPTxValid(ENDP1);  		
			break;
		default:
			break;
	}
	
  
#ifndef STM32F10X_CL   
  SetEPRxStatus(ENDP1, EP_RX_VALID);
#endif /* STM32F10X_CL */
 
}

/*******************************************************************************
* Function Name  : EP1_IN_Callback.
* Description    : EP1 IN Callback Routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP1_IN_Callback(void)
{
	  /* Set the transfer complete token to inform upper layer that the current 
  transfer has been complete */
	PrevXferComplete = 1;
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

