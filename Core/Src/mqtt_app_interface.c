/*
 * mqtt_app_interface.c
 *
 *  Created on: Sep 11, 2024
 *      Author: Lenovo310
 */
#include "mqtt_app_interface.h"

#define RECEIVE_BUFF_SIZE		(256)
#define SEND_BUFF_SIZE			RECEIVE_BUFF_SIZE

MQTTClient mqtt_client;
Network network;
MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;

uint8_t destination_ip[] = {192, 168, 254, 106};
uint16_t destination_port = 1883;

uint8_t sendbuff[SEND_BUFF_SIZE];
uint8_t receivebuff[RECEIVE_BUFF_SIZE];

static int mqtt_connected_flag = 0;

static void mqtt_print_broker_ip (void)
{
	printf("Broker IP: %d.%d.%d.%d\r\n", destination_ip[0],
			destination_ip[1], 	destination_ip[2], destination_ip[3]);
}

int mqtt_is_connected (void)
{
	return mqtt_connected_flag;
}

void mqtt_broker_connect (void)
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
	mqtt_print_broker_ip();

	// Connect on both TCP/IP levels
	printf("Connecting to MQTT Broker...\r\n");
	if (ConnectNetwork(&network, destination_ip, destination_port) != SOCK_OK) {
		printf("ERROR: Cannot connect to MQTT Broker!\r\n");
		while(1);
	}

	printf("SUCCESS\r\n");
}

void mqtt_client_init (void)
{
	MQTTClientInit(&mqtt_client, &network, 1000, sendbuff, 256, receivebuff, 256);

	printf("Sending connect packet\r\n");

	if (MQTTConnect(&mqtt_client, &connect_data) != MQTT_SUCCESS) {
		printf("ERROR!\r\n");
		while(1);
	}

	mqtt_connected_flag = 1;
}

int mqtt_subscribe_topic (const char *topic, messageHandler handler)
{
	return MQTTSubscribe(&mqtt_client, topic, QOS0, handler);
}

void mqtt_loop (void)
{
	MQTTYield(&mqtt_client, 250);
}
