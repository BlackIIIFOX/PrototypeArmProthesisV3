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
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "can.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h> 
#include "data_handler.h"
#include "gesture_model.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

typedef struct{
  unsigned long time_delay_actions;
  uint32_t indexAction;
  uint32_t currentRepeat;
  GestureModel* current_command;
} StructInfoCurrentCommand;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

// System commands
#define CommandExex 0x17
#define CommandExexRaw 0x18

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* CAN BUS PV Variables */
CAN_FilterTypeDef sFilterConfig;
CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;
uint8_t TxData[8];
uint8_t RxData[8];
uint32_t TxMailbox;

// For receive
uint8_t dataRx;
StructPackageBuffer receivePackage;
StructPackageBuffer transmitPackage;

// Current command
StructInfoCurrentCommand currentCommand;

// Time system
uint32_t time_last_receive = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void setCurrentCommand(GestureModel* new_current_command)
{
  currentCommand.current_command = new_current_command;
  currentCommand.indexAction = 0;
  currentCommand.time_delay_actions = 0;
  currentCommand.currentRepeat = 0;
}

void clearInfoCurrentCommand()
{
  currentCommand.current_command = NULL;
  currentCommand.indexAction = 0;
  currentCommand.time_delay_actions = 0;
  currentCommand.currentRepeat = 0;
}

void ClearCurrentCommand()
{
  if(currentCommand.current_command != NULL)
  {
    GestureModel_Destroy(currentCommand.current_command);
    clearInfoCurrentCommand();
  }
}

void RecognizePackage(StructPackage* recPackage)
{
  switch ( recPackage->package[0] ) 
  {
    case CommandExex:
    {
      ClearCurrentCommand();
      GestureModel* gesture = GestureModel_DeserializeGesture(&(recPackage->package[1]));
      setCurrentCommand(gesture);
      break;
    }
    
    case CommandExexRaw:
      {
        break;
      }
  }
}

/**
* @brief Выполняет необходимую программную иницилизацию CAN и 
* переменных для работы с ней.
* @retval None
*/
void CAN_Init()
{
  sFilterConfig.FilterBank = 0;
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  sFilterConfig.FilterIdHigh = 0X0000;
  sFilterConfig.FilterIdLow = 0x0000;
  sFilterConfig.FilterMaskIdHigh = 0x0000;
  sFilterConfig.FilterMaskIdLow = 0x0000;
  sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
  sFilterConfig.FilterActivation = ENABLE;
  sFilterConfig.SlaveStartFilterBank = 14;
  
  if (HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  
  if (HAL_CAN_Start(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
  
  if (HAL_CAN_ActivateNotification (&hcan, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK)
  {
    Error_Handler();
  }
  
  TxHeader.StdId = 0x3E9;
  TxHeader.ExtId = 0x01;
  TxHeader.RTR = CAN_RTR_DATA;
  TxHeader.IDE = CAN_ID_STD;
  TxHeader.TransmitGlobalTime = DISABLE;
  TxHeader.DLC = 8;
  TxData[0] = 0;
  TxData[1] = 0;
  TxData[2] = 0;
  TxData[3] = 0;
  TxData[4] = 0;
  TxData[5] = 0;
  TxData[6] = 0;
  TxData[7] = 0;
}

void ExecMotion(MotionModel* motion)
{
  TxData[0] = motion->PointerFinger;
  TxData[1] = motion->MiddleFinger;
  TxData[2] = motion->RingFinder;
  TxData[3] = motion->LittleFinger;
  TxData[4] = motion->ThumbFinger;
  HAL_StatusTypeDef status =  HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox);
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
  MX_CAN_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  Create_Data_Receive();
  CreatePackageBuffer(&receivePackage);
  CreatePackageBuffer(&transmitPackage);
  SetPackageBuf(&receivePackage);
  clearInfoCurrentCommand();
  CAN_Init();
  
  HAL_UART_Receive_IT(&huart1, &dataRx, 1);
  
  // TODO: убрать.
//  StructPackage test;
//  
//  test.countData = 64;
//  uint8_t array[64] = {0x17, 20, 121, 168, 146, 80, 155, 154, 73, 130, 116, 167,
//  175, 107, 229, 121, 75, 8, 208, 154, 208, 190, 208, 183, 208, 176, 18, 131, 48,
//  27, 31, 137, 215, 65, 1, 2, 3, 180, 0, 0, 180, 0, 0, 0, 244, 1, 2, 3, 4, 5, 1,
//  0, 0, 50, 0, 180, 0, 0, 180, 0, 0, 0, 244, 1};
//  
//  test.package = array;
//  RecognizePackage(&test);
//  while(1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    if(PackageBufferCount(&receivePackage) > 0)
    {
      StructPackage* currentPackage = PackageBufGet(&receivePackage);
      RecognizePackage(currentPackage);
    }
    
    if(currentCommand.current_command != NULL)
    {
      unsigned long millis = HAL_GetTick();
      
      if(currentCommand.time_delay_actions<=millis)
        {
          MotionModel* action = (MotionModel*)(list_get(currentCommand.current_command->ListMotions, currentCommand.indexAction)->data);
          //(MotionModel*)currentCommand.current_command->ListMotions[currentCommand.indexAction];
          // set new action in timers
          // set finger
          ExecMotion(action);
        
          // check end command
          currentCommand.time_delay_actions = action->DelMotion + millis;
          currentCommand.indexAction++;
          
          if(currentCommand.current_command->InfoGesture.NumberOfMotions == currentCommand.indexAction)
          {
            if(currentCommand.current_command->InfoGesture.IterableGesture == true)
            {
              currentCommand.indexAction = 0;
            }
            else
            {
              if(currentCommand.current_command->InfoGesture.NumberOfGestureRepetitions - 1 > currentCommand.currentRepeat)
              {
                currentCommand.indexAction = 0;
                currentCommand.currentRepeat++;
              }
              else 
              {
                ClearCurrentCommand();
              }
            } 
          }
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

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
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
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart == &huart1)
  {
    uint32_t time = HAL_GetTick();
    if((time - time_last_receive) > 1000)
    {
      Clear_Data_Receive();
    }
    
    Append_Data_Receive(dataRx);
    time_last_receive = time;
    HAL_UART_Receive_IT(&huart1, &dataRx, 1);
  }
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
