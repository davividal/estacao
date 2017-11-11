#include <wiringPi.h>
#include <stdlib.h>

#include "umidade.h"
#include "mqtt.h"
#include "debug.h"
#include "temperatura.h"
#include "pressao.h"
#include "vento.h"
#include "config.h"

volatile int terminate = 0;
static int pronto = 0;

volatile int sensor_umidade = 0;
volatile int sensor_temperatura = 0;
volatile int sensor_pressao = 0;
volatile int sensor_vento = 0;
volatile int conexao_mqtt = 0;

void esta_pronto(void);

void *thread_status(void *pVoid);

int main() {
    pthread_t status, s1, s2, s3, s4;
    status = s1 = s2 = s3 = s4 = 0;

    TRACE("Configurando wiringPi\n");
    if (wiringPiSetup()) {
        exit(1);
    }

    pthread_create(&status, NULL, thread_status, NULL);
    pthread_detach(status);

    mqtt();

    TRACE("Inicializando sensores...\n");

    pthread_create(&s1, NULL, umidade, NULL);
    pthread_create(&s2, NULL, temperatura, NULL);
    pthread_create(&s3, NULL, pressao, NULL);
    pthread_create(&s4, NULL, vento, NULL);

    pthread_join(s1, NULL);
    pthread_join(s2, NULL);
    pthread_join(s3, NULL);
    pthread_join(s4, NULL);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (1);
#pragma clang diagnostic pop
}

void *thread_status(void *pVoid) {
    TRACE("Setando pino %d como %d\n", LED_STATUS, OUTPUT);
    pinMode(LED_STATUS, OUTPUT);

    while (!terminate) {
        while (!pronto) {
            delay(500);
            digitalWrite(LED_STATUS, LOW);
            delay(500);
            digitalWrite(LED_STATUS, HIGH);

            esta_pronto();
            TRACE("pronto: [%d]\n", pronto);
        }

        digitalWrite(LED_STATUS, LOW);
    }

    digitalWrite(LED_STATUS, HIGH);

    return NULL;
}

void esta_pronto(void) {
    TRACE("sensor_umidade [%d], sensor_temperatura [%d], sensor_pressao [%d], sensor_vento [%d], conexao_mqtt [%d]\n",
           sensor_umidade, sensor_temperatura, sensor_pressao, sensor_vento, conexao_mqtt);
    pronto = sensor_umidade && sensor_temperatura && sensor_pressao && sensor_vento && conexao_mqtt;
}
