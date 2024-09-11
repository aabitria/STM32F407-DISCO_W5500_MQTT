/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "w5500_spi.h"
#include "wizchip_conf.h"
#include "socket.h"
#include "string.h"
#include "dhcp.h"
#include "MQTTClient.h"
#include "mqtt_interface.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//#define LISTEN_PORT			(5000)
#define RECEIVE_BUFF_SIZE		(256)
#define SEND_BUFF_SIZE			RECEIVE_BUFF_SIZE
#define DHCP_BUFF_SIZE      	(2048)
#define SOCKET_APP				(1)
#define SOCKET_DHCP      		(7)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
wiz_NetInfo netinfo = {
	.mac = {0x80, 0x34, 0x28, 0x74, 0xA5, 0xCB},
	.ip = {192, 168, 254, 230},
	.sn = {255, 255, 254, 0},
	.gw = {192, 168, 254, 254},
	.dns = {8, 8, 8, 8},
	.dhcp = NETINFO_STATIC,
};
wiz_NetInfo getinfo = {0};
uint8_t destination_ip[] = {192, 168, 254, 106};
uint16_t destination_port = 1883;
uint8_t receive_buff[RECEIVE_BUFF_SIZE];//to receive data from client

MQTTClient mqtt_client;
Network network;
MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;
//MQTTMessage msg = {QOS0, 1, 0, 1, "Alvin Pogi", 14};

uint8_t sendbuff[SEND_BUFF_SIZE], receivebuff[RECEIVE_BUFF_SIZE];
uint8_t dhcp_buffer[DHCP_BUFF_SIZE];

int ip_init_flag = 0;
int mqtt_init_flag = 0;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
//void serial_write (const char data);

// callbacks
static void on_topic_temp (MessageData *);
static void on_topic_humidity (MessageData *);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void phy_status_check (void)
{
	uint8_t temp;

	printf("\n\rChecking cable presence...");

	do {
		ctlwizchip(CW_GET_PHYLINK, (void *)&temp);

		if (temp == PHY_LINK_OFF) {
			printf("\n\rNo Cable Connected!");
			HAL_Delay(1000);
		}
	} while(temp == PHY_LINK_OFF);

	// If it gets out of the loop, then connection succeeded.
	printf("\n\rGood! Cable got connected!");
}

static void phy_print_conf (void)
{
	wiz_PhyConf phyconf;

	ctlwizchip(CW_GET_PHYCONF, (void*) &phyconf);

	if (phyconf.by == PHY_CONFBY_HW) {
		printf("\n\rPHY Configured by Hardware Pins");
	} else {
		printf("\n\rPHY Configured by Registers");
	}

	if (phyconf.mode == PHY_MODE_AUTONEGO) {
		printf("\n\rAutonegotiation Enabled");
	} else {
		printf("\n\rAutonegotiation NOT Enabled");
	}

	if (phyconf.duplex == PHY_DUPLEX_FULL) {
		printf("\n\rDuplex Mode: Full");
	} else {
		printf("\n\rDuplex Mode: Half");
	}

	if (phyconf.speed == PHY_SPEED_10) {
		printf("\n\rSpeed: 10Mbps");
	} else {
		printf("\n\rSpeed: 100Mbps");
	}
}

static void print_broker_ip (void)
{
	printf("Broker IP: %d.%d.%d.%d\r\n",
			destination_ip[0],
			destination_ip[1],
			destination_ip[2],
			destination_ip[3]
			);

}

static void mqtt_broker_connect (void)
{
	connect_data.willFlag = 0;
	connect_data.MQTTVersion = 3;
	connect_data.clientID.cstring = "stm32f407_w5500";
	//connect_data.username.cstring = opts.username;
	//connect_data.password.cstring = opts.password;

	connect_data.keepAliveInterval = 60;
	connect_data.cleansession = 1;

	// 1 is the socket to use
	NewNetwork(&network, 1);  // 1 is the socket to use
	print_broker_ip();

	// Connect on both TCP/IP levels
	printf("Connecting to MQTT Broker...\r\n");
	if (ConnectNetwork(&network, destination_ip, destination_port) != SOCK_OK) {
		printf("ERROR: Cannot connect to MQTT Broker!\r\n");
		while(1);
	}

	printf("SUCCESS\r\n");
}

