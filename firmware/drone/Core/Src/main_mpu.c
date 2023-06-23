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
#include <stdio.h>
#include <string.h>
#include <stdint.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MPU6050_SENSOR_ADDR                 0x68<<1     /*!< Slave address of the mpu6050 sensor */
#define MPU6050_WHO_AM_I_REG_ADDR           0x75        /*!< Register addresses of the "who am I" register */

#define MPU6050_PWR_MGMT_1_REG_ADDR         0x6B        /*!< Register addresses of the power managment register */
#define MPU6050_RESET_BIT                   7

#define MPU6050_LOW_PASS_FILTER             0x1A        /* Register adress of the low pass filter*/
#define MPU6050_LOW_PASS_FILTER_BANDWIDTH   0x05        /* Register adress of the 10Hz bandwidth */
#define MPU6050_GYRO_CONFIG                 0x1B
#define MPU6050_500_DEGREE                  0x8
#define MPU6050_GYRO_MEASURMENT             0x43
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
int init_mpu6050();
/* USER CODE BEGIN PFP */

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
 HAL_StatusTypeDef ret;
//uint8_t check;
uint8_t buf[12];
uint8_t init_mpu;
uint8_t data_register;
uint8_t Pwr_management_val, Low_pass_filter_val, Gyro_config_val;
uint8_t Gyro_values[2];
float RateRoll, RatePitch, RateYaw;
float RateCalibrationRoll, RateCalibrationPitch, RateCalibrationYaw;
int RateCalibrationNumber;
int16_t gyro_x, gyro_y, gyro_z;
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
 //power management

 data_register = 0x00;
 HAL_I2C_Mem_Write(&hi2c3, MPU6050_SENSOR_ADDR, MPU6050_PWR_MGMT_1_REG_ADDR, 1, &data_register, 1, HAL_MAX_DELAY);
 //who am i
 init_mpu = init_mpu6050();

 if(init_mpu == 0x68){

	  sprintf((char*)buf, "who am i =  %x \r\n", init_mpu);
	  HAL_UART_Transmit(&huart2,buf, strlen((char*)buf), HAL_MAX_DELAY);
	  HAL_I2C_Mem_Read(&hi2c3, MPU6050_SENSOR_ADDR, MPU6050_PWR_MGMT_1_REG_ADDR, 1, &Pwr_management_val, 1, 100);
	  sprintf((char*)buf, "Power management=  %x \r\n", Pwr_management_val);
	  HAL_UART_Transmit(&huart2,buf, strlen((char*)buf), HAL_MAX_DELAY);

	  //apply low pass filter
	  data_register = MPU6050_LOW_PASS_FILTER_BANDWIDTH;
	  HAL_I2C_Mem_Write(&hi2c3, MPU6050_SENSOR_ADDR, MPU6050_LOW_PASS_FILTER, 1, &data_register, 1, HAL_MAX_DELAY);
	  HAL_I2C_Mem_Read(&hi2c3, MPU6050_SENSOR_ADDR, MPU6050_LOW_PASS_FILTER, 1, &Low_pass_filter_val, 1, 100);
	  sprintf((char*)buf, "Low pass filter =  %x \r\n", Low_pass_filter_val);
	  HAL_UART_Transmit(&huart2,buf, strlen((char*)buf), HAL_MAX_DELAY);


	  //Gyro config
	  data_register = MPU6050_500_DEGREE;
	  HAL_I2C_Mem_Write(&hi2c3, MPU6050_SENSOR_ADDR, MPU6050_GYRO_CONFIG, 1, &data_register, 1, HAL_MAX_DELAY);
	  HAL_I2C_Mem_Read(&hi2c3, MPU6050_SENSOR_ADDR, MPU6050_GYRO_CONFIG, 1, &Gyro_config_val, 1, 100);
	  sprintf((char*)buf, "Gyro config=  %x \r\n", Gyro_config_val);
	  HAL_UART_Transmit(&huart2,buf, strlen((char*)buf), HAL_MAX_DELAY);
	    for(RateCalibrationNumber=0; RateCalibrationNumber<2000; RateCalibrationNumber++){
	    	RateCalibrationRoll+=RateRoll;
	    	RateCalibrationPitch+=RatePitch;
			RateCalibrationYaw+=RateYaw;
	  	ret = HAL_I2C_Mem_Read(&hi2c3, MPU6050_SENSOR_ADDR, MPU6050_GYRO_MEASURMENT, 1, &Gyro_values, 6, 100);
	  	if(ret != HAL_OK){
	  	  strcpy((char*)buf, "Error \r\n");

	  	}
	  	else{
		    gyro_x = (Gyro_values[0] << 8) | Gyro_values[1];
		    gyro_y = (Gyro_values[2] << 8) | Gyro_values[3];
		    gyro_z = (Gyro_values[4] << 8) | Gyro_values[5];
		    RateRoll = (float)gyro_x/65.5;
		    RatePitch = (float)gyro_y/65.5;
		    RateYaw = (float)gyro_z/65.5;
		    }

		HAL_Delay(1);

	  	}
	    RateCalibrationRoll/=2000;
	    RateCalibrationPitch/=2000;
		RateCalibrationYaw/=2000;

 }



 /* USER CODE END 2 */

 /* Infinite loop */
 /* USER CODE BEGIN WHILE */

 while (1)
 {

	  	ret = HAL_I2C_Mem_Read(&hi2c3, MPU6050_SENSOR_ADDR, MPU6050_GYRO_MEASURMENT, 1, &Gyro_values, 6, 100);
	  	if(ret != HAL_OK){
	  	  strcpy((char*)buf, "Error \r\n");

	  	}
	  	else{
		    gyro_x = (Gyro_values[0] << 8) | Gyro_values[1];
		    gyro_y = (Gyro_values[2] << 8) | Gyro_values[3];
		    gyro_z = (Gyro_values[4] << 8) s | Gyro_values[5];
		    RateRoll = (float)gyro_x/65.5;
		    RatePitch = (float)gyro_y/65.5;
		    RateYaw = (float)gyro_z/65.5;

			RateRoll-=RateCalibrationRoll;
			RatePitch-=RateCalibrationPitch;
			RateYaw-=RateCalibrationYaw;
		    sprintf((char*)buf, "Gyro X: %f\t\t Gyro Y: %f\t\t Gyro Z: %f\r\n", RateRoll, RatePitch, RateYaw);
			HAL_UART_Transmit(&huart2,buf, strlen((char*)buf), HAL_MAX_DELAY);
		  HAL_Delay(50);
	  	}

		  //RCC_HSICALIBRATION_DEFAULT
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
 RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
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

int init_mpu6050(void){
	uint8_t check;
	  HAL_I2C_Mem_Read(&hi2c3, MPU6050_SENSOR_ADDR, MPU6050_WHO_AM_I_REG_ADDR, 1, &check, 1, 100);

	return check;
}
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