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
volatile unsigned char state_mode,gcur_max; //�Ĵ�������currenth_old,
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
	char serial[10];//���0-9
	signed int vol;//��ѹ10 11
	signed int cur;//����12 13
	signed int duoji_min;	//��С����14 15
	signed int duoji_max;//������16 17
	signed int ser_tim;//����ʱ��18 19
	signed int duoji_fangxiang;//�������20 21  0���� 1����
	signed int vol_offset;//Ԥ��1 22 23
	signed int cur_offset;//Ԥ��2 24 25
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
volatile bit TX_OK,RX_OK; //��־λ����
unsigned char send_task=0;//����������
signed int VOLTAGE_SET,CURRENT_SET;
unsigned int PWM_MAX_SET,PWM_MIN_SET,PWM_MAX_SET_2;
bit FANGXIANG_FLAG;
///////////////////////////////////////////////////////////////////
unsigned int renwu_st=0;//��ִ������
unsigned char mode;
extern unsigned char code FLASH_SET[26];//
*/

//UART����ֵ��δ��ȡ��־���жϣ�
//UART����ֵ��������ɱ�־���жϣ�
//ADCֵ
//PWMOUT����������10�������ײ㺯��������λֵPWM_MIN_SET��PWM_MAX_SET��
//PWMIN1����������10������������1000-2000֮�䣩
//PWMIN2
//д��FLASH�����ò���

/*
�������ܣ��ϵ��ʼ��
ִ��ʱ�䣺�ϵ�ʱ
˵������ȡ���ò�����PWMOUT���ã��ײ��ʼ����
			����ֵ��0��ģʽ���ã�������������
*/
/*
�������ܣ��������ݽ��մ���
ִ��ʱ�䣺���յ������ж�ʱ��ÿ10MS��ѯһ������δ������
*/
/*
�������ܣ����Ӵ���������
ִ��ʱ�䣺ÿ0.1S��һ�γ������ݣ����յ���ȡ����ֵʱ������������
���յ���ȡ��������ʱ���͵�������
����ģʽ��ÿ0.1S��һ�ε�������
*/
/*
�������ܣ����ڷ���
ִ��ʱ�䣺��������ж�ʱ�������ݴ����ͣ�ÿ10MS���һ�η����Ƿ�������д���������
˵����������������ʱ������鷢�����Ƿ����
*/
/*
�������ܣ����Ʋ�����ȡ���޸�
ִ��ʱ�䣺���Ʋ����޸ĺ󡢳�ʼ��
*/
/*
�������ܣ����Ʋ�������
���յ����Ʋ������޸�ʱ
*/
/*
�������ܣ�����������㣨��
ִ��ʱ�䣺ÿ50MS�������ADCɨ������ȷ��
*/
/*
�������ܣ�����˳������
ִ��ʱ�䣺������ӦƵ����
*/
/*
�������ܣ�ģʽ��ʱ�л���CDI��Դ���ơ�״ָ̬ʾ�ƿ���
ִ��ʱ�䣺ÿ10MS
˵�����е��ֶ��Զ�ģʽ��ʱ0.5S���е�Ϩ��ģʽ��ʱ2S
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
	HAL_TIM_Base_Start_IT(&htim7);//����ʱ�����ʱ��
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
		if(loop_idx500ms>=500) 					 		//��ѭ������Ϊ400HZ����Ƶ���Եõ����������ڵ���ѯ������
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