static void mqtt_client_init (void)
{
	MQTTClientInit(&mqtt_client, &network, 1000, sendbuff, 256, receivebuff, 256);

	printf("Sending connect packet\r\n");

	if (MQTTConnect(&mqtt_client, &connect_data) != MQTT_SUCCESS) {
		printf("ERROR!\r\n");
		while(1);
	}
}

static void mqtt_subscribe_topics (void)
{
	// Subscribe to topic and register callback
	MQTTSubscribe(&mqtt_client, "room/temp", QOS0, on_topic_temp);
	printf("Subscribed to topic room/temp\r\n");

	MQTTSubscribe(&mqtt_client, "room/humidity", QOS0, on_topic_humidity);
	printf("Subscribed to topic room/humidity\r\n");

	return;
}

void display_network_configurations(void)
{
	wiz_NetInfo info;

	ctlnetwork(CN_GET_NETINFO, (void*)&info);

	printf(" MAC         : %02X:%02X:%02X:%02X:%02X:%02X\n", info.mac[0], info.mac[1], info.mac[2], info.mac[3], info.mac[4], info.mac[5]);
	printf(" IP          : %d.%d.%d.%d\n", info.ip[0], info.ip[1], info.ip[2], info.ip[3]);
	printf(" Subnet Mask : %d.%d.%d.%d\n", info.sn[0], info.sn[1], info.sn[2], info.sn[3]);
	printf(" Gateway     : %d.%d.%d.%d\n", info.gw[0], info.gw[1], info.gw[2], info.gw[3]);
	printf("====================================================================================================\n\n");
}


static void dhcp_ip_assigned (void)
{
	printf("IP obtained: \r\n");

	getIPfromDHCP(netinfo.ip);
	getGWfromDHCP(netinfo.gw);
	getSNfromDHCP(netinfo.sn);
	getDNSfromDHCP(netinfo.dns);

	netinfo.dhcp = NETINFO_DHCP;

	ctlnetwork(CN_SET_NETINFO, (void *)&netinfo);

	display_network_configurations();

	ip_init_flag = 1;
}

static void dhcp_init (void)
{
	DHCP_init(SOCKET_DHCP, dhcp_buffer);
	reg_dhcp_cbfunc(dhcp_ip_assigned, NULL, NULL);
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
  // Disable buffering
  setbuf(stdout, NULL);
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  printf("A Simple MQTT Client Publish/Subscription Application using W5500!\r\n");
  w5500_spi_init();


  // Configure PHY by software
  wiz_PhyConf phyconf;

  phyconf.by = PHY_CONFBY_SW;
  phyconf.duplex = PHY_DUPLEX_FULL;
  phyconf.speed = PHY_SPEED_10;
  phyconf.mode = PHY_MODE_AUTONEGO;

  ctlwizchip(CW_SET_PHYCONF, (void *)&phyconf);

  phy_status_check();
  phy_print_conf();

  // start DHCP
  dhcp_init();

#if 0
  // Load netinfo to W5500
  ctlnetwork(CN_SET_NETINFO, (void *)&netinfo);

  mqtt_broker_connect();
  mqtt_client_init();
  mqtt_subscribe_topics();
#endif
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  //MQTTPublish(&mqtt_client, "test/topic1", &msg);
	  //HAL_Delay(1000);
	  DHCP_run();

	  if (ip_init_flag & !mqtt_init_flag) {
		  mqtt_broker_connect();
		  mqtt_client_init();
		  mqtt_subscribe_topics();

		  mqtt_init_flag = 1;
	  }

	  if (ip_init_flag & mqtt_init_flag) {
		  MQTTYield(&mqtt_client, 250);
	  }
	  // App specific code
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
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
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
void on_topic_temp (MessageData * msg_data)
{
	printf("New msg on topic room/temp\r\n");
}

void on_topic_humidity (MessageData * msg_data)
{
	printf("New msg on topic room/humidity\r\n");
}

void serial_write (const char data)
{
    while (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TXE) == RESET);

    huart2.Instance->DR = data;
}

// This is std C; make sure to get fn name correctly
int __io_putchar(int ch)
{
	serial_write(ch);
	return ch;
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
