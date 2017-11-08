//
// Created by davi on 11/8/17.
//

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "temperatura.h"
#include "mqtt.h"
#include "debug.h"

double read_temp(void) {
    FILE *ds18b20 = fopen(SENSOR_TEMPERATURA, "r");
    double t = -1;
    char crcVar[5];
    if (ds18b20 == NULL) {
        printf("Check connections %s\n",
               SENSOR_TEMPERATURA);
        perror("\n");
    }
    if (ds18b20 != NULL) {
        if (!ferror(ds18b20)) {
            fscanf(ds18b20, "%*x %*x %*x %*x %*x %*x %*x %*x %*x : crc=%*x %s",
                   crcVar);
            if (strncmp(crcVar, "YES", 3) == 0) {
                fscanf(ds18b20, "%*x %*x %*x %*x %*x %*x %*x %*x %*x t=%lf",
                       &t);
                t /= 1000.0;
            }
        }
    }
    fclose(ds18b20);
    return t;
}

void *thread_temperatura(void *pVoid) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (1) {
        double temperatura = read_temp();
        mqtt_pub_double("temperatura", temperatura);
        printf("Temperatura: %.1f °C\n", temperatura);
    }
#pragma clang diagnostic pop
}

void * temperatura(void *pVoid) {
    pthread_t t1 = 0;

    TRACE("Criando thread de temperatura\n");
    pthread_create(&t1, NULL, thread_temperatura, NULL);
    TRACE("Iniciando thread de temperatura\n");
    pthread_join(t1, NULL);
}