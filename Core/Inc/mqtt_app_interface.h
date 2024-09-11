/*
 * mqtt_app_interface.h
 *
 *  Created on: Sep 11, 2024
 *      Author: Lenovo310
 */

#ifndef _MQTT_APP_INTERFACE_H_
#define _MQTT_APP_INTERFACE_H_

#include <stdio.h>
#include "socket.h"
#include "MQTTClient.h"
#include "mqtt_interface.h"


void mqtt_broker_connect (void);
void mqtt_client_init (void);
int mqtt_subscribe_topic (const char *topic, messageHandler handler);
int mqtt_is_connected (void);
void mqtt_loop (void);

#endif /* INC_MQTT_APP_INTERFACE_H_ */
