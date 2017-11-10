//
// Created by davi on 11/7/17.
//

#include "umidade.h"
#include "mqtt.h"
#include "debug.h"
#include "config.h"

#include <stdlib.h>
#include <stdint.h>

int data[5] = {0, 0, 0, 0, 0};

void read_dht_data() {
    int lastState = HIGH;
    uint8_t counter = 0;
    uint8_t j = 0, i;

    data[0] = data[1] = data[2] = data[3] = data[4] = 0;

    TRACE("Configurando sensor de umidade\n");
    /* pull pin down for 18 milliseconds */
    pinMode(DHT_PIN, OUTPUT);
    digitalWrite(DHT_PIN, LOW);
    delay(18);

    /* prepare to read the pin */
    pinMode(DHT_PIN, INPUT);

    /* detect change and read data */
    for (i = 0; i < MAX_TIMINGS; i++) {
        counter = 0;
        while (digitalRead(DHT_PIN) == lastState) {
            counter++;
            delayMicroseconds(1);
            if (counter == 255) {
                break;
            }
        }
        lastState = digitalRead(DHT_PIN);

        if (counter == 255)
            break;

        /* ignore first 3 transitions */
        if ((i >= 4) && (i % 2 == 0)) {
            /* shove each bit into the storage bytes */
            data[j / 8] <<= 1;
            if (counter > 16)
                data[j / 8] |= 1;
            j++;
        }
    }

    /*
     * check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
     * print it out if data is good
     */
    if ((j >= 40) &&
        (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF))) {
        float h = (float) ((data[0] << 8) + data[1]) / 10;
        if (h > 100) {
            h = data[0];    // for DHT11
        }

        mqtt_pub_double("umidade", h);
        printf("Umidade: %.1f %%\n", h);
    }
}

void *thread_dht(void *pVoid) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (1) {
        read_dht_data();
        delay(INTERVALO_UMIDADE);
    }
#pragma clang diagnostic pop
}

void *umidade(void *pVoid) {
    pthread_t t1 = 0;

    TRACE("Criando thread de umidade\n");
    pthread_create(&t1, NULL, thread_dht, NULL);
    TRACE("Iniciando thread de umidade\n");
    pthread_join(t1, NULL);

    return NULL;
}