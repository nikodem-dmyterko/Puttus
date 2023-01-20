/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "usb_otg.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "hcsr04.h"
#include "robot.h"
#include "motor.h"
#include "controller.h"
#include "fir_coeffs.h"
#include "arm_math.h"
#include "string.h"
#include "stdlib.h"
#include "lcd_i2c.h"
#include "stdbool.h"
#include "stdio.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define INPUT_VALUE 100
#define MAX_PWM 1000
#define MIN_PWM 700
#define MIN_ERROR 2
#define MAX_ERROR 200
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
struct lcd_disp disp;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
char buffer[3] = {'1', '0', '0'};

struct us_sensor_str distance_sensor;
struct Controller controller;
struct Motor left_motor;
struct Motor right_motor;
struct Robot robot;
arm_fir_instance_f32 fir;
float32_t fir_state[13];

//zmienne do wyświetlacza
int count = 0;
int count_prev = 0;
int i = 0;

char messegeUp[16];
char messegeDown[16];



void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(TIM1 == htim->Instance)
	{
		uint32_t echo_us;

		echo_us = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
		distance_sensor.distance_cm = hc_sr04_convert_us_to_cm(echo_us);
//		arm_fir_f32(&fir, &distance_sensor.distance_cm, &robot.position, 1);
		robot_linear_update(&robot, distance_sensor.distance_cm, 1/16);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(TIM4 == htim->Instance)
	{
		float error = robot_error(&robot);
		float duty = controller_control_signal(&controller, error);
		motor_move(&left_motor, duty);
		motor_move(&right_motor, duty);
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART3)
	{
		char distance[3];

		sscanf((char*)buffer, "%c%c%c", &distance[0], &distance[1], &distance[2]);

		robot_set_end_position(&robot, atoi(distance));
		HAL_UART_Receive_IT(&huart3, (uint8_t*)buffer, strlen(buffer));
	}
}

//funkcja wyświelająca
void update_lcd(int count)
{
	if(count%2 != 0)
	{
		//zadana wartosc
		sprintf((char *)messegeUp, "zadana: %f", robot.end_position);
		sprintf((char *)disp.f_line, messegeUp);
		sprintf((char *)messegeDown, "aktualna: %f", robot.position);
		sprintf((char *)disp.s_line, messegeDown);
		lcd_display(&disp);
	}
	else
	{
		//uchyb
		sprintf((char *)messegeUp, "uchyb: %f", robot.end_position - robot.position);
		sprintf((char *)disp.f_line, messegeUp);
		sprintf((char *)disp.s_line, "AiR          ");
		lcd_display(&disp);
	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

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
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_I2C1_Init();
  MX_TIM8_Init();
  /* USER CODE BEGIN 2 */
  arm_fir_init_f32(&fir, BL, B, fir_state, 1);
  hc_sr04_init(&distance_sensor, &htim1, &htim2, TIM_CHANNEL_3);
  robot_init(&robot, distance_sensor.distance_cm, 0);
  robot_set_end_position(&robot, INPUT_VALUE);
  controller_init(&controller, MAX_PWM, 300, MIN_PWM, MIN_ERROR, MAX_ERROR);
  motor_init(&left_motor, &htim3, TIM_CHANNEL_1, FORWARD_MOTOR_1_GPIO_Port, BACKWARD_MOTOR_1_GPIO_Port, FORWARD_MOTOR_1_Pin, BACKWARD_MOTOR_1_Pin);
  motor_init(&right_motor, &htim3, TIM_CHANNEL_2, FORWARD_MOTOR_2_GPIO_Port, BACKWARD_MOTOR_2_GPIO_Port, FORWARD_MOTOR_2_Pin, BACKWARD_MOTOR_2_Pin);

  HAL_TIM_Base_Start_IT(&htim4);
  HAL_UART_Receive_IT(&huart3, (uint8_t*)buffer, strlen(buffer));

  //zegar enkodera
  HAL_TIM_Encoder_Start(&htim8, TIM_CHANNEL_1);

  //inicjalizacja wyswietlacza
  disp.addr = (0x27 << 1);
  disp.bl = true;
  lcd_init(&disp);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  count = __HAL_TIM_GET_COUNTER(&htim8);
	//wyswielacz
	  if(HAL_GPIO_ReadPin(USER_Btn_GPIO_Port, USER_Btn_Pin) == 1)
	  {
			update_lcd(count);
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

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
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
