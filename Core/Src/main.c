/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "fatfs.h"
#include "sdio.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "exit.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
uint32_t loop_idx500ms=0;
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
void Main_loop_1MS_First(void);
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
/*
extern void flash_erase(unsigned int page);
extern void service_write(unsigned int x);
extern unsigned long idata record_this_time;
extern void record_write(void);
extern signed int idata service_time;
extern unsigned long idata next_servece_time;
extern unsigned long idata all_time;
extern unsigned char code flash_lock;
volatile unsigned char state_mode,gcur_max; //寄存器定义currenth_old,
volatile unsigned char idata error,rev_rpm_acc;
volatile unsigned int idata voltage;
volatile signed int idata currenth1,currenth2;
unsigned char temp1,temp2;
volatile unsigned int PWMIN1,PWMIN2;
volatile unsigned int REV_TM;
volatile unsigned int rev_ts;
volatile unsigned int PWMOUT1,PWMOUT2;
volatile unsigned int idata rev_rpm;
unsigned long idata NUM_ERRORS;
unsigned int idata record_this_time2=0;
unsigned int idata service_write_data;
signed int idata vol_offset;
signed int idata cur_offset;

typedef struct
{
	char serial[10];//编号0-9
	signed int vol;//电压10 11
	signed int cur;//电流12 13
	signed int duoji_min;	//最小舵量14 15
	signed int duoji_max;//最大舵量16 17
	signed int ser_tim;//保养时间18 19
	signed int duoji_fangxiang;//舵机方向20 21  0正向 1反向
	signed int vol_offset;//预留1 22 23
	signed int cur_offset;//预留2 24 25
}struc;
xdata union UN2
{
struc set;
signed char a[26];
}SET;

xdata union UN1
{
signed int  A[10];
signed char a[20];
}TX_BUF;

xdata union UN3
{
signed int  A[9];
signed char a[18];
}PID;

bit flash_ea,flash_eb,read_pid_flag=0,set_enabel;
static volatile bit SMB_BUSY=0,SMB_RW;
volatile bit TX_OK,RX_OK; //标志位定义
unsigned char send_task=0;//待发送任务
signed int VOLTAGE_SET,CURRENT_SET;
unsigned int PWM_MAX_SET,PWM_MIN_SET,PWM_MAX_SET_2;
bit FANGXIANG_FLAG;
///////////////////////////////////////////////////////////////////
unsigned int renwu_st=0;//待执行任务
unsigned char mode;
extern unsigned char code FLASH_SET[26];//
*/

//UART接收值（未读取标志及中断）
//UART发送值（发送完成标志及中断）
//ADC值
//PWMOUT（可以扩大10倍）（底层函数设置限位值PWM_MIN_SET、PWM_MAX_SET）
//PWMIN1（可以扩大10倍）（限制在1000-2000之间）
//PWMIN2
//写入FLASH的设置参数

/*
函数功能：上电初始化
执行时间：上电时
说明：读取设置参数，PWMOUT设置，底层初始化，
			计算值清0，模式设置，运行周期设置
*/
/*
函数功能：串口数据接收处理
执行时间：接收到数据中断时，每10MS查询一次有无未读数据
*/
/*
函数功能：增加待发送任务
执行时间：每0.1S发一次常用数据，接收到读取设置值时发送设置数据
接收到读取调试数据时发送调试数据
调试模式下每0.1S发一次调试数据
*/
/*
函数功能：串口发送
执行时间：发送完成中断时且有数据待发送，每10MS检查一次发送是否空闲且有待发送数据
说明：新增发送任务时立即检查发送器是否空闲
*/
/*
函数功能：控制参数读取并修改
执行时间：控制参数修改后、初始化
*/
/*
函数功能：控制参数保存
接收到控制参数且修改时
*/
/*
函数功能：油门输出计算（）
执行时间：每50MS，或根据ADC扫描周期确定
*/
/*
函数功能：油门顺滑控制
执行时间：与舵机响应频率致
*/
/*
函数功能：模式延时切换、CDI电源控制、状态指示灯控制
执行时间：每10MS
说明：切到手动自动模式延时0.5S，切到熄火模式延时2S
*/
//
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
	MX_EXIT_Init();
	MX_DMA_Init();
	MX_ADC1_Init();
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&ADC_Value, 5);

	MX_USART2_UART_Init();
//	MX_SDIO_MMC_Init();
//	MX_FATFS_Init();
	MX_TIM3_Init();
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
	TIM3->CCR1 = 150;
	MX_TIM7_Init();
	HAL_TIM_Base_Start_IT(&htim7);//启动时间戳定时器
	CDI_POWER_ENABLE;
//	MX_TIM5_Init();
	MX_TIM6_Init();
	HAL_TIM_Base_Start_IT(&htim6);
	/* USER CODE BEGIN 2 */

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		while(Wait_processing(1000));
//		Main_loop_1MS_First();				 
		loop_idx500ms++;
		if(loop_idx500ms>=500) 					 		//主循环周期为400HZ，分频可以得到各运行周期的轮询函数。
		{
			loop_idx500ms=0;
			HAL_GPIO_TogglePin(GPIOB, BLUE_Pin|GREED_Pin|RED_Pin);
//			Main_loop_10MS_First();  
		}////////////////////////////////////////   //200HZ	 
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
