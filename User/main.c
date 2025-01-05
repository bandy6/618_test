/**
  ******************************************************************************
  * @file    GPIO/IOToggle/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    05-1-2025
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
#include "uart.h"

TaskHandle_t StartTask_Handler;			// ������
TaskHandle_t UartTask_Handler;			// ������


/* ������ */
void start_task(void *pvParameters)
{
  (void)pvParameters;
	int i = 1;
	
	while (1)
	{
    LED1_ON;			  // ��
		vTaskDelay(1000);
		LED1_OFF;		   // ��
    vTaskDelay(1000);
	}
}

void uart_task(void *pvParameters)
{
  (void)pvParameters;

	while (1)
	{
    	Usart_SendString( USART1,"���ڷ���test\n");
      vTaskDelay(2000);
	}
}

/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{	
	/* Ƕ�������жϿ�������ѡ�� */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	/* LED �˿ڳ�ʼ�� */
	LED_GPIO_Config();	 
	
  /* ���ڳ�ʼ�� */
  /*��ʼ��USART ����ģʽΪ 115200 8-N-1���жϽ���*/
  uart_init();

	/* �������� */
    xTaskCreate((TaskFunction_t )start_task,            // ������
                (const char*    )"start_task",          // ��������
                (uint16_t       )128,        			      // �����ջ��С
                (void*          )NULL,                  // ���ݸ��������Ĳ���
                (UBaseType_t    )1,       				      // �������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   // ������

	/* ������������ */
    xTaskCreate((TaskFunction_t )uart_task,            // ������
                (const char*    )"uart_task",          // ��������
                (uint16_t       )128,        			      // �����ջ��С
                (void*          )NULL,                  // ���ݸ��������Ĳ���
                (UBaseType_t    )1,       				      // �������ȼ�
                (TaskHandle_t*  )&UartTask_Handler);   // ������

				
    vTaskStartScheduler();          					          // �����������
}
