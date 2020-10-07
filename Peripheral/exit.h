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
#ifndef __exit_H
#define __exit_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
#define MODE_IN_Pin 			GPIO_PIN_0
#define MODE_IN_GPIO_Port 		GPIOB
#define THROTTLE_IN_Pin 		GPIO_PIN_1
#define THROTTLE_IN_GPIO_Port 	GPIOB
#define SPEED_IN_Pin 			GPIO_PIN_15
#define SPEED_IN_GPIO_Port 		GPIOB

#define PWM_MODE				HAL_GPIO_ReadPin(MODE_IN_GPIO_Port,MODE_IN_Pin)
#define PWM_THROTTLE			HAL_GPIO_ReadPin(THROTTLE_IN_GPIO_Port,THROTTLE_IN_Pin)
#define PWM_SPEED				HAL_GPIO_ReadPin(SPEED_IN_GPIO_Port,SPEED_IN_Pin)
/* USER CODE END Private defines */

void MX_EXIT_Init(void);

/* USER CODE BEGIN Prototypes */
extern int64_t pwm_in_mode_plus,pwm_in_throttle_plus,pwm_in_speed_plus;

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
