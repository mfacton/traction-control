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
#include "stm32g4xx_hal.h"

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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BTN1_Pin GPIO_PIN_13
#define BTN1_GPIO_Port GPIOC
#define TACH_Pin GPIO_PIN_0
#define TACH_GPIO_Port GPIOA
#define SIG2_Pin GPIO_PIN_1
#define SIG2_GPIO_Port GPIOA
#define SIG1_Pin GPIO_PIN_3
#define SIG1_GPIO_Port GPIOA
#define BTN3_Pin GPIO_PIN_5
#define BTN3_GPIO_Port GPIOA
#define TACHB0_Pin GPIO_PIN_0
#define TACHB0_GPIO_Port GPIOB
#define BTN4_Pin GPIO_PIN_2
#define BTN4_GPIO_Port GPIOB
#define DRV2_Pin GPIO_PIN_11
#define DRV2_GPIO_Port GPIOB
#define DRV1_Pin GPIO_PIN_12
#define DRV1_GPIO_Port GPIOB
#define SDAD_Pin GPIO_PIN_8
#define SDAD_GPIO_Port GPIOA
#define SCLD_Pin GPIO_PIN_9
#define SCLD_GPIO_Port GPIOA
#define PGPS_Pin GPIO_PIN_11
#define PGPS_GPIO_Port GPIOA
#define SCLE_Pin GPIO_PIN_15
#define SCLE_GPIO_Port GPIOA
#define BTN2_Pin GPIO_PIN_5
#define BTN2_GPIO_Port GPIOB
#define GPS_TX_Pin GPIO_PIN_6
#define GPS_TX_GPIO_Port GPIOB
#define GPS_RX_Pin GPIO_PIN_7
#define GPS_RX_GPIO_Port GPIOB
#define SDAE_Pin GPIO_PIN_9
#define SDAE_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
