//
// Created by davi on 11/7/17.
//

#include <string.h>
#include <stdlib.h>
#include "mqtt.h"
#include "debug.h"
#include "config.h"

MQTTClient client;

void setup_mqtt() {
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    if (NULL != client) {
        MQTTClient_disconnect(client, 10000);
        MQTTClient_destroy(&client);
    }

    TRACE("Conectando em %s...\n", BROKER);
    MQTTClient_create(&client, BROKER, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 600;
    conn_opts.cleansession = 1;

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        terminate = 1;
        exit(-1);
    }

    conexao_mqtt = 1;
}

void mqtt() {
    setup_mqtt();
}

void mqtt_pub_double(char *topic, double measure) {
    char m[5];
    TRACE("Convertendo medida [%.1f] para string...\n", measure);
    snprintf(m, 5, "%.1f", measure);
    mqtt_pub(topic, m);
}

void mqtt_pub(char *topic, char *measure) {
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    char *dstTopic;
    size_t strSize = sizeof(char) * (strlen(ROOT_TOPIC) + strlen(topic) + 1);

    dstTopic = (char *) malloc(strSize);
    bzero(dstTopic, strSize);
    strcpy(dstTopic, ROOT_TOPIC);
    strcat(dstTopic, topic);

    TRACE("Publicando [%s] em [%s]...\n", measure, dstTopic);
    pubmsg.payload = measure;
    pubmsg.payloadlen = sizeof(measure);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    MQTTClient_publishMessage(client, dstTopic, &pubmsg, &token);
    TRACE("Waiting for up to %d seconds for publication of [%s]"
                  "on topic %s for client with ClientID: %s\n",
          (int) (TIMEOUT / 1000), pubmsg.payload, dstTopic, CLIENTID);
    MQTTClient_waitForCompletion(client, token, TIMEOUT);

    free(dstTopic);
}
