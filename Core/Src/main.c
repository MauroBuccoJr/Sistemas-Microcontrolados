/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "button.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */


uint16_t read_distance(void) {
  uint8_t timeout = 1000;
  uint8_t i = 0;
  uint16_t ticks = 0;

  while (!HAL_GPIO_ReadPin(GPIOB, ECHO_PIN)) {
    i += 1;
    if (i >= timeout) {
      break;
    }
  };

  while (HAL_GPIO_ReadPin(GPIOB, ECHO_PIN)) {
    ticks += 1;
    if (ticks > 100) {
      break;
    }
  };

  return 600;

  /*return ticks;*/
}



int main(void)
{
  /* USER CODE BEGIN 1 */

  uint16_t count = 0;
  double distance = 0.0;

  uint16_t led_pins[] = {LED_0_PIN, LED_1_PIN, LED_2_PIN};
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  
  count = 0;
  
  while (1)
  {

    if(button_release(GPIOA, BUTTON_PIN, GPIO_PIN_RESET)) {
      HAL_GPIO_WritePin(GPIOB, TRIGGER_PIN, GPIO_PIN_SET);
      HAL_Delay(120);
      HAL_GPIO_WritePin(GPIOB, TRIGGER_PIN, GPIO_PIN_RESET);

      count = read_distance();
      distance = (double) (count / PULSE_RATIO) * 340.0 * 3.6; /* Velocidade do som = 340 km/h = 340 * 3.6 m/s */

      if (distance < CLOSE_DISTANCE && distance) {
        HAL_GPIO_WritePin(GPIOB, LED_0_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, LED_1_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, LED_2_PIN, GPIO_PIN_RESET);
      }
      else {
        HAL_GPIO_WritePin(GPIOB, LED_0_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, LED_1_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, LED_2_PIN, GPIO_PIN_SET);
      }
    }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  GPIO_InitTypeDef LED_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE(); /* Essa linha */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(KIT_LED_GPIO_Port, KIT_LED_Pin, GPIO_PIN_RESET);

  /*HAL_GPIO_WritePin(LED_PIN_GPIO_Port, LED_0_PIN | LED_1_PIN | LED_2_PIN | LED_3_PIN | LED_4_PIN, GPIO_PIN_RESET);*/
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_All, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_All, GPIO_PIN_RESET);
  
  LED_InitStruct.Pin = LED_0_PIN | LED_1_PIN | LED_2_PIN;
  LED_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  LED_InitStruct.Pull = GPIO_NOPULL;
  LED_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &LED_InitStruct);

  /*Configure GPIO pin : KIT_LED_Pin */
  GPIO_InitStruct.Pin = KIT_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(KIT_LED_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitTypeDef GPIO_Button_Pin_Init_Struct = {0};

  GPIO_Button_Pin_Init_Struct.Pin = BUTTON_PIN;
  GPIO_Button_Pin_Init_Struct.Mode = GPIO_MODE_INPUT;
  GPIO_Button_Pin_Init_Struct.Pull = GPIO_NOPULL;
  GPIO_Button_Pin_Init_Struct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_Button_Pin_Init_Struct);

  GPIO_InitTypeDef GPIO_Trigger_Pin_Init_Struct = {0};

  GPIO_Trigger_Pin_Init_Struct.Pin = TRIGGER_PIN;
  GPIO_Trigger_Pin_Init_Struct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_Trigger_Pin_Init_Struct.Pull = GPIO_NOPULL;
  GPIO_Trigger_Pin_Init_Struct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_Trigger_Pin_Init_Struct);

  GPIO_InitTypeDef GPIO_Echo_Pin_Init_Struct = {0};

  GPIO_Echo_Pin_Init_Struct.Pin = ECHO_PIN;
  GPIO_Echo_Pin_Init_Struct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_Echo_Pin_Init_Struct.Pull = GPIO_NOPULL;
  GPIO_Echo_Pin_Init_Struct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_Echo_Pin_Init_Struct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
