//
// Created by davi on 11/7/17.
//

#ifndef ESTACAO_MQTT_H
#define ESTACAO_MQTT_H


#include "MQTTClient.h"
#include "MQTTClientPersistence.h"

#ifdef AUTH
#   define BROKER "iot.eclipse.org"
#else
#   define BROKER "localhost"
#endif

#define CLIENTID "rasp"
#define ROOT_TOPIC "estacao_cunha/"
#define QOS         1
#define TIMEOUT     10000L

void setup_mqtt(void);

void mqtt(void);

void mqtt_pub_double(char *topic, double measure);

void mqtt_pub(char *topic, char *measure);

extern MQTTClient client;

#endif //ESTACAO_MQTT_H
