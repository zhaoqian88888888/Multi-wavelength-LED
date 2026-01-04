/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
extern uint8_t TASK_BUFF[20]; // ÉùÃ÷ TASK_BUFF
extern uint8_t channel;       // ÉùÃ÷ channel
extern float Temp, Current_VALUE;
extern uint8_t mode_flag;
extern float USER_SETDC,NOW_SETDC;
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ARRIVE_Pin GPIO_PIN_5
#define ARRIVE_GPIO_Port GPIOA
#define ARRIVE_EXTI_IRQn EXTI9_5_IRQn
#define ORIGIN_EXIT_Pin GPIO_PIN_11
#define ORIGIN_EXIT_GPIO_Port GPIOB
#define ORIGIN_EXIT_EXTI_IRQn EXTI15_10_IRQn
#define LED1_Pin GPIO_PIN_12
#define LED1_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_13
#define LED2_GPIO_Port GPIOB
#define LED3_Pin GPIO_PIN_14
#define LED3_GPIO_Port GPIOB
#define LED4_Pin GPIO_PIN_15
#define LED4_GPIO_Port GPIOB
#define LED5_Pin GPIO_PIN_6
#define LED5_GPIO_Port GPIOC
#define INA_SDA_Pin GPIO_PIN_7
#define INA_SDA_GPIO_Port GPIOC
#define INA_SCL_Pin GPIO_PIN_8
#define INA_SCL_GPIO_Port GPIOC
#define LED6_Pin GPIO_PIN_9
#define LED6_GPIO_Port GPIOC
#define LED7_Pin GPIO_PIN_8
#define LED7_GPIO_Port GPIOA
#define LED8_Pin GPIO_PIN_9
#define LED8_GPIO_Port GPIOA
#define SDA_SHT30_Pin GPIO_PIN_12
#define SDA_SHT30_GPIO_Port GPIOC
#define SCL_SHT30_Pin GPIO_PIN_2
#define SCL_SHT30_GPIO_Port GPIOD
#define SCL_DAC_Pin GPIO_PIN_3
#define SCL_DAC_GPIO_Port GPIOB
#define SDA_DAC_Pin GPIO_PIN_4
#define SDA_DAC_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
typedef struct
{
   unsigned short flag;
   unsigned int pin_index;
   GPIO_TypeDef *gpiox;

   uint16_t dac_addr;
   uint16_t dac_channel; 
   
   int led_val; 	
	
}led_analogue;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
