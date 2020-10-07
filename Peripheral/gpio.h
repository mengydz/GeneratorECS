/**
  ******************************************************************************
  * File Name          : gpio.h
  * Description        : This file contains all the functions prototypes for
  *                      the gpio
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __gpio_H
#define __gpio_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
#define IIC_SCL_Pin 			GPIO_PIN_10
#define IIC_SCL_GPIO_Port 		GPIOB
#define IIC_SDA_Pin 			GPIO_PIN_11
#define IIC_SDA_GPIO_Port 		GPIOB
#define BLUE_Pin 				GPIO_PIN_12
#define BLUE_GPIO_Port 			GPIOB
#define GREED_Pin 				GPIO_PIN_13
#define GREED_GPIO_Port 		GPIOB
#define RED_Pin 				GPIO_PIN_14
#define RED_GPIO_Port 			GPIOB
#define SPEED_IN_Pin 			GPIO_PIN_15
#define SPEED_IN_GPIO_Port 		GPIOB
#define BOARD_LED_Pin 			GPIO_PIN_6
#define BOARD_LED_GPIO_Port 	GPIOC

#define SDIO_CD_Pin 			GPIO_PIN_7
#define SDIO_CD_GPIO_Port 		GPIOC

#define CDI_POWER_Pin 			GPIO_PIN_5
#define CDI_POWER_GPIO_Port 	GPIOB

#define CDI_POWER_ENABLE	HAL_GPIO_WritePin(CDI_POWER_GPIO_Port, CDI_POWER_Pin, GPIO_PIN_SET)
#define CDI_POWER_DISABLE	HAL_GPIO_WritePin(CDI_POWER_GPIO_Port, CDI_POWER_Pin, GPIO_PIN_SET)
/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
