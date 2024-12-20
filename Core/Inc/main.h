/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

void toggle_screen();

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Trigger_Pin GPIO_PIN_2
#define Trigger_GPIO_Port GPIOC
#define Continous_Pin GPIO_PIN_3
#define Continous_GPIO_Port GPIOC
#define Switch_Pin GPIO_PIN_2
#define Switch_GPIO_Port GPIOA
#define Controller_Y_Pin GPIO_PIN_4
#define Controller_Y_GPIO_Port GPIOA
#define Controller_X_Pin GPIO_PIN_5
#define Controller_X_GPIO_Port GPIOA
#define Jump_Pin GPIO_PIN_4
#define Jump_GPIO_Port GPIOC
#define Aim_Pin GPIO_PIN_5
#define Aim_GPIO_Port GPIOC
#define JY60_RX_Pin GPIO_PIN_10
#define JY60_RX_GPIO_Port GPIOB
#define JY60_TX_Pin GPIO_PIN_11
#define JY60_TX_GPIO_Port GPIOB
#define Crouch_Pin GPIO_PIN_8
#define Crouch_GPIO_Port GPIOA
#define USB_COM_Pin GPIO_PIN_9
#define USB_COM_GPIO_Port GPIOA
#define USB_COMA10_Pin GPIO_PIN_10
#define USB_COMA10_GPIO_Port GPIOA
#define Reload_Pin GPIO_PIN_3
#define Reload_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
