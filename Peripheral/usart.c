/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#include "fifo_buffer.h"

t_fifo_buffer rx_fifo_uart2;
uint8_t rx_buff_uart2[128];

/* USER CODE END 0 */

UART_HandleTypeDef huart2;

/* USART2 init function */

void MX_USART2_UART_Init(void)
{

	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart2) != HAL_OK)
	{
	Error_Handler();
	}
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);//ʹ��idle�ж�

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
int32_t Uart_Buffer_Register(void)
{
	fifoBuf_init(&rx_fifo_uart2, rx_buff_uart2, sizeof(rx_buff_uart2));
    return 0;
}
/*******************************************************************************
* ������ : int32_t COM_SendBufferNonBlocking(uint32_t com_id, const uint8_t *buffer, uint16_t len)
* ��  �� : ���������ڷ���
* ��  �� : com_idʹ���ĸ�id���ͣ�*buffer���͵���������ָ�룬len�������ݵĳ���
* ��  �� : ��
* ����ֵ : �Ž�ȥ���ֽڸ�����-2Ϊ�������ݹ���ʧ��
* ˵  �� : ��
*******************************************************************************/
int32_t UART_SendBufferNonBlocking(const uint8_t *buffer, uint16_t len)
{
    int32_t ret = -1;
	HAL_UART_Transmit(&huart2,(uint8_t *)buffer,len,1000);//����4��������
    return ret;
}
/*******************************************************************************
* ������ : static uint16_t Uart_Bufffer_RxIn(uint32_t context, uint8_t *buf, uint16_t buf_len)
* ��  �� : �ڽ����ж��е��ã������ջ������ڷ�����
* ��  �� : uint32_t context, uint8_t *buf, uint16_t buf_len,
* ��  �� : ��
* ����ֵ : ��
* ˵  �� : ��
*******************************************************************************/
static uint16_t Uart_Bufffer_RxIn(uint8_t *buf, uint16_t buf_len)
{
    uint16_t bytes_into_fifo;
    if (buf_len == 1) 
	{
        bytes_into_fifo = fifoBuf_putByte(&rx_fifo_uart2, buf[0]);
    } 
	else 
	{
        bytes_into_fifo = fifoBuf_putData(&rx_fifo_uart2, buf, buf_len);
    }
    return bytes_into_fifo;
}
/*******************************************************************************
* ������ : uint16_t COM_ReceiveBuffer(uint32_t com_id, uint8_t *buf, uint16_t buf_len)
* ��  �� : ��ָ��com_id�Ļ������ж���ָ�����ȵ��ֽ�
* ��  �� : com_id��ʼ��ʱע����豸ID��buf_len�ӻ�������Ҫ��ȡ���ֽڸ���
* ��  �� : *buf ����ָ��
* ����ֵ : �������ڵ����ݵ�����
* ˵  �� : ��
*******************************************************************************/
uint16_t UART_ReceiveBuffer(uint8_t *buf, uint16_t buf_len)
{
	uint16_t bytes_from_fifo;
	bytes_from_fifo = fifoBuf_getData(&rx_fifo_uart2, buf, buf_len);
	return bytes_from_fifo;
}

/*******************************************************************************
* ������ : uint16_t COM_GetRxBufferUsed(uint32_t com_id)
* ��  �� : ��ȡ���ջ������ж����Ѿ�ʹ��
* ��  �� : ��
* ��  �� : ���ջ������ж����Ѿ�ʹ��
* ����ֵ : ��
* ˵  �� : ��
*******************************************************************************/
uint16_t UART_GetRxBufferUsed(void)
{
	return fifoBuf_getUsed(&rx_fifo_uart2);
}

void USART2_IRQHandler(void)
{
	if(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_RXNE)!=RESET)
	{
		uint8_t res = huart2.Instance->DR;//��һ���ֽ�
		Uart_Bufffer_RxIn(&res,1);
        __HAL_UART_CLEAR_FLAG(&huart2,UART_FLAG_RXNE);
	}
    if(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_ORE) != RESET) 
    {
        __HAL_UART_CLEAR_OREFLAG(&huart2);
    }
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
