/**
  ******************************************************************************
  * @file    GPIO/IOToggle/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "bsp_led.h"
#include "FreeRTOS.h"
#include "task.h"


TaskHandle_t StartTask_Handler;			// ������

/* ������ */
void start_task(void *pvParameters)
{
	int i = 1;
	
	while (1)
	{
    LED1_ON;			  // ��
		vTaskDelay(1000);
		LED1_OFF;		   // ��
    vTaskDelay(1000);
	}
}

/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{	
	/* LED �˿ڳ�ʼ�� */
	LED_GPIO_Config();	 
	
	/* �������� */
    xTaskCreate((TaskFunction_t )start_task,            // ������
                (const char*    )"start_task",          // ��������
                (uint16_t       )128,        			      // �����ջ��С
                (void*          )NULL,                  // ���ݸ��������Ĳ���
                (UBaseType_t    )1,       				      // �������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   // ������
				
    vTaskStartScheduler();          					          // �����������
}

void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/
