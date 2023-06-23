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
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define numberofpixels 5
#define bytesperpixel 3
//https://cdn-shop.adafruit.com/datasheets/WS2812.pdf
	//neopixel understands a bit as high when it sees a pwm with 64% duty cycle
#define bitHightimercount 58 //if our pwm period is 90, 64%(90)=57.6 close to 58
	//neopixel understands a bit as low when it sees a pwm with 32% duty cycle
#define bitLowtimercount 29  //if our pwm period is 90, 32%(90)=28.8 close to 29

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

//we need 3 neopixel bytes (r g b) to set the colour of every pixel.
//we need 8 PWM cycles to transmit 1 neopixel byte.
//we need 1 uint8_t to be loaded in TIM1->CCR1 for every PWM cycle.
uint8_t rgbw_arr[numberofpixels * bytesperpixel * 8 + 1];//every pixel colour info is 24 bytes long

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void flushArrayPixel(//zeroes the array
		uint8_t *buffer, //address of our buffer
		uint8_t bytenumber //number of bytes to erase
		) {
	for (uint32_t i = 0; i < bytenumber-1; ++i) {
		buffer[i] = bitLowtimercount;
	}
	buffer[bytenumber] = 0;//needs to be 0 to silent PWM at the end of transaction
}

uint32_t loadArrayOnePixel(uint8_t R, uint8_t G, uint8_t B,
		uint8_t *buffer, //address of our buffer
		uint8_t pixelnumber //pixel index inside buffer
		) {
	if(pixelnumber>numberofpixels){return -1;}//in case we mess up

	for (uint32_t i = 0; i < bytesperpixel * 8; ++i) { //we need to store every bit

		if (i < 8) { //this means first byte R
			if (R & (0x80 >> i)) { //this is a mask for reading every bit inside the byte R
				buffer[i + pixelnumber * bytesperpixel * 8] = bitHightimercount;
			} else {
				buffer[i + pixelnumber * bytesperpixel * 8] = bitLowtimercount;
			}
		}

		if ((i >= 8) & (i < 16)) { //this means second byte G
			if (G & (0x80 >> (i - 8))) {
				buffer[i + pixelnumber * bytesperpixel * 8] = bitHightimercount;
			} else {
				buffer[i + pixelnumber * bytesperpixel * 8] = bitLowtimercount;
			}
		}

		if ((i >= 16) & (i < 24)) { //this means third byte B
			if (B & (0x80 >> (i - 16))) {
				buffer[i + pixelnumber * bytesperpixel * 8] = bitHightimercount;
			} else {
				buffer[i + pixelnumber * bytesperpixel * 8] = bitLowtimercount;
			}
		}

	}
	return 1;
}


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_SPI1_Init();
  MX_ADC1_Init();
  MX_I2C3_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
	//flush buffer first
	flushArrayPixel(&rgbw_arr, sizeof(rgbw_arr));

	//for led effect
	uint8_t counter=0;
	uint8_t flag=0;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    	//dont know why exactly yet but DMA needs to be stopped before trigger it again
		HAL_TIM_PWM_Stop_DMA(&htim2, TIM_CHANNEL_1);

		/////////////////////////////////////////////////////blue soft pulse of every pixel
		for (uint32_t i = 0; i < numberofpixels; ++i) {
				loadArrayOnePixel(0,0,counter,&rgbw_arr,i);
		}

		HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, &rgbw_arr,sizeof(rgbw_arr));

		if(counter==0xFF){flag=0;}

		if(counter==0x00){flag=1;}

		if(flag){
			counter++;
		}else{
			counter--;
		}

    HAL_GPIO_TogglePin(LD3_GPIO_Port,LD3_Pin);
    HAL_Delay(1000);
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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
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